/* Clock.c
 * Jonathan Valvano
 * July 9, 2023
 * Derived from sysctl_hfxt_run_LP_MSPM0G3507_nortos_ticlang
 *              systick_periodic_timer_LP_MSPM0G3507_nortos_ticlang
 *              sysctl_mclk_syspll_LP_MSPM0G3507_nortos_ticlang
 */


#include <ti/devices/msp/msp.h>
#include "../inc/Clock.h"
#define PA5INDEX 9
#define PA6INDEX 10
#define PA14INDEX 35
// 40 MHz crystal connected to
//   PA5 is HFXIN
//   PA6 is HFXOUT
// PA14 is CLK_OUT
uint32_t volatile BusFreq = 32000000; // default clock
// returns current bus clock frequency
uint32_t Clock_Freq(void){
  return BusFreq;
}
void Clock_Init80MHz(int enablePA14){
// turn on PortA portB in LaunchPad_Init()
  if(enablePA14){
    IOMUX->SECCFG.PINCM[PA14INDEX] = 0x00100086U; // CLK_OUT, hi-drive
    GPIOA->DOESET31_0 = 0x4000;
  }
/* Configure GPIO/IOMUX for HFXT functionality */
  IOMUX->SECCFG.PINCM[PA5INDEX] = 0; // disconnect PA5 from I/O
  IOMUX->SECCFG.PINCM[PA6INDEX] = 0; // disconnect PA6 from I/O
  SYSCTL->SOCLOCK.SYSOSCCFG = (SYSCTL->SOCLOCK.SYSOSCCFG & (~0x03))|0x00; // 32 MHz

    /* Check that SYSPLL is disabled before configuration */
  while ((SYSCTL->SOCLOCK.CLKSTATUS & 0x4000) != 0x4000){
     ; // you are here if PLL is already on
  }

// bit 0 SYSPLL reference is SYSOSC
  SYSCTL->SOCLOCK.SYSPLLCFG0 &= ~0x01;
// SYSPLLREF is divided by 2
  SYSCTL->SOCLOCK.SYSPLLCFG1 = (SYSCTL->SOCLOCK.SYSPLLCFG1&(~0x03)) |0x01;
// populate SYSPLLPARAM0/1 tuning registers from flash, based on input freq
  SYSCTL->SOCLOCK.SYSPLLPARAM0 = *(volatile uint32_t *) (0x41C4002C);
  SYSCTL->SOCLOCK.SYSPLLPARAM1 = *(volatile uint32_t *) (0x41C4002C +  0x4);
// set feedback divider QDIV (multiplies to give output frequency)
  SYSCTL->SOCLOCK.SYSPLLCFG1 = (SYSCTL->SOCLOCK.SYSPLLCFG1& (~0x7F00)) |0x0900;

// write clock output dividers, enable outputs, and MCLK source to SYSPLLCFG0
  SYSCTL->SOCLOCK.SYSPLLCFG0 =   0x00031062;
    // bits 19-16 RDIVCLK2X   0011 SYSPLLCLK1 is divided by 4
    // bits 15-12 RDIVCLK1    0001 SYSPLLCLK1 is divided by 4
    // bits  11-8 RDIVCLK0    0000 SYSPLLCLK0 is divided by 2
    // bit      6 ENABLECLK2X    1 SYSPLLCLK2X is enabled
    // bit      5 ENABLECLK1     1 SYSPLLCLK1 is enabled
    // bit      4 ENABLECLK0     0 SYSPLLCLK0 is disabled
    // bit      1 MCLK2XVCO      1 SYSPLLCLK2X output is sent to the HSCLK mux
    // bit      0 SYSPLLREF      0 SYSPLL reference is SYSOSC

    // enable SYSPLL, Use the HFCLK_IN digital clock input as the HFCLK source
  SYSCTL->SOCLOCK.HSCLKEN |= 0x00000100;

    // wait until SYSPLL startup is stabilized, bit 9
  while((SYSCTL->SOCLOCK.CLKSTATUS & 0x00000200) != 0x00000200) {
     ; // stuck here means PLL broken
  }

  SYSCTL->SOCLOCK.HSCLKCFG = 0; //HSCLK is sourced from the SYSPLL
// Verify HSCLK source is valid
  while ((SYSCTL->SOCLOCK.CLKSTATUS & 0x00200000) != 0x00200000) {
        ;// stuck here means HSCLK  broken
  }
// Switch MCLK to HSCLK, MCLK will use the high speed clock (HSCLK)
  SYSCTL->SOCLOCK.MCLKCFG |= 0x00010000;

// Verify HSCLK -> MCLK
  while ((SYSCTL->SOCLOCK.CLKSTATUS & 0x00000010) != 0x00000010) {
        ;
  }

  SYSCTL->SOCLOCK.MCLKCFG = (SYSCTL->SOCLOCK.MCLKCFG & (~0x000F)); // no MCLK divide
  SYSCTL->SOCLOCK.MCLKCFG = (SYSCTL->SOCLOCK.MCLKCFG & (~0x0030))|0x10; //ULPCLK divide by 2
  SYSCTL->SOCLOCK.MCLKCFG = (SYSCTL->SOCLOCK.MCLKCFG & (~0x100000))|0x10000;
  // bit 20 0h = MCLK will not use the low frequency clock (LFCLK)
  // bit 16 1h = MCLK will use the high speed clock (HSCLK)
  SYSCTL->SOCLOCK.MCLKCFG = (SYSCTL->SOCLOCK.MCLKCFG & (~0x0F00)) | 0x0200; // 2 flash wait states
  SYSCTL->SOCLOCK.GENCLKCFG = (SYSCTL->SOCLOCK.GENCLKCFG & (~0x00F7))|0x0005;
  SYSCTL->SOCLOCK.GENCLKEN |= 1;

//Low Power Mode is configured to be SLEEP0
  SYSCTL->SOCLOCK.BORTHRESHOLD = 0; // lowest level brown-out reset

  //SCB->SCR System control register, used to control low-power mode behavior
  // bit 2, 0 is sleep, 1 is deep sleep
  // bit 1, 0 do not sleep when returning to Thread mode, 1 enter sleep/deepsleep on returning from ISR to thread
  SCB->SCR &= ~(0x04); // sleep
  SCB->SCR &= ~(0x02); // no sleep on exit
  while ((SYSCTL->SOCLOCK.CLKSTATUS & 0x00200A00) != 0x00200A00){
    ; // check HSCLK LFOSC SYSPLL
  }
  BusFreq = 80000000;
}
// 40 MHz crystal connected to
//   PA5 is HFXIN
//   PA6 is HFXOUT
void Clock_Init40MHz(void){
    /* Configure GPIO/IOMUX for HFXT functionality */
  IOMUX->SECCFG.PINCM[PA5INDEX] = 0; // disconnect PA5 from I/O
  IOMUX->SECCFG.PINCM[PA6INDEX] = 0; // disconnect PA6 from I/O
  SYSCTL->SOCLOCK.SYSOSCCFG = (SYSCTL->SOCLOCK.SYSOSCCFG & (~0x03))|0x00; // 32 MHz

// HFCLKCLKCFG
// bit 28 HFCLK startup
// bits 13-12 HFXTRSEL
//     0 = 4 to 8 MHz
//     1 = 8.01 and 16 MHz
//     2 =  16.01 and 32 MHz
//     3 = 32.01 and 48 MHz//  DL_SYSCTL_setHFCLKSourceHFXTParams(DL_SYSCTL_HFXT_RANGE_32_48_MHZ, 10, true);
  SYSCTL->SOCLOCK.HFCLKCLKCFG = (SYSCTL->SOCLOCK.HFCLKCLKCFG & (~(0x03000))) | 0x3000; // 32.01 and 48 MHz
  // HFXTStartupTime(10);
  SYSCTL->SOCLOCK.HFCLKCLKCFG = (SYSCTL->SOCLOCK.HFCLKCLKCFG & (~(0x000FF))) | 10; // 16.32ms*10/255 = 0.64ms
// HSCLKEN
//   bit 0 enable
  SYSCTL->SOCLOCK.HSCLKEN |= 1;

  SYSCTL->SOCLOCK.HFCLKCLKCFG |= 1<28; // enable startup monitor
      /* Wait until HFXT oscillator is stable. If it does not stabilize, check the
      hardware/IOMUX settings */
// CLKSTATUS
//   bit 21 HSCLKGOOD
//   bit 8 HFCLKGOOD
//   bit 4 HSCLKMUX
  while((SYSCTL->SOCLOCK.CLKSTATUS & 0x0100) != 0x0100){};
// HSCLKCFG
//   bit 0, 1h = HSCLK is sourced from the HFCLK, 0h = HSCLK is sourced from the SYSPLL
  SYSCTL->SOCLOCK.HSCLKCFG = 1;

  // Verify HSCLK source is valid
  while((SYSCTL->SOCLOCK.CLKSTATUS & 0x00200000) != 0x00200000){};

  // Switch MCLK to HSCLK
  //SYSCTL->SOCLOCK.MCLKCFG |= SYSCTL_MCLKCFG_USEHSCLK_ENABLE;
  SYSCTL->SOCLOCK.MCLKCFG = (SYSCTL->SOCLOCK.MCLKCFG & (~0x100000))|0x10000;

  // Verify HSCLK -> MCLK
  while ((SYSCTL->SOCLOCK.CLKSTATUS & 0x10) != 0x10){};


  SYSCTL->SOCLOCK.MCLKCFG = (SYSCTL->SOCLOCK.MCLKCFG & (~0x000F))|0x00;   // MCLK divide by 1
  SYSCTL->SOCLOCK.MCLKCFG = (SYSCTL->SOCLOCK.MCLKCFG & (~0x0030))|0x10;   // ULPCLK divide by 2
  SYSCTL->SOCLOCK.MCLKCFG = (SYSCTL->SOCLOCK.MCLKCFG & (~0x0F00))|0x0200; // 2 flash wait states
// GENCLKCFG
//   bit 9 is MFCLKSRC =1 for HFCLK, =0 for SYSOSC
//   bit 7 Clock divider is enabled
//   bits 6-4 divider = (x+1)*2, x=1 means divide by 4
//   bits 2-0 EXCLKSRC, 4 = CLK_OUT is HFCLK
  SYSCTL->SOCLOCK.GENCLKCFG = (SYSCTL->SOCLOCK.GENCLKCFG & (~0x00F7)) | 0x94 ; // HFCLK, divide by 4
  SYSCTL->SOCLOCK.GENCLKEN |= 0x01;
  // bit 4 is MFPCLKEN
  // bit 0 is EXCLKEN
  //  MFPCLK is a continuous 4MHz to DAC module in RUN/SLEEP/STOP mode.
  //SYSCTL->SOCLOCK.GENCLKCFG = (SYSCTL->SOCLOCK.GENCLKCFG & (~0x0200)) | 0x00; // MFPCLK is sourced from SYSOSC
  //SYSCTL->SOCLOCK.GENCLKEN |= SYSCTL_GENCLKEN_MFPCLKEN_ENABLE;

  SYSCTL->SOCLOCK.BORTHRESHOLD = 0; // lowest level brown-out reset


  //SCB->SCR System control register, used to control low-power mode behavior
  // bit 2, 0 is sleep, 1 is deep sleep
  // bit 1, 0 do not sleep when returning to Thread mode, 1 enter sleep/deepsleep on returning from ISR to thread
  SCB->SCR &= ~(0x04); // sleep
  SCB->SCR &= ~(0x02); // no sleep on exit
  BusFreq = 40000000;

}

