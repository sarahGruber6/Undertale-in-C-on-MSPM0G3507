/*!
 * @defgroup ST7735
 * @brief ST7735R LCD
 <table>
<caption id="AdafruitLCDpins">Adafruit ST7735R pins </caption>
<tr><th>Pin <th>Connection <th>Description
<tr><td>10<td>+3.3<td>Backlight   
<tr><td>9 <td>nc  <td>SPI1 MISO (used for SDC)     
<tr><td>8 <td>PB9 <td>SPI1 SCLK clock out
<tr><td>7 <td>PB8 <td>SPI1 PICO data out    
<tr><td>6 <td>PB6 <td>SPI1 CS0=TFT_CS   
<tr><td>5 <td>nc  <td>CARD_CS (used for SDC)   
<tr><td>4 <td>PA13<td>Data/Command(GPIO), high for data, low for command   
<tr><td>3 <td>PB15<td>RESET, low to reset,  (GPIO)   
<tr><td>2 <td>+3.3<td>VCC   
<tr><td>1 <td>Gnd <td>ground   
</table>

 <table>
<caption id="widehkpins">wide.hk ST7735R with ADXL345 accelerometer pins </caption>
<tr><th>Pin<th>Connection
<tr><td>VCC  <td> +3.3 V
<tr><td>GND  <td> Ground
<tr><td>!SCL <td> SPI1 SCLK: PB9 clock
<tr><td>!SDA <td> SPI1 PICO: PB8 MOSI SPI data from microcontroller to TFT or SDC
<tr><td>DC   <td> GPIO       PA13 TFT data/command
<tr><td>RES  <td> GPIO       PB15 TFT reset
<tr><td>CS   <td> SPI1 CS0:  PB6 TFT_CS, active low to enable TFT
<tr><td>*CS  <td> (NC) SDC_CS, active low to enable SDC
<tr><td>MISO <td> (NC) MISO SPI data from SDC to microcontroller
<tr><td>SDA  <td> (NC) I2C data for ADXL345 accelerometer
<tr><td>SCL  <td> (NC) I2C clock for ADXL345 accelerometer
<tr><td>SDO  <td> (NC) I2C alternate address for ADXL345 accelerometer
<tr><td>Backlight + - Light, backlight connected to +3.3 V  
</table>


<table>
<caption id="widehkpins335">wide.hk ST7735R with ADXL335 accelerometer pins </caption>
<tr><th>Pin<th>Connection
<tr><td>VCC  <td> +3.3 V
<tr><td>GND  <td> Ground
/<tr><td>!SCL<td> SPI1 SCLK: PB9
<tr><td>!SDA <td> SPI1 PICO: PB8 MOSI SPI data from microcontroller to TFT or SDC
<tr><td>DC   <td> GPIO       PA13 TFT data/command
<tr><td>RES  <td> GPIO       PB15 TFT reset
<tr><td>CS   <td> SPI1 CS0:  PB6 TFT_CS, active low to enable TFT
<tr><td>*CS  <td> (NC) SDC_CS, active low to enable SDC
<tr><td>MISO <td> (NC) MISO SPI data from SDC to microcontroller
<tr><td>X<td> (NC) analog input X-axis from ADXL335 accelerometer
<tr><td>Y<td> (NC) analog input Y-axis from ADXL335 accelerometer
<tr><td>Z<td> (NC) analog input Z-axis from ADXL335 accelerometer
<tr><td>Backlight + <td> Light, backlight connected to +3.3 V
</table>

<table>
<caption id="HiLetgopins">HiLetgo ST7735 TFT and SDC pins </caption>
<tr><th>signal<th>Pin<th>Connection
<tr><td>LED-   <td>16<td>TFT, to ground
<tr><td>LED+   <td>15<td>TFT, to +3.3 V
<tr><td>SD_CS  <td>14<td>SDC, to chip select
<tr><td>MOSI   <td>13<td>SDC, to MOSI
<tr><td>MISO   <td>12<td>SDC, to MISO
<tr><td>SCK    <td>11<td>SDC, to serial clock
<tr><td>CS     <td>10<td>TFT, to PB6  SPI1 CS0
<tr><td>SCL    <td> 9<td>TFT, to PB9  SPI1 SCLK
<tr><td>SDA    <td> 8<td>TFT, to PB8  MOSI SPI1 PICO
<tr><td>A0     <td> 7<td>TFT, to PA13 Data/Command, high for data, low for command
<tr><td>RESET  <td> 6<td>TFT, to PB15 reset (GPIO), low to reset
<tr><td>NC     <td>3,4,5<td>not connected
<tr><td>VCC    <td> 2<td>to +3.3 V
<tr><td>GND    <td> 1<td>to ground
</table>

<table>
<caption id="Tyenazapins">Tyenaza Tyenazaqhf72mi9s3 pins </caption>
<tr><th>signal<th>Pin<th>Connection
<tr><td>LED <td>8<td> to +3.3 V
<tr><td>SCK <td>7<td>SCLK         to PB9 SPI1 SCLK
<tr><td>SDA <td>6<td>MOSI,        to PB8 SPI1 PICO
<tr><td>A0  <td>5<td>Data/Command to PA13 (GPIO), high for data, low for command
<tr><td>RESET <td>4<td>to PB15 (GPIO), low to reset
<tr><td>CS  <td>3<td>to PB6 SPI1 CS0:
<tr><td>Gnd <td>2<td>to ground
<tr><td>VCC <td>1<td>to +3.3 V
</table>
 * @{*/
