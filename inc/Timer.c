/* Timer.c
 * Jonathan Valvano
 * August 12, 2023
 * Derived from timx_timer_mode_periodic_sleep_LP_MSPM0G3507_nortos_ticlang
 *              gpio_toggle_output_LP_MSPM0G3507_nortos_ticlang
 *
 */


#include <ti/devices/msp/msp.h>
#include "../inc/Clock.h"


// power Domain PD0
// initialize G0/G8 for periodic interrupt
// for 32MHz bus clock, Timer clock is 32MHz
// for 40MHz bus clock, Timer clock is ULPCLK 20MHz
// for 80MHz bus clock, Timer clock is ULPCLK 40MHz
// frequency = TimerClock/prescale/period
void TimerG0_IntArm(uint16_t period, uint32_t prescale, uint32_t priority){
  TIMG0->GPRCM.RSTCTL = 0xB1000003;
  TIMG0->GPRCM.PWREN = 0x26000001;
  Clock_Delay(24); // time for TimerG0 to power up
  TIMG0->CLKSEL = 0x08; // bus clock
  TIMG0->CLKDIV = 0x00; // divide by 1
  TIMG0->COMMONREGS.CPS = prescale-1;     // divide by prescale,
  TIMG0->COUNTERREGS.LOAD  = period-1;     // set reload register
  TIMG0->COUNTERREGS.CTRCTL = 0x02;
  // bits 5-4 CM =0, down
  // bits 3-1 REPEAT =001, continue
  // bit 0 EN enable (0 for disable, 1 for enable)
  //TIMG0->INT_EVENT[0].IMASK |= 1; // zero event mask
  TIMG0->CPU_INT.IMASK = 1; // zero event mask
 // TIMG0->GEN_EVENT1.IMASK |= 1; // zero event mask
  TIMG0->COMMONREGS.CCLKCTL = 1;
  NVIC->ISER[0] = 1 << 16; // TIMG0 interrupt
  NVIC->IP[4] = (NVIC->IP[4]&(~0x000000FF))|(priority<<6);    // set priority (bits 7,6) IRQ 16
  TIMG0->COUNTERREGS.CTRCTL |= 0x01;
}




// frequency = TimerClock/prescale/period
void TimerG8_IntArm(uint16_t period, uint32_t prescale, uint32_t priority){
  TIMG8->GPRCM.RSTCTL = 0xB1000003;
  TIMG8->GPRCM.PWREN = 0x26000001;
  Clock_Delay(24); // time for TimerG8 to power up
  TIMG8->CLKSEL = 0x08; // bus clock
  TIMG8->CLKDIV = 0x00; // divide by 1
  TIMG8->COMMONREGS.CPS = prescale-1;     // divide by prescale,
  TIMG8->COUNTERREGS.LOAD  = period-1;     // set reload register
  TIMG8->COUNTERREGS.CTRCTL = 0x02;
  // bits 5-4 CM =0, down
  // bits 3-1 REPEAT =001, continue
  // bit 0 EN enable (0 for disable, 1 for enable)
  TIMG8->CPU_INT.IMASK |= 1; // zero event mask
  TIMG8->COMMONREGS.CCLKCTL = 1;
  NVIC->ISER[0] = 1 << 2; // TIMG8 interrupt
  NVIC->IP[0] = (NVIC->IP[0]&(~0x00FF0000))|(priority<<22);    // set priority (bits 23,22) IRQ 2
  TIMG8->COUNTERREGS.CTRCTL |= 0x01;
}





