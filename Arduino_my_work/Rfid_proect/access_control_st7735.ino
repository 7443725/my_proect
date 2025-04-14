#include "main_meteo.h"

//-----------------------Global-------------------------
//------------------------TFT ST7735--------------------
  Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
//-----------------------DS3231-------------------------
  MicroDS3231 rtc;
//-----------------------RC-522-------------------------
  MFRC522 rfid(SS_PIN, RST_PIN);   // Объект rfid модуля
  MFRC522::MIFARE_Key key;         // Объект ключа
  MFRC522::StatusCode status;      // Объект статуса
//----------------------Encoder-------------------------
  Encoder enc1(CLK, DT, SW);
//------------------------------------------------------



void setup(void) {
  Serial.begin(9600);              // Инициализация Serial
  SPI.begin();                     // Инициализация SPI
  //------------------------TFT ST7735--------------------
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.setRotation(2);
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(1);
  tft.fillScreen(ST77XX_WHITE);
  tft.setCursor(0,0);
  pinMode(LED_TFT,OUTPUT);
  digitalWrite(LED_TFT,HIGH);
  //-----------------------RC-522-------------------------
  rfid.PCD_Init();                 // Инициализация модуля
  rfid.PCD_SetAntennaGain(rfid.RxGain_max);  // Установка усиления антенны
  rfid.PCD_AntennaOff();           // Перезагружаем антенну
  rfid.PCD_AntennaOn();            // Включаем антенну
  for (byte i = 0; i < 6; i++) {   // Наполняем ключ
    key.keyByte[i] = 0xFF;         // Ключ по умолчанию 0xFFFFFFFFFFFF
  }
  rfid.PCD_DumpVersionToSerial();  //version
  //-----------------------Timer-------------------------
  MsTimer2::set(1, flash); // 1ms period
  MsTimer2::start();
  //-----------------------DS3231------------------------
  if (!rtc.begin()) {
    tft.print("ERR: DS3231 not found");
    delay(3000);
    tft.fillScreen(ST77XX_WHITE);
    tft.setCursor(0,0);
  }
  if (rtc.lostPower()) {            // выполнится при сбросе батарейки
    tft.print("ERR:lost power!");
    delay(3000);
    tft.fillScreen(ST77XX_WHITE);
    tft.setCursor(0,0);
    // тут можно однократно установить время == времени компиляции
  }
  rtc.setTime(BUILD_SEC, BUILD_MIN, BUILD_HOUR, BUILD_DAY, BUILD_MONTH, BUILD_YEAR);
  //----------------------Encoder-------------------------
  enc1.setType(TYPE2);
  //----------------------Buzzer--------------------------
  pinMode(BZ,OUTPUT);
}

