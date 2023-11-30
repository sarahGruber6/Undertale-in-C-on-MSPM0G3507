/***************************************************
  This is a library for the Adafruit 1.8" SPI display.
  This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
  as well as Adafruit raw 1.8" TFT display
  ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

// ST7735.c
// Runs on MSPM0G3507
// Low level drivers for the ST7735 160x128 LCD based off of
// the file described above.
//    16-bit color, 128 wide by 160 high LCD
// Daniel Valvano
// May 29, 2023
// Augmented 7/17/2014 to have a simple graphics facility
// Tested with LaunchPadDLL.dll simulator 9/2/2014
// Modified for use of CS (simultaneous with other SPI slaves) 3/28/2016

/*

 Copyright 2023 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// hardware connections
// **********ST7735 TFT and SDC*******************
// ST7735
// Backlight (pin 10)     to +3.3 V
// MISO (pin 9) (NC)
// SCK (pin 8)            to SPI1 SCLK: PB9
// MOSI (pin 7)           to SPI1 PICO: PB8
// TFT_CS (pin 6)         to SPI1 CS0:  PB6
// CARD_CS (pin 5) (NC)
// Data/Command (pin 4)   to PA13 (GPIO), high for data, low for command
// RESET (pin 3)          to PB15 (GPIO)
// VCC (pin 2)            to +3.3 V
// Gnd (pin 1)            to ground

// **********wide.hk ST7735R with ADXL345 accelerometer *******************
// Silkscreen Label (SDC side up; LCD side down) - Connection
// VCC  - +3.3 V
// GND  - Ground
// !SCL - SPI1 SCLK: PB9 clock
// !SDA - SPI1 PICO: PB8 MOSI SPI data from microcontroller to TFT or SDC
// DC   - GPIO       PA13 TFT data/command
// RES  - GPIO       PB15 TFT reset
// CS   - SPI1 CS0:  PB6 TFT_CS, active low to enable TFT
// *CS  - (NC) SDC_CS, active low to enable SDC
// MISO - (NC) MISO SPI data from SDC to microcontroller
// SDA  – (NC) I2C data for ADXL345 accelerometer
// SCL  – (NC) I2C clock for ADXL345 accelerometer
// SDO  – (NC) I2C alternate address for ADXL345 accelerometer
// Backlight + - Light, backlight connected to +3.3 V

// **********wide.hk ST7735R with ADXL335 accelerometer *******************
// Silkscreen Label (SDC side up; LCD side down) - Connection
// VCC  - +3.3 V
// GND  - Ground
// !SCL - SPI1 SCLK: PB9
// !SDA - SPI1 PICO: PB8 MOSI SPI data from microcontroller to TFT or SDC
// DC   - GPIO       PA13 TFT data/command
// RES  - GPIO       PB15 TFT reset
// CS   - SPI1 CS0:  PB6 TFT_CS, active low to enable TFT
// *CS  - (NC) SDC_CS, active low to enable SDC
// MISO - (NC) MISO SPI data from SDC to microcontroller
// X– (NC) analog input X-axis from ADXL335 accelerometer
// Y– (NC) analog input Y-axis from ADXL335 accelerometer
// Z– (NC) analog input Z-axis from ADXL335 accelerometer
// Backlight + - Light, backlight connected to +3.3 V

// **********HiLetgo ST7735 TFT and SDC (SDC not tested)*******************
// ST7735
// LED-   (pin 16) TFT, to ground
// LED+   (pin 15) TFT, to +3.3 V
// SD_CS  (pin 14) SDC, to chip select
// MOSI   (pin 13) SDC, to MOSI
// MISO   (pin 12) SDC, to MISO
// SCK    (pin 11) SDC, to serial clock
// CS     (pin 10) TFT, to PB6  SPI1 CS0
// SCL    (pin 9)  TFT, to PB9  SPI1 SCLK
// SDA    (pin 8)  TFT, to PB8  MOSI SPI1 PICO
// A0     (pin 7)  TFT, to PA13 Data/Command, high for data, low for command
// RESET  (pin 6)  TFT, to PB15 reset (GPIO), low to reset
// NC     (pins 3,4,5)
// VCC    (pin 2)       to +3.3 V
// GND    (pin 1)       to ground

// Tyenaza Tyenazaqhf72mi9s3
// ST7735
// LED (pin 8)              to +3.3 V
// SCK (pin 7) SCLK         to PB9 SPI1 SCLK
// SDA (pin 6) MOSI,        to PB8 SPI1 PICO
// A0  (pin 5) Data/Command to PA13 (GPIO), high for data, low for command
// RESET (pin 4)            to PB15 (GPIO), low to reset
// CS  (pin 3)              to PB6 SPI1 CS0:
// Gnd (pin 2)              to ground
// VCC (pin 1)              to +3.3 V

#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/SPI.h"
#include "../inc/Clock.h"
#include "file.h"
#include <stdio.h>
#include <stdlib.h>

// 16 rows (0 to 15) and 21 characters (0 to 20)
// Requires (11 + size*size*6*8) bytes of transmission for each character
uint32_t StX=0; // position along the horizontal axis 0 to 20
uint32_t StY=0; // position along the vertical axis 0 to 15
uint16_t StTextColor = ST7735_YELLOW;

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// standard ascii 5x7 font
// originally from glcdfont.c from Adafruit project
#define STANDARD_GALACTIC_ALPHABET 0
static const uint8_t Font[] = {
  0x00, 0x00, 0x00, 0x00, 0x00,  // NUL char =  0
  0x3E, 0x5B, 0x4F, 0x5B, 0x3E,  // SOH char =  1
  0x3E, 0x6B, 0x4F, 0x6B, 0x3E,  // STX char =  2
  0x1C, 0x3E, 0x7C, 0x3E, 0x1C,  // ETX char =  3
  0x18, 0x3C, 0x7E, 0x3C, 0x18,  // EOT char =  4
  0x1C, 0x57, 0x7D, 0x57, 0x1C,  // ENQ char =  5
  0x1C, 0x5E, 0x7F, 0x5E, 0x1C,  // ACK char =  6
  0x00, 0x18, 0x3C, 0x18, 0x00,  // BEL char =  7
  0xFF, 0xE7, 0xC3, 0xE7, 0xFF,  // BS  char =  8
  0x00, 0x18, 0x24, 0x18, 0x00,  // TAB char =  9
  0xFF, 0xE7, 0xDB, 0xE7, 0xFF,  // LF  char =  10
  0x30, 0x48, 0x3A, 0x06, 0x0E,  // VT  char =  11
  0x26, 0x29, 0x79, 0x29, 0x26,  // FF  char =  12
  0x40, 0x7F, 0x05, 0x05, 0x07,  // CR  char =  13
  0x40, 0x7F, 0x05, 0x25, 0x3F,  // SO  char =  14
  0x5A, 0x3C, 0xE7, 0x3C, 0x5A,  // SI  char =  15
  0x7F, 0x3E, 0x1C, 0x1C, 0x08,  // DLE char =  16
  0x08, 0x1C, 0x1C, 0x3E, 0x7F,  // DC1 char =  17
  0x14, 0x22, 0x7F, 0x22, 0x14,  // DC2 char =  18
  0x5F, 0x5F, 0x00, 0x5F, 0x5F,  // DC3 char =  19
  0x06, 0x09, 0x7F, 0x01, 0x7F,  // DC3 char =  20
  0x00, 0x66, 0x89, 0x95, 0x6A,  // NAK char =  21
  0x60, 0x60, 0x60, 0x60, 0x60,  // SYN char =  22
  0x94, 0xA2, 0xFF, 0xA2, 0x94,  // ETB char =  23
  0x08, 0x04, 0x7E, 0x04, 0x08,  // CAN char =  24
  0x10, 0x20, 0x7E, 0x20, 0x10,  // EM  char =  25
  0x08, 0x08, 0x2A, 0x1C, 0x08,  // SUB char =  26
  0x08, 0x1C, 0x2A, 0x08, 0x08,  // ESC char =  27
  0x1E, 0x10, 0x10, 0x10, 0x10,  // FS  char =  28
  0x0C, 0x1E, 0x0C, 0x1E, 0x0C,  // GS  char =  29
  0x30, 0x38, 0x3E, 0x38, 0x30,  // RS  char =  30
  0x06, 0x0E, 0x3E, 0x0E, 0x06,  // US  char =  31
  0x00, 0x00, 0x00, 0x00, 0x00,  // SP  char =  32
  0x00, 0x00, 0x5F, 0x00, 0x00,  // !   char =  33
  0x00, 0x07, 0x00, 0x07, 0x00,  // "   char =  34
  0x14, 0x7F, 0x14, 0x7F, 0x14,  // #   char =  35
  0x24, 0x2A, 0x7F, 0x2A, 0x12,  // $   char =  36
  0x23, 0x13, 0x08, 0x64, 0x62,  // %   char =  37
  0x36, 0x49, 0x56, 0x20, 0x50,  // &   char =  38
  0x00, 0x08, 0x07, 0x03, 0x00,  // '   char =  39
  0x00, 0x1C, 0x22, 0x41, 0x00,  // (   char =  40
  0x00, 0x41, 0x22, 0x1C, 0x00,  // )   char =  41
  0x2A, 0x1C, 0x7F, 0x1C, 0x2A,  // *   char =  42
  0x08, 0x08, 0x3E, 0x08, 0x08,  // +   char =  43
  0x00, 0x80, 0x70, 0x30, 0x00,  // ,   char =  44
  0x08, 0x08, 0x08, 0x08, 0x08,  // -   char =  45
#if STANDARD_GALACTIC_ALPHABET
  32,0,32,0,32,   // 46  dot
#else
  0x00, 0x00, 0x60, 0x60, 0x00,  // .   char =  46
#endif
  0x20, 0x10, 0x08, 0x04, 0x02,  // /  char =  47
  0x3E, 0x51, 0x49, 0x45, 0x3E,  // 0  char =  48
  0x00, 0x42, 0x7F, 0x40, 0x00,  // 1  char =  49
  0x72, 0x49, 0x49, 0x49, 0x46,  // 2  char =  50
  0x21, 0x41, 0x49, 0x4D, 0x33,  // 3  char =  51
  0x18, 0x14, 0x12, 0x7F, 0x10,  // 4  char =  52
  0x27, 0x45, 0x45, 0x45, 0x39,  // 5  char =  53
  0x3C, 0x4A, 0x49, 0x49, 0x31,  // 6  char =  54
  0x41, 0x21, 0x11, 0x09, 0x07,  // 7  char =  55
  0x36, 0x49, 0x49, 0x49, 0x36,  // 8  char =  56
  0x46, 0x49, 0x49, 0x29, 0x1E,  // 9  char =  57
  0x00, 0x00, 0x14, 0x00, 0x00,  // :  char =  58
  0x00, 0x40, 0x34, 0x00, 0x00,  // ;  char =  59
  0x00, 0x08, 0x14, 0x22, 0x41,  // <  char =  60
  0x14, 0x14, 0x14, 0x14, 0x14,  // =  char =  61
  0x00, 0x41, 0x22, 0x14, 0x08,  // >  char =  62
  0x02, 0x01, 0x59, 0x09, 0x06,  // ?  char =  63
  0x3E, 0x41, 0x5D, 0x59, 0x4E,  // @  char =  64
#if STANDARD_GALACTIC_ALPHABET
  64,126,1,1,2,     // 65  A
  64,64,79,80,96,   // 66  B
  0,29,112,0,0,     // 67  C
  10,10,18,34,34,   // 68  D
  127,64,64,64,65,  // 69  E
  20,4,20,4,20,     // 70  F
  0,8,8,127,0,      // 71  G
  5,5,125,5,5,      // 72  H
  0,0,119,0,0,      // 73  I
  0,0,107,0,0,      // 74  J
  8,0,127,0,8,      // 75  K
  0,126,0,36,0,     // 76  L
  66,64,64,64,126,  // 77  M
  0,67,32,24,7,     // 78  N
  0,65,33,25,7,     // 79  O
  0,122,0,94,0,     // 80  P
  36,36,37,36,60,   // 81  Q
  0,66,0,0,66,      // 82  R
  0,15,120,0,0,     // 83  S
  0,2,2,2,94,       // 84  T
  16,20,16,20,16,   // 85  U
  80,80,95,80,80,   // 86  V
  8,0,2,0,8,        // 87  W
  65,48,8,6,1,      // 88  X
  0,126,0,126,0,    // 89  Y
  124,2,1,2,124,    // 90  Z
#else
  0x7C, 0x12, 0x11, 0x12, 0x7C,  // A  char =  65
  0x7F, 0x49, 0x49, 0x49, 0x36,  // B  char =  66
  0x3E, 0x41, 0x41, 0x41, 0x22,  // C  char =  67
  0x7F, 0x41, 0x41, 0x41, 0x3E,  // D  char =  68
  0x7F, 0x49, 0x49, 0x49, 0x41,  // E  char =  69
  0x7F, 0x09, 0x09, 0x09, 0x01,  // F  char =  70
  0x3E, 0x41, 0x41, 0x51, 0x73,  // G  char =  71
  0x7F, 0x08, 0x08, 0x08, 0x7F,  // H  char =  72
  0x00, 0x41, 0x7F, 0x41, 0x00,  // I  char =  73
  0x20, 0x40, 0x41, 0x3F, 0x01,  // J  char =  74
  0x7F, 0x08, 0x14, 0x22, 0x41,  // K  char =  75
  0x7F, 0x40, 0x40, 0x40, 0x40,  // L  char =  76
  0x7F, 0x02, 0x1C, 0x02, 0x7F,  // M  char =  77
  0x7F, 0x04, 0x08, 0x10, 0x7F,  // N  char =  78
  0x3E, 0x41, 0x41, 0x41, 0x3E,  // O  char =  79
  0x7F, 0x09, 0x09, 0x09, 0x06,  // P  char =  80
  0x3E, 0x41, 0x51, 0x21, 0x5E,  // Q  char =  81
  0x7F, 0x09, 0x19, 0x29, 0x46,  // R  char =  82
  0x26, 0x49, 0x49, 0x49, 0x32,  // S  char =  83
  0x03, 0x01, 0x7F, 0x01, 0x03,  // T  char =  84
  0x3F, 0x40, 0x40, 0x40, 0x3F,  // U  char =  85
  0x1F, 0x20, 0x40, 0x20, 0x1F,  // V  char =  86
  0x3F, 0x40, 0x38, 0x40, 0x3F,  // W  char =  87
  0x63, 0x14, 0x08, 0x14, 0x63,  // X  char =  88
  0x03, 0x04, 0x78, 0x04, 0x03,  // Y  char =  89
  0x61, 0x59, 0x49, 0x4D, 0x43,  // Z  char =  90
#endif
  0x00, 0x7F, 0x41, 0x41, 0x41,  // [  char =  91
  0x02, 0x04, 0x08, 0x10, 0x20,  // \  char =  92
  0x00, 0x41, 0x41, 0x41, 0x7F,  // ]  char =  93
  0x04, 0x02, 0x01, 0x02, 0x04,  // ^  char =  94
  0x40, 0x40, 0x40, 0x40, 0x40,  // _  char =  95
  0x00, 0x03, 0x07, 0x08, 0x00,  // `  char =  96
#if STANDARD_GALACTIC_ALPHABET
  64,126,1,1,2,     // 65  A
  64,64,79,80,96,   // 66  B
  0,29,112,0,0,     // 67  C
  10,10,18,34,34,   // 68  D
  127,64,64,64,65,  // 69  E
  20,4,20,4,20,     // 70  F
  0,8,8,127,0,      // 71  G
  5,5,125,5,5,      // 72  H
  0,0,119,0,0,      // 73  I
  0,0,107,0,0,      // 74  J
  8,0,127,0,8,      // 75  K
  0,126,0,36,0,     // 76  L
  66,64,64,64,126,  // 77  M
  0,67,32,24,7,     // 78  N
  0,65,33,25,7,     // 79  O
  0,122,0,94,0,     // 80  P
  36,36,37,36,60,   // 81  Q
  0,66,0,0,66,      // 82  R
  0,15,120,0,0,     // 83  S
  0,2,2,2,94,       // 84  T
  16,20,16,20,16,   // 85  U
  80,80,95,80,80,   // 86  V
  8,0,2,0,8,        // 87  W
  65,48,8,6,1,      // 88  X
  0,126,0,126,0,    // 89  Y
  124,2,1,2,124,    // 90  Z
#else
  0x20, 0x54, 0x54, 0x78, 0x40,  // a  char =  97
  0x7F, 0x28, 0x44, 0x44, 0x38,  // b  char =  98
  0x38, 0x44, 0x44, 0x44, 0x28,  // c  char =  99
  0x38, 0x44, 0x44, 0x28, 0x7F,  // d  char =  100
  0x38, 0x54, 0x54, 0x54, 0x18,  // e  char =  101
  0x00, 0x08, 0x7E, 0x09, 0x02,  // f  char =  102
  0x18, 0xA4, 0xA4, 0x9C, 0x78,  // g  char =  103
  0x7F, 0x08, 0x04, 0x04, 0x78,  // h  char =  104
  0x00, 0x44, 0x7D, 0x40, 0x00,  // i  char =  105
  0x20, 0x40, 0x40, 0x3D, 0x00,  // j  char =  106
  0x7F, 0x10, 0x28, 0x44, 0x00,  // k  char =  107
  0x00, 0x41, 0x7F, 0x40, 0x00,  // l  char =  108
  0x7C, 0x04, 0x78, 0x04, 0x78,  // m  char =  109
  0x7C, 0x08, 0x04, 0x04, 0x78,  // n  char =  110
  0x38, 0x44, 0x44, 0x44, 0x38,  // o  char =  111
  0xFC, 0x18, 0x24, 0x24, 0x18,  // p  char =  112
  0x18, 0x24, 0x24, 0x18, 0xFC,  // q  char =  113
  0x7C, 0x08, 0x04, 0x04, 0x08,  // r  char =  114
  0x48, 0x54, 0x54, 0x54, 0x24,  // s  char =  115
  0x04, 0x04, 0x3F, 0x44, 0x24,  // t  char =  116
  0x3C, 0x40, 0x40, 0x20, 0x7C,  // u  char =  117
  0x1C, 0x20, 0x40, 0x20, 0x1C,  // v  char =  118
  0x3C, 0x40, 0x30, 0x40, 0x3C,  // w  char =  119
  0x44, 0x28, 0x10, 0x28, 0x44,  // x  char =  120
  0x4C, 0x90, 0x90, 0x90, 0x7C,  // y  char =  121
  0x44, 0x64, 0x54, 0x4C, 0x44,  // z  char =  122
#endif
  0x00, 0x08, 0x36, 0x41, 0x00,  // {  char =  123
  0x00, 0x00, 0x77, 0x00, 0x00,  // |  char =  124
  0x00, 0x41, 0x36, 0x08, 0x00,  // }  char =  125
  0x02, 0x01, 0x02, 0x04, 0x02,  // ~  char =  126
  0x3C, 0x26, 0x23, 0x26, 0x3C,  // DEL char =  127
  // for extended ASCII, see ExtendedASCII.png
  0x1E, 0xA1, 0xA1, 0x61, 0x12,  // char =  128
  0x3A, 0x40, 0x40, 0x20, 0x7A,  // char =  129
  0x38, 0x54, 0x54, 0x55, 0x59,  // char =  130
  0x21, 0x55, 0x55, 0x79, 0x41,  // char =  131
  0x21, 0x54, 0x54, 0x78, 0x41,  // char =  132
  0x21, 0x55, 0x54, 0x78, 0x40,  // char =  133
  0x20, 0x54, 0x55, 0x79, 0x40,  // char =  134
  0x0C, 0x1E, 0x52, 0x72, 0x12,  // char =  135
  0x39, 0x55, 0x55, 0x55, 0x59,  // char =  136
  0x39, 0x54, 0x54, 0x54, 0x59,  // char =  137
  0x39, 0x55, 0x54, 0x54, 0x58,  // char =  138
  0x00, 0x00, 0x45, 0x7C, 0x41,  // char =  139
  0x00, 0x02, 0x45, 0x7D, 0x42,  // char =  140
  0x00, 0x01, 0x45, 0x7C, 0x40,  // char =  141
  0xF0, 0x29, 0x24, 0x29, 0xF0,  // char =  142
  0xF0, 0x28, 0x25, 0x28, 0xF0,  // char =  143
  0x7C, 0x54, 0x55, 0x45, 0x00,  // char =  144
  0x20, 0x54, 0x54, 0x7C, 0x54,  // char =  145
  0x7C, 0x0A, 0x09, 0x7F, 0x49,  // char =  146
  0x32, 0x49, 0x49, 0x49, 0x32,  // char =  147
  0x32, 0x48, 0x48, 0x48, 0x32,  // char =  148
  0x32, 0x4A, 0x48, 0x48, 0x30,  // char =  149
  0x3A, 0x41, 0x41, 0x21, 0x7A,  // char =  150
  0x3A, 0x42, 0x40, 0x20, 0x78,  // char =  151
  0x00, 0x9D, 0xA0, 0xA0, 0x7D,  // char =  152
  0x39, 0x44, 0x44, 0x44, 0x39,  // char =  153
  0x3D, 0x40, 0x40, 0x40, 0x3D,  // char =  154
  0x3C, 0x24, 0xFF, 0x24, 0x24,  // char =  155
  0x48, 0x7E, 0x49, 0x43, 0x66,  // char =  156
  0x2B, 0x2F, 0xFC, 0x2F, 0x2B,  // char =  157
  0xFF, 0x09, 0x29, 0xF6, 0x20,  // char =  158
  0xC0, 0x88, 0x7E, 0x09, 0x03,  // char =  159
  0x20, 0x54, 0x54, 0x79, 0x41,  // char =  160
  0x00, 0x00, 0x44, 0x7D, 0x41,  // char =  161
  0x30, 0x48, 0x48, 0x4A, 0x32,  // char =  162
  0x38, 0x40, 0x40, 0x22, 0x7A,  // char =  163
  0x00, 0x7A, 0x0A, 0x0A, 0x72,  // char =  164
  0x7D, 0x0D, 0x19, 0x31, 0x7D,  // char =  165
  0x26, 0x29, 0x29, 0x2F, 0x28,  // char =  166
  0x26, 0x29, 0x29, 0x29, 0x26,  // char =  167
  0x30, 0x48, 0x4D, 0x40, 0x20,  // char =  168
  0x38, 0x08, 0x08, 0x08, 0x08,  // char =  169
  0x08, 0x08, 0x08, 0x08, 0x38,  // char =  170
  0x2F, 0x10, 0xC8, 0xAC, 0xBA,  // char =  171
  0x2F, 0x10, 0x28, 0x34, 0xFA,  // char =  172
  0x00, 0x00, 0x7B, 0x00, 0x00,  // char =  173
  0x08, 0x14, 0x2A, 0x14, 0x22,  // char =  174
  0x22, 0x14, 0x2A, 0x14, 0x08,  // char =  175
  0xAA, 0x00, 0x55, 0x00, 0xAA,  // char =  176
  0xAA, 0x55, 0xAA, 0x55, 0xAA,  // char =  177
  0x00, 0x00, 0x00, 0xFF, 0x00,  // char =  178
  0x10, 0x10, 0x10, 0xFF, 0x00,  // char =  179
  0x14, 0x14, 0x14, 0xFF, 0x00,  // char =  180
  0x10, 0x10, 0xFF, 0x00, 0xFF,  // char =  181
  0x10, 0x10, 0xF0, 0x10, 0xF0,  // char =  182
  0x14, 0x14, 0x14, 0xFC, 0x00,  // char =  183
  0x14, 0x14, 0xF7, 0x00, 0xFF,  // char =  184
  0x00, 0x00, 0xFF, 0x00, 0xFF,  // char =  185
  0x14, 0x14, 0xF4, 0x04, 0xFC,  // char =  186
  0x14, 0x14, 0x17, 0x10, 0x1F,  // char =  187
  0x10, 0x10, 0x1F, 0x10, 0x1F,  // char =  188
  0x14, 0x14, 0x14, 0x1F, 0x00,  // char =  189
  0x10, 0x10, 0x10, 0xF0, 0x00,  // char =  190
  0x00, 0x00, 0x00, 0x1F, 0x10,  // char =  191
  0x10, 0x10, 0x10, 0x1F, 0x10,  // char =  192
  0x10, 0x10, 0x10, 0xF0, 0x10,  // char =  193
  0x00, 0x00, 0x00, 0xFF, 0x10,  // char =  194
  0x10, 0x10, 0x10, 0x10, 0x10,  // char =  195
  0x10, 0x10, 0x10, 0xFF, 0x10,  // char =  196
  0x00, 0x00, 0x00, 0xFF, 0x14,  // char =  197
  0x00, 0x00, 0xFF, 0x00, 0xFF,  // char =  198
  0x00, 0x00, 0x1F, 0x10, 0x17,  // char =  199
  0x00, 0x00, 0xFC, 0x04, 0xF4,  // char =  200
  0x14, 0x14, 0x17, 0x10, 0x17,  // char =  201
  0x14, 0x14, 0xF4, 0x04, 0xF4,  // char =  202
  0x00, 0x00, 0xFF, 0x00, 0xF7,  // char =  203
  0x14, 0x14, 0x14, 0x14, 0x14,  // char =  204
  0x14, 0x14, 0xF7, 0x00, 0xF7,  // char =  205
  0x14, 0x14, 0x14, 0x17, 0x14,  // char =  206
  0x10, 0x10, 0x1F, 0x10, 0x1F,  // char =  207
  0x14, 0x14, 0x14, 0xF4, 0x14,  // char =  208
  0x10, 0x10, 0xF0, 0x10, 0xF0,  // char =  209
  0x00, 0x00, 0x1F, 0x10, 0x1F,  // char =  210
  0x00, 0x00, 0x00, 0x1F, 0x14,  // char =  211
  0x00, 0x00, 0x00, 0xFC, 0x14,  // char =  212
  0x00, 0x00, 0xF0, 0x10, 0xF0,  // char =  213
  0x10, 0x10, 0xFF, 0x10, 0xFF,  // char =  214
  0x14, 0x14, 0x14, 0xFF, 0x14,  // char =  215
  0x10, 0x10, 0x10, 0x1F, 0x00,  // char =  216
  0x00, 0x00, 0x00, 0xF0, 0x10,  // char =  217
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // char =  218
  0xF0, 0xF0, 0xF0, 0xF0, 0xF0,  // char =  219
  0xFF, 0xFF, 0xFF, 0x00, 0x00,  // char =  220
  0x00, 0x00, 0x00, 0xFF, 0xFF,  // char =  221
  0x0F, 0x0F, 0x0F, 0x0F, 0x0F,  // char =  222
  0x38, 0x44, 0x44, 0x38, 0x44,  // char =  223
  0x7C, 0x2A, 0x2A, 0x3E, 0x14,  // char =  224
  0x7E, 0x02, 0x02, 0x06, 0x06,  // char =  225
  0x02, 0x7E, 0x02, 0x7E, 0x02,  // char =  226
  0x63, 0x55, 0x49, 0x41, 0x63,  // char =  227
  0x38, 0x44, 0x44, 0x3C, 0x04,  // char =  228
  0x40, 0x7E, 0x20, 0x1E, 0x20,  // char =  229
  0x06, 0x02, 0x7E, 0x02, 0x02,  // char =  230
  0x99, 0xA5, 0xE7, 0xA5, 0x99,  // char =  231
  0x1C, 0x2A, 0x49, 0x2A, 0x1C,  // char =  232
  0x4C, 0x72, 0x01, 0x72, 0x4C,  // char =  233
  0x30, 0x4A, 0x4D, 0x4D, 0x30,  // char =  234
  0x30, 0x48, 0x78, 0x48, 0x30,  // char =  235
  0xBC, 0x62, 0x5A, 0x46, 0x3D,  // char =  236
  0x3E, 0x49, 0x49, 0x49, 0x00,  // char =  237
  0x7E, 0x01, 0x01, 0x01, 0x7E,  // char =  238
  0x2A, 0x2A, 0x2A, 0x2A, 0x2A,  // char =  239
  0x44, 0x44, 0x5F, 0x44, 0x44,  // char =  240
  0x40, 0x51, 0x4A, 0x44, 0x40,  // char =  241
  0x40, 0x44, 0x4A, 0x51, 0x40,  // char =  242
  0x00, 0x00, 0xFF, 0x01, 0x03,  // char =  243
  0xE0, 0x80, 0xFF, 0x00, 0x00,  // char =  244
  0x08, 0x08, 0x6B, 0x6B, 0x08,  // char =  245
  0x36, 0x12, 0x36, 0x24, 0x36,  // char =  246
  0x06, 0x0F, 0x09, 0x0F, 0x06,  // char =  247
  0x00, 0x00, 0x18, 0x18, 0x00,  // char =  248
  0x00, 0x00, 0x10, 0x10, 0x00,  // char =  249
  0x30, 0x40, 0xFF, 0x01, 0x01,  // char =  250
  0x00, 0x1F, 0x01, 0x01, 0x1E,  // char =  251
  0x00, 0x19, 0x1D, 0x17, 0x12,  // char =  252
  0x00, 0x3C, 0x3C, 0x3C, 0x3C,  // char =  253
  0x00, 0x00, 0x00, 0x00, 0x00,  // char =  254

};


static uint8_t ColStart, RowStart; // some displays need this changed
static uint8_t Rotation;           // 0 to 3
static enum initRFlags TabColor;
static int16_t _width = ST7735_TFTWIDTH;   // this could probably be a constant, except it is used in Adafruit_GFX and depends on image rotation
static int16_t _height = ST7735_TFTHEIGHT;





// Rather than a bazillion SPI_OutCommand() and SPI_OutData() calls, screen
// initialization commands and arguments are organized in these tables
// stored in ROM.  The table may look bulky, but that's mostly the
// formatting -- storage-wise this is hundreds of bytes more compact
// than the equivalent code.  Companion function follows.
#define DELAY 0x80
static const uint8_t
  Bcmd[] = {                  // Initialization commands for 7735B screens
    18,                       // 18 commands in list:
    ST7735_SWRESET,   DELAY,  //  1: Software reset, no args, w/delay
      50,                     //     50 ms delay
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, no args, w/delay
      255,                    //     255 = 500 ms delay
    ST7735_COLMOD , 1+DELAY,  //  3: Set color mode, 1 arg + delay:
      0x05,                   //     16-bit color
      10,                     //     10 ms delay
    ST7735_FRMCTR1, 3+DELAY,  //  4: Frame rate control, 3 args + delay:
      0x00,                   //     fastest refresh
      0x06,                   //     6 lines front porch
      0x03,                   //     3 lines back porch
      10,                     //     10 ms delay
    ST7735_MADCTL , 1      ,  //  5: Memory access ctrl (directions), 1 arg:
      0x08,                   //     Row addr/col addr, bottom to top refresh
    ST7735_DISSET5, 2      ,  //  6: Display settings #5, 2 args, no delay:
      0x15,                   //     1 clk cycle nonoverlap, 2 cycle gate
                              //     rise, 3 cycle osc equalize
      0x02,                   //     Fix on VTL
    ST7735_INVCTR , 1      ,  //  7: Display inversion control, 1 arg:
      0x0,                    //     Line inversion
    ST7735_PWCTR1 , 2+DELAY,  //  8: Power control, 2 args + delay:
      0x02,                   //     GVDD = 4.7V
      0x70,                   //     1.0uA
      10,                     //     10 ms delay
    ST7735_PWCTR2 , 1      ,  //  9: Power control, 1 arg, no delay:
      0x05,                   //     VGH = 14.7V, VGL = -7.35V
    ST7735_PWCTR3 , 2      ,  // 10: Power control, 2 args, no delay:
      0x01,                   //     Opamp current small
      0x02,                   //     Boost frequency
    ST7735_VMCTR1 , 2+DELAY,  // 11: Power control, 2 args + delay:
      0x3C,                   //     VCOMH = 4V
      0x38,                   //     VCOML = -1.1V
      10,                     //     10 ms delay
    ST7735_PWCTR6 , 2      ,  // 12: Power control, 2 args, no delay:
      0x11, 0x15,
    ST7735_GMCTRP1,16      ,  // 13: Magical unicorn dust, 16 args, no delay:
      0x09, 0x16, 0x09, 0x20, //     (seriously though, not sure what
      0x21, 0x1B, 0x13, 0x19, //      these config values represent)
      0x17, 0x15, 0x1E, 0x2B,
      0x04, 0x05, 0x02, 0x0E,
    ST7735_GMCTRN1,16+DELAY,  // 14: Sparkles and rainbows, 16 args + delay:
      0x0B, 0x14, 0x08, 0x1E, //     (ditto)
      0x22, 0x1D, 0x18, 0x1E,
      0x1B, 0x1A, 0x24, 0x2B,
      0x06, 0x06, 0x02, 0x0F,
      10,                     //     10 ms delay
    ST7735_CASET  , 4      ,  // 15: Column addr set, 4 args, no delay:
      0x00, 0x02,             //     XSTART = 2
      0x00, 0x81,             //     XEND = 129
    ST7735_RASET  , 4      ,  // 16: Row addr set, 4 args, no delay:
      0x00, 0x02,             //     XSTART = 1
      0x00, 0x81,             //     XEND = 160
    ST7735_NORON  ,   DELAY,  // 17: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735_DISPON ,   DELAY,  // 18: Main screen turn on, no args, w/delay
      255 };                  //     255 = 500 ms delay
static const uint8_t
  Rcmd1[] = {                 // Init for 7735R, part 1 (red or green tab)
    15,                       // 15 commands in list:
    ST7735_SWRESET,   DELAY,  //  1: Software reset, 0 args, w/delay
      150,                    //     150 ms delay
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, 0 args, w/delay
      255,                    //     500 ms delay
    ST7735_FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
      0x01, 0x2C, 0x2D,       //     Dot inversion mode
      0x01, 0x2C, 0x2D,       //     Line inversion mode
    ST7735_INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
      0x07,                   //     No inversion
    ST7735_PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                   //     -4.6V
      0x84,                   //     AUTO mode
    ST7735_PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
      0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735_PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
      0x0A,                   //     Opamp current small
      0x00,                   //     Boost frequency
    ST7735_PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
      0x8A,                   //     BCLK/2, Opamp current small & Medium low
      0x2A,
    ST7735_PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735_VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
      0x0E,
    ST7735_INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
    ST7735_MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
      0xC8,                   //     row addr/col addr, bottom to top refresh
    ST7735_COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
      0x05 };                 //     16-bit color
static const uint8_t
  Rcmd2green[] = {            // Init for 7735R, part 2 (green tab only)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x02,             //     XSTART = 0
      0x00, 0x7F+0x02,        //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x01,             //     XSTART = 0
      0x00, 0x9F+0x01 };      //     XEND = 159
static const uint8_t
  Rcmd2red[] = {              // Init for 7735R, part 2 (red tab only)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F,             //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x9F };           //     XEND = 159
static const uint8_t
  Rcmd3[] = {                 // Init for 7735R, part 3 (red or green tab)
    4,                        //  4 commands in list:
    ST7735_GMCTRP1, 16      , //  1: Magical unicorn dust, 16 args, no delay:
      0x02, 0x1c, 0x07, 0x12,
      0x37, 0x32, 0x29, 0x2d,
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      , //  2: Sparkles and rainbows, 16 args, no delay:
      0x03, 0x1d, 0x07, 0x06,
      0x2E, 0x2C, 0x29, 0x2D,
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST7735_NORON  ,    DELAY, //  3: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735_DISPON ,    DELAY, //  4: Main screen turn on, no args w/delay
      100 };                  //     100 ms delay


// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in ROM byte array.
void static commandList(const uint8_t *addr) {

  uint8_t numCommands, numArgs;
  uint16_t ms;

  numCommands = *(addr++);               // Number of commands to follow
  while(numCommands--) {                 // For each command...
    SPI_OutCommand(*(addr++));             //   Read, issue command
    numArgs  = *(addr++);                //   Number of args to follow
    ms       = numArgs & DELAY;          //   If hibit set, delay follows args
    numArgs &= ~DELAY;                   //   Mask out delay bit
    while(numArgs--) {                   //   For each argument...
      SPI_OutData(*(addr++));              //     Read, issue argument
    }

    if(ms) {
      ms = *(addr++);             // Read post-command delay time (ms)
      if(ms == 255) ms = 500;     // If 255, delay for 500 ms
      Clock_Delay1ms(ms);
    }
  }
}


// Initialization code common to both 'B' and 'R' type displays
void static commonInit(const uint8_t *cmdList) {
  ColStart  = RowStart = 0; // May be overridden in init func
  SPI_Init(); // 4MHz clock
  if(cmdList) commandList(cmdList);
}


//------------ST7735_InitB------------
// Initialization for ST7735B screens.
// Input: none
// Output: none
void ST7735_InitB(void) {
  commonInit(Bcmd);
  ST7735_SetCursor(0,0);
  StTextColor = ST7735_YELLOW;
  ST7735_FillScreen(0);                 // set screen to black
}


//------------ST7735_InitR------------
// Initialization for ST7735R screens (green or red tabs).
// Input: option one of the enumerated options depending on tabs
// Output: none
void ST7735_InitR(enum initRFlags option) {
  commonInit(Rcmd1);
  if(option == INITR_GREENTAB) {
    commandList(Rcmd2green);
    ColStart = 2;
    RowStart = 1;
  } else {
    // colstart, rowstart left at default '0' values
    commandList(Rcmd2red);
  }
  commandList(Rcmd3);

  // if black, change MADCTL color filter
  if (option == INITR_BLACKTAB) {
    SPI_OutCommand(ST7735_MADCTL);
    SPI_OutData(0xC0);
  }
  TabColor = option;
  ST7735_SetCursor(0,0);
  StTextColor = ST7735_YELLOW;
  ST7735_FillScreen(0);                 // set screen to black
}


// Set the region of the screen RAM to be modified
// Pixel colors are sent left to right, top to bottom
// (same as Font table is encoded; different from regular bitmap)
// Requires 11 bytes of transmission
void static setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {

  SPI_OutCommand(ST7735_CASET); // Column addr set
  SPI_OutData(0x00);
  SPI_OutData(x0+ColStart);     // XSTART
  SPI_OutData(0x00);
  SPI_OutData(x1+ColStart);     // XEND

  SPI_OutCommand(ST7735_RASET); // Row addr set
  SPI_OutData(0x00);
  SPI_OutData(y0+RowStart);     // YSTART
  SPI_OutData(0x00);
  SPI_OutData(y1+RowStart);     // YEND

  SPI_OutCommand(ST7735_RAMWR); // write to RAM
}


// Send two bytes of data, most significant byte first
// Requires 2 bytes of transmission
void static pushColor(uint16_t color) {
  SPI_OutData((uint8_t)(color >> 8));
  SPI_OutData((uint8_t)color);
}


//------------ST7735_DrawPixel------------
// Color the pixel at the given coordinates with the given color.
// Requires 13 bytes of transmission
// Input: x     horizontal position of the pixel, columns from the left edge
//               must be less than 128
//               0 is on the left, 126 is near the right
//        y     vertical position of the pixel, rows from the top edge
//               must be less than 160
//               159 is near the wires, 0 is the side opposite the wires
//        color 16-bit color, which can be produced by ST7735_Color565()
// Output: none
void ST7735_DrawPixel(int16_t x, int16_t y, uint16_t color) {

  if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;

//  setAddrWindow(x,y,x+1,y+1); // original code, bug???
  setAddrWindow(x,y,x,y);

  pushColor(color);

  //deselect();
}


//------------ST7735_DrawFastVLine------------
// Draw a vertical line at the given coordinates with the given height and color.
// A vertical line is parallel to the longer side of the rectangular display
// Requires (11 + 2*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the start of the line, columns from the left edge
//        y     vertical position of the start of the line, rows from the top edge
//        h     vertical height of the line
//        color 16-bit color, which can be produced by ST7735_Color565()
// Output: none
void ST7735_DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  uint8_t hi = color >> 8, lo = color;

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((y+h-1) >= _height) h = _height-y;
  setAddrWindow(x, y, x, y+h-1);

  while (h--) {
    SPI_OutData(hi);
    SPI_OutData(lo);
  }

 // deselect();
}


//------------ST7735_DrawFastHLine------------
// Draw a horizontal line at the given coordinates with the given width and color.
// A horizontal line is parallel to the shorter side of the rectangular display
// Requires (11 + 2*w) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the start of the line, columns from the left edge
//        y     vertical position of the start of the line, rows from the top edge
//        w     horizontal width of the line
//        color 16-bit color, which can be produced by ST7735_Color565()
// Output: none
void ST7735_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  uint8_t hi = color >> 8, lo = color;

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((x+w-1) >= _width)  w = _width-x;
  setAddrWindow(x, y, x+w-1, y);

  while (w--) {
    SPI_OutData(hi);
    SPI_OutData(lo);
  }

  //deselect();
}


//------------ST7735_FillScreen------------
// Fill the screen with the given color.
// Requires 40,971 bytes of transmission
// Input: color 16-bit color, which can be produced by ST7735_Color565()
// Output: none
void ST7735_FillScreen(uint16_t color) {
  ST7735_FillRect(0, 0, _width, _height, color);  // original
//  screen is actually 129 by 161 pixels, x 0 to 128, y goes from 0 to 160
}


//------------ST7735_FillRect------------
// Draw a filled rectangle at the given coordinates with the given width, height, and color.
// Requires (11 + 2*w*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the top left corner of the rectangle, columns from the left edge
//        y     vertical position of the top left corner of the rectangle, rows from the top edge
//        w     horizontal width of the rectangle
//        h     vertical height of the rectangle
//        color 16-bit color, which can be produced by ST7735_Color565()
// Output: none
void ST7735_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  uint8_t hi = color >> 8, lo = color;

  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= _width) || (y >= _height)) return;
  if((x + w - 1) >= _width)  w = _width  - x;
  if((y + h - 1) >= _height) h = _height - y;

  setAddrWindow(x, y, x+w-1, y+h-1);

  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
      SPI_OutData(hi);
      SPI_OutData(lo);
    }
  }

 // deselect();
}

//------------ST7735_DrawSmallCircle------------
// Draw a small circle (diameter of 6 pixels)
// rectangle at the given coordinates with the given color.
// Requires (11*6+24*2)=114 bytes of transmission (assuming image on screen)
// Input: x     horizontal position of the top left corner of the circle, columns from the left edge
//        y     vertical position of the top left corner of the circle, rows from the top edge
//        color 16-bit color, which can be produced by ST7735_Color565()
// Output: none
int16_t const smallCircle[6][3]={
    {2,3,    2},
  {1  ,  4,  4},
 {0   ,   5, 6},
 {0   ,   5, 6},
  {1  ,  4,  4},
    {2,3,    2}};
void ST7735_DrawSmallCircle(int16_t x, int16_t y, uint16_t color) {
  uint32_t i,w;
  uint8_t hi = color >> 8, lo = color;
  // rudimentary clipping 
  if((x>_width-5)||(y>_height-5)) return; // doesn't fit
  for(i=0; i<6; i++){
    setAddrWindow(x+smallCircle[i][0], y+i, x+smallCircle[i][1], y+i);
    w = smallCircle[i][2];
    while (w--) {
      SPI_OutData(hi);
      SPI_OutData(lo);
    }
  }
 // deselect();
}
//------------ST7735_DrawCircle------------
// Draw a small circle (diameter of 10 pixels)
// rectangle at the given coordinates with the given color.
// Requires (11*10+68*2)=178 bytes of transmission (assuming image on screen)
// Input: x     horizontal position of the top left corner of the circle, columns from the left edge
//        y     vertical position of the top left corner of the circle, rows from the top edge
//        color 16-bit color, which can be produced by ST7735_Color565()
// Output: none
int16_t const circle[10][3]={
      { 4,5,         2},
    {2   ,   7,      6},
  {1     ,      8,   8},
  {1     ,      8,   8},
 {0      ,       9, 10},
 {0      ,       9, 10},
  {1     ,      8,   8},
  {1     ,      8,   8},
    {2   ,    7,     6},
     {  4,5,         2}};
void ST7735_DrawCircle(int16_t x, int16_t y, uint16_t color) {
  uint32_t i,w;
  uint8_t hi = color >> 8, lo = color;
  // rudimentary clipping 
  if((x>_width-9)||(y>_height-9)) return; // doesn't fit
  for(i=0; i<10; i++){
    setAddrWindow(x+circle[i][0], y+i, x+circle[i][1], y+i);
    w = circle[i][2];
    while (w--) {
      SPI_OutData(hi);
      SPI_OutData(lo);
    }
  }
 // deselect();
}

//------------ST7735_Color565------------
// Pass 8-bit (each) R,G,B and get back 16-bit packed color.
// Input: r red value
//        g green value
//        b blue value
// Output: 16-bit color
uint16_t ST7735_Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3);
}


//------------ST7735_SwapColor------------
// Swaps the red and blue values of the given 16-bit packed color;
// green is unchanged.
// Input: x 16-bit color in format B, G, R
// Output: 16-bit color in format R, G, B
uint16_t ST7735_SwapColor(uint16_t x) {
  return (x << 11) | (x & 0x07E0) | (x >> 11);
}


//------------ST7735_DrawBitmap------------
// Displays a 16-bit color BMP image.  A bitmap file that is created
// by a PC image processing program has a header and may be padded
// with dummy columns so the data have four byte alignment.  This
// function assumes that all of that has been stripped out, and the
// array image[] has one 16-bit halfword for each pixel to be
// displayed on the screen (encoded in reverse order, which is
// standard for bitmap files).  An array can be created in this
// format from a 24-bit-per-pixel .bmp file using the associated
// converter program.
// (x,y) is the screen location of the lower left corner of BMP image
// Requires (11 + 2*w*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the bottom left corner of the image, columns from the left edge
//        y     vertical position of the bottom left corner of the image, rows from the top edge
//        image pointer to a 16-bit color BMP image
//        w     number of pixels wide
//        h     number of pixels tall
// Output: none
// Must be less than or equal to 128 pixels wide by 160 pixels high
void ST7735_DrawBitmap(int16_t x, int16_t y, const uint16_t *image, int16_t w, int16_t h){
  int16_t skipC = 0;                      // non-zero if columns need to be skipped due to clipping
  int16_t originalWidth = w;              // save this value; even if not all columns fit on the screen, the image is still this width in ROM
  int i = w*(h - 1);

  if((x >= _width) || ((y - h + 1) >= _height) || ((x + w) <= 0) || (y < 0)){
    return;                             // image is totally off the screen, do nothing
  }
  if((w > _width) || (h > _height)){    // image is too wide for the screen, do nothing
    //***This isn't necessarily a fatal error, but it makes the
    //following logic much more complicated, since you can have
    //an image that exceeds multiple boundaries and needs to be
    //clipped on more than one side.
    return;
  }
  if((x + w - 1) >= _width){            // image exceeds right of screen
    skipC = (x + w) - _width;           // skip cut off columns
    w = _width - x;
  }
  if((y - h + 1) < 0){                  // image exceeds top of screen
    i = i - (h - y - 1)*originalWidth;  // skip the last cut off rows
    h = y + 1;
  }
  if(x < 0){                            // image exceeds left of screen
    w = w + x;
    skipC = -1*x;                       // skip cut off columns
    i = i - x;                          // skip the first cut off columns
    x = 0;
  }
  if(y >= _height){                     // image exceeds bottom of screen
    h = h - (y - _height + 1);
    y = _height - 1;
  }

  setAddrWindow(x, y-h+1, x+w-1, y);

  for(y=0; y<h; y=y+1){
    for(x=0; x<w; x=x+1){
                                        // send the top 8 bits
      SPI_OutData((uint8_t)(image[i] >> 8));
                                        // send the bottom 8 bits
      SPI_OutData((uint8_t)image[i]);
      i = i + 1;                        // go to the next pixel
    }
    i = i + skipC;
    i = i - 2*originalWidth;
  }

//  deselect();
}


//------------ST7735_DrawCharS------------
// Simple character draw function.  This is the same function from
// Adafruit_GFX.c but adapted for this processor.  However, each call
// to ST7735_DrawPixel() calls setAddrWindow(), which needs to send
// many extra data and commands.  If the background color is the same
// as the text color, no background will be printed, and text can be
// drawn right over existing images without covering them with a box.
// Requires (11 + 2*size*size)*6*8 (image fully on screen; textcolor != bgColor)
// Input: x         horizontal position of the top left corner of the character, columns from the left edge
//        y         vertical position of the top left corner of the character, rows from the top edge
//        c         character to be printed
//        textColor 16-bit color of the character
//        bgColor   16-bit color of the background
//        size      number of pixels per character pixel (e.g. size==2 prints each pixel of font as 2x2 square)
// Output: none
void ST7735_DrawCharS(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size){
  uint8_t line; // vertical column of pixels of character in font
  int32_t i, j;
  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + 6 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0))   // Clip top
    return;

  for (i=0; i<6; i++ ) {
    if (i == 5)
      line = 0x0;
    else
      line = Font[(c*5)+i];
    for (j = 0; j<8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          ST7735_DrawPixel(x+i, y+j, textColor);
        else {  // big size
          ST7735_FillRect(x+(i*size), y+(j*size), size, size, textColor);
        }
      } else if (bgColor != textColor) {
        if (size == 1) // default size
          ST7735_DrawPixel(x+i, y+j, bgColor);
        else {  // big size
          ST7735_FillRect(x+i*size, y+j*size, size, size, bgColor);
        }
      }
      line >>= 1;
    }
  }
}


//------------ST7735_DrawChar------------
// Advanced character draw function.  This is similar to the function
// from Adafruit_GFX.c but adapted for this processor.  However, this
// function only uses one call to setAddrWindow(), which allows it to
// run at least twice as fast.
// Requires (11 + size*size*6*8) bytes of transmission (assuming image fully on screen)
// Input: x         horizontal position of the top left corner of the character, columns from the left edge
//        y         vertical position of the top left corner of the character, rows from the top edge
//        c         character to be printed
//        textColor 16-bit color of the character
//        bgColor   16-bit color of the background
//        size      number of pixels per character pixel (e.g. size==2 prints each pixel of font as 2x2 square)
// Output: none
void ST7735_DrawChar(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size){
  uint8_t line; // horizontal row of pixels of character
  int32_t col, row, i, j;// loop indices
  if(((x + 6*size - 1) >= _width)  || // Clip right
     ((y + 8*size - 1) >= _height) || // Clip bottom
     ((x + 6*size - 1) < 0)        || // Clip left
     ((y + 8*size - 1) < 0)){         // Clip top
    return;
  }

  setAddrWindow(x, y, x+6*size-1, y+8*size-1);

  line = 0x01;        // print the top row first
  // print the rows, starting at the top
  for(row=0; row<8; row=row+1){
    for(i=0; i<size; i=i+1){
      // print the columns, starting on the left
      for(col=0; col<5; col=col+1){
        if(Font[(c*5)+col]&line){
          // bit is set in Font, print pixel(s) in text color
          for(j=0; j<size; j=j+1){
            pushColor(textColor);
          }
        } else{
          // bit is cleared in Font, print pixel(s) in background color
          for(j=0; j<size; j=j+1){
            pushColor(bgColor);
          }
        }
      }
      // print blank column(s) to the right of character
      for(j=0; j<size; j=j+1){
        pushColor(bgColor);
      }
    }
    line = line<<1;   // move up to the next row
  }

  //deselect();
}
//------------ST7735_DrawString------------
// String draw function.
// 16 rows (0 to 15) and 21 characters (0 to 20)
// Requires (11 + size*size*6*8) bytes of transmission for each character
// Input: x         columns from the left edge (0 to 20)
//        y         rows from the top edge (0 to 15)
//        pt        pointer to a null terminated string to be printed
//        textColor 16-bit color of the characters
// bgColor is Black and size is 1
// Output: number of characters printed
uint32_t ST7735_DrawString(uint16_t x, uint16_t y, char *pt, int16_t textColor){
  uint32_t count = 0;
  if(y>15) return 0;
  while(*pt){
    ST7735_DrawCharS(x*6, y*10, *pt, textColor, ST7735_BLACK, 1);
    pt++;
    x = x+1;
    if(x>20) return count;  // number of characters printed
    count++;
  }
  return count;  // number of characters printed
}

//-----------------------fillmessage-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
char Message[12];
uint32_t Messageindex;

void fillmessage(uint32_t n){
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string
  if(n >= 10){
    fillmessage(n/10);
    n = n%10;
  }
  Message[Messageindex] = (n+'0'); /* n is between 0 and 9 */
  if(Messageindex<11)Messageindex++;
}

