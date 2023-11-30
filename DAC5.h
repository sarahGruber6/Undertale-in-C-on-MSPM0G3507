/**
 * @file      DAC5.h
 * @brief     5-bit binary-weighted DAC
 * @details   Lab 5. Digital Piano using a Digital to Analog Converter<br>
 * For more information see<br> https://docs.google.com/document/d/1DVmHjqsx24FvLp4x8KKyr5UmDeDlBlfeQPsVvKFQF_k/edit<br>
 * @version   ECE319K Fall 2023
 * @author    Your names go here
 * @copyright Lab solution, do not post,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      October 2, 2023
  <table>
<caption id="Lab5pins">Pins on the 5-bit DAC</caption>
<tr><th>Pin <th>GPIO<th>Hardware
<tr><td>PB4 <td>output<td>bit 4 (most significant)
<tr><td>PB3 <td>output<td>bit 3
<tr><td>PB2 <td>output<td>bit 2
<tr><td>PB1 <td>output<td>bit 1
<tr><td>PB0 <td>output<td>bit 0 (least significant)
</table>
  ******************************************************************************/
#ifndef __DAC5_H__
#define __DAC5_H__

/**
 * Initialize 5-bit DAC
 *    - PB4 output MSBit
 *    - PB3 output
 *    - PB2 output
 *    - PB1 output
 *    - PB0 output LSBit
 * @param none
 * @return none
 * @brief  Initialize DAC
 * @note LaunchPad_Init has been called; this program should not reset Port B
 */
void DAC5_Init(void);

/**
 * Output to 5-bit DAC
 * @param data is 5-bit value to output, 0 to 31
 * @return none
 * @brief  Output to DAC5
 * @note this solution must be friendly
 */
void DAC5_Out(uint32_t data);

#endif // __DAC5_H__
