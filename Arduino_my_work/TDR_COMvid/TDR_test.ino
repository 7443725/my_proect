void Tdr(){
  lcd.setCursor(1, 0);
  lcd.print("ТЕСТЕР TDR 68%");
  koif.fl_koif = 0;
  digitalWrite(shutdownPin,LOW);
  lcd.setCursor(1, 1);
  lcd.print("Rg58");
  lcd.setCursor(11, 1);
  lcd.print("Coax");
  delay(800);
  koif.fl_koif|=(1<<pF);
  koif.fl_koif|=(1<<fk);
  unsigned long tmr3 = millis()+5000;
  while(millis()<=tmr3&&((koif.fl_koif&(1<<fk))>>fk)){
    if (button1.pressed()){
      koif.fl_koif = 0; 
      koif.fl_koif|=(1<<rg);
    }
    if(button2.pressed()){
      koif.fl_koif = 0; 
      koif.fl_koif|=(1<<cx);
    }
  }
  lcd.clear();
  lcd.setCursor(1, 0);
  if ((koif.fl_koif&(1<<pF))>>pF)
            lcd.print("ТЕСТЕР TDR 68%");
  else if ((koif.fl_koif&(1<<rg))>>rg)
            lcd.print("ТЕСТЕР TDR 66%");
  else if ((koif.fl_koif&(1<<cx))>>cx)
            lcd.print("ТЕСТЕР TDR 92%");
  delay(3000);          
  Batt_zar();
  Step posSteps[maxSteps], negSteps[maxSteps];
  size_t numPosSteps = findSteps(true, posSteps, maxSteps);
  size_t numNegSteps = findSteps(false, negSteps, maxSteps);
  if (numPosSteps == 0)
  {
    lcd.setCursor(1, 0);
    lcd.print("Короткий каб.");      // короткий кабель
  }
  else if (numPosSteps == 1 && numNegSteps == 0)
  {
    if (posSteps[0].amplitude >= 200)
    {
      lcd.setCursor(1, 0);
      lcd.print("Нет кабеля");  // кабель не подключен
    }
    else
    {
      lcd.setCursor(1, 0);
      lcd.print("Волнов. сопр.");  //кабель подключен правильно и равен волновому сопротивлению
    }
  }
  else if (numPosSteps >= 2 && numNegSteps == 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("Откр(м): ");  // открыт + измерение
    lcd.setCursor(9, 0);
    lcd.print((clocksToMetres(posSteps[1].time - posSteps[0].time)), DEC); 
  }
  else if (numPosSteps == 1 && numNegSteps == 1)
  {
    lcd.setCursor(0, 0);
    lcd.print("к/з(м):"); //к/з + измерение
    lcd.setCursor(9, 0);
    lcd.print(clocksToMetres(negSteps[0].time - posSteps[0].time), DEC);
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Ошибка каб.");  //Не удалось диагностировать неисправность
  }
  delay(3000);
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print("Выход");
  lcd.setCursor(10, 1);
  lcd.print("Измер");
  koif.fl_koif|=(1<<fk1);
  unsigned long tmr4 = millis()+10000;
  while(millis()<=tmr4&&((koif.fl_koif&(1<<fk1))>>fk1)){
     if (button1.pressed()){
      koif.fl_koif&=~(1<<fk1);
     }
     if (button2.pressed()){
      lcd.clear();
      Tdr();
     }
  } 
  digitalWrite(shutdownPin,HIGH); 
}










unsigned int takeMeasurement(bool posEdge)
{
  byte reg1b = (posEdge) ? 0 : (1 << ICES1);    // input capture noise canceller disabled, set up input capture polarity, stop timer
  reg1b |= (1 << CS10);
  TCCR1B = reg1b;
  TCNT1H = 0;
  TCNT1L = 0;              // clear timer
  unsigned int capture = 0;
  unsigned long start = micros();  // get the time
  
  cli();
  TCNT1H = 0;
  TCNT1L = 0;              // clear timer
  TIFR1 = (1 << ICF1);     // clear timer 1 input capture bit
  PORTD |= (1 << 4);       // set output high
  sei();

  do
  {
    if ((TIFR1 & (1 << ICF1)) && capture == 0)
    {
      byte temp = ICR1L;
      capture = (ICR1H << 8) | temp;
    }
  } while (micros() - start < 100);
  
  PORTD &= ~(1 << 4);          // set output low
  return capture;
}

size_t findSteps(bool positive, struct Step *results, size_t maxResults)
{
  byte amplitude = (positive) ? 5 : 250;
  analogWrite(refPin, amplitude);
  delay(100);      // wait 100ms for the output to stabilise
  unsigned int lastReading = 0;
  size_t numResults = 0;
  unsigned int stepSize = 0;        // 0 means not in a step

  for (int i = 0; i < 50; ++i)
  {
    analogWrite(refPin, amplitude);
    delay(10);
    unsigned int currentReading = takeMeasurement(positive);
    unsigned int currentDiff = currentReading - lastReading;    // diff since start of possible step
    if (stepSize == 0)
    {
      // Not currently in a step
      if (i != 0 && currentReading != 0 && currentDiff == 0)
      {
        // Found the start of a possible step
        ++stepSize;
      }
      lastReading = currentReading;
    }
    else
    {
      if (currentDiff > 2 || i + 1 == 50)
      {
        // Step has endeed, so record it if it is big enough
        if (stepSize >= 2)
        {
          results->time = lastReading;
          results->amplitude = amplitude - 5;
          ++results;
          ++numResults;
          if (numResults == maxResults) break;
        }
        stepSize = 0;
        lastReading = currentReading;
      }
      else if (currentDiff == 0)
      {
        ++stepSize;
      }
    }
    if (positive)
    {
      amplitude += 5;
    }
    else
    {
      amplitude -= 5;
    }
  }
  return numResults;
}

// Convert a number of clocks delay to a cable length in metres
float clocksToMetres(unsigned int clocks)
{
  float delayTime = (float)clocks/(float)F_CPU;    // delay in seconds
  return (delayTime * 3.0e8 * (((koif.fl_koif&(1<<pF))>>pF)==1?koif.propagationFactor:((koif.fl_koif&(1<<rg))>>rg)==1?koif.rg58_rg213:((koif.fl_koif&(1<<cx))>>cx)==1?koif.coax:1))/2.0;
}

// Diagnose the cable condition. We have the following common possibilities:
// 1. No cable connected, or a very short open-circuit cable connected.
//    In this case we should see the original positive step, very close to zero delay, with amplitude nearly 5V.
// 2. Direct short, or very short cable with shorted end.
//    In this case, we will not even see the original positive-going step, or it will have a very low amplitude.
// 3. Cable with open circuit.
//    We see the original positive step close to zero delay, and a further positive step going to nearly twice the amplitude later.
// 4. Cable with short circuit.
//    We see the original positive step close to zero delay, and a negative step going back down to nearly zero later.
// 5. Correctly terminated cable.
//    We see only the original positive step, with amplitudfe well below 5V.