void fillmessage4(uint32_t n){
  if(n>9999)n=9999;
  if(n>=1000){  // 1000 to 9999
    Messageindex = 0;
  } else if(n>=100){  // 100 to 999
    Message[0] = ' '; 
    Messageindex = 1;
  }else if(n>=10){ // 
    Message[0] = ' '; /* n is between 10 and 99 */
    Message[1] = ' '; 
    Messageindex = 2;
  }else{
    Message[0] = ' '; /* n is between 0 and 9 */
    Message[1] = ' '; 
    Message[2] = ' '; 
    Messageindex = 3;
  }
  fillmessage(n);
}
void fillmessage5(uint32_t n){
  if(n>99999)n=99999;
  if(n>=10000){  // 10000 to 99999
    Messageindex = 0;
  } else if(n>=1000){  // 1000 to 9999
    Message[0] = ' '; 
    Messageindex = 1;
  }else if(n>=100){  // 100 to 999
    Message[0] = ' '; 
    Message[1] = ' '; 
    Messageindex = 2;
  }else if(n>=10){ // 
    Message[0] = ' '; /* n is between 10 and 99 */
    Message[1] = ' '; 
    Message[2] = ' '; 
    Messageindex = 3;
  }else{
    Message[0] = ' '; /* n is between 0 and 9 */
    Message[1] = ' '; 
    Message[2] = ' '; 
    Message[3] = ' '; 
    Messageindex = 4;
  }
  fillmessage(n);
}

