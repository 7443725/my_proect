

#ifndef JOOSTIC_HH_
#define JOOSTIC_HH_

#define F_CPU 8000000UL
#define FOR for(;;)
#include <avr/interrupt.h>
#include "C:/Users/x/Documents/Arduino/Joostic_manip/twi.h"


typedef unsigned int uint16_t;
typedef unsigned char uint8_t;
typedef uint8_t byte;

#include <avr/io.h>

struct data_ic{
    uint8_t x;
    uint8_t y;
    uint8_t z;
    unsigned short clamp : 1;
    unsigned short unclamp : 1; 
    uint8_t u_crc; 
};

enum Adc_chan_en{
    _ADC0,
    _ADC1,
    _ADC2
};

void adc_init();
uint16_t adc_measur(Adc_chan_en adc_chan);
void button_init();
long l_map(long, long, long, long, long);
void data_ic_rl(data_ic *);
/////////////////////////////////////////////////////
void I2C_init(uint8_t address);
void I2C_stop(void);
void I2C_setCallbacks(void (*recv)(uint8_t), void (*req)());

inline void __attribute__((always_inline)) I2C_transmitByte(uint8_t data)
{
  TWDR = data;
}

ISR(TWI_vect);

#endif
