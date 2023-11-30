// SSD1306.c
// Runs on TM4C123
// Use I2C to send an 8-bit data/commands to the SSD1306 128 by 64 pixel oLED
// pixel LCD to display text, images, or other information.
// Jonathan and Daniel Valvano
// Jan 9, 2021
// Derived from:  Adafruit_SSD1306.cpp
// Font table, initialization, and other functions based
// off of Nokia_5110_Example from Spark Fun:
// 7-17-2011
// Spark Fun Electronics 2011
// Nathan Seidle
// http://dlnmh9ip6v2uc.cloudfront.net/datasheets/LCD/Monochrome/Nokia_5110_Example.pde

/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2020
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2021, Jonathan Valvano, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of the FreeBSD Project.
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../inc/SSD1306.h"
#include "../inc/CortexM.h"
// Tested for four possible hardware connections 
// compile parameter I2C in SSD1306.h
/*
 *  I2C0 Conncection | I2C1 Conncection | I2C2 Conncection | I2C3 Conncection
 *  ---------------- | ---------------- | ---------------- | ----------------
 *  SCL -------- PB2 | SCL -------- PA6 | SCL -------- PE4 | SCL -------- PD0
 *  SDA -------- PB3 | SDA -------- PA7 | SDA -------- PE5 | SDA -------- PD1
 */
#if I2C == 0
#include "../inc/I2CB1.h"
#define I2C_Init I2C0_Init
#define I2C_Send2 I2C0_Send2
#define I2C_Send I2C0_Send
#define I2C_SendData I2C0_SendData
#elif I2C == 2
#include "../inc/I2C2.h"
#define I2C_Init I2C2_Init
#define I2C_Send2 I2C2_Send2
#define I2C_Send I2C2_Send
#define I2C_SendData I2C2_SendData
#elif I2C == 3
#include "../inc/I2C3.h"
#define I2C_Init I2C3_Init
#define I2C_Send2 I2C3_Send2
#define I2C_Send I2C3_Send
#define I2C_SendData I2C3_SendData
#else
#include "../inc/I2C1.h"
#define I2C_Init I2C1_Init
#define I2C_Send2 I2C1_Send2
#define I2C_Send I2C1_Send
#define I2C_SendData I2C1_SendData
#endif
 //
// rough estimate of execution times (runs a little faster with 1.5k pullup from SDA to 3.3V, and no glitch)
//                      with 1.5k   without
// SSD1306_OutClear      24.8ms    26.2ms, clears entire display 
// SSD1306_OutBuffer     24.7ms    26.1ms, sends 1024 bytes, 128 by 64 pixels to OLED 
// SSD1306_DrawBMP         91us      91us, (10 by 16 size), doesn't output, just fills buffer 
// SSD1306_OutChar        196us     207us, does output to OLED 
// SSD1306_OutUDec        983us    1038us, outputs 5 characters to OLED
// SSD1306_ClearBuffer    153us     153us, empties buffer, not OLED output
#define SSD1306ADDR 0x3C
// Typo on OLED has a 0-ohm resistor selection 
// that says        "IIC ADDRESS SELECT 0x78 0x7A"
// should have said "IIC ADDRESS SELECT 0x3C 0x3D"
// search for I2C SSD1306 on Amazon.com
// https://www.amazon.com/gp/product/B0871KW7BD
// VCC   3.3V power to OLED
// GND   ground
// SCL   PB2 I2C clock
// SDA   PB3 I2C data

// *************************** Screen dimensions ***************************
#define WIDTH     128
#define HEIGHT     64
#define CR   0x0D

uint8_t buffer[WIDTH*HEIGHT/8]; // 8192 bits or 1024 bytes
int vccstate;
// vccstate = SSD1306_SWITCHCAPVCC for normal internal 3.3V power
//            SSD1306_EXTERNALVCC for separate external power
int rotation;
// private function prototypes
void ssd1306drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color);
void ssd1306drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color);


#define SSD1306_MEMORYMODE          0x20 ///< See datasheet
#define SSD1306_COLUMNADDR          0x21 ///< See datasheet
#define SSD1306_PAGEADDR            0x22 ///< See datasheet
#define SSD1306_SETCONTRAST         0x81 ///< See datasheet
#define SSD1306_CHARGEPUMP          0x8D ///< See datasheet
#define SSD1306_SEGREMAP            0xA0 ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON        0xA5 ///< Not currently used
#define SSD1306_NORMALDISPLAY       0xA6 ///< See datasheet
#define SSD1306_INVERTDISPLAY       0xA7 ///< See datasheet
#define SSD1306_SETMULTIPLEX        0xA8 ///< See datasheet
#define SSD1306_DISPLAYOFF          0xAE ///< See datasheet
#define SSD1306_DISPLAYON           0xAF ///< See datasheet
#define SSD1306_COMSCANINC          0xC0 ///< Not currently used
#define SSD1306_COMSCANDEC          0xC8 ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET    0xD3 ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5 ///< See datasheet
#define SSD1306_SETPRECHARGE        0xD9 ///< See datasheet
#define SSD1306_SETCOMPINS          0xDA ///< See datasheet
#define SSD1306_SETVCOMDETECT       0xDB ///< See datasheet

#define SSD1306_SETLOWCOLUMN        0x00 ///< Not currently used
#define SSD1306_SETHIGHCOLUMN       0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE        0x40 ///< See datasheet

#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26 ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27 ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL                    0x2E ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 ///< Set scroll range