/**
 * @file      ST7735.h
 * @brief     160 by 128 pixel LCD
 * @details   Software driver functions for ST7735R display<br>
 * This is a library for the Adafruit 1.8" SPI display.<br>
 *  This library works with the Adafruit 1.8" TFT Breakout w/SD card<br>
 *  ----> http://www.adafruit.com/products/358<br>
 *  as well as Adafruit raw 1.8" TFT display<br>
 *  ----> http://www.adafruit.com/products/618<br>
 *   Check out the links above for our tutorials and wiring diagrams<br>
 *  These displays use SPI to communicate, 4 or 5 pins are required to
 *  interface (RST is optional)
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing
 *  products from Adafruit!
 *  Written by Limor Fried/Ladyada for Adafruit Industries.
 *  MIT license, all text above must be included in any redistribution
 * @version   ECE319K v1.0
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2023 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      August 14, 2023
 * interface <br>
 * \image html ST7735interface.png width=500px 
 * <br><br>ST7735 160 by 128 pixel LCD<br>
 * \image html ST7735.png width=500px 
  <table>
<caption id="AdafruitLCDpins2">Adafruit ST7735R pins </caption>
<tr><th>Pin <th>Connection <th>Description
<tr><td>10<td>+3.3<td>Backlight   
<tr><td>9 <td>nc  <td>SPI1 MISO (used for SDC)     
<tr><td>8 <td>PB9 <td>SPI1 SCLK clock out
<tr><td>7 <td>PB8 <td>SPI1 PICO data out    
<tr><td>6 <td>PB6 <td>SPI1 CS0=TFT_CS   
<tr><td>5 <td>nc  <td>CARD_CS (used for SDC)   
<tr><td>4 <td>PA13<td>Data/Command(GPIO), high for data, low for command   
<tr><td>3 <td>PB15<td>RESET, low to reset,  (GPIO)   
<tr><td>2 <td>+3.3<td>VCC   
<tr><td>1 <td>Gnd <td>ground   
</table>

 <table>
<caption id="widehkpins2">wide.hk ST7735R with ADXL345 accelerometer pins </caption>
<tr><th>Pin<th>Connection
<tr><td>VCC  <td> +3.3 V
<tr><td>GND  <td> Ground
<tr><td>!SCL <td> SPI1 SCLK: PB9 clock
<tr><td>!SDA <td> SPI1 PICO: PB8 MOSI SPI data from microcontroller to TFT or SDC
<tr><td>DC   <td> GPIO       PA13 TFT data/command
<tr><td>RES  <td> GPIO       PB15 TFT reset
<tr><td>CS   <td> SPI1 CS0:  PB6 TFT_CS, active low to enable TFT
<tr><td>*CS  <td> (NC) SDC_CS, active low to enable SDC
<tr><td>MISO <td> (NC) MISO SPI data from SDC to microcontroller
<tr><td>SDA  <td> (NC) I2C data for ADXL345 accelerometer
<tr><td>SCL  <td> (NC) I2C clock for ADXL345 accelerometer
<tr><td>SDO  <td> (NC) I2C alternate address for ADXL345 accelerometer
<tr><td>Backlight + - Light, backlight connected to +3.3 V  
</table>


<table>
<caption id="widehkpins3352">wide.hk ST7735R with ADXL335 accelerometer pins </caption>
<tr><th>Pin<th>Connection
<tr><td>VCC  <td> +3.3 V
<tr><td>GND  <td> Ground
/<tr><td>!SCL<td> SPI1 SCLK: PB9
<tr><td>!SDA <td> SPI1 PICO: PB8 MOSI SPI data from microcontroller to TFT or SDC
<tr><td>DC   <td> GPIO       PA13 TFT data/command
<tr><td>RES  <td> GPIO       PB15 TFT reset
<tr><td>CS   <td> SPI1 CS0:  PB6 TFT_CS, active low to enable TFT
<tr><td>*CS  <td> (NC) SDC_CS, active low to enable SDC
<tr><td>MISO <td> (NC) MISO SPI data from SDC to microcontroller
<tr><td>X<td> (NC) analog input X-axis from ADXL335 accelerometer
<tr><td>Y<td> (NC) analog input Y-axis from ADXL335 accelerometer
<tr><td>Z<td> (NC) analog input Z-axis from ADXL335 accelerometer
<tr><td>Backlight + <td> Light, backlight connected to +3.3 V
</table>

<table>
<caption id="HiLetgopins2">HiLetgo ST7735 TFT and SDC pins </caption>
<tr><th>signal<th>Pin<th>Connection
<tr><td>LED-   <td>16<td>TFT, to ground
<tr><td>LED+   <td>15<td>TFT, to +3.3 V
<tr><td>SD_CS  <td>14<td>SDC, to chip select
<tr><td>MOSI   <td>13<td>SDC, to MOSI
<tr><td>MISO   <td>12<td>SDC, to MISO
<tr><td>SCK    <td>11<td>SDC, to serial clock
<tr><td>CS     <td>10<td>TFT, to PB6  SPI1 CS0
<tr><td>SCL    <td> 9<td>TFT, to PB9  SPI1 SCLK
<tr><td>SDA    <td> 8<td>TFT, to PB8  MOSI SPI1 PICO
<tr><td>A0     <td> 7<td>TFT, to PA13 Data/Command, high for data, low for command
<tr><td>RESET  <td> 6<td>TFT, to PB15 reset (GPIO), low to reset
<tr><td>NC     <td>3,4,5<td>not connected
<tr><td>VCC    <td> 2<td>to +3.3 V
<tr><td>GND    <td> 1<td>to ground
</table>

<table>
<caption id="Tyenazapins2">Tyenaza Tyenazaqhf72mi9s3 pins </caption>
<tr><th>signal<th>Pin<th>Connection
<tr><td>LED <td>8<td> to +3.3 V
<tr><td>SCK <td>7<td>SCLK         to PB9 SPI1 SCLK
<tr><td>SDA <td>6<td>MOSI,        to PB8 SPI1 PICO
<tr><td>A0  <td>5<td>Data/Command to PA13 (GPIO), high for data, low for command
<tr><td>RESET <td>4<td>to PB15 (GPIO), low to reset
<tr><td>CS  <td>3<td>to PB6 SPI1 CS0:
<tr><td>Gnd <td>2<td>to ground
<tr><td>VCC <td>1<td>to +3.3 V
</table>
<br>

  ******************************************************************************/


