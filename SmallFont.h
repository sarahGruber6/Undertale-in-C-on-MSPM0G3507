// SmallFont.h
// Jonathan Valvano 
// draws small numbers
// 11/5/2023
#ifndef __SmallFont_h
#define __SmallFont_h
#include <stdint.h>

// **********SmallFont_OutHorizontal*******************
// draw score on screen horizontally as 4-digit number
// font is 5 wide-6 high Courier bold yellow on black
// Input: n is between 0 and 9999
//        x     horizontal position of the bottom left corner of the image, columns from the left edge
//        y     vertical position of the bottom left corner of the image, rows from the top edge
// Output: none
void SmallFont_OutHorizontal(uint32_t n, int16_t x, int16_t y);

// **********smallFont_OutVertical*******************
// draw score on screen vertically as 4-digit number
// font is 6 wide-5 high Courier bold yellow on black
// Input: n is between 0 and 9999
//        x     horizontal position of the bottom left corner of the image, columns from the left edge
//        y     vertical position of the bottom left corner of the image, rows from the top edge
// Output: none
void SmallFont_OutVertical(uint32_t n, int16_t x, int16_t y);

#endif
