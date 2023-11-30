/*!
 * @defgroup ADC
 * @brief Analog to digital conversion
  <table>
<caption id="ADCpins">ADC pins on the MSPM0G3507</caption>
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
 * @{*/
/**
 * @file      ADC.h
 * @brief     Initialize 12-bit ADC0 and/or ADC1
 * @details   ADC input, software trigger, 12-bit conversion<br>
 * The ADC allows two possible references: 2.5V or 3.3V.<br>
 * The internal 2.5V reference is lower noise, but limits the range to 0 to 2.5V.<br>
 * The other possibility is to use the analog voltage supply at 3.3V,
 * making the ADC range 0 to 3.3V. In this driver, the 3.3V range is selected.
 * There are several configurations (each with initialization and a
 * read ADC with software trigger, busy-wait function)<br>

 * @version   ECE319K v1.0
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2023 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      August 13, 2023
 <table>
<caption id="ADCpins2">ADC pins on the MSPM0G3507</caption>
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



#ifndef __ADC_H__
#define __ADC_H__
#include <ti/devices/msp/msp.h>

/**
 * \brief using ADCVREF_INT means choose internal 2.5V reference for accuracy
 */
#define ADCVREF_INT  0x200
/**
 * \brief using ADCVREF_EXT means choose external reference not tested
 */ 
#define ADCVREF_EXT  0x100
/**
 * \brief using ADCVREF_VDDA means choose power line 3.3V reference for 0 to 3.3V range
 */
#define ADCVREF_VDDA 0x000

/**
 * Initialize 12-bit ADC0 in software-triggered mode to take
 * measurements when the associated function is called.
 * One channel is to be measured
 * @param channel is the 0 to 7
 * @param reference is ADCVREF_INT, ADCVREF_EXT, ADCVREF_VDDA
 * @return none 
 * @brief  Initialize 12-bit ADC0
 */
void ADC0_Init(uint32_t channel, uint32_t reference);

/**
 * Trigger a single ADC0 measurement,
 * wait for it to complete, and return the 12-bit result
 * as 0 to 4095.
 * The ADC input voltage range is 0 to 3.3V.
 * Busy-wait synchronization used.
 * @param none
 * @return 12-bit result
 * @note  Assumes ADC0_Init has been called.
 * @brief  Trigger ADC measurement and wait for result.
 */
uint32_t ADC0_In(void);

/**
 * Initialize 12-bit ADC1 in software-triggered mode to take
 * measurements when the associated function is called.
 * One channel is to be measured
 * @param channel is the 0 to 7
 * @param reference is ADCVREF_INT, ADCVREF_EXT, ADCVREF_VDDA
 * @return none 
 * @brief  Initialize 12-bit ADC0
 */
void ADC1_Init(uint32_t channel, uint32_t reference);

/**
 * Trigger a single ADC1 measurement,
 * wait for it to complete, and return the 12-bit result
 * as 0 to 4095.
 * The ADC input voltage range is 0 to 3.3V.
 * Busy-wait synchronization used.
 * @param none
 * @return 12-bit result
 * @note  Assumes ADC1_Init has been called.
 * @brief  Trigger ADC measurement and wait for result.
 */
uint32_t ADC1_In(void);

/**
 * Initialize 12-bit ADC0 or ADC1 in software-triggered mode to take
 * measurements when the associated function is called.
 * One channel is to be measured
 * @param adc12 is ADC0 or ADC1
 * @param channel is the 0 to 7
 * @param reference is ADCVREF_INT, ADCVREF_EXT, ADCVREF_VDDA
 * @return none
 * @brief  Initialize 12-bit ADC0
 */
void ADC_Init(ADC12_Regs *adc12, uint32_t channel, uint32_t reference);

/**
 * Trigger a single ADC0 or ADC1 measurement,
 * wait for it to complete, and return the 12-bit result
 * as 0 to 4095.
 * The ADC input voltage range is 0 to 3.3V.
 * Busy-wait synchronization used.
 * @param adc12 is ADC0 or ADC1
 * @return 12-bit result
 * @note  Assumes ADC_Init has been called.
 * @brief  Trigger ADC measurement and wait for result.
 */
uint32_t ADC_In(ADC12_Regs *adc12);

/**
 * Initialize 12-bit ADC0 or ADC1 in software-triggered mode to take
 * measurements when the associated function is called.
 * Two channels are to be measured
 * @param adc12 is ADC0 or ADC1
 * @param channel1 is the 0 to 7
 * @param channel2 is the 0 to 7
 * @param reference is ADCVREF_INT, ADCVREF_EXT, ADCVREF_VDDA
 * @return none 
 * @brief  Initialize 12-bit ADC0
 */
void ADC_InitDual(ADC12_Regs *adc12, uint32_t channel1, uint32_t channel2, uint32_t reference);

/**
 * Trigger a dual ADC0 or ADC1 measurement,
 * wait for it to complete, and return the two 12-bit results
 * as 0 to 4095.
 * The ADC input voltage range is 0 to 3.3V.
 * Busy-wait synchronization used.
 * @param adc12 is ADC0 or ADC1
 * @param *d1 address into which the first ADC sample will be stored
 * @param *d2 address into which the second ADC sample will be stored
 * @return none 
 * @note  Assumes ADC_InitDual has been called.
 * @brief  Trigger two ADC measurements and wait for results.
 */
void ADC_InDual(ADC12_Regs *adc12, uint32_t *d1, uint32_t *d2);

/**
 * Initialize 12-bit ADC0 in software-triggered mode to take
 * measurements when the associated function is called.
 * One channel is to be measured 2^n times in average.
 * @param channel is the 0 to 7
 * @param n 0 to 7
 * @return none 
 * @brief  Initialize 12-bit ADC0 for hardware averaging
 * @note uses ADCVREF_VDDA, so range is 0 to 3.3V
 */
void ADC0_InitAve(uint32_t channel, uint32_t n);

#endif //  __ADC_H__
/** @}*/