#ifndef _ST7735H_
#define _ST7735H_
#include <stdint.h>
/**
 * \brief some flags for ST7735_InitR()
 */
enum initRFlags{
  none,
  INITR_GREENTAB,
  INITR_REDTAB,
  INITR_BLACKTAB
};

/**
 * \brief 128 pixels wide
 */
#define ST7735_TFTWIDTH  128
/**
 * \brief 160 pixels tall
 */
#define ST7735_TFTHEIGHT 160


/**
 * \brief The following constants are possible colors for the LCD in RGB format
 */
#define ST7735_BLACK   0x0000
#define ST7735_BLUE    0xF800
#define ST7735_RED     0x001F
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0xFFE0
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0x07FF
#define ST7735_WHITE   0xFFFF
#define ST7735_LIGHTGREY ST7735_Color565(228,228,228)
#define ST7735_DARKGREY ST7735_Color565(32,32,32)
#define ST7735_ORANGE ST7735_Color565(255,102,0)
#define ST7735_PURPLE ST7735_Color565(106,13,173)

/**
 * Initialize ST7735B color 128x160-pixel TFT LCD 
 * @param none
 * @return none 
 * @brief  Initialize ST7735B LCD
 * @note assumes GPIOA and GPIOB are reset and powered previously
 */
void ST7735_InitB(void);



/**
 * Initialize ST7735R color 128x160-pixel TFT LCD 
 * @param option one of none,INITR_GREENTAB,INITR_REDTAB,INITR_BLACKTAB
 * @return none 
 * @brief  Initialize ST7735R LCD
 * @note assumes GPIOA and GPIOB are reset and powered previously
 */