// Power Domain PD1
// initialize A0/A1/G6/G7/G12 for periodic interrupt
// for 32MHz bus clock, Timer clock is 32MHz
// for 40MHz bus clock, Timer clock is MCLK 40MHz
// for 80MHz bus clock, Timer clock is MCLK 80MHz
// frequency = TimerClock/prescale/period
void TimerA0_IntArm(uint16_t period, uint32_t prescale, uint32_t priority){
  TIMA0->GPRCM.RSTCTL = (uint32_t)0xB1000003;
  TIMA0->GPRCM.PWREN = (uint32_t)0x26000001;
  Clock_Delay(24); // time for TimerA0 to power up
  TIMA0->CLKSEL = 0x08; // bus clock
  TIMA0->CLKDIV = 0x00; // divide by 1
  TIMA0->COMMONREGS.CPS = prescale-1;     // divide by prescale,
  TIMA0->COUNTERREGS.LOAD  = period-1;     // set reload register
  TIMA0->COUNTERREGS.CTRCTL = 0x02;
  // bits 5-4 CM =0, down
  // bits 3-1 REPEAT =001, continue
  // bit 0 EN enable (0 for disable, 1 for enable)
  TIMA0->CPU_INT.IMASK |= 1; // zero event mask
  TIMA0->COMMONREGS.CCLKCTL = 1;
  NVIC->ISER[0] = 1 << 18; // TIMA0 interrupt
  NVIC->IP[4] = (NVIC->IP[4]&(~0x00FF0000))|(priority<<22);    // set priority (bits 23,22) IRQ 18
  TIMA0->COUNTERREGS.CTRCTL |= 0x01;
}



// initialize A1 for periodic interrupt
// frequency = TimerClock/prescale/period
void TimerA1_IntArm(uint16_t period, uint32_t prescale, uint32_t priority){
  TIMA1->GPRCM.RSTCTL = (uint32_t)0xB1000003;
  TIMA1->GPRCM.PWREN = (uint32_t)0x26000001;
  Clock_Delay(2); // time for TimerA1 to power up
  TIMA1->CLKSEL = 0x08; // bus clock
  TIMA1->CLKDIV = 0x00; // divide by 1
  TIMA1->COMMONREGS.CPS = prescale-1;     // divide by prescale
  TIMA1->COUNTERREGS.LOAD  = period-1;    // set reload register
  TIMA1->COUNTERREGS.CTRCTL = 0x02;
  // bits 5-4 CM =0, down
  // bits 3-1 REPEAT =001, continue
  // bit 0 EN enable (0 for disable, 1 for enable)
  TIMA1->CPU_INT.IMASK |= 1; // zero event mask
  TIMA1->COMMONREGS.CCLKCTL = 1;
  NVIC->ISER[0] = 1 << 19; // TIMA1 interrupt
  NVIC->IP[4] = (NVIC->IP[4]&(~0xFF000000))|(priority<<30);    // set priority (bits 31,30) IRQ 19
  TIMA1->COUNTERREGS.CTRCTL |= 0x01;
}


// initialize G7 for periodic interrupt
// frequency = TimerClock/prescale/period
void TimerG7_IntArm(uint16_t period, uint32_t prescale, uint32_t priority){
  TIMG7->GPRCM.RSTCTL = (uint32_t)0xB1000003;
  TIMG7->GPRCM.PWREN = (uint32_t)0x26000001;
  Clock_Delay(24); // time for TimerG7 to power up
  TIMG7->CLKSEL = 0x08; // bus clock
  TIMG7->CLKDIV = 0x00; // divide by 1
  TIMG7->COMMONREGS.CPS = prescale-1;     // divide by prescale
  TIMG7->COUNTERREGS.LOAD  = period-1;    // set reload register
  TIMG7->COUNTERREGS.CTRCTL = 0x02;
    // bits 5-4 CM =0, down
    // bits 3-1 REPEAT =001, continue
    // bit 0 EN enable (0 for disable, 1 for enable)
  TIMG7->CPU_INT.IMASK |= 1; // zero event mask
  TIMG7->COMMONREGS.CCLKCTL = 1;
  NVIC->ISER[0] = 1 << 20; // TIMG7 interrupt
  NVIC->IP[5] = (NVIC->IP[5]&(~0x000000FF))|(priority<<6);    // set priority (bits 7,6) IRQ 20
  TIMG7->COUNTERREGS.CTRCTL |= 0x01;
}



