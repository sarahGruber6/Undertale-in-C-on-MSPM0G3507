/*!
 * @defgroup DAC
 * @brief Digital to analog conversion
 <table>
<caption id="DACpins">DAC pin on the MSPM0G3507</caption>
<tr><th>Pin  <th>Description
<tr><td>PA15 <td>DAC digital to analog output
</table>
 * @{*/
/**
 * @file      DAC.h
 * @brief     Initialize 12-bit built-in DAC
 * @details   DAC output, 12-bit precision<br>
 * The DAC range is 0 to 2.5V.<br>

 * @version   ECE319K v1.0
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2023 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      August 13, 2023
 <table>
<caption id="DACpins2">DAC pin on the MSPM0G3507</caption>
<tr><th>Pin  <th>Description
<tr><td>PA15 <td>DAC digital to analog output
</table>
  ******************************************************************************/


#ifndef __DAC_H__
#define __DAC_H__


/**
 * Initialize 12-bit DAC output on PA15 
 * @param none
 * @return none 
 * @see DAC_Out()
 * @brief  Initialize 12-bit DAC
 * @note Range is 0 to 3.3V
 */
void DAC_Init(void);
 
 
/**
 * Output to 12-bit DAC, 0 to 2.5V range
 * @param data is 12-bit data
 * @return none 
 * @see DAC_Init()
 * @brief  Output to DAC
 */
void DAC_Out(uint32_t data);

#endif
/** @}*/