// This table contains the hex values that represent pixels
// for a font that is 6 pixels wide and 8 pixels high (last column 0 for separation)
static const uint8_t ASCII[][6] = {
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00} // 20
  ,{0x00, 0x00, 0x5f, 0x00, 0x00, 0x00} // 21 !
  ,{0x00, 0x07, 0x00, 0x07, 0x00, 0x00} // 22 "
  ,{0x14, 0x7f, 0x14, 0x7f, 0x14, 0x00} // 23 #
  ,{0x24, 0x2a, 0x7f, 0x2a, 0x12, 0x00} // 24 $
  ,{0x23, 0x13, 0x08, 0x64, 0x62, 0x00} // 25 %
  ,{0x36, 0x49, 0x55, 0x22, 0x50, 0x00} // 26 &
  ,{0x00, 0x05, 0x03, 0x00, 0x00, 0x00} // 27 '
  ,{0x00, 0x1c, 0x22, 0x41, 0x00, 0x00} // 28 (
  ,{0x00, 0x41, 0x22, 0x1c, 0x00, 0x00} // 29 )
  ,{0x14, 0x08, 0x3e, 0x08, 0x14, 0x00} // 2a *
  ,{0x08, 0x08, 0x3e, 0x08, 0x08, 0x00} // 2b +
  ,{0x00, 0x50, 0x30, 0x00, 0x00, 0x00} // 2c ,
  ,{0x08, 0x08, 0x08, 0x08, 0x08, 0x00} // 2d -
  ,{0x00, 0x60, 0x60, 0x00, 0x00, 0x00} // 2e .
  ,{0x20, 0x10, 0x08, 0x04, 0x02, 0x00} // 2f /
  ,{0x3e, 0x51, 0x49, 0x45, 0x3e, 0x00} // 30 0
  ,{0x00, 0x42, 0x7f, 0x40, 0x00, 0x00} // 31 1
  ,{0x42, 0x61, 0x51, 0x49, 0x46, 0x00} // 32 2
  ,{0x21, 0x41, 0x45, 0x4b, 0x31, 0x00} // 33 3
  ,{0x18, 0x14, 0x12, 0x7f, 0x10, 0x00} // 34 4
  ,{0x27, 0x45, 0x45, 0x45, 0x39, 0x00} // 35 5
  ,{0x3c, 0x4a, 0x49, 0x49, 0x30, 0x00} // 36 6
  ,{0x01, 0x71, 0x09, 0x05, 0x03, 0x00} // 37 7
  ,{0x36, 0x49, 0x49, 0x49, 0x36, 0x00} // 38 8
  ,{0x06, 0x49, 0x49, 0x29, 0x1e, 0x00} // 39 9
  ,{0x00, 0x36, 0x36, 0x00, 0x00, 0x00} // 3a :
  ,{0x00, 0x56, 0x36, 0x00, 0x00, 0x00} // 3b ;
  ,{0x08, 0x14, 0x22, 0x41, 0x00, 0x00} // 3c <
  ,{0x14, 0x14, 0x14, 0x14, 0x14, 0x00} // 3d =
  ,{0x00, 0x41, 0x22, 0x14, 0x08, 0x00} // 3e >
  ,{0x02, 0x01, 0x51, 0x09, 0x06, 0x00} // 3f ?
  ,{0x32, 0x49, 0x79, 0x41, 0x3e, 0x00} // 40 @
  ,{0x7e, 0x11, 0x11, 0x11, 0x7e, 0x00} // 41 A
  ,{0x7f, 0x49, 0x49, 0x49, 0x36, 0x00} // 42 B
  ,{0x3e, 0x41, 0x41, 0x41, 0x22, 0x00} // 43 C
  ,{0x7f, 0x41, 0x41, 0x22, 0x1c, 0x00} // 44 D
  ,{0x7f, 0x49, 0x49, 0x49, 0x41, 0x00} // 45 E
  ,{0x7f, 0x09, 0x09, 0x09, 0x01, 0x00} // 46 F
  ,{0x3e, 0x41, 0x49, 0x49, 0x7a, 0x00} // 47 G
  ,{0x7f, 0x08, 0x08, 0x08, 0x7f, 0x00} // 48 H
  ,{0x00, 0x41, 0x7f, 0x41, 0x00, 0x00} // 49 I
  ,{0x20, 0x40, 0x41, 0x3f, 0x01, 0x00} // 4a J
  ,{0x7f, 0x08, 0x14, 0x22, 0x41, 0x00} // 4b K
  ,{0x7f, 0x40, 0x40, 0x40, 0x40, 0x00} // 4c L
  ,{0x7f, 0x02, 0x0c, 0x02, 0x7f, 0x00} // 4d M
  ,{0x7f, 0x04, 0x08, 0x10, 0x7f, 0x00} // 4e N
  ,{0x3e, 0x41, 0x41, 0x41, 0x3e, 0x00} // 4f O
  ,{0x7f, 0x09, 0x09, 0x09, 0x06, 0x00} // 50 P
  ,{0x3e, 0x41, 0x51, 0x21, 0x5e, 0x00} // 51 Q
  ,{0x7f, 0x09, 0x19, 0x29, 0x46, 0x00} // 52 R
  ,{0x46, 0x49, 0x49, 0x49, 0x31, 0x00} // 53 S
  ,{0x01, 0x01, 0x7f, 0x01, 0x01, 0x00} // 54 T
  ,{0x3f, 0x40, 0x40, 0x40, 0x3f, 0x00} // 55 U
  ,{0x1f, 0x20, 0x40, 0x20, 0x1f, 0x00} // 56 V
  ,{0x3f, 0x40, 0x38, 0x40, 0x3f, 0x00} // 57 W
  ,{0x63, 0x14, 0x08, 0x14, 0x63, 0x00} // 58 X
  ,{0x07, 0x08, 0x70, 0x08, 0x07, 0x00} // 59 Y
  ,{0x61, 0x51, 0x49, 0x45, 0x43, 0x00} // 5a Z
  ,{0x00, 0x7f, 0x41, 0x41, 0x00, 0x00} // 5b [
  ,{0x02, 0x04, 0x08, 0x10, 0x20, 0x00} // 5c '\'
  ,{0x00, 0x41, 0x41, 0x7f, 0x00, 0x00} // 5d ]
  ,{0x04, 0x02, 0x01, 0x02, 0x04, 0x00} // 5e ^
  ,{0x40, 0x40, 0x40, 0x40, 0x40, 0x00} // 5f _
  ,{0x00, 0x01, 0x02, 0x04, 0x00, 0x00} // 60 `
  ,{0x20, 0x54, 0x54, 0x54, 0x78, 0x00} // 61 a
  ,{0x7f, 0x48, 0x44, 0x44, 0x38, 0x00} // 62 b
  ,{0x38, 0x44, 0x44, 0x44, 0x20, 0x00} // 63 c
  ,{0x38, 0x44, 0x44, 0x48, 0x7f, 0x00} // 64 d
  ,{0x38, 0x54, 0x54, 0x54, 0x18, 0x00} // 65 e
  ,{0x08, 0x7e, 0x09, 0x01, 0x02, 0x00} // 66 f
  ,{0x0c, 0x52, 0x52, 0x52, 0x3e, 0x00} // 67 g
  ,{0x7f, 0x08, 0x04, 0x04, 0x78, 0x00} // 68 h
  ,{0x00, 0x44, 0x7d, 0x40, 0x00, 0x00} // 69 i
  ,{0x20, 0x40, 0x44, 0x3d, 0x00, 0x00} // 6a j
  ,{0x7f, 0x10, 0x28, 0x44, 0x00, 0x00} // 6b k
  ,{0x00, 0x41, 0x7f, 0x40, 0x00, 0x00} // 6c l
  ,{0x7c, 0x04, 0x18, 0x04, 0x78, 0x00} // 6d m
  ,{0x7c, 0x08, 0x04, 0x04, 0x78, 0x00} // 6e n
  ,{0x38, 0x44, 0x44, 0x44, 0x38, 0x00} // 6f o
  ,{0x7c, 0x14, 0x14, 0x14, 0x08, 0x00} // 70 p
  ,{0x08, 0x14, 0x14, 0x18, 0x7c, 0x00} // 71 q
  ,{0x7c, 0x08, 0x04, 0x04, 0x08, 0x00} // 72 r
  ,{0x48, 0x54, 0x54, 0x54, 0x20, 0x00} // 73 s
  ,{0x04, 0x3f, 0x44, 0x40, 0x20, 0x00} // 74 t
  ,{0x3c, 0x40, 0x40, 0x20, 0x7c, 0x00} // 75 u
  ,{0x1c, 0x20, 0x40, 0x20, 0x1c, 0x00} // 76 v
  ,{0x3c, 0x40, 0x30, 0x40, 0x3c, 0x00} // 77 w
  ,{0x44, 0x28, 0x10, 0x28, 0x44, 0x00} // 78 x
  ,{0x0c, 0x50, 0x50, 0x50, 0x3c, 0x00} // 79 y
  ,{0x44, 0x64, 0x54, 0x4c, 0x44, 0x00} // 7a z
  ,{0x00, 0x08, 0x36, 0x41, 0x00, 0x00} // 7b {
  ,{0x00, 0x00, 0x7f, 0x00, 0x00, 0x00} // 7c |
  ,{0x00, 0x41, 0x36, 0x08, 0x00, 0x00} // 7d }
  ,{0x10, 0x08, 0x08, 0x10, 0x08, 0x00}, // 7e ~
