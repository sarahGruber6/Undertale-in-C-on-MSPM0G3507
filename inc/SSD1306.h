/*!
 * @defgroup Display
 * @brief OLED and LCD displays
 * @{*/
 /**
 * @file      SSD1306.h
 * @brief     Provide simple I/O functions for the SSD 1306 OLED
 * @details   Use I2C to send an 8-bit code to the 1306 64x128
 * pixel OLED to display text, images, or other information.
 * @version   EE319K Spring 2021
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2020 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      Nov 9, 2020
 * @remark Font table based
 * off of Nokia_5110_Example from Spark Fun: 7-17-2011<br>
 * Spark Fun Electronics 2011, Nathan Seidle<br>
 * http://dlnmh9ip6v2uc.cloudfront.net/datasheets/LCD/Monochrome/Nokia_5110_Example.pde
 *
 * Typo on OLED has a 0-ohm resistor selection <br>
 * that says        "IIC ADDRESS SELECT 0x78 0x7A"<br>
 * should have said "IIC ADDRESS SELECT 0x3C 0x3D"<br>
 * https://www.amazon.com/gp/product/B0871KW7BD<br>
 * search for I2C SSD1306 on Amazon.com<br>
 * rough estimate of execution times<br>
 * SSD1306_Clear         26.5ms<br>
 * SSD1306_OutBuffer     25.9ms, sends 128 by 64 pixels to OLED<br>
 * SSD1306_DrawBMP       75us (10 by 16 size), doesn't print, just fills buffer<br>
 * SSD1306_OutChar       206us, does print<br>
 * SSD1306_OutUDec       1032us, outputs 5 characters<br>
<table>
<caption id="I2C0 SSD1306">I2C0 SSD1306 OLED</caption>
<tr><th>SSD1306 <th> LaunchPad pin
<tr><td>GND     <td>ground
<tr><td>3.3V    <td>3.3V power to OLED
<tr><td>SCL     <td>PB2 I2C clock
<tr><td>SDA     <td>PB3 I2C data
</table>
<table>
<caption id="I2C1 SSD1306">I2C1 SSD1306 OLED</caption>
<tr><th>SSD1306 <th> LaunchPad pin
<tr><td>GND     <td>ground
<tr><td>3.3V    <td>3.3V power to OLED
<tr><td>SCL     <td>PA6 I2C clock
<tr><td>SDA     <td>PA7 I2C data
</table>
<table>
<caption id="I2C2 SSD1306">I2C2 SSD1306 OLED</caption>
<tr><th>SSD1306 <th> LaunchPad pin
<tr><td>GND     <td>ground
<tr><td>3.3V    <td>3.3V power to OLED
<tr><td>SCL     <td>PE4 I2C clock
<tr><td>SDA     <td>PE5 I2C data
</table>
<table>
<caption id="I2C3 SSD1306">I2C3 SSD1306 OLED</caption>
<tr><th>SSD1306 <th> LaunchPad pin
<tr><td>GND     <td>ground
<tr><td>3.3V    <td>3.3V power to OLED
<tr><td>SCL     <td>PD0 I2C clock
<tr><td>SDA     <td>PD1 I2C data
</table>
 This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2020
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2020, Jonathan Valvano, All rights reserved.

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

#ifndef _SSD1306_H_
#define _SSD1306_H_
#include <stdint.h>
/**
 * \brief Tested for four possible hardware connections I2C=0 I2C=1 I2C=2 and I2C=3
 */
#define I2C 3
/*
 *  I2C0 Conncection | I2C1 Conncection | I2C2 Conncection | I2C3 Conncection
 *  ---------------- | ---------------- | ---------------- | ----------------
 *  SCL -------- PB2 | SCL -------- PA6 | SCL -------- PE4 | SCL -------- PD0
 *  SDA -------- PB3 | SDA -------- PA7 | SDA -------- PE5 | SDA -------- PD1
 */
/**
 * \brief Black is 0 or off
 */
#define SSD1306_BLACK               0 
/**
 * \brief White is 1 or on
 */
#define SSD1306_WHITE               1 
/**
 * \brief Inverse will flip bits
 */