void ST7735_InitR(enum initRFlags option);



/**
 * Color the pixel at the given coordinates with the given color.<br>
 * Requires 13 bytes of transmission<br>
 * x must be less than 128<br>
 * x 0 is on the left, 126 is near the right  <br>
 * y must be less than 160
 * y 159 is near the wires, 0 is the side opposite the wires
 * @param x     horizontal position of the pixel, columns from the left edge
 * @param y     vertical position of the pixel, rows from the top edge
 * @param color 16-bit color, which can be produced by LCD_Color565()
 * @return none
 * @brief  Color one pixel
 */
void ST7735_DrawPixel(int16_t x, int16_t y, uint16_t color);


/**
 * Draw a vertical line at the given coordinates with the given height and color.<br>
 * A vertical line is parallel to the longer side of the rectangular display<br>
 * Requires (11 + 2*h) bytes of transmission (assuming image fully on screen)
 * @param x     horizontal position of the start of the line, columns from the left edge
 * @param y     vertical position of the start of the line, rows from the top edge
 * @param h     vertical height of the line
 * @param color 16-bit color, which can be produced by LCD_Color565()
 * @return none
 * @brief  Draw a vertical line
 */
void ST7735_DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);



/**
 * Draw a horizontal line at the given coordinates with the given width and color.
 * A horizontal line is parallel to the shorter side of the rectangular display<br>
 * Requires (11 + 2*w) bytes of transmission (assuming image fully on screen)
 * @param x     horizontal position of the start of the line, columns from the left edge
 * @param y     vertical position of the start of the line, rows from the top edge
 * @param w     horizontal width of the line
 * @param color 16-bit color, which can be produced by LCD_Color565()
 * @return none
 * @brief  Draw a horizontal line
 */
void ST7735_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);


/**
 * Fill the screen with the given color.<br>
 * Requires 40,971 bytes of transmission
 * @param color 16-bit color, which can be produced by ST7735_Color565()
 * @return none
 * @brief  Fill the screen
 */
void ST7735_FillScreen(uint16_t color);



/**
 * Draw a filled rectangle at the given coordinates with the given width, height, and color.<br>
 * Requires (11 + 2*w*h) bytes of transmission (assuming image fully on screen)
 * @param x     horizontal position of the top left corner of the rectangle, columns from the left edge
 * @param y     vertical position of the top left corner of the rectangle, rows from the top edge
 * @param w     horizontal width of the rectangle
 * @param h     vertical height of the rectangle
 * @param color 16-bit color, which can be produced by ST7735_Color565()
 * @return none
 * @brief  Draw a filled rectangle 
 */
void ST7735_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);


/**
 * Draw a small circle (diameter of 6 pixels) at the given coordinates with the given color.<br>
 * Requires (11*6+24*2)=114 bytes of transmission (assuming image fully on screen)
 * @param x     horizontal position of the top left corner of the circle, columns from the left edge
 * @param y     vertical position of the top left corner of the circle, rows from the top edge
 * @param color 16-bit color, which can be produced by ST7735_Color565()
 * @return none
 * @brief  Draw a small circle 
 */
void ST7735_DrawSmallCircle(int16_t x, int16_t y, uint16_t color);


/**
 * Draw a circle (diameter of 10 pixels) at the given coordinates with the given color.<br>
 * Requires (11*10+68*2)=178 bytes of transmission (assuming image on screen)
 * @param x     horizontal position of the top left corner of the circle, columns from the left edge
 * @param y     vertical position of the top left corner of the circle, rows from the top edge
 * @param color 16-bit color, which can be produced by ST7735_Color565()
 * @return none
 * @brief  Draw a circle 
 */
void ST7735_DrawCircle(int16_t x, int16_t y, uint16_t color);

/**
 * Pass 8-bit (each) R,G,B and get back 16-bit packed color.
 * @param r red value
 * @param g green value
 * @param b blue value
 * @return 16-bit color
 * @brief  RGB to color creation 
*/
uint16_t ST7735_Color565(uint8_t r, uint8_t g, uint8_t b);

/**
 * Swaps the red and blue values of the given 16-bit packed color;
 * green is unchanged.
 * @param x 16-bit color in format B, G, R
 * @return 16-bit color in format R, G, B
 * @brief  Swaps red and blue 
*/
uint16_t ST7735_SwapColor(uint16_t x) ;


