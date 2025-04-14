#pragma once
//------------------------TFT ST7735--------------------
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h> 
#include <string.h>
//-----------------------Timer2-------------------------
#include <MsTimer2.h>
//-----------------------RC-522-------------------------
#include <MFRC522.h>
//-----------------------DS3231-------------------------
#include <microDS3231.h>
//----------------------Encoder-------------------------
#include <GyverEncoder.h>
//------------------------------------------------------
//------------------------TFT ST7735--------------------
#define TFT_CS     9
#define TFT_RST    7 
#define TFT_DC     8
#define LED_TFT    2
//-----------------------RC-522-------------------------
#define RST_PIN         A0        // Пин rfid модуля RST
#define SS_PIN          A1       // Пин rfid модуля SS
//----------------------Encoder-------------------------
#define CLK 5
#define DT 6
#define SW 4
//----------------------BUZZ----------------------------
#define BZ 3
//----------------------Global_PV-----------------------
volatile uint16_t tictime_msec = 0;
volatile struct
{
  char name_u[6];
  uint8_t namber[4];
}user[4]={{"user1",0,0,0,0},{"user2",0,0,0,0},{"user3",0,0,0,0},{"user4",0,0,0,0}};
volatile uint16_t timer_buzz = 0;
bool flag_BZ_cl = false;
bool flag_BZ_op = false;
//------------------------FUNC--------------------------
void setclock(void);
void menu(void);
void Clock_Set(void);
void Erase_Uid(void);
void Delet_Uid(void);
void Game(void);