void static fillmessage2_1(uint32_t n){
  if(n>999)n=999;
  if(n>=100){  // 100 to 999
    Message[0] = (n/100+'0'); /* tens digit */
    n = n%100; //the rest
  }else { // 0 to 99
    Message[0] = ' '; /* n is between 0.0 and 9.9 */
  }
  Message[1] = (n/10+'0'); /* ones digit */
  n = n%10; //the rest
  Message[2] = '.';
  Message[3] = (n+'0'); /* tenths digit */
  Message[4] = 0;
}

void static fillmessage2_Hex(uint32_t n){ char digit;
  if(n>255){
    Message[0] = '*';
    Message[1] = '*';
  }else{
    digit = n/16;
    if(digit<10){
      digit = digit+'0';
    }else{
      digit = digit+'A'-10;
    }
    Message[0] = digit; /* 16's digit */
    digit = n%16;
    if(digit<10){
      digit = digit+'0';
    }else{
      digit = digit+'A'-10;
    }
    Message[1] = digit; /* ones digit */
  }
  Message[2] = ',';
  Message[3] = 0;
}


//********ST7735_SetCursor*****************
// Move the cursor to the desired X- and Y-position.  The
// next character will be printed here.  X=0 is the leftmost
// column.  Y=0 is the top row.
// inputs: newX  new X-position of the cursor (0<=newX<=20)
//         newY  new Y-position of the cursor (0<=newY<=15)
// outputs: none
void ST7735_SetCursor(uint32_t newX, uint32_t newY){
  if((newX > 20) || (newY > 15)){       // bad input
    return;                             // do nothing
  }
  StX = newX;
  StY = newY;
}
//-----------------------ST7735_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Position determined by ST7735_SetCursor command
// Color set by ST7735_SetTextColor
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void ST7735_OutUDec(uint32_t n){
  Messageindex = 0;
  fillmessage(n);
  Message[Messageindex] = 0; // terminate
  ST7735_DrawString(StX,StY,Message,StTextColor);
  StX = StX+Messageindex;
  if(StX>20){
    StX = 20;
    ST7735_DrawCharS(StX*6,StY*10,'*',ST7735_RED,ST7735_BLACK, 1);
  }
}

