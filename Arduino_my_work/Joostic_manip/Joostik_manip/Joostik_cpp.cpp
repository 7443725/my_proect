/*
	Button - a small library for Arduino to handle button debouncing
	
	MIT licensed.
*/

#include "Joostik_h.h"


void adc_init(){
    ADCSRA|=(1<<ADEN);
    ADCSRA|=(1<<ADPS2);
    ADCSRA&=~(1<<ADPS1)&~(1<<ADPS0);                 //16
    ADCSRA&=~(1<<ADATE);                 // режим работы одиночное преобразование
    ADMUX&=~(1<<REFS1);
    ADMUX|=(1<<REFS0);
    ADMUX&=~(1<<ADLAR);//ADLAR = 0 
  }

  uint16_t adc_measur(Adc_chan_en adc_chan){
    switch (adc_chan){
      case _ADC0:
       ADMUX&=~(1<<MUX4)&~(1<<MUX3)&~(1<<MUX2)&~(1<<MUX1)&~(1<<MUX0);
      break;
      case _ADC1:
       ADMUX&=~(1<<MUX4)&~(1<<MUX3)&~(1<<MUX2)&~(1<<MUX1);
       ADMUX|=(1<<MUX0);
      break;
      case _ADC2:
       ADMUX&=~(1<<MUX4)&~(1<<MUX3)&~(1<<MUX2)&~(1<<MUX0);
       ADMUX|=(1<<MUX1);
      break;
      default:
      break;
    }
    ADCSRA|=(1<<ADSC);
    while(!(ADCSRA&(1<<ADIF)));
    ADCSRA|=(1<<ADIF);
    return ADC;
  }

  void button_init(){
    DDRB&=~(1<<PB0);
    DDRB&=~(1<<PB1);
    PORTB|=(1<<PB0);
    PORTB|=(1<<PB1);
  }

  long l_map(long x, long in_min, long in_max, long out_min, long out_max)
  {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }
  
  void data_ic_rl(data_ic *data_ic_buf){
    
    for(uint8_t i = 0; i < 3; i++){
    if(i == 0)
        data_ic_buf->x = l_map((adc_measur(_ADC0)),71,912,0,180);  // 75/900
    else if(i == 1)
        data_ic_buf->y = l_map((adc_measur(_ADC1)),71,912,0,180);
    else if(i == 2)
        data_ic_buf->z = l_map((adc_measur(_ADC2)),71,910,0,180);
         
    if(!(PINB&(1<<PB0))){
           data_ic_buf->clamp = 1;
      }
    else if(!(PINB&(1<<PB1))){  
           data_ic_buf->unclamp = 1;
      } 
    else{
      data_ic_buf->clamp = 0;
      data_ic_buf->unclamp = 0;
    }
    }             
  }
////////////////////////////////////////////////////////////////////////
static void (*I2C_recv)(uint8_t);
static void (*I2C_req)();

void I2C_setCallbacks(void (*recv)(uint8_t), void (*req)())
{
  I2C_recv = recv;
  I2C_req = req;
}

void I2C_init(uint8_t address)
{
  cli();
  // load address into TWI address register
  TWAR = address << 1;
  // set the TWCR to enable address matching and enable TWI, clear TWINT, enable TWI interrupt
  TWCR = (1<<TWIE) | (1<<TWEA) | (1<<TWINT) | (1<<TWEN);
  sei();
}

void I2C_stop(void)
{
  // clear acknowledge and enable bits
  cli();
  TWCR = 0;
  TWAR = 0;
  sei();
}

ISR(TWI_vect)
{
  switch(TW_STATUS)
  {
    case TW_SR_DATA_ACK:
      // received data from master, call the receive callback
      I2C_recv(TWDR); 
      TWCR = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
      break;
    case TW_ST_SLA_ACK:
      // master is requesting data, call the request callback
      I2C_req();
      TWCR = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
      break;
    case TW_ST_DATA_ACK:
      // master is requesting data, call the request callback
      I2C_req();
      TWCR = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
      break;
    case TW_BUS_ERROR:
      // some sort of erroneous state, prepare TWI to be readdressed
      TWCR = 0;
      TWCR = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN); 
      break;
    default:
      TWCR = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
      break;
  }
} 