/**
 * Displays a 16-bit color BMP image.  A bitmap file that is created
 * by a PC image processing program has a header and may be padded
 * with dummy columns so the data have four byte alignment.  This
 * function assumes that all of that has been stripped out, and the
 * array image[] has one 16-bit halfword for each pixel to be
 * displayed on the screen (encoded in reverse order, which is
 * standard for bitmap files).  An array can be created in this
 * format from a 24-bit-per-pixel .bmp file using the associated
 * <b>BmpConvert16.exe</b> converter program.
 * (x,y) is the screen location of the lower left corner of BMP image<br>
 * Requires (11 + 2*w*h) bytes of transmission (assuming image fully on screen)
 * @param x     horizontal position of the bottom left corner of the image, columns from the left edge
 * @param y     vertical position of the bottom left corner of the image, rows from the top edge
 * @param image pointer to a 16-bit color BMP image
 * @param w     number of pixels wide
 * @param h     number of pixels tall
 * @note Must be less than or equal to 128 pixels wide by 160 pixels high
 * @return none
 * @brief  Displays a BMP image 
 */
void ST7735_DrawBitmap(int16_t x, int16_t y, const uint16_t *image, int16_t w, int16_t h);

/**
 * Simple character draw function.  This is the same function from
 * Adafruit_GFX.c but adapted for this processor.  However, each call
 * to ST7735_DrawPixel() calls setAddrWindow(), which needs to send
 * many extra data and commands.  If the background color is the same
 * as the text color, no background will be printed, and text can be
 * drawn right over existing images without covering them with a box.<br>
 * Requires (11 + 2*size*size)*6*8 bytes of transmission (image fully on screen; textcolor != bgColor)
 * @param x         horizontal position of the top left corner of the character, columns from the left edge
 * @param y         vertical position of the top left corner of the character, rows from the top edge
 * @param c         character to be printed
 * @param textColor 16-bit color of the character
 * @param bgColor   16-bit color of the background
 * @param size      number of pixels per character pixel (e.g. size==2 prints each pixel of font as 2x2 square)
 * @return none
 * @brief  Draw a character 
 */
void ST7735_DrawCharS(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size);


/**
 * Advanced character draw function.  This is similar to the function
 * from Adafruit_GFX.c but adapted for this processor.  However, this
 * function only uses one call to setAddrWindow(), which allows it to
 * run at least twice as fast.<br>
 * Requires (11 + size*size*6*8) bytes of transmission (assuming image fully on screen)
 * @param x         horizontal position of the top left corner of the character, columns from the left edge
 * @param y         vertical position of the top left corner of the character, rows from the top edge
 * @param c         character to be printed
 * @param textColor 16-bit color of the character
 * @param bgColor   16-bit color of the background
 * @param size      number of pixels per character pixel (e.g. size==2 prints each pixel of font as 2x2 square)
 * @return none
 * @brief  Draw a character 
 */
 void ST7735_DrawChar(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size);


/**
 * String draw function.
 * 16 rows (0 to 15) and 21 characters (0 to 20)<br>
 * Requires (11 + size*size*6*8) bytes of transmission for each character
 * @param x         columns from the left edge (0 to 20)
 * @param y         rows from the top edge (0 to 12)
 * @param pt        pointer to a null terminated string to be printed
 * @param textColor 16-bit color of the characters
 * @return number of characters printed
 * @note bgColor is Black and size is 1
 * @brief  Draw a string 
*/
uint32_t ST7735_DrawString(uint16_t x, uint16_t y, char *pt, int16_t textColor);;


/**
 * Move the cursor to the desired X- and Y-position.  The
 * next character of the next unsigned decimal will be
 * printed here.  X=0 is the leftmost column.  Y=0 is the top row. 
 * The cursor is used by the "Out" functions, but not the "Draw" functions
 * @param newX  new X-position of the cursor (0<=newX<=20)
 * @param newY  new Y-position of the cursor (0<=newY<=15)
 * @return none
 * @brief  Move the cursor 
 */
void ST7735_SetCursor(uint32_t newX, uint32_t newY);

/**
 * Output a 32-bit number in unsigned decimal format.
 * Position determined by ST7735_SetCursor command.
 * Color set by ST7735_SetTextColor.
 * @param n         32-bit number to be transferred
 * @return none
 * @note Variable format 1-10 digits with no space before or after
 * @brief  Output an unsigned decimal 
 */
void ST7735_OutUDec(uint32_t n);


