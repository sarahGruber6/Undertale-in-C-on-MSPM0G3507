/*!
 * @defgroup EdgeTriggered
 * @brief Edge triggered interrupts
 <table>
<caption id="Edgepins">Edge triggered pin</caption>
<tr><th>Pin  <th>Description
<tr><td>PB21 <td>S3 negative logic switch
</table>
 * @{*/
/**
 * @file      EdgeTriggered.h
 * @brief     Edge triggered interrupts
 * @details   Edge triggered interrupts on PB21.
 * Interrupts on both rise and fall. 
 * Since the switch is negative logic, there is a fall on touch,
 * and a rise on release.
 * @version   ECE319K v1.0
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2023 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      August 13, 2023
 <table>
<caption id="Edgepins2">Edge triggered pin</caption>
<tr><th>Pin  <th>Description
<tr><td>PB21 <td>S3 negative logic switch
</table>
  ******************************************************************************/
  

#ifndef __EDGETRIGGERED_H__
#define __EDGETRIGGERED_H__

/**
 * Arm interrupts on rise and fall of PB21 
 * @param none
 * @return none 
 * @see LaunchPad_Init()
 * @brief  Initialize edge-triggered interrupt
 * @note interrupts will be enabled in main after all initialization
 * User defines their own ISR like the following
 * @code
uint32_t Count;
void GROUP1_IRQHandler(void){
  Count++; // number of touches
  GPIOB->DOUTTGL31_0 = RED; // toggle PB26
  GPIOB->CPU_INT.ICLR = 0x00200000;   // clear bit 21
}
 @endcode
 */
void EdgeTriggered_Init(void);


#endif // __EDGETRIGGERED_H__
/** @}*/