//-----------------------ST7735_OutUDec4-----------------------
// Output a 32-bit number in unsigned 4-digit decimal format
// Position determined by ST7735_SetCursor command
// Color set by ST7735_SetTextColor
// Input: 32-bit number to be transferred
// Output: none
// Fixed format 4 digits with no space before or after
void ST7735_OutUDec4(uint32_t n){
  Messageindex = 0;
  fillmessage4(n);
  Message[Messageindex] = 0; // terminate
  ST7735_DrawString(StX,StY,Message,StTextColor);
  StX = StX+Messageindex;
  if(StX>20){
    StX = 20;
    ST7735_DrawCharS(StX*6,StY*10,'*',ST7735_RED,ST7735_BLACK, 1);
  }
}
//-----------------------ST7735_OutUDec5-----------------------
// Output a 32-bit number in unsigned 5-digit decimal format
// Position determined by ST7735_SetCursor command
// Color set by ST7735_SetTextColor
// Input: 32-bit number to be transferred
// Output: none
// Fixed format 5 digits with no space before or after
void ST7735_OutUDec5(uint32_t n){
  Messageindex = 0;
  fillmessage5(n);
  Message[Messageindex] = 0; // terminate
  ST7735_DrawString(StX,StY,Message,StTextColor);
  StX = StX+Messageindex;
  if(StX>20){
    StX = 20;
    ST7735_DrawCharS(StX*6,StY*10,'*',ST7735_RED,ST7735_BLACK, 1);
  }
}