#define SSD1306_INVERSE             2 
/**
 * \brief SSD1306_EXTERNALVCC specifies External display voltage source
 */
#define SSD1306_EXTERNALVCC         0x01 
/**
 * \brief SSD1306_SWITCHCAPVCC specifies display voltage from 3.3V
 */
#define SSD1306_SWITCHCAPVCC        0x02 
/**
 * \brief true means success
 */
#define true 1  
/**
 * \brief false means failure
 */
#define false 0  

/*!
    @brief  II2 driver for SSD1306 OLED display
*/
// With rotation at 0
// 0,0       <-x->        0,127
//   ------------------------
//   |                      |
//   |                      |
//   |                      |
//   ------------------------
// 63,0                   63,127
/**
 * Initialize OLED
 * @param vccst Vcc voltage parameter, uSSD1306_EXTERNALVCC or SSD1306_SWITCHCAPVCC
 * @return success or failure
 * @note for EE319K use vccst=SSD1306_SWITCHCAPVCC
 */
int  SSD1306_Init(int vccst);

/**
 * Copy all of RAM image to OLED. Use this command with SSD1306_ClearBuffer, and all Draw functions
 * @param none
 * @return success or failure
 */
void SSD1306_OutBuffer(void);

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
void SSD1306_InvertDisplay(int i);
/*!
    @brief  Dim the display.
    @param  dim
            true to enable lower brightness mode, false for full brightness.
    @return None (void).
    @note   This has an immediate effect on the display, no need to call the
            display() function -- buffer contents are not changed.
*/
void SSD1306_Dim(int dim);

/*!
    @brief  Set/clear/invert a single pixel. This is also invoked by the
            Adafruit_GFX library in generating many higher-level graphics
            primitives.
    @param  x
            Column of display -- 0 at left to (screen width - 1) at right.
    @param  y
            Row of display -- 0 at top to (screen height - 1) at bottom.
    @param  color
            Pixel color, one of: SSD1306_BLACK, SSD1306_WHITE or SSD1306_INVERT.
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to SSD1306_DisplayBuffer(), or with other
            graphics commands as needed by one's own application.
*/
void SSD1306_DrawPixel(int16_t x, int16_t y, uint16_t color);

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
            Line color, one of: SSD1306_BLACK, SSD1306_WHITE or SSD1306_INVERSE.
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to SSD1306_DisplayBuffer(), or with other
            graphics commands as needed by one's own application.
*/
void SSD1306_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

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
            Line color, one of: SSD1306_BLACK, SSD1306_WHITE or SSD1306_INVERSE.
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to SSD1306_DisplayBuffer(), or with other
            graphics commands as needed by one's own application.
*/
void SSD1306_DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);

/*!
    @brief  Activate a right-handed scroll for all or part of the display.
    @note To scroll the whole display, run: SSD1306_startscrollright(0x00, 0x0F)
    @param  start
            First row.
    @param  stop
            Last row.
    @return None (void).
*/

void SSD1306_startscrollright(uint8_t start, uint8_t stop);
/*!
    @brief  Activate a left-handed scroll for all or part of the display.
    @note To scroll the whole display, run: SSD1306_startscrollleft(0x00, 0x0F)
    @param  start
            First row.
    @param  stop
            Last row.
    @return None (void).
*/
void SSD1306_startscrollleft(uint8_t start, uint8_t stop);

/*!
    @brief  Activate a diagonal scroll for all or part of the display.
    @note SSD1306_startscrolldiagright(0x00, 0x0F)
    @param  start
            First row.
    @param  stop
            Last row.
    @return None (void).
*/
void SSD1306_startscrolldiagright(uint8_t start, uint8_t stop);

/*!
    @brief  Activate alternate diagonal scroll for all or part of the display.
    @note To scroll the whole display, run: SSD1306_startscrolldiagleft(0x00, 0x0F)
    @param  start
            First row.
    @param  stop
            Last row.
    @return None (void).
*/
void SSD1306_startscrolldiagleft(uint8_t start, uint8_t stop);

/*!
    @brief  Cease a previously-begun scrolling action.
    @return None (void).
*/
void SSD1306_stopscroll(void);

