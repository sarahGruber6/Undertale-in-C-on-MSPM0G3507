/* DAC.c
 * Jonathan Valvano
 * November 30, 2022
 * Derived from dac12_fixed_voltage_vref_internal_LP_MSPM0G3507_nortos_ticlang
 *              dac12_fifo_timer_event_LP_MSPM0G3507_nortos_ticlang
 */
// PA15 J3.30  DACout


#include <ti/devices/msp/msp.h>
#include "../inc/DAC.h"
#include "../inc/Clock.h"
// Assumes 40 or 80 MHz MHz bus
void DAC_Init(void){
    // Reset DAC and VREF
    // RSTCLR
    //   bits 31-24 unlock key 0xB1
    //   bit 1 is Clear reset sticky bit
    //   bit 0 is reset ADC
  VREF->GPRCM.RSTCTL = 0xB1000003;
  DAC0->GPRCM.RSTCTL = 0xB1000003;

    // Enable power ADC and VREF
    // PWREN
    //   bits 31-24 unlock key 0x26
    //   bit 0 is Enable Power
  VREF->GPRCM.PWREN = 0x26000001;
  DAC0->GPRCM.PWREN = 0x26000001;

  Clock_Delay(24); // time for ADC and VREF to power up

  VREF->CLKSEL = 0x00000008; // bus clock
  VREF->CLKDIV = 0; // divide by 1
  VREF->CTL0 = 0x0001;
  // bit 8 SHMODE = off
  // bit 7 BUFCONFIG=0 for 2.5 (=1 for 1.4)
  // bit 0 is enable
  VREF->CTL2 = 0;
  // bits 31-16 HCYCLE=0
    // bits 15-0 SHCYCLE=0
  while((VREF->CTL1&0x01)==0){}; // wait for VREF to be ready
  // CTL0
  // bit 16 DFM 0 straight binary
  // bit 8 RES 1 12-bit
  // bit 0 ENABLE 1 for on, 0 for disable
  DAC0->CTL0 = 0x0100; // 12-bit, straight, disable
  // CTL1
  // bit 24 OPS 1 OUT0 is selected
  // bit 9 REFSN 0 VR- is VrefN, 1 for analog ground
  // bit 8 REFSP 1 VR+ is VrefP, 0 for analog supply
  // bit 1 AMPHIZ 0 HiZ when disabled
  // bit 0 AMPEN 1 enabled
  DAC0->CTL1 = (1<<24)|(1<<9)|(1<<8)|1;
 // DAC0->CTL1 = (1<<24)|(1<<9)|1;
  DAC0->CTL2 = 0; // no DMA, no FIFO trigger
  // CTL3
  // bits 11-8 STIMCONFIG n=0 for 500 sps
  //     for n = 0 to 5 500*2^n sps (500 to  16k)
  //     n=6 100 ksps
  //     n=7 200 ksps
  //     n=8 500 ksps
  //     n=9 1 Msps
  // bit 0 STIMEN =0 disable sample time generator
  DAC0->CTL3 = 0x0700; // no sample time generator
  DAC0->CTL0 = 0x0101; // 12-bit, straight, enable
}

// output to 12-bit DAC
void DAC_Out(uint32_t data){
  DAC0->DATA0 = data;
}