//-----------------------ST7735_OutUFix2_1-----------------------
// Output a 32-bit number in unsigned 3-digit fixed point, 0.1 resolution
// numbers 0 to 999 printed as " 0.0" to "99.9"
// Position determined by ST7735_SetCursor command
// Input: 32-bit number to be transferred
//        textColor 16-bit color of the numbers
// Output: none
// Fixed format 4 characters with no space before or after
void ST7735_OutUFix2_1(uint32_t n, int16_t textColor){
  fillmessage2_1(n);
  ST7735_DrawString(StX,StY,Message,textColor);
  StX = StX+4;
  if(StX>20){
    StX = 20;
    ST7735_DrawChar(StX*6,StY*10,'*',ST7735_RED,ST7735_BLACK, 1);
  }
}

//-----------------------ST7735_OutUHex2-----------------------
// Output a 32-bit number in unsigned 2-digit hexadecimal format
// numbers 0 to 255 printed as "00," to "FF,"
// Position determined by ST7735_SetCursor command
// Input: 32-bit number to be transferred
//        textColor 16-bit color of the numbers
// Output: none
// Fixed format 3 characters with comma after
void ST7735_OutUHex2(uint32_t n, int16_t textColor){
  fillmessage2_Hex(n);
  ST7735_DrawString(StX,StY,Message,textColor);
  StX = StX+3;
  if(StX>20){
    StX = 20;
    ST7735_DrawChar(StX*6,StY*10,'*',ST7735_RED,ST7735_BLACK, 1);
  }
}



