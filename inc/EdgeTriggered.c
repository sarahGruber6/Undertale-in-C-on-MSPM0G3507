/* EdgeTriggered.c
 * Jonathan Valvano
 * August 8, 2023
 * Derived
 * Derived from gpio_toggle_output_LP_MSPM0G3507_nortos_ticlang
 *              gpio_input_capture_LP_MSPM0G3507_nortos_ticlang
 * Will invoke LaunchPad_Init
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
#include "../inc/EdgeTriggered.h"
#include "../inc/LaunchPad.h"

// PB21 is S3 negative logic switch,  index 48 in IOMUX PINCM table

// Arm interrupts on fall of PB21
// interrupts will be enabled in main after all initialization
void EdgeTriggered_Init(void){
  LaunchPad_Init(); // PB21 is input with internal pull up resistor
  GPIOB->POLARITY31_16 = 0x00000800;     // falling
  GPIOB->CPU_INT.ICLR = 0x00200000;   // clear bit 21
  GPIOB->CPU_INT.IMASK = 0x00200000;  // arm PB21
  NVIC->IP[0] = (NVIC->IP[0]&(~0x0000FF00))|2<<14;    // set priority (bits 15,14) IRQ 1
  NVIC->ISER[0] = 1 << 1; // Group1 interrupt
}