// initialize G6 for periodic interrupt
// frequency = TimerClock/prescale/period
void TimerG6_IntArm(uint16_t period, uint32_t prescale, uint32_t priority){
  TIMG6->GPRCM.RSTCTL = (uint32_t)0xB1000003;
  TIMG6->GPRCM.PWREN = (uint32_t)0x26000001;
  Clock_Delay(24); // time for TimerG10 to power up
  TIMG6->CLKSEL = 0x08; // bus clock
  TIMG6->CLKDIV = 0x00; // divide by 1
  TIMG6->COMMONREGS.CPS = prescale-1;     // divide by prescale
  TIMG6->COUNTERREGS.LOAD  = period-1;     // set reload register
  TIMG6->COUNTERREGS.CTRCTL = 0x02;
    // bits 5-4 CM =0, down
    // bits 3-1 REPEAT =001, continue
    // bit 0 EN enable (0 for disable, 1 for enable)
  TIMG6->CPU_INT.IMASK |= 1; // zero event mask
  TIMG6->COMMONREGS.CCLKCTL = 1;
  NVIC->ISER[0] = 1 << 17; // TIMG6 interrupt
  NVIC->IP[4] = (NVIC->IP[4]&(~0x0000FF00))|(priority<<14);    // set priority (bits 15,14) IRQ 17
  TIMG6->COUNTERREGS.CTRCTL |= 0x01;
}



// initialize G12 for periodic interrupt, 32 bit
// no prescale
// frequency = TimerClock/period
void TimerG12_IntArm(uint32_t period, uint32_t priority){
  TIMG12->GPRCM.RSTCTL = (uint32_t)0xB1000003;
  TIMG12->GPRCM.PWREN = (uint32_t)0x26000001;
  Clock_Delay(24); // time for TimerG12 to power up
  TIMG12->CLKSEL = 0x08; // bus clock
  TIMG12->CLKDIV = 0x00; // divide by 1
  TIMG12->COUNTERREGS.LOAD  = period-1;     // set reload register
  TIMG12->COUNTERREGS.CTRCTL = 0x02;
    // bits 5-4 CM =0, down
    // bits 3-1 REPEAT =001, continue
    // bit 0 EN enable (0 for disable, 1 for enable)
  TIMG12->CPU_INT.IMASK |= 1; // zero event mask
  TIMG12->COMMONREGS.CCLKCTL = 1;
  NVIC->ISER[0] = 1 << 21; // TIMG12 interrupt
  NVIC->IP[5] = (NVIC->IP[5]&(~0x0000FF00))|(priority<<14);    // set priority (bits 15,14) IRQ 21
  TIMG12->COUNTERREGS.CTRCTL |= 0x01;
}

// initialize G12 for continuous counting
// simply read TIMG12->COUNTERREGS.CTR for 32 bit time
// resolution is bus clock
// precision is 32 bits
// for 80 MHz clock, 12.5ns
// power Domain PD1 for G12
// for 32MHz bus clock, Timer clock is 32MHz
// for 40MHz bus clock, Timer clock is ULPCLK 40MHz
// for 80MHz bus clock, Timer clock is ULPCLK 80MHz
void TimerG12_Init(void){
  TIMG12->GPRCM.RSTCTL = (uint32_t)0xB1000003;
  TIMG12->GPRCM.PWREN = (uint32_t)0x26000001;
  Clock_Delay(24);     // time for TimerG8 to power up
  TIMG12->CLKSEL = 0x08; // bus clock
  TIMG12->CLKDIV = 0; // divide by 1
//  TIMG8->COMMONREGS.CPS = prescale-1;    // divide by prescale,
  TIMG12->COUNTERREGS.LOAD  = 0xFFFFFFFF;     // set reload register
  TIMG12->COUNTERREGS.CTRCTL = 0x02;
    // bits 5-4 CM =0, down
    // bits 3-1 REPEAT =001, continue
    // bit 0 EN enable (0 for disable, 1 for enable)
  TIMG12->COMMONREGS.CCLKCTL = 1;
  TIMG12->COUNTERREGS.CTRCTL |= 0x01;
}
