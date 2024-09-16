#include <LCD_1602_RUS.h>
#include <Button.h>


#define rel 2
#define pF 0
#define rg 1
#define cx 2
#define fk 3
#define fk1 4

LCD_1602_RUS lcd(0x27, 16, 2);
Button button1(14); 
Button button2(15);

void Batt_zar();
void Tdr();
unsigned int takeMeasurement(bool posEdge);
size_t findSteps(bool positive, struct Step *results, size_t maxResults);
float clocksToMetres(unsigned int clocks);
/******///////////////////
void(* resetFunc) (void) = 0;   //resetFunc(); cbross!!!!!!!
/******////////////////////

const int stepPin = 4;       // digital output pin we generate the step on. Later in the code, we assume this is port D bit 4.
const int refPin = 11;       // PWM output pin that we use to generate the comparator reference voltage. Must be on timer 2.
const int shutdownPin = 6;   // digital output port used to put comparator in shutdown mode - drive HIGH before sleeping  //РАзобраться с пином выбрать другой
const int maxSteps = 4;      // maximum number of steps in each direction that we capture



struct KOIFF{
const float propagationFactor = 0.68;  // fraction of speed of light that waves travel in the cable under test //  cat 5 cable 68%
const float rg58_rg213 = 0.66;         //  RG58 or RG213 coax (solid polyethylene dieletric) 66%
const float coax = 0.92;                //  Airspaced coax up to 92%
uint8_t fl_koif = 0;
} koif;

struct Step
{
  unsigned int time;
  unsigned int amplitude;
};

void setup(){
  button1.begin();
  button2.begin();
  lcd.init();                                        
  lcd.backlight();
  lcd.setCursor(5, 0);
  lcd.print("ТЕСТЕР");
  lcd.setCursor(1, 1);
  lcd.print("TDR");
  lcd.setCursor(12, 1);
  lcd.print("VID");
  pinMode(rel,OUTPUT);
  digitalWrite(rel,LOW);
  ///////////////////////////
  pinMode(stepPin, OUTPUT);
  pinMode(refPin, OUTPUT);
  pinMode(shutdownPin, OUTPUT);  
  TCCR1A = 0;
  TCCR1B = (1 << ICNC1);   // input capture noise canceller enabled, capture on falling edge
  TIMSK1 = 0;              // timer 1 interrupts disabled
  ACSR = 0;                // input capture from ICP1 pin
  TCCR2B = (1 << CS20);    // change timer 2 PWM frequency to 31.25kHz because we're using pin 11 as a DAC
  ///////////////////////////
}

void loop() {
  lcd.setCursor(5, 0);
  lcd.print("ТЕСТЕР");
  lcd.setCursor(1, 1);
  lcd.print("TDR");
  lcd.setCursor(12, 1);
  lcd.print("VID");
  static unsigned long tmr1 = 0;
  if(millis()-tmr1 >= 5000){
    tmr1=millis();
    Batt_zar();
  }
  if (button1.pressed())
  {
      lcd.clear();
      digitalWrite(rel,LOW);
      Tdr();
      lcd.clear();
  }
  if (button2.pressed())
  {  
      lcd.clear();
      digitalWrite(rel,HIGH);
      
      while(!(button1.pressed())){
        lcd.setCursor(1, 0);
        lcd.print("ТЕСТЕР-VID-PAL");
        lcd.setCursor(0, 1);
        lcd.print("Выход");
        static unsigned long tmr2 = 0;
        if(millis()-tmr2 >= 5000){
        tmr2=millis();
        Batt_zar();
      }
      }
      digitalWrite(rel,LOW);
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Выключение");
      unsigned long tmr = millis()+15000;
      uint8_t i = 3;
      while(millis()<=tmr)
      {
        lcd.setCursor(i++, 1);
        lcd.print("*");
        delay(500);
        if(i>=13){
          i = 3;
          lcd.clear();
          lcd.setCursor(3, 0);
          lcd.print("Выключение");
        }
        }
      lcd.clear();
  }  
}

void Batt_zar(){
  lcd.clear();
  uint16_t val = analogRead(A7);
  float val1 = (val * (5.0 / 1023.0))*2;
  lcd.setCursor(6, 1);
  lcd.print(val1);
  lcd.setCursor(9, 1);
  lcd.print('B');
  if(val1 <= 3.1){
    lcd.setCursor(1, 0);
    lcd.print("Низкий заряд");
    delay(500);
    lcd.clear();
  }
}
