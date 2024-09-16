  /*    I2C:
   * PC0 - SCL 
   * PC1 - SDA
   *    ADC:
   *PA0 - ADC0 - blue - X   
   *PA1 - ADC1 - yellow - Y
   *PA2 - ADC2 - green - Z
   *  button:
   *PB0 - orange - butt1  
   *PB1 - violet - butt2
    white - gnd
  */
  #define DEBUG
  #define I2C_ADDR 0x10
  #include <avr/io.h>
  #include"C:/Users/x/Documents/Arduino/Atmega_16_proba/AT_16_TM0.h"  
  #include"Joostik_h.h"
  //#include <Wire.h>

  data_ic data_ic_buff = {0,0,0,0,0};
  uint8_t *uptr = (uint8_t*)&data_ic_buff;
  
  /*void requestEvent() {
    for(uint8_t i = 0; i < sizeof(data_ic_buff);i++){
    Wire.write(uptr[i]);
    } 
  }*/

  volatile uint8_t data;

  void I2C_received(uint8_t received_data)
  {
    data = received_data;
  }

  void I2C_requested()
  {
    static uint8_t i = 0;
    I2C_transmitByte(uptr[i++]);
    if(i > 4)
           i = 0;
  }

  void vSetup()
{
  // set received/requested callbacks
  I2C_setCallbacks(I2C_received, I2C_requested);

  // init I2C
  I2C_init(I2C_ADDR);
}


 
  int main(){
    #ifdef DEBUG
      Serial.begin(9600);
    #endif
    
    INIT_TM0();
    adc_init();
    button_init();
    vSetup();
    /****************************************/
   /* Wire.begin(I2C_ADDR);                
    Wire.onRequest(requestEvent); */
    /*******************************************/
    FOR{
      data_ic_rl(&data_ic_buff);
      data_ic_buff.u_crc = 0;
      data_ic_buff.u_crc = crc8_asm((byte*)&data_ic_buff, 4);
      #ifdef DEBUG
      for(uint8_t i = 0; i < 5; i++){
        Serial.print(uptr[i]);
        Serial.print(' ');
      }
      Serial.println();
      #endif
      
    }
  }


  
