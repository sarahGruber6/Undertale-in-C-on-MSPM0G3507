/**
 * @file      TExaS.h
 * @brief     Creates either an oscilloscope or a logic analyzer
 * @details   TExaS is an oscilloscope or a logic analyzer, sampling at 10 kHz<br>
 * - to create an 8-bit analog oscilloscope
 *    -# set adc12 to either ADC0 or ADC1
 *    -# set channel to 0 to 7
 *    -# scope ISR is 11 instructions, 1.5us, 1.5% overhead
 * - to create a 7-bit digital logic analyzer
 *    -# set adc12 to 0, channel doesn't not matter
 *    -# create a function to read any 7-bit digital measurement, add bit7=1
 *    -# pass that function into TExaS_Init
 *
 * @note Works at 32MHz or 40MHz or 80 MHz

 * @version   ECE319K v1.0
 * @author    Put your name here
 * @copyright Put your copyright here,
 * @warning   AS-IS
 * @date      August 13, 2023
   <table>
<caption id="ADCpins3">ADC pins on the MSPM0G3507</caption>
<tr><th>Pin  <th>ADC channel<th>Sensor
<tr><td>PA27 <td>ADC0 channel 0 <td>J1.8 also MKII light interrupt
<tr><td>PA26 <td>ADC0 channel 1 <td>J1.6 MKII microphone in
<tr><td>PA25 <td>ADC0 channel 2 <td>J1.2 MKII Joystick X
<tr><td>PA24 <td>ADC0 channel 3 <td>J3.27 ***free***
<tr><td>PB25 <td>ADC0 channel 4 <td>J19.7 (insert 0ohm R74, no U3 OPA2365)
<tr><td>PB24 <td>ADC0 channel 5 <td>J1.5 also MKII joystick select button
<tr><td>PB20 <td>ADC0 channel 6 <td>J4.36 ***free***
<tr><td>PA22 <td>ADC0 channel 7 <td>J24 MKII Accelerometer Y

<tr><td>PA15 <td>ADC1 channel 0 <td>J3.30 (also DACout)
<tr><td>PA16 <td>ADC1 channel 1 <td>J3.29 ***free***
<tr><td>PA17 <td>ADC1 channel 2 <td>J3.28 ***free***
<tr><td>PA18 <td>ADC1 channel 3 <td>J3.26 MKII Joystick Y
<tr><td>PB17 <td>ADC1 channel 4 <td>J2.18 ***free***
<tr><td>PB18 <td>ADC1 channel 5 <td>J3.25 MKII Accelerometer Z
<tr><td>PB19 <td>ADC1 channel 6 <td>J3.23 MKII Accelerometer X
<tr><td>PA21 <td>ADC1 channel 7 <td>J17.8 (insert R20, remove R3)

</table>
  ******************************************************************************/
  /*!
 * @defgroup Debugging
 * @brief Debugging tools
  * @{*/

#ifndef __TEXAS_H__
#define __TEXAS_H__
#include <ti/devices/msp/msp.h>

/**
 * \brief using PAoutPin0Bit0 places PA0 output on bit 0
 */
#define PAoutPin0Bit0 (GPIOA->DOUT31_0&1)
/**
 * \brief using PBoutPin22Bit1 places PB22 output on bit 1
 */
#define PBoutPin22Bit1 (((GPIOB->DOUT31_0&(1<<22))>>21))
/**
 * \brief using PBoutPin26Bit2 places PB26 output on bit 2
 */
#define PBoutPin26Bit2 (((GPIOB->DOUT31_0&(1<<26))>>24))
/**
 * \brief using PBoutPin27Bit3 places PB27 output on bit 3
 */
#define PBoutPin27Bit3 (((GPIOB->DOUT31_0&(1<<27))>>24))
/**
 * \brief using PAinPin18Bit4 places PA18 input on bit 4
 */
#define PAinPin18Bit4 (((GPIOA->DIN31_0&(1<<18))>>14))
/**
 * \brief using PBinPin21Bit5 places PA21 input on bit 5
 */
#define PBinPin21Bit5 (((GPIOB->DIN31_0&(1<<21))>>16))

/**
 * To configure logic analyzer for PB21,PA18,PB27,PB26,PB22,PA0
 * call TExaS_Init(0,0,&TExaS_LaunchPadLogic); 
 * @param none
 * @return 8-bit PB21,PA18,PB27,PB26,PB22,PA0 
 * @note  40 instructions, 2us, 2%overhead
 * @see TExaS_Init
 * @brief  PB21,PA18,PB27,PB26,PB22,PA0
 */
uint8_t TExaS_LaunchPadLogic(void);

/**
 * To configure  logic analyzer for PB22,PA0 
 * call TExaS_Init(0,0,&TExaS_PB22PA0Logic); 
 * @param none
 * @return 8-bit PB22,PA0 
 * @note  16 instructions, 1.5us, 1.5% overhead
 * @see TExaS_Init
 * @brief  PB22,PA0
 */
uint8_t TExaS_PB22PA0Logic(void);

/**
 * To configure  logic analyzer for PA6-PA0 outputs 
 * call TExaS_Init(0,0,&TExaS_PA60Logic); 
 * @param none
 * @return 8-bit PA6-PA0 outputs 
 * @note 11 instructions, 1.5us, 1.5% overhead
 * @see TExaS_Init
 * @brief  PA6-PA0
 */
uint8_t TExaS_PA60Logic(void);
 
/**
 * To configure  logic analyzer for PB18-16 outputs, PB2-0 input 
 * call TExaS_Init(0,0,&TExaS_PB18PB17PB16PB2PB1PB0Logic); 
 * @param none
 * @return 8-bit PB18-16 outputs, PB2-0 input
 * @see TExaS_Init
 * @brief  PB18-16 outputs, PB2-0 input
 */
uint8_t TExaS_PB18PB17PB16PB2PB1PB0Logic(void);

/**
 * Initialize TExaS scope or logic analyzer<br>
 * Sends one 8-bit data to PC at 10kHz
 * @param adc12 is ADC0 or ADC1 (set to 0 for logic analyzer)
 * @param channel specifies ADC channel 0 to 7
 * @param logic function that returns an 8-bit value with bit 7 set
 * @return none 
 * @brief  Initialize 12-bit ADC0
 */
 void TExaS_Init(ADC12_Regs *adc12, uint32_t channel, uint8_t (*logic)(void));

#endif //  __TEXAS_H__
/** @}*/