#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

//------------ST7735_SetRotation------------
// Change the image rotation.
// Requires 2 bytes of transmission
// Input: m new rotation value (0 to 3)
// Output: none
void ST7735_SetRotation(uint8_t m) {

  SPI_OutCommand(ST7735_MADCTL);
  Rotation = m % 4; // can't be higher than 3
  switch (Rotation) {
   case 0:
     if (TabColor == INITR_BLACKTAB) {
       SPI_OutData(MADCTL_MX | MADCTL_MY | MADCTL_RGB);
     } else {
       SPI_OutData(MADCTL_MX | MADCTL_MY | MADCTL_BGR);
     }
     _width  = ST7735_TFTWIDTH;
     _height = ST7735_TFTHEIGHT;
     break;
   case 1:
     if (TabColor == INITR_BLACKTAB) {
       SPI_OutData(MADCTL_MY | MADCTL_MV | MADCTL_RGB);
     } else {
       SPI_OutData(MADCTL_MY | MADCTL_MV | MADCTL_BGR);
     }
     _width  = ST7735_TFTHEIGHT;
     _height = ST7735_TFTWIDTH;
     break;
  case 2:
     if (TabColor == INITR_BLACKTAB) {
       SPI_OutData(MADCTL_RGB);
     } else {
       SPI_OutData(MADCTL_BGR);
     }
     _width  = ST7735_TFTWIDTH;
     _height = ST7735_TFTHEIGHT;
    break;
   case 3:
     if (TabColor == INITR_BLACKTAB) {
       SPI_OutData(MADCTL_MX | MADCTL_MV | MADCTL_RGB);
     } else {
       SPI_OutData(MADCTL_MX | MADCTL_MV | MADCTL_BGR);
     }
     _width  = ST7735_TFTHEIGHT;
     _height = ST7735_TFTWIDTH;
     break;
  }

//  deselect();
}


//------------ST7735_InvertDisplay------------
// Send the command to invert all of the colors.
// Requires 1 byte of transmission
// Input: i 0 to disable inversion; non-zero to enable inversion
// Output: none
void ST7735_InvertDisplay(int i) {
  if(i){
    SPI_OutCommand(ST7735_INVON);
  } else{
    SPI_OutCommand(ST7735_INVOFF);
  }  
 // deselect();
}
// graphics routines
// y coordinates 0 to 31 used for labels and messages
// y coordinates 32 to 159  128 pixels high
// x coordinates 0 to 127   128 pixels wide

int32_t Ymax,Ymin,X;        // X goes from 0 to 127
int32_t Yrange; //YrangeDiv2;
int TimeIndex;               // horizontal position of next point to plot on graph (0 to 99)
uint16_t PlotBGColor;        // background color of the plot used whenever clearing plot area

