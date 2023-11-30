/* InputCapture.c
 * Jonathan Valvano
 * December 18, 2022
 * Derived from timx_timer_mode_capture_edge_capture_LP_MSPM0G3507_nortos_ticlang
 
 */

/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/InputCapture.h"
#include "../inc/Clock.h"

// input wave connected to PA12
#define PA12INDEX 33
// G0/G8 in power domain PD0, so
// for 32MHz bus clock, SYSCLK clock is 32MHz
// for 40MHz bus clock, SYSCLK clock is ULPCLK 20MHz
// for 80MHz bus clock, SYSCLK clock is ULPCLK 40MHz
// Arm interrupts on rising edge of PA12
// interrupts will be enabled in main after all initialization
// timerClkSrc = 2 for 32768 Hz LFCLK
//             = 4 for 4MHz MFCLK
//             = 8 for 48/32/4 BUSCLK
// prescale divide clock by prescale+1, 0 to 255
// priority is 0(highest),1,2 or 3(lowest)
  /*
   * timerClkSrc could be 40 MHz, 20MHz, 32MHz, 4MHz, or 32767Hz
   * timerClkDivRatio = 1
   * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
   * resolution = (timerClkDivRatio * (timerClkPrescale + 1)))/timerClkSrc
   * For example, source=LFCLK, prescale=255, resolution = 7.8125ms
   * For example, source=BUSCLK, prescale=47, resolution = 1us
   */
void InputCapture_Init(uint32_t timerClkSrc, uint32_t timerClkPrescale, uint32_t priority){
  LaunchPad_Init(); // PB27 is output
  TIMG0->GPRCM.RSTCTL = 0xB1000003;
  TIMG0->GPRCM.PWREN = 0x26000001;
  Clock_Delay(24); // time for TimerG0 to power up
  IOMUX->SECCFG.PINCM[PA12INDEX]  = 0x00040084; // TIMG0 CCP0

  TIMG0->CLKSEL = timerClkSrc; // 8=BUSCLK, 4= MFCLK, 2= LFCLK clock
  TIMG0->CLKDIV = 0x00; // divide by 1
  TIMG0->COMMONREGS.CPS = timerClkPrescale;     // divide by prescale+1,
  // 32768Hz/256 = 256Hz, 7.8125
  TIMG0->COUNTERREGS.LOAD  = 0xFFFF;  // set reload register
  // interrupts at 200kHz/period, 5us*period
  TIMG0->COUNTERREGS.CTRCTL = 0x02;
  // bits 5-4 CM =0, down
  // bits 3-1 REPEAT =001, continue
  // bit 0 EN enable (0 for disable, 1 for enable)
  TIMG0->COUNTERREGS.CCCTL_01[0] = 0x00020001;
  // CCCTL_01
  // bit 17 1 capture
  // bits 14-12 ZCOND zero condition
  // bits 10-8 LCOND load condition
  // bits 6-4 ACOND advance condition
  // bits 2-0 CCOND capture condition, 1 is rising edge of CCP
  TIMG0->CPU_INT.IMASK |= 0x10; // CCD0 mask
  TIMG0->COMMONREGS.CCLKCTL = 1;
  TIMG0->COUNTERREGS.IFCTL_01[0] = 0x0002;
  // bit 12 FE =0 bypass
  // bit 11 CPV=0 voting
  // bit 9-8 FP=00 filter period 3
  // bit 7 INV=0 noninverted
  // bits 3-0 =0010 input is CCP0
  NVIC->ISER[0] = 1 << 16; // TIMG0 interrupt
  NVIC->IP[4] = (NVIC->IP[4]&(~0x000000FF))|(priority<<6);    // set priority (bits 7,6) IRQ 16
  TIMG0->COUNTERREGS.CTRCTL |= 0x01;
}


