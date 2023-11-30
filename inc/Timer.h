/*!
 * @defgroup Timer
 * @brief Hardware timers
 * @{*/
/**
 * @file      Timer.h
 * @brief     Initialize Periodic interrupt timers
 * @details   Functions to initialize timers for interrupts
 * or just simple time measurements.<br>

 * @version   ECE319K v1.0
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2023 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      August 13, 2023

  ******************************************************************************/

/* Timer.h
 * Derived from timx_timer_mode_periodic_sleep_LP_MSPM0G3507_nortos_ticlang
 *              gpio_toggle_output_LP_MSPM0G3507_nortos_ticlang
 */


#ifndef __TIMER_H__
#define __TIMER_H__


/**
 * Arm 16-bit periodic interrupts on Timer G0.
 * TimerG0 is on power Domain PD0 
 * - 32MHz bus clock, Timer clock is 32MHz
 * - 40MHz bus clock, Timer clock is ULPCLK 20MHz
 * - 80MHz bus clock, Timer clock is ULPCLK 40MHz
 *
 * frequency = TimerClock/prescale/period 
 * @param period sets the interrupt 16-bit period
 * @param prescale sets the timer prescale
 * @param priority is 0(highest),1,2 or 3(lowest)
 * @return none 
 * @brief  Initialize Timer G0 interrupt
 * @note interrupts will be enabled in main after all initialization
 * User defines their own ISR like the following
 * @code
uint32_t count;
void TIMG0_IRQHandler(void){
  if((TIMG0->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
    count++;
  }
}
 @endcode
 */
void TimerG0_IntArm(uint16_t period, uint32_t prescale, uint32_t priority);



/**
 * Arm 16-bit periodic interrupts on Timer G8.
 * TimerG8 is on power Domain PD0 
 * - 32MHz bus clock, Timer clock is 32MHz
 * - 40MHz bus clock, Timer clock is ULPCLK 20MHz
 * - 80MHz bus clock, Timer clock is ULPCLK 40MHz
 *
 * frequency = TimerClock/prescale/period 
 * @param period sets the interrupt 16-bit period
 * @param prescale sets the timer prescale
 * @param priority is 0(highest),1,2 or 3(lowest)
 * @return none 
 * @brief  Initialize Timer G8 interrupt
 * @note interrupts will be enabled in main after all initialization
 * User defines their own ISR like the following
 * @code
uint32_t count;
void TIMG8_IRQHandler(void){
  if((TIMG8->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
    count++;
  }
}
 @endcode
 */
void TimerG8_IntArm(uint16_t period, uint32_t prescale, uint32_t priority);




/**
 * Arm 16-bit periodic interrupts on Timer A0.
 * TimerA0 is on power Domain PD1 
 * - 32MHz bus clock, Timer clock is 32MHz
 * - 40MHz bus clock, Timer clock is ULPCLK 40MHz
 * - 80MHz bus clock, Timer clock is ULPCLK 80MHz
 *
 * frequency = TimerClock/prescale/period 
 * @param period sets the interrupt 16-bit period
 * @param prescale sets the timer prescale
 * @param priority is 0(highest),1,2 or 3(lowest)
 * @return none 
 * @brief  Initialize Timer A0 interrupt
 * @note interrupts will be enabled in main after all initialization
 * User defines their own ISR like the following
 * @code
uint32_t count;
void TIMA0_IRQHandler(void){
  if((TIMA0->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
    count++;
  }
}
 @endcode
 */
void TimerA0_IntArm(uint16_t period, uint32_t prescale, uint32_t priority);



/**
 * Arm 16-bit periodic interrupts on Timer A1.
 * TimerA1 is on power Domain PD1 
 * - 32MHz bus clock, Timer clock is 32MHz
 * - 40MHz bus clock, Timer clock is ULPCLK 40MHz
 * - 80MHz bus clock, Timer clock is ULPCLK 80MHz
 *
 * frequency = TimerClock/prescale/period 
 * @param period sets the interrupt 16-bit period
 * @param prescale sets the timer prescale
 * @param priority is 0(highest),1,2 or 3(lowest)
 * @return none 
 * @brief  Initialize Timer A1 interrupt
 * @note interrupts will be enabled in main after all initialization
 * User defines their own ISR like the following
 * @code
uint32_t count;
void TIMA1_IRQHandler(void){
  if((TIMA1->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
    count++;
  }
}
 @endcode
 */