/*!
    @brief  Return color of a single pixel in display buffer.
    @param  x
            Column of display -- 0 at left to (screen width - 1) at right.
    @param  y
            Row of display -- 0 at top to (screen height -1) at bottom.
    @return true if pixel is set (usually WHITE, unless display invert mode
            is enabled), false if clear (BLACK).
    @note   Reads from buffer contents; may not reflect current contents of
            screen if SSD1306_DisplayBuffer() has not been called.
*/
int  SSD1306_GetPixel(int16_t x, int16_t y);

/*!
    @brief  Get base address of display buffer for direct reading or writing.
    @return Pointer to an unsigned 8-bit array, column-major, columns padded
            to full byte boundary if needed.
*/
uint8_t *SSD1306_getBuffer(void);

/*!
    @brief  Draw a character
    @param  x
            Column of display -- 0 at left to (screen width - 1) at right.
    @param  y
            Row of display -- 0 at top to (screen height - 1) at bottom.
    @param  letter
            ASCII code of character to draw.
    @param  color
            Pixel color, one of: SSD1306_BLACK, SSD1306_WHITE or SSD1306_INVERSE.
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to SSD1306_DisplayBuffer(), or with other
            graphics commands as needed by one's own application.
*/
void SSD1306_DrawChar(int16_t x, int16_t y, char letter, uint16_t color);

/*!
    @brief  Draw a string
    @param  x
            Column of display -- 0 at left to (screen width - 1) at right.
    @param  y
            Row of display -- 0 at top to (screen height - 1) at bottom.
    @param  pt
            pointer to null-terminated ASCII string of characters to draw.
    @param  color
            Pixel color, one of: SSD1306_BLACK, SSD1306_WHITE or SSD1306_INVERSE.
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to SSD1306_DisplayBuffer(), or with other
            graphics commands as needed by one's own application.      
    @image html SSD1306DrawString.png width=300px            

*/
void SSD1306_DrawString(int16_t x, int16_t y, char *pt, uint16_t color);

/*!
    @brief  Clear contents of display buffer (set all pixels to off).
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to display(), or with other graphics
            commands as needed by one's own application.
*/
void SSD1306_ClearBuffer(void);

/*!
    @brief  Clear the LCD by writing zeros to the entire screen and
            reset the cursor to (0,0) (top left corner of screen).
    @return None (void).
*/
void SSD1306_OutClear(void);

/*!
    @brief  Fill the whole screen by drawing a 128x64 bitmap image.
    @param  ptr  Pointer to 1024-byte image with no header or format
                 data.
    @return None (void).
    @note   Assumes: OLED is in horizontal addressing mode (command 0x20, 0x00)<br>
            Image is sent to screen directly, RAM buffer is unchanged.
*/
void SSD1306_DrawFullImage(const uint8_t *ptr);

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
              SSD1306_DisplayBuffer();<br>
            The interface and operation of this process is modeled
            after RIT128x96x4_BMP(x, y, image);
*/
void SSD1306_DrawBMP(uint8_t xpos, uint8_t ypos, const uint8_t *ptr, uint8_t threshold, uint16_t color);

/**
 * Move the cursor to the desired X- and Y-position.  The
 * next character will be printed here.  X=0 is the leftmost
 * column.  Y=0 is the top row.
 * @param newX new X-position of the cursor (0<=newX<=20)
 * @param newY new Y-position of the cursor (0<=newY<=7)
 * @return none
 */
void SSD1306_SetCursor(uint16_t newX, uint16_t newY);

/**
 * Print a character to the SSD1306 OLED.  The
 * character will be printed at the current cursor position,
 * the cursor will automatically be updated, and it will
 * wrap to the next row or back to the top if necessary.
 * One blank column of pixels will be printed on one side
 * of the character for readability.  Since characters are 8
 * pixels tall and 6 pixels wide, 12 characters fit per row,
 * and there are six rows.
 * @param data  character to print
 * @return none
 * @brief  Print a character to the OLED
 * @note use SSD1306_SetCursor to specify position
 */
void SSD1306_OutChar(char data);