// *************** ST7735_PlotClear ********************
// Clear the graphics buffer, set X coordinate to 0
// This routine clears the display
// Inputs: ymin and ymax are range of the plot
// Outputs: none
void ST7735_PlotClear(int32_t ymin, int32_t ymax){
  ST7735_FillRect(0, 32, 128, 128, ST7735_Color565(228,228,228)); // light grey
  if(ymax>ymin){
    Ymax = ymax;
    Ymin = ymin;
    Yrange = ymax-ymin;
  } else{
    Ymax = ymin;
    Ymin = ymax;
    Yrange = ymax-ymin;
  }
  //YrangeDiv2 = Yrange/2;
  X = 0;
}
// ------------ST7735_Drawaxes------------
// Set up the axes, labels, and other variables to
// allow data to be plotted in a chart using the
// functions ST7735_PlotPoint() and
// ST7735_PlotIncrement().
// Input: axisColor   16-bit color for axes, which can be produced by ST7735_Color565()
//        bgColor     16-bit color for plot background, which can be produced by ST7735_Color565()
//        xLabel      pointer to a null terminated string for x-axis (~4 character space)
//        yLabel1     pointer to a null terminated string for top of y-axis (~3-5 character space)
//        label1Color 16-bit color for y-axis label1, which can be produced by ST7735_Color565()
//        yLabel2     pointer to a null terminated string for bottom of y-axis (~3 character space)
//                      if yLabel2 is empty string, no yLabel2 is printed, and yLabel1 is centered
//        label2Color 16-bit color for y-axis label2, which can be produced by ST7735_Color565()
//        ymax        maximum value to be printed
//        ymin        minimum value to be printed
// Output: none
// Assumes: ST7735_Init() has been called
void ST7735_Drawaxes(uint16_t axisColor, uint16_t bgColor, char *xLabel,
  char *yLabel1, uint16_t label1Color, char *yLabel2, uint16_t label2Color,
  int32_t ymax, int32_t ymin){
  int i;
  // assume that ymax > ymin
  Ymax = ymax;
  Ymin = ymin;
  Yrange = Ymax - Ymin;
  TimeIndex = 0;
  PlotBGColor = bgColor;
  ST7735_FillRect(0, 17, 111, 111, bgColor);
  ST7735_DrawFastHLine(10, 117, 101, axisColor);
  ST7735_DrawFastVLine(10, 17, 101, axisColor);
  for(i=20; i<=110; i=i+10){
    ST7735_DrawPixel(i, 118, axisColor);
  }
  for(i=17; i<117; i=i+10){
    ST7735_DrawPixel(9, i, axisColor);
  }
  i = 50;
  while((*xLabel) && (i < 100)){
    ST7735_DrawChar(i, 120, *xLabel, axisColor, bgColor, 1);
    i = i + 6;
    xLabel++;
  }
  if(*yLabel2){ // two labels
    i = 26;
    while((*yLabel2) && (i < 50)){
      ST7735_DrawChar(0, i, *yLabel2, label2Color, bgColor, 1);
      i = i + 8;
      yLabel2++;
    }
    i = 82;
  }else{ // one label
    i = 42;
  }
  while((*yLabel1) && (i < 120)){
    ST7735_DrawChar(0, i, *yLabel1, label1Color, bgColor, 1);
    i = i + 8;
    yLabel1++;
  }
}

// *************** ST7735_PlotPoint ********************
// Used in the voltage versus time plot, plot one point at y
// It does output to display
// Inputs: y is the y coordinate of the point plotted
// Outputs: none
void ST7735_PlotPoint(int32_t y){int32_t j;
  if(y<Ymin) y=Ymin;
  if(y>Ymax) y=Ymax;
  // X goes from 0 to 127
  // j goes from 159 to 32
  // y=Ymax maps to j=32
  // y=Ymin maps to j=159
  j = 32+(127*(Ymax-y))/Yrange;
  if(j<32) j = 32;
  if(j>159) j = 159;
  ST7735_DrawPixel(X,   j,   ST7735_BLUE);
  ST7735_DrawPixel(X+1, j,   ST7735_BLUE);
  ST7735_DrawPixel(X,   j+1, ST7735_BLUE);
  ST7735_DrawPixel(X+1, j+1, ST7735_BLUE);
}

// ------------ST7735_PlotPoint2------------
// Plot a point on the chart.  To plot several points in the
// same column, call this function repeatedly before calling
// ST7735_PlotIncrement().  The units of the data are the
// same as the ymax and ymin values specified in the
// initialization function.
// Input: data1  value to be plotted (units not specified)
//        color1 16-bit color for the point, which can be produced by LCD_Color565()
// Output: none
// Assumes: ST7735_Init() and ST7735_Drawaxes() have been called
void ST7735_PlotPoint2(int32_t data1, uint16_t color1){
  data1 = ((data1 - Ymin)*100)/Yrange;
  if(data1 > 98){
    data1 = 98;
    color1 = ST7735_RED;
  }
  if(data1 < 0){
    data1 = 0;
    color1 = ST7735_RED;
  }
  ST7735_DrawPixel(TimeIndex + 11, 116 - data1, color1);
  ST7735_DrawPixel(TimeIndex + 11, 115 - data1, color1);
}
// ------------ST7735_PlotIncrement------------
// Increment the plot between subsequent calls to
// LCD_PlotPoint().  Automatically wrap and clear the
// column to be printed to.
// Input: none
// Output: none
// Assumes: ST7735_Init() and ST7735_Drawaxes() have been called
void ST7735PlotIncrement(void){
  TimeIndex = TimeIndex + 1;
  if(TimeIndex > 99){
    TimeIndex = 0;
  }
  ST7735_DrawFastVLine(TimeIndex + 11, 17, 100, PlotBGColor);
}
// *************** ST7735_PlotLine ********************
// Used in the voltage versus time plot, plot line to new point
// It does output to display
// Inputs: y is the y coordinate of the point plotted
// Outputs: none
int32_t lastj=0;
void ST7735_PlotLine(int32_t y){int32_t i,j;
  if(y<Ymin) y=Ymin;
  if(y>Ymax) y=Ymax;
  // X goes from 0 to 127
  // j goes from 159 to 32
  // y=Ymax maps to j=32
  // y=Ymin maps to j=159
  j = 32+(127*(Ymax-y))/Yrange;
  if(j < 32) j = 32;
  if(j > 159) j = 159;
  if(lastj < 32) lastj = j;
  if(lastj > 159) lastj = j;
  if(lastj < j){
    for(i = lastj+1; i<=j ; i++){
      ST7735_DrawPixel(X,   i,   ST7735_BLUE) ;
      ST7735_DrawPixel(X+1, i,   ST7735_BLUE) ;
    }
  }else if(lastj > j){
    for(i = j; i<lastj ; i++){
      ST7735_DrawPixel(X,   i,   ST7735_BLUE) ;
      ST7735_DrawPixel(X+1, i,   ST7735_BLUE) ;
    }
  }else{
    ST7735_DrawPixel(X,   j,   ST7735_BLUE) ;
    ST7735_DrawPixel(X+1, j,   ST7735_BLUE) ;
  }
  lastj = j;
}

// *************** ST7735_PlotPoints ********************
// Used in the voltage versus time plot, plot two points at y1, y2
// It does output to display
// Inputs: y1 is the y coordinate of the first point plotted
//         y2 is the y coordinate of the second point plotted
// Outputs: none
void ST7735_PlotPoints(int32_t y1,int32_t y2){int32_t j;
  if(y1<Ymin) y1=Ymin;
  if(y1>Ymax) y1=Ymax;
  // X goes from 0 to 127
  // j goes from 159 to 32
  // y=Ymax maps to j=32
  // y=Ymin maps to j=159
  j = 32+(127*(Ymax-y1))/Yrange;
  if(j<32) j = 32;
  if(j>159) j = 159;
  ST7735_DrawPixel(X, j, ST7735_BLUE);
  if(y2<Ymin) y2=Ymin;
  if(y2>Ymax) y2=Ymax;
  j = 32+(127*(Ymax-y2))/Yrange;
  if(j<32) j = 32;
  if(j>159) j = 159;
  ST7735_DrawPixel(X, j, ST7735_BLACK);
}
// *************** ST7735_PlotBar ********************
// Used in the voltage versus time bar, plot one bar at y
// It does not output to display until RIT128x96x4ShowPlot called
// Inputs: y is the y coordinate of the bar plotted
// Outputs: none
void ST7735_PlotBar(int32_t y){
int32_t j;
  if(y<Ymin) y=Ymin;
  if(y>Ymax) y=Ymax;
  // X goes from 0 to 127
  // j goes from 159 to 32
  // y=Ymax maps to j=32
  // y=Ymin maps to j=159
  j = 32+(127*(Ymax-y))/Yrange;
  ST7735_DrawFastVLine(X, j, 159-j, ST7735_BLACK);

}

// full scaled defined as 3V
// Input is 0 to 511, 0 => 159 and 511 => 32
uint8_t const dBfs[512]={
159, 159, 145, 137, 131, 126, 123, 119, 117, 114, 112, 110, 108, 107, 105, 104, 103, 101,
  100, 99, 98, 97, 96, 95, 94, 93, 93, 92, 91, 90, 90, 89, 88, 88, 87, 87, 86, 85, 85, 84,
  84, 83, 83, 82, 82, 81, 81, 81, 80, 80, 79, 79, 79, 78, 78, 77, 77, 77, 76, 76, 76, 75,
  75, 75, 74, 74, 74, 73, 73, 73, 72, 72, 72, 72, 71, 71, 71, 71, 70, 70, 70, 70, 69, 69,
  69, 69, 68, 68, 68, 68, 67, 67, 67, 67, 66, 66, 66, 66, 66, 65, 65, 65, 65, 65, 64, 64,
  64, 64, 64, 63, 63, 63, 63, 63, 63, 62, 62, 62, 62, 62, 62, 61, 61, 61, 61, 61, 61, 60,
  60, 60, 60, 60, 60, 59, 59, 59, 59, 59, 59, 59, 58, 58, 58, 58, 58, 58, 58, 57, 57, 57,
  57, 57, 57, 57, 56, 56, 56, 56, 56, 56, 56, 56, 55, 55, 55, 55, 55, 55, 55, 55, 54, 54,
  54, 54, 54, 54, 54, 54, 53, 53, 53, 53, 53, 53, 53, 53, 53, 52, 52, 52, 52, 52, 52, 52,
  52, 52, 52, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 50, 50, 50, 50, 50, 50, 50, 50, 50,
  50, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48,
  48, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 46, 46, 46, 46, 46, 46, 46, 46, 46,
  46, 46, 46, 46, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 44, 44, 44, 44, 44,
  44, 44, 44, 44, 44, 44, 44, 44, 44, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
  43, 43, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 41, 41, 41, 41, 41,
  41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
  40, 40, 40, 40, 40, 40, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
  39, 39, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 37,
  37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 36, 36, 36, 36,
  36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 35, 35, 35, 35, 35,
  35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 34, 34, 34, 34, 34, 34,
  34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 33, 33, 33, 33, 33,
  33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 32, 32, 32,
  32, 32, 32, 32, 32, 32, 32, 32, 32, 32
};

// *************** ST7735_PlotdBfs ********************
// Used in the amplitude versus frequency plot, plot bar point at y
// 0 to 0.625V scaled on a log plot from min to max
// It does output to display
// Inputs: y is the y ADC value of the bar plotted
// Outputs: none
void ST7735_PlotdBfs(int32_t y){
int32_t j;
  y = y/2; // 0 to 2047
  if(y<0) y=0;
  if(y>511) y=511;
  // X goes from 0 to 127
  // j goes from 159 to 32
  // y=511 maps to j=32
  // y=0 maps to j=159
  j = dBfs[y];
  ST7735_DrawFastVLine(X, j, 159-j, ST7735_BLACK);

}

// *************** ST7735_PlotNext ********************
// Used in all the plots to step the X coordinate one pixel
// X steps from 0 to 127, then back to 0 again
// It does not output to display
// Inputs: none
// Outputs: none
void ST7735_PlotNext(void){
  if(X==127){
    X = 0;
  } else{
    X++;
  }
}

