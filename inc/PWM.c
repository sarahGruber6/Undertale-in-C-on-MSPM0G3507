/* PWM.c
 * Jonathan Valvano
 * December 19, 2022
 * Derived from timx_timer_mode_pwm_edge_sleep_LP_MSPM0G3507_nortos_ticlang
 
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
#define CONFIG_MSPM0G350X
#define SYSCONFIG_WEAK __attribute__((weak))
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/PWM.h"
#include "../inc/Clock.h"
//  G0/G8  is on Power domain PD0
// for 32MHz bus clock, BUSCLK clock is 32MHz
// for 40MHz bus clock, BUSCLK clock is ULPCLK 20MHz
// for 80MHz bus clock, BUSCLK clock is ULPCLK 40MHz
// CCP0 PWM output on PA12
// CCP1 PWM output on PA13
#define PA12INDEX 33
#define PA13INDEX 34
// Initialize PWM outputs on PA12 PA13
// Rising edge synchronized
// timerClkSrc = 2 for 32768 Hz LFCLK
//             = 4 for 4MHz MFCLK
//             = 8 for 40/32/4 BUSCLK
// divide clock by timerClkPrescale+1, 0 to 255
// period sets the PWM period
// timerClkSrc could be 40MHz, 32MHz, 4MHz, or 32767Hz
// timerClkDivRatio = 1
// PWMFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1) * period))
// For example, source=LFCLK, prescale=0, period = 1000, PWM frequency = 32.768 Hz
// For example, source=BUSCLK, 40MHz bus, prescale=19, period = 10000, PWM frequency = 10kHz
void PWM_Init(uint32_t timerClkSrc, uint32_t timerClkPrescale, uint32_t period, uint32_t duty0, uint32_t duty1){
  LaunchPad_Init(); // PB21 is input with internal pull up resistor
  TIMG0->GPRCM.RSTCTL = (uint32_t)0xB1000003;
  TIMG0->GPRCM.PWREN = (uint32_t)0x26000001;
  Clock_Delay(2); // time for TimerG0 to power up
  IOMUX->SECCFG.PINCM[PA12INDEX]  = 0x00000084; // TIMG0 output CCP0
  IOMUX->SECCFG.PINCM[PA13INDEX]  = 0x00000085; // TIMG0 output CCP1

  TIMG0->CLKSEL = timerClkSrc; // 8=BUSCLK, 4= MFCLK, 2= LFCLK clock
  TIMG0->CLKDIV = 0x00; // divide by 1
  TIMG0->COMMONREGS.CPS = timerClkPrescale;     // divide by prescale+1,
  // 32768Hz/256 = 256Hz, 7.8125
  TIMG0->COUNTERREGS.LOAD  = period-1;  // reload register sets period
  // interrupts at 200kHz/period, 5us*period
  TIMG0->COUNTERREGS.CTRCTL = 0x02;
  // bits 5-4 CM =0, down
  // bits 3-1 REPEAT =001, continue
  // bit 0 EN enable (0 for disable, 1 for enable)
  TIMG0->COUNTERREGS.CCCTL_01[0] = 0; // no capture
  TIMG0->COUNTERREGS.CCCTL_01[1] = 0; // no capture
  // COC compare mode
  TIMG0->GEN_EVENT0.IMASK = 0x00; // no interrupts
  TIMG0->COMMONREGS.CCPD = 0x03;  // output CCP1 CCP0
  TIMG0->COMMONREGS.CCLKCTL = 1;
  TIMG0->COUNTERREGS.CC_01[0] = duty0;
  TIMG0->COUNTERREGS.CC_01[1] = duty1;
  TIMG0->COUNTERREGS.OCTL_01[0] = 0x0000; // connected to PWM
  TIMG0->COUNTERREGS.OCTL_01[1] = 0x0000; // connected to PWM
  TIMG0->COUNTERREGS.CCACT_01[0] = 0x0088;
  TIMG0->COUNTERREGS.CCACT_01[1] = 0x0088;
  // bits 10-9 CUACT 0 for no action on up compare
  // bits 7-6 CDACT 10 for make low on compare event down
  // bits 4-3 LACT 01 for make high on load event
  // bits 1-0 ZACT 00 for no action on zero event
  TIMG0->COUNTERREGS.CTRCTL |= 0x01;
}
void PWM_SetDuty(uint32_t duty0, uint32_t duty1){
  TIMG0->COUNTERREGS.CC_01[0] = duty0;
  TIMG0->COUNTERREGS.CC_01[1] = duty1;
}


