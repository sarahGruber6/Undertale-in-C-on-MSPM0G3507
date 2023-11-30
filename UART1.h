/*!
 * @defgroup UART
 * @brief Asynchronous serial communication
 <table>
<caption id="UARTpins4">UART pins on the MSPM0G3507</caption>
<tr><th>Pin <th>Description
<tr><td>PA8 <td>UART1 Tx to other microcontroller PA9 Rx
</table>
 * @{*/
/**
 * @file      UART1.h
 * @brief     Initialize UART1, no synchronization on transmit
 * @details   UART1 initialization. 2000 bps baud,
 * 1 start, 8 data bits, 1 stop, no parity.<br>

 * @version   ECE319K v1.0
 * @author    your name
 * @copyright lab 8 solution, do not post,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      October 29, 2023
 <table>
<caption id="UARTpins3">UART pins on the MSPM0G3507</caption>
<tr><th>Pin  <th>Description
<tr><td>PA8 <td>UART1 Tx to other microcontroller PA22 UART2 Rx
</table>
  ******************************************************************************/
#ifndef __UART1_H__
#define __UART1_H__

/**
 * initialize UART1 for 2000 bps baud rate.<br>
 * PA8 UART1 Tx to other microcontroller PA9 Rx<br>
 * no synchronization on transmit, no receiving
 * @param none
 * @return none
 * @brief  Initialize UART1
*/
void UART1_Init(void);



/**
 * Output 8-bit to serial port UART1<br>
 * Uses no synchronization<br>
 * This function does not wait, if the transmitter is busy, data will be lost
 * @param data is an 8-bit ASCII character to be transferred
 * @return none
 * @brief output character to UART1
 */
void UART1_OutChar(char data);


#endif // __UART1_H__
/** @}*/
