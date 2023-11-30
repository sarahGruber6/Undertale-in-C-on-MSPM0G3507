/*!
 * @defgroup PWM
 * @brief Pulse width modulation
 <table>
<caption id="PWMpins">PWM pins on the MSPM0G3507</caption>
<tr><th>Pin  <th>Description
<tr><td>PA12 <td>CCP0 PWM output  
<tr><td>PA13 <td>CCP1 PWM output  
</table>
 * @{*/
/**
 * @file      PWM.h
 * @brief     Pulse width modulation
 * @details   Hardware creates fixed period, variable duty cycle waves
 * \image html PWM_100Hz.png  width=500px
 * @version   ECE319K v1.0
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2023 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      August 13, 2023
 <table>
<caption id="PWMpins2">PWM pins on the MSPM0G3507</caption>
<tr><th>Pin  <th>Description
<tr><td>PA12 <td>CCP0 PWM output  
<tr><td>PA13 <td>CCP1 PWM output  
</table>
  ******************************************************************************/

/* 
 * Derived from timx_timer_mode_pwm_edge_sleep_LP_MSPM0G3507_nortos_ticlang
 
 */

#ifndef __PWM_H__
#define __PWM_H__

/**
 * \brief use PWMUSELFCLK to select LFCLK
 */
#define PWMUSELFCLK 2
/**
 * \brief use PWMUSEMFCLK to select MFCLK
 */
#define PWMUSEMFCLK 4
/**
 * \brief use PWMUSEBUSCLK to select bus CLK
 */
#define PWMUSEBUSCLK 8
/**
 * Initialize PWM outputs on PA12 PA13.
 * Rising edge synchronized. timerClkDivRatio = 1.
 * Once started, hardware will continuously output the waves.
 * - timerClkSrc = 
 *    - 2 for 32768 Hz LFCLK
 *    - 4 for 4MHz MFCLK (not tested)
 *    - 8 for 80/32/4 BUSCLK
 * - G0/G8  is on Power domain PD0
 *    - 32MHz bus clock, BUSCLK clock is 32MHz
 *    - 40MHz bus clock, BUSCLK clock is ULPCLK 20MHz
 *    - 80MHz bus clock, BUSCLK clock is ULPCLK 40MHz
 * - PWMFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1) * period))
 *    - For example, source=LFCLK, prescale = 0, period = 1000, PWM frequency = 32.768 Hz
 *    - For example, source=BUSCLK, bus=40MHz, prescale=19, period = 10000, PWM frequency = 10kHz 
 *
 * @param timerClkSrc is 2 4 or 8
 * @param timerClkPrescale divide clock by timerClkPrescale+1, 0 to 255
 * @param period sets the PWM period
 * @param duty0 sets the duty cycle on PA12 
 * @param duty1 sets the duty cycle on PA13 
 * @return none 
 * @note  Will call LaunchPad_Init to reset and activate power
 * @see PWM_SetDuty
 * @brief Initialize PWM
 */
void PWM_Init(uint32_t timerClkSrc, uint32_t timerClkPrescale, 
              uint32_t period, uint32_t duty0, uint32_t duty1);
/**
 * Set duty cycles on PA12 PA13.
 * @param duty0 sets the duty cycle on PA12 
 * @param duty1 sets the duty cycle on PA13 
 * @return none 
 * @note  assumes PWM_Init was called
 * @see PWM_Init
 * @brief Set duty cycles
 */			  
void PWM_SetDuty(uint32_t duty0, uint32_t duty1);

#endif // __PWM_H__
/** @}*/

