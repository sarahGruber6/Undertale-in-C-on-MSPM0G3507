/*!
 * @defgroup MKII
 * @brief MKII boosterpack LCD
 <table>
<caption id="MKIILCDpins">MKII LCD pins </caption>
<tr><th>Pin <th>Function  <th>Description
<tr><td>PB9 <td>SPI1 SCLK <td>J1.7 LCD SPI clock (SPI)   
<tr><td>PB6 <td>SPI1 CS0  <td>J2.13 LCD SPI CS (SPI)     
<tr><td>PB8 <td>SPI1 PICO <td>J2.15 LCD SPI data (SPI)    
<tr><td>PB15<td>GPIO      <td>J2.17 LCD !RST (digital)   
<tr><td>PA13<td>GPIO      <td>J4.31 LCD RS (digital)   
</table>
 * @{*/
/**
 * @file      LCD.h
 * @brief     LCD on MKII boosterpack
 * @details   Software driver functions for joystick on MKII boosterpack<br>
 * https://www.ti.com/tool/BOOSTXL-EDUMKII <br>
 * Runs with an Educational BoosterPack MKII (BOOSTXL-EDUMKII).
 * This file contains the function prototypes for the software interface to the MKII BoosterPack.
 * This board support package (BSP) is an abstraction layer,
 * forming a bridge between the low-level hardware and the high-level software.
 * The Crystalfontz CFAF128128B-0145T color 128x128-pixel TFT LCD supports display updates up to 20 frames
 * per second (FPS) while only requiring a few lines to control the TFT LCD module through the SPI interface. This
 * module has a color depth of 262K colors and a contrast ratio of 350. The reference designator for the color LCD
 * is LCD1. More information on the color LCD can be found at<br>
 * https://www.crystalfontz.com 
 * @version   ECE319K v1.0
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2023 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      August 14, 2023
 * \image html MKIIBoosterPack.png width=500px 
<br>
 <table>
<caption id="MKIILCDpins2">MKII LCD pins </caption>
<tr><th>Pin <th>Function  <th>Description
<tr><td>PB9 <td>SPI1 SCLK <td>J1.7 LCD SPI clock (SPI)   
<tr><td>PB6 <td>SPI1 CS0  <td>J2.13 LCD SPI CS (SPI)     
<tr><td>PB8 <td>SPI1 PICO <td>J2.15 LCD SPI data (SPI)    
<tr><td>PB15<td>GPIO      <td>J2.17 LCD !RST (digital)   
<tr><td>PA13<td>GPIO      <td>J4.31 LCD RS (digital)   
</table>
  ******************************************************************************/





//  J1   J3               J4   J2
// [ 1] [21]             [40] [20]
// [ 2] [22]             [39] [19]
// [ 3] [23]             [38] [18]
// [ 4] [24]             [37] [17]
// [ 5] [25]             [36] [16]
// [ 6] [26]             [35] [15]
// [ 7] [27]             [34] [14]
// [ 8] [28]             [33] [13]
// [ 9] [29]             [32] [12]
// [10] [30]             [31] [11]