/**
 * Output a 32-bit number in unsigned 4-digit decimal format
 * with no space before or after.
 * Color set by ST7735_SetTextColor.
 * Position determined by ST7735_SetCursor command
 * @param n 32-bit number to be transferred
 * @return none
 * @note Fixed format 4 digits with no space before or after
 * @brief  Output a 4-digit unsigned decimal 
 */
void ST7735_OutUDec4(uint32_t n);

/**
 * Output a 32-bit number in unsigned 5-digit decimal format
 * with no space before or after.
 * Color set by ST7735_SetTextColor.
 * Position determined by ST7735_SetCursor command
 * @param n 32-bit number to be transferred
 * @return none
 * @note Fixed format 5 digits with no space before or after
 * @brief  Output a 5-digit unsigned decimal 
 */
void ST7735_OutUDec5(uint32_t n);

/**
 * Output a 32-bit number in unsigned 3-digit fixed point, 0.1 resolution
 * numbers 0 to 999 printed as " 0.0" to "99.9"
 * Position determined by ST7735_SetCursor command
 * @param  n 32-bit number to be transferred
 * @param textColor 16-bit color of the numbers
 * @return none
 * @note Fixed format 4 characters with no space before or after
 * @brief  Output a 2-digit fixed-point decimal 
 */
void ST7735_OutUFix2_1(uint32_t n, int16_t textColor);

/**
 * Output a 32-bit number in unsigned 2-digit hexadecimal format
 * numbers 0 to 255 printed as "00," to "FF,"
 * Position determined by ST7735_SetCursor command
 * @param  n 32-bit number to be transferred
 * @param textColor 16-bit color of the numbers
 * @return none
 * @note Fixed format 3 characters with comma after
 * @brief  Output a 2-digit hexadecimal number 
 */
void ST7735_OutUHex2(uint32_t n, int16_t textColor);

/**
 * Change the image rotation.
 * Requires 2 bytes of transmission
 * @param  m new rotation value (0 to 3)
 * @return none
 * @brief Change rotation
 */
void ST7735_SetRotation(uint8_t m) ;



/**
 * Send the command to invert all of the colors.
 * Requires 1 byte of transmission
 * @param  i 0 to disable inversion; non-zero to enable inversion
 * @return none
 * @brief  invert display
 */
void ST7735_InvertDisplay(int i) ;



/**
 * Set up the axes, labels, and other variables to
 * allow data to be plotted in a chart using the
 * functions ST7735_PlotPoint() and
 * ST7735_PlotIncrement().
 * If yLabel2 is empty string, no yLabel2 is printed, and yLabel1 is centered
 *  - graphics routines
 *       - y coordinates 0 to 31 used for labels and messages
 *       - y coordinates 32 to 159  128 pixels high
 *       - x coordinates 0 to 127   128 pixels wide
 *
 * @param axisColor   16-bit color for axes, which can be produced by LCD_Color565()
 * @param bgColor     16-bit color for plot background, which can be produced by LCD_Color565()
 * @param xLabel      pointer to a null terminated string for x-axis (~4 character space)
 * @param yLabel1     pointer to a null terminated string for top of y-axis (~3-5 character space)
 * @param label1Color 16-bit color for y-axis label1, which can be produced by LCD_Color565()
 * @param yLabel2     pointer to a null terminated string for bottom of y-axis (~3 character space)
 * @param label2Color 16-bit color for y-axis label2, which can be produced by LCD_Color565()
 * @param ymax        maximum value to be printed
 * @param ymin        minimum value to be printed
 * @return none
 * @note Assumes: ST7735_InitR() has been called
 * @brief  Initializes a plot 
 */
void ST7735_Drawaxes(uint16_t axisColor, uint16_t bgColor, char *xLabel,
  char *yLabel1, uint16_t label1Color, char *yLabel2, uint16_t label2Color,
  int32_t ymax, int32_t ymin);
  
/**
 * Clear the graphics buffer, set X coordinate to 0
 * This routine clears the display
 * @param ymin minimum plot value
 * @param ymax maximum plot value
 * @return none
 * @brief  Clear plot 
 */
void ST7735_PlotClear(int32_t ymin, int32_t ymax);


/**
 * Plot a point on the chart.  To plot several points in the
 * same column, call this function repeatedly before calling
 * ST7735PlotIncrement().  The units of the data are the
 * same as the ymax and ymin values specified in the
 * initialization function.
 * @param y  value to be plotted (units not specified)
 * @return none
 * @note Assumes: ST7735_InitR() and ST7735_Drawaxes() have been called
 * @brief  Plot one point 
 */