/**
 * Print a string of characters to the SSD1306 OLED.
 * The string will automatically wrap, so padding spaces may
 * be needed to make the output look optimal.
 * @param ptr  pointer to NULL-terminated ASCII string
 * @return none
 * @brief  Print a string of characters to the OLED
 * @note use SSD1306_SetCursor to specify position
 */
void SSD1306_OutString(char *ptr);

/**
 * Output a 16-bit number in unsigned decimal format with a
 * fixed size of five right-justified digits of output.
 * @param n  16-bit unsigned number
 * @return none
 * @brief  Print a 16-bit unsigned number to the OLED
 * @note use SSD1306_SetCursor to specify position
 */
void SSD1306_OutUDec(uint16_t n);

/**
 * Output a 16-bit number in signed decimal format with a
 * fixed size of six right-justified digits of output.
 * @param n  16-bit signed number
 * @return none
 * @brief  Print a 16-bit signed number to the OLED
 * @note use SSD1306_SetCursor to specify position
 */
void SSD1306_OutSDec(int16_t n);

/**
 * Output a 16-bit number in unsigned 3-digit fixed point, 0.1 resolution
 * fixed size of four right-justified characters.<br>
 * numbers 0 to 999 printed as " 0.0" to "99.9"
 * @param n  16-bit unsigned number
 * @return none
 * @brief  Print a 16-bit unsigned fixed-point number to the OLED
 * @note use SSD1306_SetCursor to specify position
 */
void SSD1306_OutUFix1(uint16_t n);

/**
 * Output a 32-bit number in signed 4-digit fixed point, 0.1 resolution
 * fixed size of six right-justified characters.<br>
 * numbers -9999 to 9999 printed as "-999.9" to "999.9"
 * @param n  32-bit signed number
 * @return none
 * @brief  Print a 32-bit signed fixed-point number to the OLED
 * @note use SSD1306_SetCursor to specify position
 */
void SSD1306_OutSFix1(int32_t n);

/**
 * Output one hex digit to the SSD1306 OLED
 * Just one character with no leading 0x
 * @param n unsigned number 0-15 to print
 * @return none
 * @brief  Print one hex digit
 * @note use SSD1306_SetCursor to specify position
 */
void SSD1306_OutHex7(uint8_t n);

/**
 * Output two hex digits to the SSD1306 OLED.
 * Prints 4 characters with leading 0x
 * @param n unsigned number 0-255 to print
 * @return none
 * @brief  Print two hex digits
 * @note use SSD1306_SetCursor to specify position
 */
void SSD1306_OutUHex7(uint8_t n);

/**
 * Output three decimal digits to the SSD1306 OLED.
 * Prints 4 characters with leading space
 * @param n unsigned number 0-999 to print
 * @return none
 * @brief  Print three decimal digits
 * @note use SSD1306_SetCursor to specify position
 */
void SSD1306_OutUDec16(uint32_t n);

/**
 * Output two decimal digits to the SSD1306 OLED.
 * Prints 2 characters without leading space
 * @param n unsigned number 0-99 to print
 * @return none
 * @brief  Print two decimal digits
 * @note use SSD1306_SetCursor to specify position
 */
void SSD1306_OutUDec2(uint32_t n);

/*!
    @brief  Initialize parameters for plotting
    @param  minX minimum X value
    @param  maxX maxamum X value
    @param  minY minimum Y value
    @param  maxY maxamum Y value
    @param  color
            Pixel color, one of: SSD1306_BLACK, SSD1306_WHITE or SSD1306_INVERT.
    @return None (void).
    @note   minimum must be less than maximum
*/
void SSD1306_SetPlot(int32_t minX, int32_t maxX, int32_t minY, int32_t maxY, uint16_t color);
/*!
    @brief  Set/clear/invert a single pixel. SSD1306_SetPlot initializes scaling
    @param  x
            Column of display -- minX at left to maxX at right.
    @param  y
            Row of display -- maxY at top to minY at bottom.
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to SSD1306_DisplayBuffer(), or with other
            graphics commands as needed by one's own application.
*/
void SSD1306_DrawPoint(int32_t x, int32_t y);

#endif // _SSD1306_H_
/** @}*/