// Connected pins in physical order
// J1.1 +3.3V (power)
// J1.2 joystick horizontal (X) (analog) {TM4C123 PB5/AIN11, MSP432 P6.0}
// J1.3 UART from Bluetooth to LaunchPad (UART) {TM4C123 PB0, MSP432 P3.2}
// J1.4 UART from LaunchPad to Bluetooth (UART) {TM4C123 PB1, MSP432 P3.3}
// J1.5 joystick Select button (digital) {TM4C123 PE4, MSP432 P4.1}
// J1.6 microphone (analog)              {TM4C123 PE5/AIN8, MSP432 P4.3}
// J1.7 LCD SPI clock (SPI)              {TM4C123 PB4, MSP432 P1.5}
// J1.8 ambient light (OPT3001) interrupt (digital) {TM4C123 PA5, MSP432 P4.6}
// J1.9 ambient light (OPT3001) and temperature sensor (TMP006) I2C SCL (I2C)  {TM4C123 PA6, MSP432 P6.5}
// J1.10 ambient light (OPT3001) and temperature sensor (TMP006) I2C SDA (I2C) {TM4C123 PA7, MSP432 P6.4}
//--------------------------------------------------
// J2.11 temperature sensor (TMP006) interrupt (digital) {TM4C123 PA2, MSP432 P3.6}
// J2.12 nothing                         {TM4C123 PA3, MSP432 P5.2}
// J2.13 LCD SPI CS (SPI)                {TM4C123 PA4, MSP432 P5.0}
// J2.14 nothing                         {TM4C123 PB6, MSP432 P1.7}
// J2.15 LCD SPI data (SPI)              {TM4C123 PB7, MSP432 P1.6}
// J2.16 nothing (reset)
// J2.17 LCD !RST (digital)              {TM4C123 PF0, MSP432 P5.7}
// J2.18 Profile 4                       {TM4C123 PE0, MSP432 P3.0}
// J2.19 servo PWM                       {TM4C123 PB2, MSP432 P2.5}
// J2.20 GND (ground)
//--------------------------------------------------
// J3.21 +5V (power)
// J3.22 GND (ground)
// J3.23 accelerometer X (analog)        {TM4C123 PD0/AIN7, MSP432 P6.1}
// J3.24 accelerometer Y (analog)        {TM4C123 PD1/AIN6, MSP432 P4.0}
// J3.25 accelerometer Z (analog)        {TM4C123 PD2/AIN5, MSP432 P4.2}
// J3.26 joystick vertical (Y) (analog)  {TM4C123 PD3/AIN4, MSP432 P4.4}
// J3.27 Profile 0                       {TM4C123 PE1, MSP432 P4.5}
// J3.28 Profile 1                       {TM4C123 PE2, MSP432 P4.7}
// J3.29 Profile 2                       {TM4C123 PE3, MSP432 P5.4}
// J3.30 Profile 3                       {TM4C123 PF1, MSP432 P5.5}
//--------------------------------------------------
// J4.31 LCD RS (digital)                {TM4C123 PF4, MSP432 P3.7}
// J4.32 user Button2 (bottom) (digital) {TM4C123 PD7, MSP432 P3.5}
// J4.33 user Button1 (top) (digital)    {TM4C123 PD6, MSP432 P5,1}
// J4.34 Profile 6/gator hole switch     {TM4C123 PC7, MSP432 P2.3}
// J4.35 nothing                         {TM4C123 PC6, MSP432 P6.7}
// J4.36 Profile 5                       {TM4C123 PC5, MSP432 P6.6}
// J4.37 RGB LED blue (PWM)              {TM4C123 PC4, MSP432 P5.6}
// J4.38 RGB LED green (PWM)             {TM4C123 PB3, MSP432 P2.4}
// J4.39 RGB LED red (jumper up) or LCD backlight (jumper down) (PWM) {TM4C123 PF3, MSP432 P2.6}
// J4.40 buzzer (PWM)                    {TM4C123 PF2, MSP432 P2.7}
//--------------------------------------------------
// Connected pins in logic order
// power and reset
// J1.1 +3.3V (power)
// J3.21 +5V (power)
// J3.22 GND (ground)
// J2.20 GND (ground)
// J2.16 nothing (reset)
//--------------------------------------------------
// LCD graphics
// J1.7 LCD SPI clock (SPI)              {TM4C123 PB4, MSP432 P1.5}
// J2.13 LCD SPI CS (SPI)                {TM4C123 PA4, MSP432 P5.0}
// J2.15 LCD SPI data (SPI)              {TM4C123 PB7, MSP432 P1.6}
// J2.17 LCD !RST (digital)              {TM4C123 PF0, MSP432 P5.7}
// J4.31 LCD RS (digital)                {TM4C123 PF4, MSP432 P3.7}
//--------------------------------------------------
// 3-color LED
// J4.37 RGB LED blue (PWM)              {TM4C123 PC4, MSP432 P5.6}
// J4.38 RGB LED green (PWM)             {TM4C123 PB3, MSP432 P2.4}
// J4.39 RGB LED red (jumper up) or LCD backlight (jumper down) (PWM) {TM4C123 PF3, MSP432 P2.6}
//--------------------------------------------------
// user buttons
// J4.32 user Button2 (bottom) (digital) {TM4C123 PD7, MSP432 P3.5}
// J4.33 user Button1 (top) (digital)    {TM4C123 PD6, MSP432 P5.1}
//--------------------------------------------------
// buzzer output
// J4.40 buzzer (PWM)                    {TM4C123 PF2, MSP432 P2.7}
//--------------------------------------------------
// Joystick
// J1.5 joystick Select button (digital) {TM4C123 PE4, MSP432 P4.1}
// J1.2 joystick horizontal (X) (analog) {TM4C123 PB5/AIN11, MSP432 P6.0}
// J3.26 joystick vertical (Y) (analog)  {TM4C123 PD3/AIN4, MSP432 P4.4}
//--------------------------------------------------
// accelerometer
// J3.23 accelerometer X (analog)        {TM4C123 PD0/AIN7, MSP432 P6.1}
// J3.24 accelerometer Y (analog)        {TM4C123 PD1/AIN6, MSP432 P4.0}
// J3.25 accelerometer Z (analog)        {TM4C123 PD2/AIN5, MSP432 P4.2}
//--------------------------------------------------
// microphone
// J1.6 microphone (analog)              {TM4C123 PE5/AIN8, MSP432 P4.3}
//--------------------------------------------------
// light and temperature sensors (I2C)
// J1.8 ambient light (OPT3001) interrupt (digital) {TM4C123 PA5, MSP432 P4.6}
// J1.9 ambient light (OPT3001) and temperature sensor (TMP006) I2C SCL (I2C)  {TM4C123 PA6, MSP432 P6.5}
// J1.10 ambient light (OPT3001) and temperature sensor (TMP006) I2C SDA (I2C) {TM4C123 PA7, MSP432 P6.4}
// J2.11 temperature sensor (TMP006) interrupt (digital) {TM4C123 PA2, MSP432 P3.6}
//--------------------------------------------------
// Bluetooth booster
// J1.3 UART from Bluetooth to LaunchPad (UART) {TM4C123 PB0, MSP432 P3.2}
// J1.4 UART from LaunchPad to Bluetooth (UART) {TM4C123 PB1, MSP432 P3.3}
//--------------------------------------------------
// profile pins
// J3.27 Profile 0                       {TM4C123 PE1, MSP432 P4.5}
// J3.28 Profile 1                       {TM4C123 PE2, MSP432 P4.7}
// J3.29 Profile 2                       {TM4C123 PE3, MSP432 P5.4}
// J3.30 Profile 3                       {TM4C123 PF1, MSP432 P5.5}
// J2.18 Profile 4                       {TM4C123 PE0, MSP432 P3.0}
// J4.36 Profile 5                       {TM4C123 PC5, MSP432 P6.6}
// J4.34 Profile 6                       {TM4C123 PC7, MSP432 P2.3}
//--------------------------------------------------
// unconnected pins
// J2.12 nothing                         {TM4C123 PA3, MSP432 P5.2}
// J2.14 nothing                         {TM4C123 PB6, MSP432 P1.7}
// J2.19 servo PWM                       {TM4C123 PB2, MSP432 P2.5}
// J4.35 nothing                         {TM4C123 PC6, MSP432 P6.7}
#ifndef __LCD_H__
#define __LCD_H__
/**
 * \brief The following constants are possible colors for the LCD in RGB format
 */