void Clock_Init(uint32_t freq){
// SYSOSCCFG bits 1,0
// bits 1,0 set rate of SYSOSC
//   0 for 32 MHz base oscillator
//   1 for 4 MHz
//   2 for user trim
// bit 10 disable (0 to run, 1 to disable SYSOSC)
  SYSCTL->SOCLOCK.SYSOSCCFG = (SYSCTL->SOCLOCK.SYSOSCCFG & (~0x03))|freq; // 4 or 32 MHz
// MCLKCFG
//   bits 3-0 MCLK divider is value+1 (1 to 16)
//   bits 5-4 ULPCLK divider is value+1 (1 to 3)
//   bit 20 is USELFCLK
  SYSCTL->SOCLOCK.MCLKCFG = (SYSCTL->SOCLOCK.MCLKCFG & (~0x0F))|0x00; // divide by 1
  SYSCTL->SOCLOCK.MCLKCFG = (SYSCTL->SOCLOCK.MCLKCFG & (~0x30))|0x00; // divide by 1
// Only one should have been set, but clear both because unknown incoming state
// Clear SYSOSCCFG.DISABLE to get SYSOSC running again
// Clear MCLKCFG.USELFCLK to switch MCLK source from LFCLK to SYSOSC
  SYSCTL->SOCLOCK.SYSOSCCFG &= ~0x00000400; // get SYSOSC running again
  SYSCTL->SOCLOCK.MCLKCFG &= ~0x00100000;   // do not use LFCLK
  //  MFPCLK is a continuous 4MHz to DAC module in RUN/SLEEP/STOP mode.
  SYSCTL->SOCLOCK.GENCLKCFG = (SYSCTL->SOCLOCK.GENCLKCFG & (~0x0200)) | 0x00; // MFPCLK is sourced from SYSOSC
  SYSCTL->SOCLOCK.GENCLKEN |= SYSCTL_GENCLKEN_MFPCLKEN_ENABLE;

// Verify SYSOSC -> MCLK
// bit 17, 0 means MSLK not sourced from LFCLK, 1 means MCLK sourced from LFCLK
  while((SYSCTL->SOCLOCK.CLKSTATUS & 0x00020000) == 0x00020000){};
//SCB->SCR System control register, used to control low-power mode behavior
//   bit 2, 0 is sleep, 1 is deep sleep
// bit 1, 0 do not sleep when returning to Thread mode, 1 enter sleep/deepsleep on returning from ISR to thread
  SCB->SCR &= ~(0x04); // sleep
  SCB->SCR &= ~(0x02); // no sleep on exit
  SYSCTL->SOCLOCK.BORTHRESHOLD = 0; // lowest level brown-out reset
  if(freq == 1){
    BusFreq = 4000000;
  }else{
    BusFreq = 32000000;
  }

}