void loop() {
  //-------------------------Game--------------------------
      Game();
  //-------------------------Buzzer------------------------
    if(flag_BZ_op || flag_BZ_cl)
    {    
      if(flag_BZ_op)
      {
        digitalWrite(BZ,HIGH);                                          //Buzz
        if(timer_buzz >= 200)
        {
          flag_BZ_op = false;
          digitalWrite(BZ,LOW);
        }
      }
      else if(flag_BZ_cl)
      {
        static uint16_t count = 0;
        if(timer_buzz - count >= 100)
        {
          count = timer_buzz;
          digitalWrite(BZ,!digitalRead(BZ));  
        }                                     
        if(timer_buzz >= 1000)
        {
          count = 0;
          flag_BZ_cl = false;
          digitalWrite(BZ,LOW);
        }
      }
    }
   //------------------------DS3231------------------------
    setclock();
   //------------------------TFT ST7735-------------------- 
   static uint16_t tick1 = tictime_msec;
   if(tictime_msec - tick1 >= 3000)
   {
      tick1 = tictime_msec;
      tft.fillRoundRect(3,80,123,50,7,ST77XX_WHITE);
      tft.setCursor(10,100);
      tft.setTextSize(2);
      tft.println("Toch card");
   }
   //--------------menu---------------------------------------------------------
   if (enc1.isSingle())
     menu();
   //-----------------------RC-522-------------------------
    static uint32_t rebootTimer = millis(); // Важный костыль против зависания модуля!
    if (millis() - rebootTimer >= 1000) {   // Таймер с периодом 1000 мс
      rebootTimer = millis();               // Обновляем таймер
      digitalWrite(RST_PIN, HIGH);          // Сбрасываем модуль
      delayMicroseconds(2);                 // Ждем 2 мкс
      digitalWrite(RST_PIN, LOW);           // Отпускаем сброс
      rfid.PCD_Init();                      // Инициализируем заного
  }
    if (!rfid.PICC_IsNewCardPresent()) return;  // Если новая метка не поднесена - вернуться в начало loop
    if (!rfid.PICC_ReadCardSerial()) return;    // Если метка не читается - вернуться в начало loop
    for(uint8_t i = 0; i < 4 ; i++)
    {
      if(!memcmp(user[i].namber,rfid.uid.uidByte,4))
      {
        tictime_msec = 0;
        tft.fillRoundRect(3,80,123,50,7,ST77XX_GREEN);
        tft.setCursor(30,100);
        tft.setTextSize(2);
        tft.println((char*)user[i].name_u);
        timer_buzz = 0;
        flag_BZ_op = true;
        flag_BZ_cl = false;
        return;
      }
    }
    tictime_msec = 0;
    timer_buzz = 0;
    flag_BZ_cl = true;
    flag_BZ_op = false;
    tft.fillRoundRect(3,80,123,50,7,ST77XX_RED);
    tft.setCursor(30,100);
    tft.setTextSize(2);
    tft.println("Closed"); 
}


///------------------------------------------------------------------------------///
void setclock()
{
  static uint32_t t_clock = millis(); 
  if (millis() - t_clock >= 500) {
    t_clock = millis();
    tft.setCursor(5,5);
    tft.setTextSize(1);
    tft.fillRect(0, 0 , 127, 15, ST77XX_WHITE);
    tft.print(rtc.getTimeString());
    tft.print("  ");
    tft.print(rtc.getDateString());
  }  
}

void flash(void)
{ 
  enc1.tick();                 //tick encoder and sw
  tictime_msec++;
  timer_buzz ++;
}

void menu()
{
  digitalWrite(BZ,LOW);
  tft.fillScreen(ST77XX_WHITE);
  tft.setCursor(40,20);
  tft.setTextSize(2);
  tft.print("MENU");
  tft.fillRoundRect(5,45,118,30,7,ST77XX_RED);
  tft.setCursor(10,55);
  tft.print("Clock Set");
  tft.fillRoundRect(5,80,118,30,7,ST77XX_RED);
  tft.setCursor(10,90);
  tft.print("Erase Uid");
  tft.fillRoundRect(5,115,118,30,7,ST77XX_RED);
  tft.setCursor(10,125);
  tft.print("Delet Uid");
  uint8_t num = 0;
  uint8_t oldnum = 0;
  tictime_msec = 0;               //sbros msek
  while(1)
  {
    setclock();
    if (enc1.isRight())
    {
      num++;
      if(num > 3)
        num = 3;
    }      
    if (enc1.isLeft())
    {
      num--;
      if(num < 1 )
        num = 1;
    }
    if(oldnum != num)
    {
      switch(num)
      {
        case 1:
        {
          tft.setTextSize(2);
          tft.fillRoundRect(5,45,118,30,7,ST77XX_GREEN);
          tft.setCursor(10,55);
          tft.write("Clock Set");
          tft.fillRoundRect(5,80,118,30,7,ST77XX_RED);
          tft.setCursor(10,90);
          tft.print("Erase Uid");
          tft.fillRoundRect(5,115,118,30,7,ST77XX_RED);
          tft.setCursor(10,125);
          tft.print("Delet Uid");
        }
        break;
        case 2:
        {
          tft.setTextSize(2);
          tft.fillRoundRect(5,45,118,30,7,ST77XX_RED);
          tft.setCursor(10,55);
          tft.write("Clock Set");
          tft.fillRoundRect(5,80,118,30,7,ST77XX_GREEN);
          tft.setCursor(10,90);
          tft.print("Erase Uid");
          tft.fillRoundRect(5,115,118,30,7,ST77XX_RED);
          tft.setCursor(10,125);
          tft.print("Delet Uid");
        }
        break;
        case 3:
        {
          tft.setTextSize(2);
          tft.fillRoundRect(5,45,118,30,7,ST77XX_RED);
          tft.setCursor(10,55);
          tft.write("Clock Set");
          tft.fillRoundRect(5,80,118,30,7,ST77XX_RED);
          tft.setCursor(10,90);
          tft.print("Erase Uid");
          tft.fillRoundRect(5,115,118,30,7,ST77XX_GREEN);
          tft.setCursor(10,125);
          tft.print("Delet Uid");
        }
        break;
        default:
        break;
      }
      oldnum = num;
      tictime_msec = 0;
    }
    if(tictime_msec >= 30000)
    {
      tft.fillScreen(ST77XX_WHITE);
      return;
    }
    if (enc1.isDouble())
    {
      tft.fillScreen(ST77XX_WHITE);
      return;
    }
    if (enc1.isSingle())
    {
      tft.fillScreen(ST77XX_WHITE);
      switch(num)
      {
        case 1:
          Clock_Set();
        break;
        case 2:
          Erase_Uid();
        break;
        case 3:
          Delet_Uid();
        break;
        default:
        break;
      }
      return;      
    }
  }  
}

