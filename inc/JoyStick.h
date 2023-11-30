/*!
 * @defgroup MKII
 * @brief MKII boosterpack joystick
 <table>
<caption id="MKIIjoystickpins">MKII Joystick pins </caption>
<tr><th>Pin <th>Function<th>Description
<tr><td>PA26<td>GPIO  <td>J1.5 joystick Select button (digital)   
<tr><td>PA15<td>ADC0.2<td>J1.2 joystick horizontal (X) (analog)     
<tr><td>PA18<td>ADC1.3<td>J1.10 ambient light (OPT3001) and temperature sensor (TMP006)    
<tr><td>PA12<td>GPIO  <td>J4.33 user Button1 (bottom) (digital)   
<tr><td>PA11<td>GPIO  <td>J4.32 user Button2 (top) (digital) (conflicts with UART0 Rx from XDS Tx)   
</table>
 * @{*/
/**
 * @file      JoyStick.h
 * @brief     Joystick on MKII boosterpack
 * @details   Software driver functions for joystick on MKII boosterpack<br>
 * https://www.ti.com/tool/BOOSTXL-EDUMKII <br>

 * @version   ECE319K v1.0
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2023 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      August 14, 2023
 * \image html MKIIBoosterPack.png width=500px 
<br>
 <table>
<caption id="MKIIjoystickpins2">MKII Joystick pins </caption>
<tr><th>Pin <th>Function<th>Description
<tr><td>PA26<td>GPIO  <td>J1.5 joystick Select button (digital)   
<tr><td>PA15<td>ADC0.2<td>J1.2 joystick horizontal (X) (analog)     
<tr><td>PA18<td>ADC1.3<td>J1.10 ambient light (OPT3001) and temperature sensor (TMP006)    
<tr><td>PA12<td>GPIO  <td>J4.33 user Button1 (bottom) (digital)   
<tr><td>PA11<td>GPIO  <td>J4.32 user Button2 (top) (digital) (conflicts with UART0 Rx from XDS Tx)   
</table>
  ******************************************************************************/


#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__
/**
 * Initialize MKII JoyStick, JoyStick button, and MKI button1 
 * @param none
 * @return none 
 * @see JoyStick_In() JoyStick_InButton() JoyStick_InButton1()
 * @brief  Initialize JoyStick
 * @note Button2 not initialized because it conflicts with UART0 Rx from XDS Tx
 */
void JoyStick_Init(void);


/**
 * Read JoyStick negative logic button. Input from MKII JoyStick button (pushing down on stick) 
 * @param none
 * @return 0 if pressed, nonzero if not pressed 
 * @see JoyStick_Init()
 * @brief  Input JoyStick button
 */
uint32_t JoyStick_InButton(void);


/**
 * ead JoyStick position (x,y) 
 * @param x pointer to empty 32 bit unsigned variable
 * @param y pointer to empty 32 bit unsigned variable
 * @return none 
 * @see JoyStick_Init()
 * @brief  Read JoyStick
 */
void JoyStick_In(uint32_t *x, uint32_t *y);

// Read Button1 button (bottom)
// Input: none
// Output: 0 if pressed, nonzero if not pressed
/**
 * Read Button1 button on MKII  
 * @param none
 * @return 0 if pressed, nonzero if not pressed 
 * @see JoyStick_Init()
 * @brief  Input MKII button1
 * @note Button2 not initialized because it conflicts with UART0 Rx from XDS Tx
 */
uint32_t JoyStick_InButton1(void);


#endif
/** @}*/