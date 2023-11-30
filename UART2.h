/*!
 * @defgroup UART
 * @brief Asynchronous serial communication
 <table>
<caption id="UARTpins6">UART pins on the MSPM0G3507</caption>
<tr><th>Pin <th>Description
<tr><td>PA22 <td>UART1 Rx from other microcontroller PA8 Tx
</table>
 * @{*/
/**
 * @file      UART2.h
 * @brief     Initialize UART2, no transmit, interrupt on receive timeout
 * @details   UART2 initialization. 2000 bps baud,
 * 1 start, 8 data bits, 1 stop, no parity.<br>

 * @version   ECE319K v1.0
 * @author    your name
 * @copyright lab 8 solution, do not post,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      October 29, 2023
 <table>
<caption id="UARTpins7">UART pins on the MSPM0G3507</caption>
<tr><th>Pin  <th>Description
<tr><td>PA22 <td>UART2 Rx from other microcontroller PA8 Tx
<tr><td>J14 <td>Connect SW1 to PA9 (not PB23)
</table>
  ******************************************************************************/
#ifndef __UART2_H__
#define __UART2_H__

/**
 * initialize UART2 for 2000 bps baud rate.<br>
 * PA22 UART2 Rx from other microcontroller PA8 Tx<br>
 * no transmit, interrupt on receive timeout
 * @param none
 * @return none
 * @brief  Initialize UART1
*/
void UART2_Init(void);

/**
 * Get new serial port receive data from FIFO1<br>
 * Return 0 if the FIFO1 is empty<br>
 * Return nonzero data from the FIFO1 if available
 * @param none
 * @return char ASCII code from other computer
 * @brief get data from FIFO1
 */
char UART2_InChar(void);




#endif // __UART2_H__
/** @}*/