void Clock_Set()
{
  tft.setTextSize(2);
  tft.setCursor(18,50);
  tictime_msec = 0;
  DateTime now = rtc.getTime(); 
  tft.print(now.hour);
  tft.print(":");
  tft.print(now.minute);
  tft.print(":");
  tft.println(now.second);
  tft.println();
  tft.print("");
  tft.print(now.date);
  tft.print("/");
  tft.print(now.month);
  tft.print("/");
  tft.println(now.year);
  tft.fillRoundRect(5,40,118,70,7,ST77XX_WHITE);
  uint8_t num_m = 0;
  uint8_t old_num_m = 0;
  while(1)
  {
    switch(num_m)
    {
      case 0:
      {
        tft.fillRoundRect(18,66,20,3,1,ST77XX_GREEN);
        if (enc1.isRight())
        {
          if(++now.hour > 24)
            now.hour=24;
        }      
        if (enc1.isLeft())
        {
          if(--now.hour == 255)
            now.hour=0;
        }
        if (enc1.isSingle())
        {
          tft.fillRoundRect(18,66,20,3,1,ST77XX_WHITE);
          num_m++;
        }
      }
      break;
      case 1:
      {
        tft.fillRoundRect(55,66,20,3,1,ST77XX_GREEN);
        if (enc1.isRight())
        {
          if(++now.minute > 59)
            now.minute=59;
        }      
        if (enc1.isLeft())
        {
          if(--now.minute == 255)
            now.minute=0;
        }
        if (enc1.isSingle())
        {
          tft.fillRoundRect(55,66,20,3,1,ST77XX_WHITE);
          num_m++;
        }
      }
      break;
      case 2:
      {
        tft.fillRoundRect(90,66,20,3,1,ST77XX_GREEN);
        if (enc1.isRight())
        {
          if(++now.second > 59)
            now.second=59;
        }      
        if (enc1.isLeft())
        {
          if(--now.second == 255)
            now.second=0;
        }
        if (enc1.isSingle())
        {
          tft.fillRoundRect(90,66,20,3,1,ST77XX_WHITE);
          num_m++;
        }
      }
      break;
      case 3:
      {
        tft.fillRoundRect(3,98,20,3,1,ST77XX_GREEN);
        if (enc1.isRight())
        {
          if(++now.date > 31)
            now.date=31;
        }      
        if (enc1.isLeft())
        {
          if(--now.date < 1)
            now.date=1;
        }
        if (enc1.isSingle())
        {
          tft.fillRoundRect(3,98,20,3,1,ST77XX_WHITE);
          num_m++;
        }
      }
      break;
      case 4:
      {
        tft.fillRoundRect(35,98,20,3,1,ST77XX_GREEN);
        if (enc1.isRight())
        {
          if(++now.month > 12)
            now.month=12;
        }      
        if (enc1.isLeft())
        {
          if(--now.month < 1)
            now.month=1;
        }
        if (enc1.isSingle())
        {
          tft.fillRoundRect(35,98,20,3,1,ST77XX_WHITE);
          num_m++;
        }
      }
      break;
      case 5:
      {
        tft.fillRoundRect(65,98,40,3,1,ST77XX_GREEN);
        if (enc1.isRight())
        {
          if(++now.year > 2030)
            now.year=2030;
        }      
        if (enc1.isLeft())
        {
          if(--now.year < 2000)
            now.year=2000;
        }
        if (enc1.isSingle())
        {
          tft.fillScreen(ST77XX_WHITE);
          rtc.setTime(now);
          return;
        }
      }
      break;
      default:
      break;
    }
     if (enc1.isTurn())
     { 
       tft.fillRoundRect(0,40,123,70,7,ST77XX_WHITE);
       tft.setCursor(18,50);
       tft.print(now.hour);
       tft.print(":");
       tft.print(now.minute);
       tft.print(":");
       tft.println(now.second);
       tft.println();
       tft.print("");
       tft.print(now.date);
       tft.print("/");
       tft.print(now.month);
       tft.print("/");
       tft.println(now.year);      
     }
       
      if(num_m != old_num_m )
      {
        tictime_msec = 0; 
        old_num_m = num_m;
      } 
      
      if (enc1.isDouble())
      {
        tft.fillScreen(ST77XX_WHITE);
        return;
      }
      if(tictime_msec >= 30000)
      {
        tft.fillScreen(ST77XX_WHITE);
        return;
      }
  }
}