void ST7735_PlotPoint(int32_t y);


/**
 * Plot a point on the chart.  To plot several points in the
 * same column, call this function repeatedly before calling
 * ST7735_PlotIncrement().  The units of the data are the
 * same as the ymax and ymin values specified in the
 * initialization function.
 * @param data1  value to be plotted (units not specified)
 * @param color1 16-bit color for the point, which can be produced by ST7735_Color565()
 * @return none
 * @note Assumes: ST7735_InitR() and ST7735_Drawaxes() have been called
 * @brief  Plot one point with color
 */
void ST7735_PlotPoint2(int32_t data1, uint16_t color1);


/**
 * Increment the plot between subsequent calls to
 * ST7735_PlotPoint().  Automatically wrap and clear the
 * column to be printed to.
 * ST7735_PlotIncrement will erase the new line (clearing the display as it goes).
 * ST7735_PlotNext does not erase the new line (plots over itself as it wraps).
 * @param none
 * @return none
 * @note Assumes: ST7735_InitR() and ST7735_Drawaxes() have been called
 * @brief  Moves the plot cursor in time 
 */
void ST7735PlotIncrement(void);


/**
 * Used in the voltage versus time plot, plot line to new point
 * It does output to display
 * @param y  value to be plotted 
 * @return none
 * @note Assumes: ST7735_InitR() and ST7735_Drawaxes() have been called
 * @brief  Plot one line 
 */
void ST7735_PlotLine(int32_t y);

// *************** ST7735_PlotPoints ********************
// Used in the voltage versus time plot, plot two points at y1, y2
// It does output to display
// Inputs: y1 is the y coordinate of the first point plotted
//         y2 is the y coordinate of the second point plotted
// Outputs: none
/**
 * Used in the voltage versus time plot, plot two points at y1, y2
 * It does output to display. The units of the data are the
 * same as the ymax and ymin values specified in the
 * initialization function. Call ST7735_PlotIncrement() to move time.
 * @param y1 is the y coordinate of the first point plotted
 * @param y2 is the y coordinate of the second point plotted
 * @return none
 * @note Assumes: ST7735_InitR() and ST7735_Drawaxes() have been called
 * @brief  Plot two points 
 */
void ST7735_PlotPoints(int32_t y1,int32_t y2);


/**
 * Used in the voltage versus time bar plot, plot one bar at y
 * It does output to display. The units of the data are the
 * same as the ymax and ymin values specified in the
 * initialization function. Call ST7735_PlotIncrement() to move time.
 * @param y is the y coordinate of the bar plotted
 * @return none
 * @note Assumes: ST7735_InitR() and ST7735_Drawaxes() have been called
 * @brief  Plot one bar 
 */
void ST7735_PlotBar(int32_t y);


/**
 * Used in the amplitude versus frequency plot, plot one bar at y
 * It does output to display. 0 to 0.625V scaled on a log plot from min to max. 
 * Call ST7735_PlotIncrement() to move x axis.
 * @param y is the y ADC value of the bar plotted
 * @return none
 * @note Assumes: ST7735_InitR() and ST7735_Drawaxes() have been called
 * @brief  Plot one bar 
 */
void ST7735_PlotdBfs(int32_t y);

/**
 * Used in all the plots to step the X coordinate one pixel.
 * ST7735_PlotIncrement will erase the new line (clearing the display as it goes).
 * ST7735_PlotNext does not erase the new line (plots over itself as it wraps).
 * X steps from 0 to 127, then back to 0 again
 * It does not output to display
 * @param none
 * @return  none
 * @brief  Move x-axis parameter 
 */
void ST7735_PlotNext(void);

/**
 * Used in all the plots to step the X coordinate one pixel
 * X steps from 0 to 127, then back to 0 again
 * It clears the vertical space into which the next pixel will be drawn
 * @param none
 * @return  none
 * @brief  Move X coordinate  
 */
void ST7735_PlotNextErase(void);

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


/**
 * Output one character to the LCD
 * Position determined by ST7735_SetCursor command
 * Color set by ST7735_SetTextColor
 * @param ch 8-bit ASCII character
 * @return none
 * @brief  Output a character 
 */
void ST7735_OutChar(char ch);


/**
 * Print a string of characters to the ST7735 LCD.
 * Position determined by ST7735_SetCursor command
 * Color set by ST7735_SetTextColor
 * The string will not automatically wrap.
 * @param ptr  pointer to NULL-terminated ASCII string
 * @return none
 * @brief  Output a string 
 */