// *************** ST7735_PlotNextErase ********************
// Used in all the plots to step the X coordinate one pixel
// X steps from 0 to 127, then back to 0 again
// It clears the vertical space into which the next pixel will be drawn
// Inputs: none
// Outputs: none
void ST7735_PlotNextErase(void){
  if(X==127){
    X = 0;
  } else{
    X++;
  }
  ST7735_DrawFastVLine(X,32,128,ST7735_Color565(228,228,228));
}

// Used in all the plots to write buffer to LCD
// Example 1 Voltage versus time
//    ST7735_PlotClear(0,4095);  // range from 0 to 4095
//    ST7735_PlotPoint(data); ST7735_PlotNext(); // called 128 times

// Example 2a Voltage versus time (N data points/pixel, time scale)
//    ST7735_PlotClear(0,4095);  // range from 0 to 4095
//    {   for(j=0;j<N;j++){
//          ST7735_PlotPoint(data[i++]); // called N times
//        }
//        ST7735_PlotNext();
//    }   // called 128 times

// Example 2b Voltage versus time (N data points/pixel, time scale)
//    ST7735_PlotClear(0,4095);  // range from 0 to 4095
//    {   for(j=0;j<N;j++){
//          ST7735_PlotLine(data[i++]); // called N times
//        }
//        ST7735_PlotNext();
//    }   // called 128 times

// Example 3 Voltage versus frequency (512 points)
//    perform FFT to get 512 magnitudes, mag[i] (0 to 4095)
//    ST7735_PlotClear(0,1023);  // clip large magnitudes
//    {
//        ST7735_PlotBar(mag[i++]); // called 4 times
//        ST7735_PlotBar(mag[i++]);
//        ST7735_PlotBar(mag[i++]);
//        ST7735_PlotBar(mag[i++]);
//        ST7735_PlotNext();
//    }   // called 128 times

// Example 4 Voltage versus frequency (512 points), dB scale
//    perform FFT to get 512 magnitudes, mag[i] (0 to 4095)
//    ST7735_PlotClear(0,511);  // parameters ignored
//    {
//        ST7735_PlotdBfs(mag[i++]); // called 4 times
//        ST7735_PlotdBfs(mag[i++]);
//        ST7735_PlotdBfs(mag[i++]);
//        ST7735_PlotdBfs(mag[i++]);
//        ST7735_PlotNext();
//    }   // called 128 times

// *************** ST7735_OutChar ********************
// Output one character to the LCD
// Position determined by ST7735_SetCursor command
// Color set by ST7735_SetTextColor
// Inputs: 8-bit ASCII character
// Outputs: none
void ST7735_OutChar(char ch){
  if((ch == 10) || (ch == 13) || (ch == 27)){
    StY++; StX=0;
    if(StY>15){
      StY = 0;
    }
    ST7735_DrawString(0,StY,"                     ",StTextColor);
    return;
  }
  ST7735_DrawCharS(StX*6,StY*10,ch,ST7735_YELLOW,ST7735_BLACK, 1);
  StX++;
  if(StX>20){
    StX = 20;
    ST7735_DrawCharS(StX*6,StY*10,'*',ST7735_RED,ST7735_BLACK, 1);
  }
  return;
}
//********ST7735_OutString*****************
// Print a string of characters to the ST7735 LCD.
// Position determined by ST7735_SetCursor command
// Color set by ST7735_SetTextColor
// The string will not automatically wrap.
// inputs: ptr  pointer to NULL-terminated ASCII string
// outputs: none
void ST7735_OutString(char *ptr){
  while(*ptr){
    ST7735_OutChar(*ptr);
    ptr = ptr + 1;
  }
}
// ************** ST7735_SetTextColor ************************
// Sets the color in which the characters will be printed
// Background color is fixed at black
// Input:  16-bit packed color
// Output: none
// ********************************************************
void ST7735_SetTextColor(uint16_t color){
  StTextColor = color;
}




int ST7735_open(const char *path, unsigned flags, int llv_fd){
  ST7735_InitR(INITR_REDTAB);
  return 0;
}
int ST7735_close( int dev_fd){
    return 0;
}
int ST7735_read(int dev_fd, char *buf, unsigned count){char ch;
// not implemented
  return 1;
}
int ST7735_write(int dev_fd, const char *buf, unsigned count){ unsigned int num=count;
  while(num){
    ST7735_OutChar(*buf);
    buf++;
    num--;
  }
  return count;
}
off_t ST7735_lseek(int dev_fd, off_t ioffset, int origin){
    return 0;
}
int ST7735_unlink(const char * path){
    return 0;
}
int ST7735_rename(const char *old_name, const char *new_name){
    return 0;
}

//------------ST7735_InitPrintf------------
// Initialize the ST7735 for printf
// Input: none
// Output: none
void ST7735_InitPrintf(void){int ret_val; FILE *fptr;
  ST7735_InitR(INITR_REDTAB);
  ret_val = add_device("st7735", _SSA, ST7735_open, ST7735_close, ST7735_read, ST7735_write, ST7735_lseek, ST7735_unlink, ST7735_rename);
  if(ret_val) return; // error
  fptr = fopen("st7745","w");
  if(fptr == 0) return; // error
  freopen("st7735:", "w", stdout); // redirect stdout to uart
  setvbuf(stdout, NULL, _IONBF, 0); // turn off buffering for stdout

}



/****************ST7735_sDecOut2***************
 converts fixed point number to LCD
 format signed 32-bit with resolution 0.01
 range -99.99 to +99.99
 Inputs:  signed 32-bit integer part of fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
Parameter LCD display
 12345    " **.**"
  2345    " 23.45"  
 -8100    "-81.00"
  -102    " -1.02" 
    31    "  0.31" 
-12345    "-**.**"
 */ 
void ST7735_sDecOut2(int32_t n){
    

    
// EE445L lab assignment
}


/**************ST7735_uBinOut6***************
 unsigned 32-bit binary fixed-point with a resolution of 1/64. 
 The full-scale range is from 0 to 999.99. 
 If the integer part is larger than 63999, it signifies an error. 
 The ST7735_uBinOut6 function takes an unsigned 32-bit integer part 
 of the binary fixed-point number and outputs the fixed-point value on the LCD
 Inputs:  unsigned 32-bit integer part of binary fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
Parameter LCD display
     0    "  0.00"
     1    "  0.01"
    16    "  0.25"
    25    "  0.39"
   125    "  1.95"
   128    "  2.00"
  1250    " 19.53"
  7500    "117.19"
 63999    "999.99"
 64000    "***.**"
*/
void ST7735_uBinOut6(uint32_t n){
    
// EE445L lab assignment

} 


/**************ST7735_XYplotInit***************
 Specify the X and Y axes for an x-y scatter plot
 Draw the title and clear the plot area
 Inputs:  title  ASCII string to label the plot, null-termination
          minX   smallest X data value allowed, resolution= 0.001
          maxX   largest X data value allowed, resolution= 0.001
          minY   smallest Y data value allowed, resolution= 0.001
          maxY   largest Y data value allowed, resolution= 0.001
 Outputs: none
 assumes minX < maxX, and miny < maxY
*/
void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY){
  
// EE445L lab assignment

}

/**************ST7735_XYplot***************
 Plot an array of (x,y) data
 Inputs:  num    number of data points in the two arrays
          bufX   array of 32-bit fixed-point data, resolution= 0.001
          bufY   array of 32-bit fixed-point data, resolution= 0.001
 Outputs: none
 assumes ST7735_XYplotInit has been previously called
 neglect any points outside the minX maxY minY maxY bounds
*/
void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]){
  
// EE445L lab assignment
}

// plotLine function that is used when dx is greater than dy
void plotLineX(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
  int32_t d = 0;
  int32_t dx = x2-x1;
  int32_t dy = y2-y1;
  int32_t plotY = 0;

  // int32_t yi = 1
  // y = y0
  d = 2*abs(dy) - dx;
  
  plotY = y1;
  
  for(int32_t plotX = x1; plotX < x2; plotX++){
    if(abs(dx) > abs(dy)){
      ST7735_DrawPixel(plotX, plotY, color);
      ST7735_DrawPixel(plotX, plotY+1, color);
    }else{
      ST7735_DrawPixel(plotX, plotY, color);
      ST7735_DrawPixel(plotX+1, plotY, color);
    }
    if(d > 0){
      if(dy < 0){
        plotY--;
      }else{
        plotY++;
      }
      d -= 2*dx;
    }
    d += 2*abs(dy);
  }
}

// plotLine function that is used when dy is greater than dx
void plotLineY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
  int32_t d = 0;
  int32_t dx = x2-x1;
  int32_t dy = y2-y1;
  int32_t plotX = 0;

  // int32_t yi = 1
  // y = y0
  d = 2*abs(dx) - dy;
  
  plotX = x1;
  
  for(int32_t plotY = y1; plotY < y2; plotY++){
    if(abs(dx) > abs(dy)){
      ST7735_DrawPixel(plotX, plotY, color);
      ST7735_DrawPixel(plotX, plotY+1, color);
    }else{
      ST7735_DrawPixel(plotX, plotY, color);
      ST7735_DrawPixel(plotX+1, plotY, color);
    }
    if(d > 0){
      if(dx < 0){
        plotX--;
      }else{
        plotX++;
      }
      d -= 2*dy;
    }
    d += 2*abs(dx);
  }
}

//************* ST7735_Line********************************************
//  Draws one line on the ST7735 color LCD
//  Inputs: (x1,y1) is the start point
//          (x2,y2) is the end point
// x1,x2 are horizontal positions, columns from the left edge
//               must be less than 128
//               0 is on the left, 126 is near the right
// y1,y2 are vertical positions, rows from the top edge
//               must be less than 160
//               159 is near the wires, 0 is the side opposite the wires
//        color 16-bit color, which can be produced by ST7735_Color565() 
// Output: none
void ST7735_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){

  if(abs(y2 - y1) < abs(x2 - x1)){ // if dx is greater than dy
    
    if(x1 < x2){  // if x1 is to the left of x2
      plotLineX(x1, y1, x2, y2, color);
    }else{
      plotLineX(x2, y2, x1, y1, color);
    }
  }else{
    
    if(y1 < y2){  // if y1 is below y2
      plotLineY(x1, y1, x2, y2, color);
    }else{
      plotLineY(x2, y2, x1, y1, color);
    }
  }
}

// *************** ST7735_SetX ********************
// Used in all the plots to change the X coordinate to new location
// X exists in the range from 0 to 127,
// Input values less than 0 get changed to 0,
// Input values greater than 127 get changed to 127
// It does not output to display
// Inputs: newX is the new value that the global X will be
// Outputs: none
void ST7735_SetX(int32_t newX){
  if(newX > 127){
    X = 127;
  }else if(newX < 0){
    X = 0;
  }else{
    X = newX;
  }
}