void Erase_Uid(void)
{
  tictime_msec = 0;
  uint8_t num_us = 0;
  while(1)
  {
    if (enc1.isTurn())
    {
      tictime_msec = 0;
      tft.fillRoundRect(0,10,123,50,7,ST77XX_WHITE);
      tft.setCursor(0,10);
      for(uint8_t i = 0; i < 4;i++)
      {
        tft.print("  ");
        tft.print((char*)user[i].name_u);
        tft.print("  ");
        for(uint8_t ii = 0; ii < 4;ii++)
          tft.print(user[i].namber[ii]);
        tft.println();
      }
      tft.println();
      tft.print("  ");
      tft.print("Vvod # US: ");
      if (enc1.isRight())
      {
        if(++num_us > 3)
          num_us=3;
      }      
      if (enc1.isLeft())
      {
        if(--num_us == 255)
        num_us = 0;
      }
      tft.println(num_us+1);
    }
    if (enc1.isSingle())
    {
      tft.setCursor(8,68);
      tft.println("Toch card...");
      while(1)
      {
        if(tictime_msec >= 30000)
        {
          tft.fillScreen(ST77XX_WHITE);
          return;
        }
        static uint32_t rebootTimer1 = millis(); 
        if (millis() - rebootTimer1 >= 1000) 
        {  
          rebootTimer1 = millis();               
          digitalWrite(RST_PIN, HIGH);          // Сбрасываем модуль
          delayMicroseconds(2);                 // Ждем 2 мкс
          digitalWrite(RST_PIN, LOW);           // Отпускаем сброс
          rfid.PCD_Init();                      // Инициализируем заного
        }
        if (!rfid.PICC_IsNewCardPresent()) continue;  // Если новая метка не поднесена - вернуться в начало loop
        if (!rfid.PICC_ReadCardSerial()) continue;    // Если метка не читается - вернуться в начало loop
        tictime_msec = 0;
        for (uint8_t u = 0; u < 4; u++)
          user[num_us].namber[u]=rfid.uid.uidByte[u];
        break;
      }
      tft.println();
      tft.println("  User name: > 6 sim ");
      uint8_t sim = 0;
      while(1)
      {
        if(tictime_msec >= 30000)
        {
          tft.fillScreen(ST77XX_WHITE);
          return;
        }
        if (enc1.isRight())
        {
          if(++user[num_us].name_u[sim] > 122)
            user[num_us].name_u[sim] = 'z';
        }      
        if (enc1.isLeft())
        {
          if(--user[num_us].name_u[sim] == 96)
            user[num_us].name_u[sim] = 'a';
        }
        if (enc1.isTurn())
        { 
          tictime_msec = 0;
          tft.setCursor(8,100);
          tft.fillRoundRect(3,100,80,10,1,ST77XX_WHITE);
          tft.print((char*)user[num_us].name_u);
        }
        if (enc1.isSingle())
          sim++;
        if(sim > 4 || enc1.isDouble())
          break;
      }
      tft.fillRoundRect(3,65,123,50,7,ST77XX_WHITE);
    }
    
    if (enc1.isDouble())
    {
      tft.fillScreen(ST77XX_WHITE);
      return;
    }
    if(tictime_msec >= 30000)
    {
      tft.fillScreen(ST77XX_WHITE);
      return;
    }
  }
  
}