void ST7735_OutString(char *ptr);


/**
 * Sets the color in which the characters will be printed
 * Background color is fixed at black
 * @param color 16-bit packed color
 * @return none
 * @brief  sets the text color 
 */
void ST7735_SetTextColor(uint16_t color);

/**
 * Initialize the ST7735 for printf
 * @param none
 * @return none
 * @brief  use ST7735 LCD to output from printf 
 */
void ST7735_InitPrintf(void);


/**
 * Outputs signed fixed point number to LCD.
 * The format signed 32-bit with resolution 0.01.
 * The range -99.99 to +99.99
<table>
<caption id="ST7735_sDecOut2">ST7735_sDecOut2 </caption>
<tr><th>Parameter <th>LCD display
<tr><td> 12345    <td>" **.**"
<tr><td>  2345    <td>" 23.45"  
<tr><td> -8100    <td><td>"-81.00"
<tr><td>  -102    <td>" -1.02" 
<tr><td>    31    <td>"  0.31" 
<tr><td>-12345    <td>"-**.**"
</table>
 * @param n signed 32-bit integer part of fixed-point number
 * @return none
 * @brief fixed point output resolution 0.01
 * @note send exactly 6 characters to the LCD 
 */ 
void ST7735_sDecOut2(int32_t n);



/**
 * unsigned 32-bit binary fixed-point with a resolution of 1/64. 
 * The full-scale range is from 0 to 999.99. 
 * If the integer part is larger than 63999, it signifies an error. 
 * The ST7735_uBinOut6 function takes an unsigned 32-bit integer part 
 * of the binary fixed-point number and outputs the fixed-point value on the LCD
<table>
<caption id="ST7735_uBinOut6">ST7735_uBinOut6 </caption>
<tr><th>Parameter <th>LCD display
<tr><td>    0	  <td>"  0.00"
<tr><td>    1	  <td>"  0.01"
<tr><td>   16     <td>"  0.25"
<tr><td>   25	  <td>"  0.39"
<tr><td>  125	  <td>"  1.95"
<tr><td>  128	  <td>"  2.00"
<tr><td> 1250	  <td>" 19.53"
<tr><td> 7500	  <td>"117.19"
<tr><td>63999	  <td>"999.99"
<tr><td>64000	  <td>"***.**"
</table>
 * @param n unsigned 32-bit integer part of binary fixed-point number
 * @return none
 * @brief fixed point output resolution 1/64
 * @note send exactly 6 characters to the LCD 
 */ 
void ST7735_uBinOut6(uint32_t n); 

/**
 * Specify the X and Y axes for an x-y scatter plot
 * Draw the title and clear the plot area
 * @param title  ASCII string to label the plot, null-termination
 * @param minX   smallest X data value allowed, resolution= 0.001
 * @param maxX   largest X data value allowed, resolution= 0.001
 * @param minY   smallest Y data value allowed, resolution= 0.001
 * @param maxY   largest Y data value allowed, resolution= 0.001
 * @return none
 * @note assumes minX < maxX, and miny < maxY
 * @brief initialize XY plot
*/
void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY);

/**
 * Plot an array of (x,y) data, neglect any points outside the minX maxY minY maxY bounds
 * @param num    number of data points in the two arrays
 * @param bufX   array of 32-bit fixed-point data, resolution= 0.001
 * @param bufY   array of 32-bit fixed-point data, resolution= 0.001
 * @return none
 * @note assumes ST7735_XYplotInit has been previously called
 * @brief XY plot
*/
void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]);

/**
 * Draws one line on the ST7735 color LCD<br>
 * - (x1,y1) is the start point
 * - (x2,y2) is the end point
 * - x1 x2 must be less than 128, 0 is on the left, 126 is near the right
 * - y1 y2 must be less than 160, 159 is near the wires, 0 is the side opposite the wires
 *
 * @param x1 horizonal position
 * @param x2 horizonal position    
 * @param y1 vertical position
 * @param y2 vertical position
 * @param color 16-bit color, which can be produced by ST7735_Color565() 
 * @return none
 * @brief Draws line
*/
void ST7735_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, 
                 uint16_t color);

/**
 * Used in all the plots to change the X coordinate to new location
 * X exists in the range from 0 to 127,
 * Input values less than 0 get changed to 0,
 * Input values greater than 127 get changed to 127
 * It does not output to display
 * @param newX is the new value that the global X will be
 * @return none
 * @brief set X-position
*/
void ST7735_SetX(int32_t newX);


#endif
/** @}*/