void Clock_Delay(uint32_t cycles){
    /* There will be a 2 cycle delay here to fetch & decode instructions
     * if branch and linking to this function */

    /* Subtract 2 net cycles for constant offset: +2 cycles for entry jump,
     * +2 cycles for exit, -1 cycle for a shorter loop cycle on the last loop,
     * -1 for this instruction */
#ifdef __GNUC__
    __asm(".syntax unified");
#endif
    __asm volatile(
"            SUBS  R0, R0, #2; \n"
"Clock_Loop: SUBS  R0, R0, #4; \n" // C=1 if no overflow
"            NOP;              \n" // C=0 when R0 passes through 0
"            BHS   Clock_Loop; \n"
        /* Return: 2 cycles */
    );
}
// approximate delay
// uses BusFreq clock
void Clock_Delay1ms(uint32_t ms){
// each call to Clock_Delay is count cycles
  uint32_t count = BusFreq/1000;
  while(ms){
    Clock_Delay(count);
    ms--;
  }
}


// initialize 24 bit timer
void SysTick_Init(void){
  SysTick->CTRL = 0;           // 1) disable SysTick during setup
  SysTick->LOAD = 0;           // 2) stops
  SysTick->VAL = 0;            // 3) any write to current clears it
  SysTick->CTRL = 0x00000005;  // 4) enable SysTick with core clock
}
// The delay parameter is in units of the core clock.
// about a 50 cycle overhead
void SysTick_Wait(uint32_t delay){
  SysTick->LOAD = delay-1;  // number of counts to wait
  SysTick->VAL = 0;         // any value written to VAL clears
  while((SysTick->CTRL&0x00010000)==0){} // wait for count flag
}
// The t parameter is in units 10ms.
void SysTick_Wait10ms(uint32_t t){
  for(; t; t--){
    SysTick_Wait(800000);  // wait 10ms, assuming 80 MHz clock
  }
}

