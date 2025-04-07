#pragma once

#include <Arduino.h>


class Timer                                          //класс для отсчета времени
            { 
               public : 
                    Timer(uint16_t nprd = 0)         //конструктор
                     { 
                       #ifndef PRIMER                //зашита для обязательного создания обьекта
                       #define PRIMER1
                       #endif 
                       _prd = nprd;
                      } 
                     void setPeriod(uint16_t nprd)    //установка времени
                      { 
                        _prd = nprd; 
                      } 
                      void ready()                     //готовность отсчета
                      { 
                        if (millis() - _tmr >= _prd)
                       {
                          _tmr = millis();
                        }
                        
                      } 
                     
                      
                 private : 
                      uint32_t _tmr; 
                      uint16_t _prd; 
                    
 };

#ifndef PRIMER1
#ifndef PRIMER  
#error "TimerObject"                                   //ошибка создания объекта
#endif 
#ifdef PRIMER
 //extern Timer TimerObject;
Timer TimerObject =  Timer();                          //автоматическое создание обьекта
#endif
#endif
