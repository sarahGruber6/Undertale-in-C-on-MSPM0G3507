/*!
 * @defgroup Clock
 * @brief Provide functions that initialize the MSPM0 clock module
  <table>
<caption id="Clockpins2">Clock pins</caption>
<tr><th>Pin  <th>Description
<tr><td>PA5 <td>HFXIN crystal
<tr><td>PA6 <td>HFXOUT crystal
<tr><td>PA14 <td>Possible clock output
</table> 
 * @{*/
/**
 * @file      Clock.h
 * @brief     Provide functions that initialize the MSPM0 clock module
 * @details   Reconfigure MSPM0 to run at 4, 32, 40 or 80 MHz
 * @version   ECE319K v1.0
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2023 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      August 13, 2023
 <table>
<caption id="Clockpins">Clock pins</caption>
<tr><th>Pin  <th>Description
<tr><td>PA5 <td>HFXIN crystal
<tr><td>PA6 <td>HFXOUT crystal
<tr><td>PA14 <td>Possible clock output
</table> 
 */


#ifndef __CLOCK_H__
#define __CLOCK_H__

/**
 * Configure the MSPM0 clock to run at 80 MHz
 * @param enablePA14 1 means clock output on PA14, 0 means no output on PA14
 * @return none
 * @note  Since the crystal is used, the bus clock will be very accurate
 * @see Clock_Freq()
 * @brief  Initialize clock to 80 MHz
 */
void Clock_Init80MHz(int enablePA14);

/**
 * Configure the MSPM0 clock to run at 40 MHz
 * @param none
 * @return none 
 * @note  Since the crystal is used, the bus clock will be very accurate
 * @see Clock_Freq()
 * @brief  Initialize clock to 40 MHz
 */
void Clock_Init40MHz(void);


/**
 * \brief using OSCFREQ32MHZ Clock_Init will create 32 MHz bus clock
 */
#define OSCFREQ32MHZ 0
/**
 * \brief using OSCFREQ4MHZ Clock_Init will create 4 MHz bus clock
 */
#define OSCFREQ4MHZ 1

/**
 * Configure the MSPM0 clock to run at 4MHz or 32MHz
 * @param freq is either OSCFREQ32MHZ or OSCFREQ4MHZ
 * @return none 
 * @note  Since the crystal is not used, the bus clock will not be  accurate
 * @see Clock_Freq()
 * @brief  Initialize clock to 4 or 32 MHz
 */
void Clock_Init(uint32_t freq);

/**
 * Return the current bus clock frequency
 * @param none
 * @return frequency of the system clock in Hz
 * @note  In this module, the return result will be 3000000, 32000000, 40000000 or 80000000
 * @see Clock_Init(),Clock_Init40MHz(),Clock_Init80MHz() 
 * @brief Returns current clock bus frequency in Hz
 */
uint32_t Clock_Freq(void);

/**
 * Simple delay function which delays about cycles bus cycles.
 * It is implemented with a for-loop and is approximate.
 * @param  cycles is the number of bus cycles to wait
 * @return none 
 * @note For a more accurate time delay, you could use the SysTick module.
 * @brief  Software implementation of a busy-wait delay
 */
void Clock_Delay(uint32_t cycles) __attribute__((noinline));

/**
 * Simple delay function which delays about ms milliseconds.
 * It is implemented with a nested for-loop and is approximate.
 * @param  ms is the number of milliseconds to wait
 * @return none 
 * @note For a more accurate time delay, you could use the SysTick module.
 * @brief  Software implementation of a busy-wait delay
 */
void Clock_Delay1ms(uint32_t ms);

/**
 * Initialize the 24-bit SysTick timer.
 * @param  none
 * @return none
 * @note Call this before using SysTick_Wait or SysTick_Wait10ms.
 * @brief  SysTick for busy-wait delay
 */
void SysTick_Init(void);

/**
 * Delay function which delays about delay bus cycles.
 * It is implemented with SysTick and is accurate.
 * @param  delay is the number of bus cycles to wait
 * @return none 
 * @brief  SysTick implementation of a busy-wait delay
 */
void SysTick_Wait(uint32_t delay);

/**
 * Delay function which delays about t*10ms time,
 * assuming the bus clock is 80 MHz.
 * It is implemented with SysTick and is accurate.
 * @param  t is the time to wait (units 10ms)
 * @return none
 * @brief  SysTick implementation of a busy-wait delay
 */
void SysTick_Wait10ms(uint32_t t);

/**
 * Initialize the 16-bit G8 timer for continuous counting.
 * The precision is 16 bits.
 * Simply read TIMG8->COUNTERREGS.CTR for 16 bit time<br>
<table>
<caption id="TimerG8">TimerG8 examples for 80 MHz clock</caption>
<tr><th>divider<th>prescale<th>resolution<th>range
<tr><td>1      <td>  1     <td>12.5ns    <td>819 us
<tr><td>1      <td> 80     <td>   1us    <td> 65 ms
<tr><td>1      <td>160     <td>   2us    <td>131 ms
<tr><td>5      <td>160     <td>  10us    <td>655 ms
<tr><td>8      <td>256     <td> 25.6us   <td>1.7 s
</table>
 * @param  divider must be 1 to 8
 * @param  prescale must be 1 to 256
 * @return none 
 * @note resolution is busperiod*divider*prescale
 * @brief  SysTick for busy-wait delay
 */
void TimerG8_Init(uint32_t divider, uint32_t prescale);

#endif //  __CLOCK_H__
/** @}*/