//color constants                  red  grn  blu
#define LCD_BLACK      0x0000   //   0,   0,   0
#define LCD_BLUE       0x001F   //   0,   0, 255
#define LCD_DARKBLUE   0x34BF   //  50, 150, 255
#define LCD_RED        0xF800   // 255,   0,   0
#define LCD_GREEN      0x07E0   //   0, 255,   0
#define LCD_LIGHTGREEN 0x07EF   //   0, 255, 120
#define LCD_ORANGE     0xFD60   // 255, 175,   0
#define LCD_CYAN       0x07FF   //   0, 255, 255
#define LCD_MAGENTA    0xF81F   // 255,   0, 255
#define LCD_YELLOW     0xFFE0   // 255, 255,   0
#define LCD_WHITE      0xFFFF   // 255, 255, 255
#define LCD_GREY       0x8410   // 128, 128, 128
/**
 * \brief Set STANDARD_GALACTIC_ALPHABET to 1 for Commander Keen alphabet
 */
#define STANDARD_GALACTIC_ALPHABET 0


/**
 * Initialize Crystalfontz CFAF128128B-0145T color 128x128-pixel TFT LCD 
 * BoosterPack pins 
 *    - J1.7  PB9 (SPI CLK) 
 *    - J2.13 PB6 (SPI CS) 
 *    - J2.15 PB8 (SPI MOSI) 
 *    - J2.17 PB15(LCD ~RST) and 
 *    - J4.31 PA13(LCD DC)
 * @param none
 * @return none 
 * @brief  Initialize MKII LCD
 * @note assumes GPIOA and GPIOB are reset and powered previously
 */