// initialize G8 for continuous counting
// Program 4.2.1
// simply read TIMG8->CTR for 16 bit time
// divider must be 1 to 8
// prescale must be 1 to 256
// resolution is timerperiod*divider*prescale
// precision is 16 bits
// examples for 80 MHz clock
// divider prescale resolution range
//   1        1         25ns    1.6 us
//   1       40          1us    65 ms
//   1       80          2us   131 ms
//   4      100         10us   655 ms
//   8      256       51.2us   3.4 s
// power Domain PD0 for G0/G1/G8
// for 32MHz bus clock, Timer clock is 32MHz
// for 40MHz bus clock, Timer clock is ULPCLK 20MHz
// for 80MHz bus clock, Timer clock is ULPCLK 40MHz
void TimerG8_Init(uint32_t divider, uint32_t prescale){
  TIMG8->GPRCM.RSTCTL = (uint32_t)0xB1000003;
  TIMG8->GPRCM.PWREN = (uint32_t)0x26000001;
  Clock_Delay(24);     // time for TimerG8 to power up
  TIMG8->CLKSEL = 0x08; // bus clock
  TIMG8->CLKDIV = divider-1; // divide by divider
  TIMG8->COMMONREGS.CPS = prescale-1;    // divide by prescale,
  TIMG8->COUNTERREGS.LOAD  = 0xFFFF;     // set reload register
  TIMG8->COUNTERREGS.CTRCTL = 0x02;
    // bits 5-4 CM =0, down
    // bits 3-1 REPEAT =001, continue
    // bit 0 EN enable (0 for disable, 1 for enable)
  TIMG8->COMMONREGS.CCLKCTL = 1;
  TIMG8->COUNTERREGS.CTRCTL |= 0x01;
}