void TimerA1_IntArm(uint16_t period, uint32_t prescale, uint32_t priority);



/**
 * Arm 16-bit periodic interrupts on Timer G7.
 * TimerG7 is on power Domain PD1 
 * - 32MHz bus clock, Timer clock is 32MHz
 * - 40MHz bus clock, Timer clock is ULPCLK 40MHz
 * - 80MHz bus clock, Timer clock is ULPCLK 80MHz
 *
 * frequency = TimerClock/prescale/period 
 * @param period sets the interrupt 16-bit period
 * @param prescale sets the timer prescale
 * @param priority is 0(highest),1,2 or 3(lowest)
 * @return none 
 * @brief  Initialize Timer G7 interrupt
 * @note interrupts will be enabled in main after all initialization
 * User defines their own ISR like the following
 * @code
uint32_t count;
void TIMG7_IRQHandler(void){
  if((TIMG7->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
    count++;
  }
}
 @endcode
 */
void TimerG7_IntArm(uint16_t period, uint32_t prescale, uint32_t priority);



/**
 * Arm 16-bit periodic interrupts on Timer G6.
 * TimerG6 is on power Domain PD1 
 * - 32MHz bus clock, Timer clock is 32MHz
 * - 40MHz bus clock, Timer clock is ULPCLK 40MHz
 * - 80MHz bus clock, Timer clock is ULPCLK 80MHz
 *
 * frequency = TimerClock/prescale/period 
 * @param period sets the interrupt 16-bit period
 * @param prescale sets the timer prescale
 * @param priority is 0(highest),1,2 or 3(lowest)
 * @return none 
 * @brief  Initialize Timer G6 interrupt
 * @note interrupts will be enabled in main after all initialization
 * User defines their own ISR like the following
 * @code
uint32_t count;
void TIMG6_IRQHandler(void){
  if((TIMG6->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
    count++;
  }
}
 @endcode
 */
void TimerG6_IntArm(uint16_t period, uint32_t prescale, uint32_t priority);



/**
 * Arm 32-bit periodic interrupts on Timer G12.
 * No prescale
 * TimerG12 is on power Domain PD1 
 * - 32MHz bus clock, Timer clock is 32MHz
 * - 40MHz bus clock, Timer clock is ULPCLK 40MHz
 * - 80MHz bus clock, Timer clock is ULPCLK 80MHz
 *
 * frequency = TimerClock/period 
 * @param period 32-bit sets the interrupt period
 * @param priority is 0(highest),1,2 or 3(lowest)
 * @return none 
 * @brief  Initialize Timer G12 interrupt
 * @note interrupts will be enabled in main after all initialization
 * User defines their own ISR like the following
 * @code
uint32_t count;
void TIMG12_IRQHandler(void){
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27
    count++;
  }
}
 @endcode
 */
void TimerG12_IntArm(uint32_t period, uint32_t priority);


/**
 * initialize G12 for continuous counting.
 * No prescale<br>
 * simply read TIMG12->COUNTERREGS.CTR for 32 bit time
 * - resolution is bus clock, for 80 MHz clock, 12.5ns
 * - precision is 32 bits,
 * 
 * TimerG12 is on power Domain PD1 
 * - 32MHz bus clock, Timer clock is 32MHz
 * - 40MHz bus clock, Timer clock is ULPCLK 40MHz
 * - 80MHz bus clock, Timer clock is ULPCLK 80MHz
 *
 * @param none
 * @return none 
 * @brief  Initialize Timer G12
 * @note Example use to measure execution time of myfunction
 * @code
uint32_t elapsed,start,end;
  TimerG12_Init(); // start timer G12 
  start = TIMG12->COUNTERREGS.CTR;
  myfunction();
  end = TIMG12->COUNTERREGS.CTR;
  elapsed = start-end; // 12.5ns resolution
 @endcode
 */
void TimerG12_Init(void);

#endif // __TIMER_H__
/** @}*/