//  ,{0x78, 0x46, 0x41, 0x46, 0x78, 0x00} // 7f DEL
  {0x1f, 0x24, 0x7c, 0x24, 0x1f, 0x00}, // 7f UT sign
  {0x1E, 0xA1, 0xA1, 0x61, 0x12, 0x00},  // char =  128
  {0x3A, 0x40, 0x40, 0x20, 0x7A, 0x00},  // char =  129
  {0x38, 0x54, 0x54, 0x55, 0x59, 0x00},  // char =  130
  {0x21, 0x55, 0x55, 0x79, 0x41, 0x00},  // char =  131
  {0x21, 0x54, 0x54, 0x78, 0x41, 0x00},  // char =  132
  {0x21, 0x55, 0x54, 0x78, 0x40, 0x00},  // char =  133
  {0x20, 0x54, 0x55, 0x79, 0x40, 0x00},  // char =  134
  {0x0C, 0x1E, 0x52, 0x72, 0x12, 0x00},  // char =  135
  {0x39, 0x55, 0x55, 0x55, 0x59, 0x00},  // char =  136
  {0x39, 0x54, 0x54, 0x54, 0x59, 0x00},  // char =  137
  {0x39, 0x55, 0x54, 0x54, 0x58, 0x00},  // char =  138
  {0x00, 0x00, 0x45, 0x7C, 0x41, 0x00},  // char =  139
  {0x00, 0x02, 0x45, 0x7D, 0x42, 0x00},  // char =  140
  {0x00, 0x01, 0x45, 0x7C, 0x40, 0x00},  // char =  141
  {0xF0, 0x29, 0x24, 0x29, 0xF0, 0x00},  // char =  142
  {0xF0, 0x28, 0x25, 0x28, 0xF0, 0x00},  // char =  143
  {0x7C, 0x54, 0x55, 0x45, 0x00, 0x00},  // char =  144
  {0x20, 0x54, 0x54, 0x7C, 0x54, 0x00},  // char =  145
  {0x7C, 0x0A, 0x09, 0x7F, 0x49, 0x00},  // char =  146
  {0x32, 0x49, 0x49, 0x49, 0x32, 0x00},  // char =  147
  {0x32, 0x48, 0x48, 0x48, 0x32, 0x00},  // char =  148
  {0x32, 0x4A, 0x48, 0x48, 0x30, 0x00},  // char =  149
  {0x3A, 0x41, 0x41, 0x21, 0x7A, 0x00},  // char =  150
  {0x3A, 0x42, 0x40, 0x20, 0x78, 0x00},  // char =  151
  {0x00, 0x9D, 0xA0, 0xA0, 0x7D, 0x00},  // char =  152
  {0x39, 0x44, 0x44, 0x44, 0x39, 0x00},  // char =  153
  {0x3D, 0x40, 0x40, 0x40, 0x3D, 0x00},  // char =  154
  {0x3C, 0x24, 0xFF, 0x24, 0x24, 0x00},  // char =  155
  {0x48, 0x7E, 0x49, 0x43, 0x66, 0x00},  // char =  156
  {0x2B, 0x2F, 0xFC, 0x2F, 0x2B, 0x00},  // char =  157
  {0xFF, 0x09, 0x29, 0xF6, 0x20, 0x00},  // char =  158
  {0xC0, 0x88, 0x7E, 0x09, 0x03, 0x00},  // char =  159
  {0x20, 0x54, 0x54, 0x79, 0x41, 0x00},  // char =  160
  {0x00, 0x00, 0x44, 0x7D, 0x41, 0x00},  // char =  161
  {0x30, 0x48, 0x48, 0x4A, 0x32, 0x00},  // char =  162
  {0x38, 0x40, 0x40, 0x22, 0x7A, 0x00},  // char =  163
  {0x00, 0x7A, 0x0A, 0x0A, 0x72, 0x00},  // char =  164
  {0x7D, 0x0D, 0x19, 0x31, 0x7D, 0x00},  // char =  165
  {0x26, 0x29, 0x29, 0x2F, 0x28, 0x00},  // char =  166
  {0x26, 0x29, 0x29, 0x29, 0x26, 0x00},  // char =  167
  {0x30, 0x48, 0x4D, 0x40, 0x20, 0x00},  // char =  168
  {0x38, 0x08, 0x08, 0x08, 0x08, 0x00},  // char =  169
  {0x08, 0x08, 0x08, 0x08, 0x38, 0x00},  // char =  170
  {0x2F, 0x10, 0xC8, 0xAC, 0xBA, 0x00},  // char =  171
  {0x2F, 0x10, 0x28, 0x34, 0xFA, 0x00},  // char =  172
  {0x00, 0x00, 0x7B, 0x00, 0x00, 0x00},  // char =  173
  {0x08, 0x14, 0x2A, 0x14, 0x22, 0x00},  // char =  174
  {0x22, 0x14, 0x2A, 0x14, 0x08, 0x00},  // char =  175
  {0xAA, 0x00, 0x55, 0x00, 0xAA, 0x00},  // char =  176
  {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x00},  // char =  177
  {0x00, 0x00, 0x00, 0xFF, 0x00, 0x00},  // char =  178
  {0x10, 0x10, 0x10, 0xFF, 0x00, 0x00},  // char =  179
  {0x14, 0x14, 0x14, 0xFF, 0x00, 0x00},  // char =  180
  {0x10, 0x10, 0xFF, 0x00, 0xFF, 0x00},  // char =  181
  {0x10, 0x10, 0xF0, 0x10, 0xF0, 0x00},  // char =  182
  {0x14, 0x14, 0x14, 0xFC, 0x00, 0x00},  // char =  183
  {0x14, 0x14, 0xF7, 0x00, 0xFF, 0x00},  // char =  184
  {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00},  // char =  185
  {0x14, 0x14, 0xF4, 0x04, 0xFC, 0x00},  // char =  186
  {0x14, 0x14, 0x17, 0x10, 0x1F, 0x00},  // char =  187
  {0x10, 0x10, 0x1F, 0x10, 0x1F, 0x00},  // char =  188
  {0x14, 0x14, 0x14, 0x1F, 0x00, 0x00},  // char =  189
  {0x10, 0x10, 0x10, 0xF0, 0x00, 0x00},  // char =  190
  {0x00, 0x00, 0x00, 0x1F, 0x10, 0x00},  // char =  191
  {0x10, 0x10, 0x10, 0x1F, 0x10, 0x00},  // char =  192
  {0x10, 0x10, 0x10, 0xF0, 0x10, 0x00},  // char =  193
  {0x00, 0x00, 0x00, 0xFF, 0x10, 0x00},  // char =  194
  {0x10, 0x10, 0x10, 0x10, 0x10, 0x00},  // char =  195
  {0x10, 0x10, 0x10, 0xFF, 0x10, 0x00},  // char =  196
  {0x00, 0x00, 0x00, 0xFF, 0x14, 0x00},  // char =  197
  {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00},  // char =  198
  {0x00, 0x00, 0x1F, 0x10, 0x17, 0x00},  // char =  199
  {0x00, 0x00, 0xFC, 0x04, 0xF4, 0x00},  // char =  200
  {0x14, 0x14, 0x17, 0x10, 0x17, 0x00},  // char =  201
  {0x14, 0x14, 0xF4, 0x04, 0xF4, 0x00},  // char =  202
  {0x00, 0x00, 0xFF, 0x00, 0xF7, 0x00},  // char =  203
  {0x14, 0x14, 0x14, 0x14, 0x14, 0x00},  // char =  204
  {0x14, 0x14, 0xF7, 0x00, 0xF7, 0x00},  // char =  205
  {0x14, 0x14, 0x14, 0x17, 0x14, 0x00},  // char =  206
  {0x10, 0x10, 0x1F, 0x10, 0x1F, 0x00},  // char =  207
  {0x14, 0x14, 0x14, 0xF4, 0x14, 0x00},  // char =  208
  {0x10, 0x10, 0xF0, 0x10, 0xF0, 0x00},  // char =  209
  {0x00, 0x00, 0x1F, 0x10, 0x1F, 0x00},  // char =  210
  {0x00, 0x00, 0x00, 0x1F, 0x14, 0x00},  // char =  211
  {0x00, 0x00, 0x00, 0xFC, 0x14, 0x00},  // char =  212
  {0x00, 0x00, 0xF0, 0x10, 0xF0, 0x00},  // char =  213
  {0x10, 0x10, 0xFF, 0x10, 0xFF, 0x00},  // char =  214
  {0x14, 0x14, 0x14, 0xFF, 0x14, 0x00},  // char =  215
  {0x10, 0x10, 0x10, 0x1F, 0x00, 0x00},  // char =  216
  {0x00, 0x00, 0x00, 0xF0, 0x10, 0x00},  // char =  217
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00},  // char =  218
  {0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x00},  // char =  219
  {0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00},  // char =  220
  {0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00},  // char =  221
  {0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x00},  // char =  222
  {0x38, 0x44, 0x44, 0x38, 0x44, 0x00},  // char =  223
  {0x7C, 0x2A, 0x2A, 0x3E, 0x14, 0x00},  // char =  224
  {0x7E, 0x02, 0x02, 0x06, 0x06, 0x00},  // char =  225
  {0x02, 0x7E, 0x02, 0x7E, 0x02, 0x00},  // char =  226
  {0x63, 0x55, 0x49, 0x41, 0x63, 0x00},  // char =  227
  {0x38, 0x44, 0x44, 0x3C, 0x04, 0x00},  // char =  228
  {0x40, 0x7E, 0x20, 0x1E, 0x20, 0x00},  // char =  229
  {0x06, 0x02, 0x7E, 0x02, 0x02, 0x00},  // char =  230
  {0x99, 0xA5, 0xE7, 0xA5, 0x99, 0x00},  // char =  231
  {0x1C, 0x2A, 0x49, 0x2A, 0x1C, 0x00},  // char =  232
  {0x4C, 0x72, 0x01, 0x72, 0x4C, 0x00},  // char =  233
  {0x30, 0x4A, 0x4D, 0x4D, 0x30, 0x00},  // char =  234
  {0x30, 0x48, 0x78, 0x48, 0x30, 0x00},  // char =  235
  {0xBC, 0x62, 0x5A, 0x46, 0x3D, 0x00},  // char =  236
  {0x3E, 0x49, 0x49, 0x49, 0x00, 0x00},  // char =  237
  {0x7E, 0x01, 0x01, 0x01, 0x7E, 0x00},  // char =  238
  {0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x00},  // char =  239
  {0x44, 0x44, 0x5F, 0x44, 0x44, 0x00},  // char =  240
  {0x40, 0x51, 0x4A, 0x44, 0x40, 0x00},  // char =  241
  {0x40, 0x44, 0x4A, 0x51, 0x40, 0x00},  // char =  242
  {0x00, 0x00, 0xFF, 0x01, 0x03, 0x00},  // char =  243
  {0xE0, 0x80, 0xFF, 0x00, 0x00, 0x00},  // char =  244
  {0x08, 0x08, 0x6B, 0x6B, 0x08, 0x00},  // char =  245
  {0x36, 0x12, 0x36, 0x24, 0x36, 0x00},  // char =  246
  {0x06, 0x0F, 0x09, 0x0F, 0x06, 0x00},  // char =  247
  {0x00, 0x00, 0x18, 0x18, 0x00, 0x00},  // char =  248
  {0x00, 0x00, 0x10, 0x10, 0x00, 0x00},  // char =  249
  {0x30, 0x40, 0xFF, 0x01, 0x01, 0x00},  // char =  250
  {0x00, 0x1F, 0x01, 0x01, 0x1E, 0x00},  // char =  251
  {0x00, 0x19, 0x1D, 0x17, 0x12, 0x00},  // char =  252
  {0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x00},  // char =  253
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // char =  254
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}   // char =  255
};
#define ssd1306_swap(a, b) \
(((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation




// This is a helper function that sends 8-bit commands to the LCD.
// Inputs: command  8-bit function code to transmit
// Outputs: none
// Assumes: UCA3 and Port 9 have already been initialized and enabled
// The Data/Command pin must be valid when the eighth bit is
// sent.  The eUSCI module has no hardware FIFOs.
// 1) Wait for SPI to be idle (let previous frame finish)
// 2) Set DC for command (0)
// 3) Write command to TXBUF, starts SPI
// 4) Wait for SPI to be idle (after transmission complete)
//void static commandwrite(uint8_t command){
////  volatile uint8_t dummy;
//  while((EUSCI_A3->STATW&0x0001)==0x0001){};   // UCBUSY, wait until SPI not busy
//  DC = 0;
//  EUSCI_A3->TXBUF = command;                   // command out
//  while((EUSCI_A3->STATW&0x0001)==0x0001){};   // UCBUSY, wait until SPI not busy
////  dummy = UCA3->RXBUF;                         // response meaningless here
//}
//void ssd1306_Testcommandwrite(void){
//  while(1){
//    commandwrite(0x21);
//  }
//}
// This is a helper function that sends 8-bit data to the LCD.
// Inputs: data  8-bit data to transmit
// Outputs: none
// Assumes: SPI have already been initialized and enabled
// The Data/Command pin must be valid when the eighth bit is
// sent.  The SPI module has no hardware FIFOs.
// 1) Wait for transmitter to be empty (let previous frame finish)
// 2) Set DC for data (1)
// 3) Write data to TXBUF, starts SPI
// Note: takes 2us to output a byte
//void static datawrite(uint8_t data){//SPI version
////  volatile uint8_t dummy;
//  while((EUSCI_A3->IFG&0x0002)==0x0000){};     // wait until UCA3TXBUF empty
//  DC = 1;
//  EUSCI_A3->TXBUF = data;                      // data out
////  dummy = UCA3->RXBUF;                         // response meaningless here
//}
// 8000000 bps, msb first, mode0

void ssd1306command(uint8_t c) {
//  commandwrite(c);
  I2C_Send2(SSD1306ADDR,0,c);
}
void ssd1306command1(uint8_t c) {
//  commandwrite(c);
  I2C_Send2(SSD1306ADDR,0,c);

}
void ssd1306commandList(const uint8_t *c, uint32_t n) {
  // added 0 in front of each list
  I2C_Send(SSD1306ADDR,(uint8_t *)c,n);
//  while(n--) {
//    commandwrite(*c);
//    c++;
//  }
  
}

// ALLOCATE & INIT DISPLAY -------------------------------------------------

/*!
    @brief  Allocate RAM for image buffer, initialize peripherals and pins.
    @param  vcs
            VCC selection. Pass SSD1306_SWITCHCAPVCC to generate the display
            voltage (step up) from the 3.3V source, or SSD1306_EXTERNALVCC
            otherwise. Most situations with Adafruit SSD1306 breakouts will
            want SSD1306_SWITCHCAPVCC.
    @param  addr
            I2C address of corresponding SSD1306 display (or pass 0 to use
            default of 0x3C for 128x32 display, 0x3D for all others).
            SPI displays (hardware or software) do not use addresses, but
            this argument is still required (pass 0 or any value really,
            it will simply be ignored). Default if unspecified is 0.
    @param  reset
            If true, and if the reset pin passed to the constructor is
            valid, a hard reset will be performed before initializing the
            display. If using multiple SSD1306 displays on the same bus, and
            if they all share the same reset pin, you should only pass true
            on the first display being initialized, false on all others,
            else the already-initialized displays would be reset. Default if
            unspecified is true.
    @param  periphBegin
            If true, and if a hardware peripheral is being used (I2C or SPI,
            but not software SPI), call that peripheral's begin() function,
            else (false) it has already been done in one's sketch code.
            Cases where false might be used include multiple displays or
            other devices sharing a common bus, or situations on some
            platforms where a nonstandard begin() function is available
            (e.g. a TwoWire interface on non-default pins, as can be done
            on the ESP8266 and perhaps others).
    @return true on successful allocation/init, false otherwise.
            Well-behaved code should check the return value before
            proceeding.
    @note   MUST call this function before any drawing or updates!
*/
// Init sequence, added 0 for command
static const uint8_t init1[] = {
  0, // D/C = 0 for command
  SSD1306_DISPLAYOFF,                   // 0xAE
  SSD1306_SETDISPLAYCLOCKDIV,           // 0xD5
  0x80,                                 // the suggested ratio 0x80
  SSD1306_SETMULTIPLEX };               // 0xA8
static const uint8_t init2[] = {
  0, // D/C = 0 for command
  SSD1306_SETDISPLAYOFFSET,             // 0xD3
  0x0,                                  // no offset
  SSD1306_SETSTARTLINE | 0x0,           // line #0
  SSD1306_CHARGEPUMP };                 // 0x8D
static const uint8_t init3[] = {
  0, // D/C = 0 for command
  SSD1306_MEMORYMODE,                   // 0x20
  0x00,                                 // 0x0 act like Nokia 5110
  SSD1306_SEGREMAP | 0x1,
  SSD1306_COMSCANDEC };
static const uint8_t init4b[] = {
  0, // D/C = 0 for command
  SSD1306_SETCOMPINS,                 // 0xDA
  0x12,
  SSD1306_SETCONTRAST };              // 0x81
static const uint8_t init5[] = {
  0, // D/C = 0 for command
  SSD1306_SETVCOMDETECT,               // 0xDB
  0x40,
  SSD1306_DISPLAYALLON_RESUME,         // 0xA4
  SSD1306_NORMALDISPLAY,               // 0xA6
  SSD1306_DEACTIVATE_SCROLL,
  SSD1306_DISPLAYON };                 // Main screen turn on
// vccstate = SSD1306_SWITCHCAPVCC for normal internal 3.3V power
//            SSD1306_EXTERNALVCC for separate external power
int SSD1306_Init(int vccst) {
//  volatile uint32_t delay;
  Clock_Delay1ms(300);
  I2C_Init(400,80000); // 100kHz
  vccstate = vccst;
//  RESET = 0;                            // reset the LCD to a known state, RESET low
//  for(delay=0; delay<10; delay=delay+1);// delay minimum 100 ns
//  RESET = 1;                            // hold RESET high
  SSD1306_ClearBuffer();
  ssd1306commandList(init1, sizeof(init1));
  ssd1306command1(HEIGHT - 1);
  ssd1306commandList(init2, sizeof(init2));
  ssd1306command1((vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0x14);
  ssd1306commandList(init3, sizeof(init3));
  ssd1306commandList(init4b, sizeof(init4b));
  ssd1306command1((vccstate == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF);
  ssd1306command1(SSD1306_SETPRECHARGE); // 0xd9
  ssd1306command1((vccstate == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1);
  ssd1306commandList(init5, sizeof(init5));
  SSD1306_OutBuffer();
	SSD1306_SetCursor(0, 0);
  return true; // Success
}

// DRAWING FUNCTIONS -------------------------------------------------------

/*!
    @brief  Set/clear/invert a single pixel. This is also invoked by the
            Adafruit_GFX library in generating many higher-level graphics
            primitives.
    @param  x
            Column of display -- 0 at left to (screen width - 1) at right.
    @param  y
            Row of display -- 0 at top to (screen height - 1) at bottom.
    @param  color
            Pixel color, one of: SSD1306_BLACK, SSD1306_WHITE or SSD1306_INVERSE.
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to SSD1306_OutBuffer(), or with other
            graphics commands as needed by one's own application.
*/
void SSD1306_DrawPixel(int16_t x, int16_t y, uint16_t color) {
  if((x >= 0) && (x < WIDTH) && (y >= 0) && (y < HEIGHT)) {
    // Pixel is in-bounds. Rotate coordinates if needed.
    switch(rotation) {
     case 1:
      ssd1306_swap(x, y);
      x = WIDTH - x - 1;
      break;
     case 2:
      x = WIDTH  - x - 1;
      y = HEIGHT - y - 1;
      break;
     case 3:
      ssd1306_swap(x, y);
      y = HEIGHT - y - 1;
      break;
    }
    switch(color) {
     case SSD1306_WHITE:   buffer[x + (y/8)*WIDTH] |=  (1 << (y&7)); break;
     case SSD1306_BLACK:   buffer[x + (y/8)*WIDTH] &= ~(1 << (y&7)); break;
     case SSD1306_INVERSE: buffer[x + (y/8)*WIDTH] ^=  (1 << (y&7)); break;
    }
  }
}
static int32_t DeltaX,MaxX,MinX; // MaxX must be greater than MinX
static int32_t DeltaY,MaxY,MinY; // MaxY must be greater than MinY
static uint16_t PlotColor;
void SSD1306_SetPlot(int32_t minX, int32_t maxX, int32_t minY, int32_t maxY, uint16_t color){
  if(minX >= maxX) return;// MaxX must be greater than MinX
  if(minY >= maxY) return;// MaxY must be greater than MinY
  if(color > SSD1306_INVERSE) return;
  MaxX = maxX;
  MinX = minX; 
  DeltaX = MaxX-MinX;
  MaxY = maxY;
  MinY = minY;
  DeltaY = MaxY-MinY;
  PlotColor = color;
}
void SSD1306_DrawPoint(int32_t x, int32_t y){
  if((x<MinX)||(x>MaxX)||(y<MinY)||(y>MaxY)) return; // out of bounds
  SSD1306_DrawPixel(((x-MinX)*(WIDTH-1))/DeltaX,
                    ((MaxY-y)*(HEIGHT-1))/DeltaY,PlotColor);
  //MaxX at right
  //MaxY at top
}
// color is white or black, not invert
void SSD1306_DrawChar(int16_t x, int16_t y, char letter, uint16_t color){int mask;
uint16_t dot;
	if(letter < 0x20) return;
  for(int i=0;i<5;i++){
    mask = 0x01;
    for(int j=0; j<7; j++){
      if(ASCII[letter-0x20][i]&mask){
        dot = color;
      }else{
        dot = color^1;
      }
      SSD1306_DrawPixel(x+i,y+j,dot);
      mask=mask<<1;
    }
  }
}
// color is white or black, not invert
void SSD1306_DrawString(int16_t x, int16_t y, char *pt, uint16_t color){
  while(*pt){
    SSD1306_DrawChar(x,y,*pt,color);
    x = x+6; // skip a line
    pt++;
  }
}

/*!
    @brief  Clear contents of display buffer (set all pixels to off).
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to SSD1306_OutBuffer(), or with other
            graphics commands as needed by one's own application.
*/
void SSD1306_ClearBuffer(void) {int i;
  for(i=0; i<WIDTH*HEIGHT/8; i++){
    buffer[i] = 0;
  }
  SSD1306_SetCursor(0, 0);
}

/*!
    @brief  Clear the LCD by writing zeros to the entire screen and
            reset the cursor to (0,0) (top left corner of screen).
    @return None (void).
*/
void SSD1306_OutClear(void) {
  //int i;
  // clear the entire screen, even any columns on the right that did not contain text
//  ssd1306command(SSD1306_PAGEADDR);
//  ssd1306command(0);                   // page start address
//  ssd1306command((HEIGHT/8) - 1);      // page end (bottom of screen)
//  ssd1306command(SSD1306_COLUMNADDR);
//  ssd1306command(0);                   // column start address
//  ssd1306command(WIDTH - 1);           // column end address
//  for(i=0; i<WIDTH*HEIGHT/8; i++){
//    datawrite(0x00);//SPI version
//  }
	SSD1306_ClearBuffer();
	SSD1306_OutBuffer();
  
}



/*!
    @brief  Draw a raw bitmap to the RAM buffer.
    @param  xpos      Horizontal position of bottom left corner of image, columns from the left edge.<br>
                        must be less than 128<br>
                        0 is on the left; 126 is near the right
    @param  ypos      Vertical position of bottom left corner of image, rows from the top edge.<br>
                        must be less than 64<br>
                        2 is near the top; 63 is at the bottom
    @param  ptr       Pointer to a 16-color BMP image.
    @param  threshold Grayscale colors above this number make corresponding pixel 'on'.<br>
                        0 to 14<br>
                        0 is fine for ships, explosions, projectiles, and bunkers
    @param  color
            Pixel color, one of: SSD1306_BLACK, SSD1306_WHITE or SSD1306_INVERSE.
    @return None (void).
    @note   Bitmaps defined above were created for the LM3S1968 or
            LM3S8962's 4-bit grayscale OLED display.  More recently
            they are created using Microsoft Paint (or your
            favorite drawing program), saved as a 16-color bitmap,
            and converted from binary to text using BmpConvert.exe.
            Then copy and paste this text into the program, which
            will create a constant containing the image that will
            be loaded into ROM at compile time.  These images will
            still contain their header data and may contain padding
            to preserve 4-byte alignment.  This function takes a
            bitmap in the previously described format and puts its
            image data in the proper location in the buffer so the
            image will appear on the screen after the next call to<br>
              SSD1306_OutBuffer();<br>
            The interface and operation of this process is modeled
            after RIT128x96x4_BMP(x, y, image);
*/
void SSD1306_DrawBMP(uint8_t xpos, uint8_t ypos, const uint8_t *ptr, uint8_t threshold, uint16_t color){
  int32_t width = ptr[18], height = ptr[22], i, j;
  uint16_t screenx, screeny;
  uint8_t mask;
  // check for clipping
  if((height <= 0) ||              // bitmap is unexpectedly encoded in top-to-bottom pixel order
     ((width%2) != 0) ||           // must be even number of columns
     ((xpos + width) > WIDTH) ||   // right side cut off
     (ypos < (height - 1)) ||      // top cut off
     (ypos > HEIGHT)){             // bottom cut off
    return;
  }
  if(threshold > 14){
    threshold = 14;             // only full 'on' turns pixel on
  }
  // bitmaps are encoded backwards, so start at the bottom left corner of the image
  screeny = ypos/8;
  screenx = xpos + WIDTH*screeny;
  mask = ypos%8;                // row 0 to 7
  mask = 0x01<<mask;            // now stores a mask 0x01 to 0x80
  j = ptr[10];                  // byte 10 contains the offset where image data can be found
  for(i=1; i<=(width*height/2); i=i+1){
    // the left pixel is in the upper 4 bits
    if(((ptr[j]>>4)&0xF) > threshold){
      switch(color) {
        case SSD1306_WHITE:   buffer[screenx] |= mask; break;
        case SSD1306_BLACK:   buffer[screenx] &= ~mask; break;
        case SSD1306_INVERSE: buffer[screenx] ^= mask; break;
      }      
    } else{
      switch(color) {
        case SSD1306_WHITE:   buffer[screenx] &= ~mask; break;
        case SSD1306_BLACK:   buffer[screenx] |= mask; break;
        case SSD1306_INVERSE:  break;
      }      
    }
    screenx = screenx + 1;
    // the right pixel is in the lower 4 bits
    if((ptr[j]&0xF) > threshold){
      buffer[screenx] |= mask;
    } else{
      buffer[screenx] &= ~mask;
    }
    screenx = screenx + 1;
    j = j + 1;
    if((i%(width/2)) == 0){     // at the end of a row
      if(mask > 0x01){
        mask = mask>>1;
      } else{
        mask = 0x80;
        screeny = screeny - 1;
      }
      screenx = xpos + WIDTH*screeny;
      // bitmaps are 32-bit word aligned
      switch((width/2)%4){      // skip any padding
        case 0: j = j + 0; break;
        case 1: j = j + 3; break;
        case 2: j = j + 2; break;
        case 3: j = j + 1; break;
      }
    }
  }
}

/*!
    @brief  Draw a horizontal line. This is also invoked by the Adafruit_GFX
            library in generating many higher-level graphics primitives.
    @param  x
            Leftmost column -- 0 at left to (screen width - 1) at right.
    @param  y
            Row of display -- 0 at top to (screen height -1) at bottom.
    @param  w
            Width of line, in pixels.
    @param  color
            Line color, one of: SSD1306_BLACK, SSD1306_WHITE or SSD1306_INVERT.
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to SSD1306_OutBuffer(), or with other
            graphics commands as needed by one's own application.
*/
void SSD1306_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  int bSwap = false;
  switch(rotation) {
   case 1:
    // 90 degree rotation, swap x & y for rotation, then invert x
    bSwap = true;
    ssd1306_swap(x, y);
    x = WIDTH - x - 1;
    break;
   case 2:
    // 180 degree rotation, invert x and y, then shift y around for height.
    x  = WIDTH  - x - 1;
    y  = HEIGHT - y - 1;
    x -= (w-1);
    break;
   case 3:
    // 270 degree rotation, swap x & y for rotation,
    // then invert y and adjust y for w (not to become h)
    bSwap = true;
    ssd1306_swap(x, y);
    y  = HEIGHT - y - 1;
    y -= (w-1);
    break;
  }

  if(bSwap) ssd1306drawFastVLineInternal(x, y, w, color);
  else      ssd1306drawFastHLineInternal(x, y, w, color);
}

void ssd1306drawFastHLineInternal(
  int16_t x, int16_t y, int16_t w, uint16_t color) {

  if((y >= 0) && (y < HEIGHT)) { // Y coord in bounds?
    if(x < 0) { // Clip left
      w += x;
      x  = 0;
    }
    if((x + w) > WIDTH) { // Clip right
      w = (WIDTH - x);
    }
    if(w > 0) { // Proceed only if width is positive
      uint8_t *pBuf = &buffer[(y / 8) * WIDTH + x],
               mask = 1 << (y & 7);
      switch(color) {
       case SSD1306_WHITE:               while(w--) { *pBuf++ |= mask; }; break;
       case SSD1306_BLACK: mask = ~mask; while(w--) { *pBuf++ &= mask; }; break;
       case SSD1306_INVERSE:             while(w--) { *pBuf++ ^= mask; }; break;
      }
    }
  }
}

/*!
    @brief  Draw a vertical line. This is also invoked by the Adafruit_GFX
            library in generating many higher-level graphics primitives.
    @param  x
            Column of display -- 0 at left to (screen width -1) at right.
    @param  y
            Topmost row -- 0 at top to (screen height - 1) at bottom.
    @param  h
            Height of line, in pixels.
    @param  color
            Line color, one of: SSD1306_BLACK, SSD1306_WHITE or SSD1306_INVERT.
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to SSD1306_OutBuffer(), or with other
            graphics commands as needed by one's own application.
*/
void SSD1306_DrawFastVLine(
  int16_t x, int16_t y, int16_t h, uint16_t color) {
  int bSwap = false;
  switch(rotation) {
   case 1:
    // 90 degree rotation, swap x & y for rotation,
    // then invert x and adjust x for h (now to become w)
    bSwap = true;
    ssd1306_swap(x, y);
    x  = WIDTH - x - 1;
    x -= (h-1);
    break;
   case 2:
    // 180 degree rotation, invert x and y, then shift y around for height.
    x = WIDTH  - x - 1;
    y = HEIGHT - y - 1;
    y -= (h-1);
    break;
   case 3:
    // 270 degree rotation, swap x & y for rotation, then invert y
    bSwap = true;
    ssd1306_swap(x, y);
    y = HEIGHT - y - 1;
    break;
  }

  if(bSwap) ssd1306drawFastHLineInternal(x, y, h, color);
  else      ssd1306drawFastVLineInternal(x, y, h, color);
}

static const uint8_t premask[8] =
  { 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
static const uint8_t postmask[8] =
  { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
void ssd1306drawFastVLineInternal(
  int16_t x, int16_t __y, int16_t __h, uint16_t color) {

  if((x >= 0) && (x < WIDTH)) { // X coord in bounds?
    if(__y < 0) { // Clip top
      __h += __y;
      __y = 0;
    }
    if((__y + __h) > HEIGHT) { // Clip bottom
      __h = (HEIGHT - __y);
    }
    if(__h > 0) { // Proceed only if height is now positive
      // this display doesn't need ints for coordinates,
      // use local byte registers for faster juggling
      uint8_t  y = __y, h = __h;
      uint8_t *pBuf = &buffer[(y / 8) * WIDTH + x];

      // do the first partial byte, if necessary - this requires some masking
      uint8_t mod = (y & 7);
      if(mod) {
        // mask off the high n bits we want to set
        mod = 8 - mod;
        // note - lookup table results in a nearly 10% performance
        // improvement in fill* functions
        // uint8_t mask = ~(0xFF >> mod);

        uint8_t mask = premask[mod];
        // adjust the mask if we're not going to reach the end of this byte
        if(h < mod) mask &= (0XFF >> (mod - h));

        switch(color) {
         case SSD1306_WHITE:   *pBuf |=  mask; break;
         case SSD1306_BLACK:   *pBuf &= ~mask; break;
         case SSD1306_INVERSE: *pBuf ^=  mask; break;
        }
        pBuf += WIDTH;
      }

      if(h >= mod) { // More to go?
        h -= mod;
        // Write solid bytes while we can - effectively 8 rows at a time
        if(h >= 8) {
          if(color == SSD1306_INVERSE) {
            // separate copy of the code so we don't impact performance of
            // black/white write version with an extra comparison per loop
            do {
              *pBuf ^= 0xFF;  // Invert byte
              pBuf  += WIDTH; // Advance pointer 8 rows
              h     -= 8;     // Subtract 8 rows from height
            } while(h >= 8);
          } else {
            // store a local value to work with
            uint8_t val = (color != SSD1306_BLACK) ? 255 : 0;
            do {
              *pBuf = val;    // Set byte
              pBuf += WIDTH;  // Advance pointer 8 rows
              h    -= 8;      // Subtract 8 rows from height
            } while(h >= 8);
          }
        }

        if(h) { // Do the final partial byte, if necessary
          mod = h & 7;
          // this time we want to mask the low bits of the byte,
          // vs the high bits we did above
          // uint8_t mask = (1 << mod) - 1;
          // note - lookup table results in a nearly 10% performance
          // improvement in fill* functions

          uint8_t mask = postmask[mod];
          switch(color) {
           case SSD1306_WHITE:   *pBuf |=  mask; break;
           case SSD1306_BLACK:   *pBuf &= ~mask; break;
           case SSD1306_INVERSE: *pBuf ^=  mask; break;
          }
        }
      }
    } // endif positive height
  } // endif x in bounds
}

/*!
    @brief  Return color of a single pixel in display buffer.
    @param  x
            Column of display -- 0 at left to (screen width - 1) at right.
    @param  y
            Row of display -- 0 at top to (screen height -1) at bottom.
    @return true if pixel is set (usually WHITE, unless display invert mode
            is enabled), false if clear (BLACK).
    @note   Reads from buffer contents; may not reflect current contents of
            screen if SSD1306_OutBuffer() has not been called.
*/
int SSD1306_GetPixel(int16_t x, int16_t y) {
  if((x >= 0) && (x < WIDTH) && (y >= 0) && (y < HEIGHT)) {
    // Pixel is in-bounds. Rotate coordinates if needed.
    switch(rotation) {
     case 1:
      ssd1306_swap(x, y);
      x = WIDTH - x - 1;
      break;
     case 2:
      x = WIDTH  - x - 1;
      y = HEIGHT - y - 1;
      break;
     case 3:
      ssd1306_swap(x, y);
      y = HEIGHT - y - 1;
      break;
    }
    return (buffer[x + (y / 8) * WIDTH] & (1 << (y & 7)));
  }
  return false; // Pixel out of bounds
}

/*!
    @brief  Get base address of display buffer for direct reading or writing.
    @return Pointer to an unsigned 8-bit array, column-major, columns padded
            to full byte boundary if needed.
*/
uint8_t *SSD1306_GetBuffer(void) {
  return buffer;
}

// REFRESH DISPLAY ---------------------------------------------------------

/*!
    @brief  Push data currently in RAM to SSD1306 display.
    @return None (void).
    @note   Drawing operations are not visible until this function is
            called. Call after each graphics command, or after a whole set
            of graphics commands, as best needed by one's own application.
*/
static const uint8_t dlist1[] = {
  SSD1306_PAGEADDR,
  0,                         // Page start address
  0xFF,                      // Page end (not really, but works here)
  SSD1306_COLUMNADDR,
  0 };

  

void SSD1306_OutBuffer(void) {
  // Column start address
  ssd1306commandList(dlist1, sizeof(dlist1));
  ssd1306command1(WIDTH - 1); // Column end address
// SPI
//  uint16_t count = WIDTH*HEIGHT/8;
//  while(count--) datawrite(*ptr++);//SPI version
// I2C
  I2C_SendData(SSD1306ADDR,buffer,(WIDTH*HEIGHT/8)); // 1024 bytes

}
/*!
    @brief  Fill the whole screen by drawing a 128x64 bitmap image.
    @param  ptr  Pointer to 1024-byte image with no header or format
                 data.
    @return None (void).
    @note   Assumes: OLED is in horizontal addressing mode (command 0x20, 0x00)<br>
            Image is sent to screen directly, RAM buffer is unchanged.
*/
void SSD1306_DrawFullImage(const uint8_t *ptr){
//  int i;
  // over-write the entire screen, even any columns on the right that did not contain text
//  ssd1306command(SSD1306_PAGEADDR);
//  ssd1306command(0);                   // page start address
//  ssd1306command((HEIGHT/8) - 1);      // page end (bottom of screen)
//  ssd1306command(SSD1306_COLUMNADDR);
//  ssd1306command(0);                   // column start address
//  ssd1306command(WIDTH - 1);           // column end address
  ssd1306commandList(dlist1, sizeof(dlist1));
  ssd1306command1(WIDTH - 1); // Column end address
  I2C_SendData(SSD1306ADDR,(uint8_t *)ptr,(WIDTH*HEIGHT/8));

//  for(i=0; i<WIDTH*HEIGHT/8; i++){
//    datawrite(ptr[i]); //SPI version
//  }
  SSD1306_SetCursor(0, 0);
}

// SCROLLING FUNCTIONS -----------------------------------------------------

/*!
    @brief  Activate a right-handed scroll for all or part of the display.
    @param  start
            First row.
    @param  stop
            Last row.
    @return None (void).
*/
// To scroll the whole display, run: display.startscrollright(0x00, 0x0F)
static const uint8_t scrollList1a[] = {
  SSD1306_RIGHT_HORIZONTAL_SCROLL,
  0X00 };
static const uint8_t scrollList1b[] = {
  0X00,
  0XFF,
  SSD1306_ACTIVATE_SCROLL };
void SSD1306_startscrollright(uint8_t start, uint8_t stop) {

  ssd1306commandList(scrollList1a, sizeof(scrollList1a));
  ssd1306command1(start);
  ssd1306command1(0X00);
  ssd1306command1(stop);

  ssd1306commandList(scrollList1b, sizeof(scrollList1b));

}

/*!
    @brief  Activate a left-handed scroll for all or part of the display.
    @param  start
            First row.
    @param  stop
            Last row.
    @return None (void).
*/
// To scroll the whole display, run: display.startscrollleft(0x00, 0x0F)
static const uint8_t scrollList2a[] = {
  SSD1306_LEFT_HORIZONTAL_SCROLL,
  0X00 };
static const uint8_t scrollList2b[] = {
  0X00,
  0XFF,
  SSD1306_ACTIVATE_SCROLL };
void SSD1306_startscrollleft(uint8_t start, uint8_t stop) {


  ssd1306commandList(scrollList2a, sizeof(scrollList2a));
  ssd1306command1(start);
  ssd1306command1(0X00);
  ssd1306command1(stop);

  ssd1306commandList(scrollList2b, sizeof(scrollList2b));

}

/*!
    @brief  Activate a diagonal scroll for all or part of the display.
    @param  start
            First row.
    @param  stop
            Last row.
    @return None (void).
*/
// display.startscrolldiagright(0x00, 0x0F)
static const uint8_t scrollList3a[] = {
  SSD1306_SET_VERTICAL_SCROLL_AREA,
  0X00 };
static const uint8_t  scrollList3b[] = {
  SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL,
  0X00 };
static const uint8_t scrollList3c[] = {
  0X01,
  SSD1306_ACTIVATE_SCROLL };
void SSD1306_startscrolldiagright(uint8_t start, uint8_t stop) {

  ssd1306commandList(scrollList3a, sizeof(scrollList3a));
  ssd1306command1(HEIGHT);

  ssd1306commandList(scrollList3b, sizeof(scrollList3b));
  ssd1306command1(start);
  ssd1306command1(0X00);
  ssd1306command1(stop);

  ssd1306commandList(scrollList3c, sizeof(scrollList3c));

}

/*!
    @brief  Activate alternate diagonal scroll for all or part of the display.
    @param  start
            First row.
    @param  stop
            Last row.
    @return None (void).
*/
// To scroll the whole display, run: display.startscrolldiagleft(0x00, 0x0F)
static const uint8_t scrollList4a[] = {
  SSD1306_SET_VERTICAL_SCROLL_AREA,
  0X00 };
static const uint8_t scrollList4b[] = {
  SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL,
  0X00 };
static const uint8_t scrollList4c[] = {
  0X01,
  SSD1306_ACTIVATE_SCROLL };
void SSD1306_startscrolldiagleft(uint8_t start, uint8_t stop) {

  ssd1306commandList(scrollList4a, sizeof(scrollList4a));
  ssd1306command1(HEIGHT);

  ssd1306commandList(scrollList4b, sizeof(scrollList4b));
  ssd1306command1(start);
  ssd1306command1(0x00);
  ssd1306command1(stop);

  ssd1306commandList(scrollList4c, sizeof(scrollList4c));

}

/*!
    @brief  Cease a previously-begun scrolling action.
    @return None (void).
*/
void SSD1306_stopscroll(void) {

  ssd1306command1(SSD1306_DEACTIVATE_SCROLL);

}

// OTHER HARDWARE SETTINGS -------------------------------------------------

/*!
    @brief  Enable or disable display invert mode (white-on-black vs
            black-on-white).
    @param  i
            If true, switch to invert mode (black-on-white), else normal
            mode (white-on-black).
    @return None (void).
    @note   This has an immediate effect on the display, no need to call the
            display() function -- buffer contents are not changed, rather a
            different pixel mode of the display hardware is used. When
            enabled, drawing BLACK (value 0) pixels will actually draw white,
            WHITE (value 1) will draw black.
*/
void SSD1306_InvertDisplay(int i) {

  ssd1306command1(i ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);

}

/*!
    @brief  Dim the display.
    @param  dim
            true to enable lower brightness mode, false for full brightness.
    @return None (void).
    @note   This has an immediate effect on the display, no need to call the
            display() function -- buffer contents are not changed.
*/
void SSD1306_Dim(int dim) {
  uint8_t contrast;

  if(dim) {
    contrast = 0; // Dimmed display
  } else {
    contrast = (vccstate == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF;
  }
  // the range of contrast to too small to be really useful
  // it is useful to dim the display

  ssd1306command1(SSD1306_SETCONTRAST);
  ssd1306command1(contrast);

}

/*
 * This deprecated function maintains a cursor and moves it
 * with this function.  The new function synchronizes this
 * cursor with the one in the OLED display.
 */
//********SSD1306_SetCursor*****************
// Move the cursor to the desired X- and Y-position.  The
// next character will be printed here.  X=0 is the leftmost
// column.  Y=0 is the top row.
// Inputs: newX  new X-position of the cursor (0<=newX<=20)
//         newY  new Y-position of the cursor (0<=newY<=7)
// Outputs: none
/*uint16_t X, Y;
void SSD1306_SetCursor(uint16_t newX, uint16_t newY){
  if((newX > 20) || (newY > 7)){        // bad input
    return;                             // do nothing
  }
  // multiply newX by 6 because each character is 6 columns wide
  X = newX*6;
  Y = newY*8;
}*/

//********SSD1306_SetCursor*****************
// Move the cursor to the desired X- and Y-position.  The
// next character will be printed here.  X=0 is the leftmost
// column.  Y=0 is the top row.
// Inputs: newX  new X-position of the cursor (0<=newX<=20)
//         newY  new Y-position of the cursor (0<=newY<=7)
// Outputs: none
uint16_t StartX, CurrentX = 0, StartY, CurrentY = 0;
void SSD1306_SetCursor(uint16_t newX, uint16_t newY){
  if((newX > ((WIDTH/6) - 1)) || (newY > ((HEIGHT/8) - 1))){
    return;                             // bad input; do nothing
  }
  // multiply newX by 6 because each character is 6 columns wide
  CurrentX = newX*6;
  StartX = CurrentX;
  CurrentY = newY;
  StartY = CurrentY;
  ssd1306command(SSD1306_PAGEADDR);
  ssd1306command(CurrentY);            // page start address
  ssd1306command((HEIGHT/8) - 1);      // page end (bottom of screen)
  ssd1306command(SSD1306_COLUMNADDR);
  ssd1306command(CurrentX);            // column start address
  ssd1306command(WIDTH - (WIDTH%6) - 1);// column end address
}

/*
 * This deprecated function maintains a cursor and draws a
 * character to the RAM buffer.  Use DrawChar() instead.
 * Use OutChar() to print one character immediately, which
 * does not use the RAM buffer.
 */
//********SSD1306_OutChar*****************
// Print a character to the SSD1306 128by64 OLED.  The
// character will be printed at the current cursor position,
// the cursor will automatically be updated, and it will
// wrap to the next row or back to the top if necessary.
// One blank column of pixels will be printed to the right
// of the character for readability.  Since characters are 8
// pixels tall and 5 pixels wide, 21 characters fit per row,
// and there are 8 rows.
// Inputs: data  character to print
// Outputs: none
/*void SSD1306_OutChar(char data){
  if((X>20*6)||(data == CR)){
    Y = Y+8;
    X = 0;
    if(Y>7*8){
      Y = 0;
    }
  }
  if(data<0x20) return;
  SSD1306_DrawChar(X, Y, data, SSD1306_WHITE);
  X = X+6;
}*/

//********SSD1306_OutChar*****************
// Print a character to the SSD1306 128by64 OLED.  The
// character will be printed at the current cursor position,
// the cursor will automatically be updated, and it will
// wrap to the next row or back to the top if necessary.
// One blank column of pixels will be printed to the right
// of the character for readability.  Since characters are 8
// pixels tall and 6 pixels wide, 21 characters fit per row,
// and there are 8 rows.
// Inputs: data  character to print
// Outputs: none
// Assumes: OLED is in horizontal addressing mode (command 0x20, 0x00)
void SSD1306_OutChar(char data){//int i;
  if((data == 0x0A) || (data == 0x0D)){ // line feed or carriage return
    // go to the first column
    CurrentX = 0;
    StartX = CurrentX;
    ssd1306command(SSD1306_COLUMNADDR);
    ssd1306command(CurrentX);          // column start address
    ssd1306command(WIDTH - (WIDTH%6) - 1);// column end address
    // go to the next row or wrap back to the first row
    CurrentY = CurrentY + 1;
    if(CurrentY > ((HEIGHT/8) - 1)){
      CurrentY = 0;
    }
    StartY = CurrentY;
    ssd1306command(SSD1306_PAGEADDR);
    ssd1306command(CurrentY);          // page start address
    ssd1306command((HEIGHT/8) - 1);    // page end (bottom of screen)
  }else if(data >= 0x20){ // printable character
    CurrentX = CurrentX + 6;
    if(CurrentX > (WIDTH - (WIDTH%6))){
      // go to the first column
      if(StartX != 0){
        // The SSD1306 OLED display will wrap to the column start
        // address (the first parameter of the SSD1306_COLUMNADDR
        // command).  See Figure 10-5 on page 36 of the datasheet.
        // To be sure that the mid- and high-level functions work
        // like they did with the Nokia 5110 LCD, this start
        // address might need to be re-sent as zero when attempting
        // to automatically wrap.
        CurrentX = 0;
        StartX = CurrentX;
        ssd1306command(SSD1306_COLUMNADDR);
        ssd1306command(CurrentX);      // column start address
        ssd1306command(WIDTH - (WIDTH%6) - 1);// column end address
      }
      // go to the next row or wrap back to the first row
      CurrentY = CurrentY + 1;
      if(CurrentY > ((HEIGHT/8) - 1)){
        CurrentY = 0;
        if(StartY != 0){
          // The SSD1306 OLED display will wrap to the row (sometimes
          // called "page") start address (the first parameter of the
          // SSD1306_PAGEADDR command).  See Figure 10-5 on page 36
          // of the datasheet. To be sure that the mid- and high-
          // level functions work like they did with the Nokia 5110
          // LCD, this start address might need to be re-sent as zero
          // when attempting to automatically wrap.
          StartY = CurrentY;
          ssd1306command(SSD1306_PAGEADDR);
          ssd1306command(CurrentY);    // page start address
          ssd1306command((HEIGHT/8) - 1);// page end (bottom of screen)
        }
      }
    }
    I2C_SendData(SSD1306ADDR,(uint8_t *)&ASCII[data - 0x20],6);
//    for(i=0; i<5; i=i+1){
//      datawrite(ASCII[data - 0x20][i]);//SPI version
//    }
//    datawrite(0x00);            // blank vertical line padding
  }
}

//********SSD1306_OutString*****************
// Print a string of characters to the SSD1306 128by64 OLED.
// The string will automatically wrap, so padding spaces may
// be needed to make the output look optimal.
// Inputs: ptr  pointer to NULL-terminated ASCII string
// Outputs: none
// Assumes: OLED is in horizontal addressing mode (command 0x20, 0x00)
void SSD1306_OutString(char *ptr){
// you write this as part of Lab 11

// ******solution*********
  while(*ptr){
    SSD1306_OutChar(*ptr);
    ptr = ptr + 1;
  }
}

//********SSD1306_OutUDec*****************
// Output a 16-bit number in unsigned decimal format with a
// fixed size of five right-justified digits of output.
// Inputs: n  16-bit unsigned number
// Outputs: none
// Assumes: OLED is in horizontal addressing mode (command 0x20, 0x00)
void SSD1306_OutUDec(uint16_t n){
// you write this as part of Lab 11

// ******solution*********

  if(n < 10){
    SSD1306_OutString("    ");
    SSD1306_OutChar(n+'0'); /* n is between 0 and 9 */
  } else if(n<100){
    SSD1306_OutString("   ");
    SSD1306_OutChar(n/10+'0'); /* tens digit */
    SSD1306_OutChar(n%10+'0'); /* ones digit */
  } else if(n<1000){
    SSD1306_OutString("  ");
    SSD1306_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    SSD1306_OutChar(n/10+'0'); /* tens digit */
    SSD1306_OutChar(n%10+'0'); /* ones digit */
  }
  else if(n<10000){
    SSD1306_OutChar(' ');
    SSD1306_OutChar(n/1000+'0'); /* thousands digit */
    n = n%1000;
    SSD1306_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    SSD1306_OutChar(n/10+'0'); /* tens digit */
    SSD1306_OutChar(n%10+'0'); /* ones digit */
  }
  else {
    SSD1306_OutChar(n/10000+'0'); /* ten-thousands digit */
    n = n%10000;
    SSD1306_OutChar(n/1000+'0'); /* thousands digit */
    n = n%1000;
    SSD1306_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    SSD1306_OutChar(n/10+'0'); /* tens digit */
    SSD1306_OutChar(n%10+'0'); /* ones digit */
  }
}

//********SSD1306_OutSDec*****************
// Output a 16-bit number in signed decimal format with a
// fixed size of six right-justified digits of output.
// Inputs: n  16-bit signed number
// Outputs: none
// Assumes: OLED is in horizontal addressing mode (command 0x20, 0x00)
void SSD1306_OutSDec(int16_t n){char sign=' ';
// you write this as part of Lab 11

// ******solution*********
  if(n<0){
    sign = '-';
    n =  -n; // positive now
  }
  if(n < 10){
    SSD1306_OutString("    ");
    SSD1306_OutChar(sign);
    SSD1306_OutChar(n+'0'); /* n is between 0 and 9 */
  } else if(n<100){
    SSD1306_OutString("   ");
    SSD1306_OutChar(sign);
    SSD1306_OutChar(n/10+'0'); /* tens digit */
    SSD1306_OutChar(n%10+'0'); /* ones digit */
  } else if(n<1000){
    SSD1306_OutString("  ");
    SSD1306_OutChar(sign);
    SSD1306_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    SSD1306_OutChar(n/10+'0'); /* tens digit */
    SSD1306_OutChar(n%10+'0'); /* ones digit */
  }
  else if(n<10000){
    SSD1306_OutChar(' ');
    SSD1306_OutChar(sign);
    SSD1306_OutChar(n/1000+'0'); /* thousands digit */
    n = n%1000;
    SSD1306_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    SSD1306_OutChar(n/10+'0'); /* tens digit */
    SSD1306_OutChar(n%10+'0'); /* ones digit */
  }
  else {
    SSD1306_OutChar(sign);
    SSD1306_OutChar(n/10000+'0'); /* ten-thousands digit */
    n = n%10000;
    SSD1306_OutChar(n/1000+'0'); /* thousands digit */
    n = n%1000;
    SSD1306_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    SSD1306_OutChar(n/10+'0'); /* tens digit */
    SSD1306_OutChar(n%10+'0'); /* ones digit */
  }
}
//********SSD1306_OutUFix1*****************
// Output a 16-bit number in unsigned 3-digit fixed point, 0.1 resolution
// numbers 0 to 999 printed as " 0.0" to "99.9"
// Inputs: n  16-bit unsigned number
// Outputs: none
void SSD1306_OutUFix1(uint16_t n){
  char message[5];
  if(n>999)n=999;
  if(n>=100){  // 100 to 999
    message[0] = (n/100+'0'); /* tens digit */
    n = n%100; //the rest
  }else { // 0 to 99
    message[0] = ' '; /* n is between 0.0 and 9.9 */
  }
  message[1] = (n/10+'0'); /* ones digit */
  n = n%10; //the rest
  message[2] = '.';
  message[3] = (n+'0'); /* tenths digit */
  message[4] = 0;
  SSD1306_OutString(message);
}
//********SSD1306_OutSFix1*****************
// Output a 16-bit number in signed 4-digit fixed point, 0.1 resolution
// numbers -9999 to 9999 printed as "-999.0" to " 999.9"
// Inputs: n  32-bit signed number
// Outputs: none
void SSD1306_OutSFix1(int32_t n){
  char message[8];
  if(n<-9999) n=-9999;
  if(n>9999)  n=9999;
  if(n<0){
    message[0] = '-';
    n = -n; // now positive
  }else{
    message[0] = ' ';
  }
  if(n>=1000){  // 1000 to 9999
    message[1] = (n/1000+'0'); /* hundreds digit */
    n = n%1000; // the rest
    message[2] = (n/100+'0'); /* tens digit */
    n = n%100;  // the rest
  }else{
    if(n>=100){  // 100 to 999
      message[1] = ' '; /* n is between 0.0 and 9.9 */
      message[2] = (n/100+'0'); /* tens digit */
      n = n%100; //the rest
    }else { // 0 to 99
      message[1] = ' '; /* n is between 0.0 and 9.9 */
      message[2] = ' ';
    }
  }
  message[3] = (n/10+'0'); /* ones digit */
  n = n%10; //the rest
  message[4] = '.';
  message[5] = (n+'0'); /* tenths digit */
  message[6] = 0;
  SSD1306_OutString(message);
}


void SSD1306_OutHex7(uint8_t n){
  n = n&0x0F;
  if(n>9){
    SSD1306_OutChar('A'+n-10);
  }else{
    SSD1306_OutChar(n+'0');        /* ones digit */
  }
}
void SSD1306_OutUHex7(uint8_t n){
  SSD1306_OutString(" 0x");
  SSD1306_OutHex7(n/16); /* 16s digit */
  SSD1306_OutHex7(n);    /* ones digit */
}

void SSD1306_OutUDec16(uint32_t n){
  SSD1306_OutChar(' ');
  if(n>=100){
    SSD1306_OutChar(n/100+'0'); /* 100 digit */
    n = n%100;
    SSD1306_OutChar(n/10+'0'); /* 10 digit */
    SSD1306_OutChar(n%10+'0'); /* ones digit */
  }else{
    SSD1306_OutChar(' ');
    if(n>=10){
      SSD1306_OutChar(n/10+'0'); /* 10 digit */
      SSD1306_OutChar(n%10+'0'); /* ones digit */
    }else{
      SSD1306_OutChar(' ');
      SSD1306_OutChar(n+'0'); /* ones digit */
    }
  }
}
void SSD1306_OutUDec2(uint32_t n){
  if(n>=100){
    SSD1306_OutChar(' ');  /* 100 digit */
    SSD1306_OutChar('*'); /* illegal */
  }else{
    if(n>=10){
      SSD1306_OutChar(n/10+'0'); /* 10 digit */
      SSD1306_OutChar(n%10+'0'); /* ones digit */
    }else{
      SSD1306_OutChar(' ');
      SSD1306_OutChar(n+'0'); /* ones digit */
    }
  }
}



// Print a character to ST7735 LCD.
int fputc(int ch, FILE *f){
  SSD1306_OutChar(ch);
  return 1;
}
// No input from Nokia, always return data.
int fgetc (FILE *f){
  return 0;
}
// Function called when file error occurs.
int ferror(FILE *f){
  /* Your implementation of ferror */
  return EOF;
}