void LCD_Init(void);


/**
 * Color the pixel at the given coordinates with the given color.<br>
 * Requires 13 bytes of transmission<br>
 * x must be less than 128<br>
 * x 0 is on the left, 126 is near the right  <br>
 * y must be less than 128
 * y 126 is near the wires, 0 is the side opposite the wires
 * @param x     horizontal position of the pixel, columns from the left edge
 * @param y     vertical position of the pixel, rows from the top edge
 * @param color 16-bit color, which can be produced by LCD_Color565()
 * @return none
 * @brief  Color one pixel
 */
void LCD_DrawPixel(int16_t x, int16_t y, uint16_t color);


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
void LCD_DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);


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
void LCD_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);


/**
 * Fill the screen with the given color.<br>
 * Requires 33,293 bytes of transmission
 * @param color 16-bit color, which can be produced by LCD_Color565()
 * @return none
 * @brief  Fill the screen
 */
void LCD_FillScreen(uint16_t color);


/**
 * Draw a filled rectangle at the given coordinates with the given width, height, and color.<br>
 * Requires (11 + 2*w*h) bytes of transmission (assuming image fully on screen)
 * @param x     horizontal position of the top left corner of the rectangle, columns from the left edge
 * @param y     vertical position of the top left corner of the rectangle, rows from the top edge
 * @param w     horizontal width of the rectangle
 * @param h     vertical height of the rectangle
 * @param color 16-bit color, which can be produced by LCD_Color565()
 * @return none
 * @brief  Draw a filled rectangle 
 */
void LCD_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);


/**
 * Pass 8-bit (each) R,G,B and get back 16-bit packed color.
 * @param r red value
 * @param g green value
 * @param b blue value
 * @return 16-bit color
 * @brief  RGB to color creation 
*/
uint16_t LCD_Color565(uint8_t r, uint8_t g, uint8_t b);


/**
 * Swaps the red and blue values of the given 16-bit packed color;
 * green is unchanged.
 * @param x 16-bit color in format B, G, R
 * @return 16-bit color in format R, G, B
 * @brief  Swaps red and blue 
*/
uint16_t LCD_SwapColor(uint16_t x);


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
 * @note Must be less than or equal to 128 pixels wide by 128 pixels high
 * @return none
 * @brief  Displays a BMP image 
 */
void LCD_DrawBitmap(int16_t x, int16_t y, const uint16_t *image, int16_t w, int16_t h);


/**
 * Simple character draw function.  This is the same function from
 * Adafruit_GFX.c but adapted for this processor.  However, each call
 * to LCD_DrawPixel() calls setAddrWindow(), which needs to send
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
void LCD_DrawCharS(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size);


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
void LCD_DrawChar(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size);


/**
 * String draw function.
 * 13 rows (0 to 12) and 21 characters (0 to 20)<br>
 * Requires (11 + size*size*6*8) bytes of transmission for each character
 * @param x         columns from the left edge (0 to 20)
 * @param y         rows from the top edge (0 to 12)
 * @param pt        pointer to a null terminated string to be printed
 * @param textColor 16-bit color of the characters
 * @return number of characters printed
 * @note bgColor is Black and size is 1
 * @brief  Draw a string 
*/
uint32_t LCD_DrawString(uint16_t x, uint16_t y, char *pt, int16_t textColor);


/**
 * Move the cursor to the desired X- and Y-position.  The
 * next character of the next unsigned decimal will be
 * printed here.  X=0 is the leftmost column.  Y=0 is the top row. 
 * The cursor is used by the "Out" functions, but not the "Draw" functions
 * @param newX  new X-position of the cursor (0<=newX<=20)
 * @param newY  new Y-position of the cursor (0<=newY<=12)
 * @return none
 * @brief  Move the cursor 
 */
void LCD_SetCursor(uint32_t newX, uint32_t newY);

/**
 * Output a 32-bit number in unsigned decimal format
 * Position determined by LCD_SetCursor command
 * @param n         32-bit number to be transferred
 * @param textColor 16-bit color of the numbers
 * @return none
 * @note Variable format 1-10 digits with no space before or after
 * @brief  Output an unsigned decimal 
 */