void Delet_Uid(void)
{
  tictime_msec = 0;
  tft.fillRoundRect(0,10,123,50,7,ST77XX_WHITE);
  tft.setCursor(0,10);
  for(uint8_t i = 0; i < 4;i++)
  {
    tft.print("  ");
    tft.print((char*)user[i].name_u);
    tft.print("  ");
    for(uint8_t ii = 0; ii < 4;ii++)
      tft.print(user[i].namber[ii]);
    tft.println();
   }
   char buff[4][6]={{"user1"},{"user2"},{"user3"},{"user4"}};
   while(1)
  {
    if (enc1.isSingle())
    {
      for(uint8_t i = 0; i < 4;i++)
      {
         memcpy(user[i].name_u,buff[i],6);
         for(uint8_t ii = 0; ii < 4;ii++)
           user[i].namber[ii] = 0;
      }
      
      tft.fillRoundRect(0,10,123,50,7,ST77XX_WHITE);
      tft.setCursor(0,10);
      for(uint8_t i = 0; i < 4;i++)
      {
         tft.print("  ");
         tft.print((char*)user[i].name_u);
         tft.print("  ");
         for(uint8_t ii = 0; ii < 4;ii++)
           tft.print(user[i].namber[ii]);
         tft.println();
      }
      delay(2000);
      tft.fillScreen(ST77XX_WHITE);
      return;
    }
    if (enc1.isDouble())
    {
      tft.fillScreen(ST77XX_WHITE);
      return;
    }
    if(tictime_msec >= 30000)
    {
      tft.fillScreen(ST77XX_WHITE);
      return;
    }    
  }
}

void Game()
{
  static uint32_t game_time = millis();
  static uint8_t x,y,x1,y1,x2,y2,x3,y3,by = 65,bx = 120;
  if(millis()-game_time >= 500)
  {
    game_time = millis();
    tft.fillRoundRect(x,y,16,16,8,ST77XX_WHITE);
    tft.fillRoundRect(x1,y1,16,16,8,ST77XX_WHITE);
    tft.fillRoundRect(x2,y2,16,16,8,ST77XX_WHITE);
    tft.fillRoundRect(x3,y3,16,16,8,ST77XX_WHITE);
    x = random(6,bx);
    y = random(20,by);
    x1 = random(6,bx);
    y1 = random(20,by);
    x2 = random(6,bx);
    y2 = random(20,by);
    x3 = random(6,bx);
    y3 = random(20,by);    
    tft.fillRoundRect(x,y,16,16,8,ST77XX_YELLOW);
    tft.fillRoundRect(x1,y1,16,16,8,ST77XX_MAGENTA);   
    tft.fillRoundRect(x2,y2,16,16,8,ST77XX_GREEN);  
    tft.fillRoundRect(x3,y3,16,16,8,ST77XX_RED);    
  }
  if (enc1.isRight())
  {
    by++;
    bx++;
    if(by > 65)
      by = 65;
    if(bx > 120)
      bx = 120;
  }
  if (enc1.isLeft())
  {
    by--;
    bx--;
    if(by < 20)
      by = 20;
    if(bx < 6)
      bx = 6;  
  }
}
