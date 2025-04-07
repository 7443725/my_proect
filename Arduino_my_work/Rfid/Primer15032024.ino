//#define PRIMER                                                   //автоматическое создание объекта
#define BTN_PIN 2                                                  //кнопка

#include "Primer_15.01.2024H.h"
#include "GyverButton.h"
GButton butt1(BTN_PIN);                                            //объект кнопка

int main()
{
  init();                                                          //для того чтоб работало в ардуино
  Serial.begin(9600);
  //DDRD&=~(1<<BTN_PIN);                                           //настройка кнопки, если не ипользовать библиотеку Алекса(без подтяжки)
  DDRB|=1<<5;                                                      //настройка светодиода
  PORTD|=(1<<BTN_PIN);                                             //единица на светодиод
 
  for(;;)                                                          //бесконечный цикл
  {
    butt1.tick();                                                  //опрос кнопки
   /* if(!(PIND&(1<<BTN_PIN)))                                     //опрос кнопки
     {
      Timer time1(50);                                             //антидребезг
      while(!(PIND&(1<<BTN_PIN)))
      {
        time1.ready();
        }
        //delete time1;
      PORTB^=(1<<5);                                                //инверсия светодиода
      } */   
    
    if (butt1.isSingle()) {Serial.println("Single");PORTB^=(1<<5);}     // проверка на один клик инверсия светодиода
    if (butt1.isDouble()) Serial.println("Double");     // проверка на двойной клик
    if (butt1.isTriple()) Serial.println("Triple");     // проверка на тройной клик
  }
  }