void LCD_OutUDec(uint32_t n, int16_t textColor);

/**
 * Output a 32-bit number in unsigned 4-digit decimal format
 * with no space before or after.
 * Position determined by LCD_SetCursor command
 * @param n 32-bit number to be transferred
 * @param textColor 16-bit color of the numbers
 * @return none
 * @note Fixed format 4 digits with no space before or after
 * @brief  Output a 4-digit unsigned decimal 
 */
void LCD_OutUDec4(uint32_t n, int16_t textColor);

/**
 * Output a 32-bit number in unsigned 5-digit decimal format
 * Position determined by LCD_SetCursor command
 * @param n 32-bit number to be transferred
 * @param textColor 16-bit color of the numbers
 * @return none
 * @note Fixed format 5 digits with no space before or after
 * @brief  Output a 5-digit unsigned decimal 
 */
void LCD_OutUDec5(uint32_t n, int16_t textColor);

/**
 * Output a 32-bit number in unsigned 3-digit fixed point, 0.1 resolution
 * numbers 0 to 999 printed as " 0.0" to "99.9"
 * Position determined by LCD_SetCursor command
 * @param  n 32-bit number to be transferred
 * @param textColor 16-bit color of the numbers
 * @return none
 * @note Fixed format 4 characters with no space before or after
 * @brief  Output a 2-digit fixed-point decimal 
 */
void LCD_OutUFix2_1(uint32_t n, int16_t textColor);

/**
 * Output a 32-bit number in unsigned 2-digit hexadecimal format
 * numbers 0 to 255 printed as "00," to "FF,"
 * Position determined by LCD_SetCursor command
 * @param  n 32-bit number to be transferred
 * @param textColor 16-bit color of the numbers
 * @return none
 * @note Fixed format 3 characters with comma after
 * @brief  Output a 2-digit hexadecimal number 
 */
void LCD_OutUHex2(uint32_t n, int16_t textColor);

/**
 * Output one character to the LCD
 * Position determined by LCD_SetCursor command
 * Color set by LCD_SetTextColor
 * @param ch 8-bit ASCII character
 * @return none
 * @brief  Output a character 
 */
void LCD_OutChar(char ch);

/**
 * Print a string of characters to the ST7735 LCD.
 * Position determined by LCD_SetCursor command
 * Color set by LCD_SetTextColor
 * The string will not automatically wrap.
 * @param ptr  pointer to NULL-terminated ASCII string
 * @return none
 * @brief  Output a string 
 */
void LCD_OutString(char *ptr);

/**
 * Sets the color in which the characters will be printed
 * Background color is fixed at black
 * @param color 16-bit packed color
 * @return none
 * @brief  sets the text color 
 */
void LCD_SetTextColor(uint16_t color);

/**
 * Set up the axes, labels, and other variables to
 * allow data to be plotted in a chart using the
 * functions LCD_PlotPoint() and
 * LCD_PlotIncrement().
 * If yLabel2 is empty string, no yLabel2 is printed, and yLabel1 is centered
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
 * @note Assumes: LCD_Init() has been called
 * @brief  Initializes a plot 
 */
void LCD_Drawaxes(uint16_t axisColor, uint16_t bgColor, char *xLabel,
  char *yLabel1, uint16_t label1Color, char *yLabel2, uint16_t label2Color,
  int32_t ymax, int32_t ymin);


/**
 * Plot a point on the chart.  To plot several points in the
 * same column, call this function repeatedly before calling
 * LCD_PlotIncrement().  The units of the data are the
 * same as the ymax and ymin values specified in the
 * initialization function.
 * @param data1  value to be plotted (units not specified)
 * @param color1 16-bit color for the point, which can be produced by LCD_Color565()
 * @return none
 * @note Assumes: LCD_Init() and LCD_Drawaxes() have been called
 * @brief  Plot one point 
 */
void LCD_PlotPoint(int32_t data1, uint16_t color1);


/**
 * Increment the plot between subsequent calls to
// LCD_PlotPoint().  Automatically wrap and clear the
// column to be printed to.
 * @param none
 * @return none
 * @note Assumes: LCD_Init() and LCD_Drawaxes() have been called
 * @brief  Moves the plot cursor in time 
 */
void LCD_PlotIncrement(void);

#endif
/** @}*/
