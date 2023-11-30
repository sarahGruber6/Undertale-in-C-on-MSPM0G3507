/* ADC.c
 * Jonathan Valvano
 * December 20, 2022
 * Derived from adc12_single_conversion_vref_internal_LP_MSPM0G3507_nortos_ticlang
 *              adc12_single_conversion_LP_MSPM0G3507_nortos_ticlang
 * PB24_ADC0.5 thermistor input, V_temperature
 * connect J27.1 to J27.2
 */


#include <ti/devices/msp/msp.h>
#include "../inc/ADC.h"
#include "../inc/Clock.h"
// PA27 ADC0 channel 0 J1.8 also MKII light interrupt
// PA26 ADC0 channel 1 J1.5 Joystick select button
// PA25 ADC0 channel 2 J1.2 MKII Joystick X
// PA24 ADC0 channel 3 J3.27 ***free***
// PB25 ADC0 channel 4 J19.7 (insert 0ohm R74, no U3 OPA2365)
// PB24 ADC0 channel 5 J1.6 MKII microphone in
// PB20 ADC0 channel 6 J4.36 ***free***
// PA22 ADC0 channel 7 J24 MKII Accelerometer Y
//
// PA15 ADC1 channel 0 J3.30 (also DACout)
// PA16 ADC1 channel 1 J3.29 ***free***
// PA17 ADC1 channel 2 J3.28 ***free***
// PA18 ADC1 channel 3 J3.26 MKII Joystick Y
// PB17 ADC1 channel 4 J2.18 ***free***
// PB18 ADC1 channel 5 J3.25 MKII Accelerometer Z
// PB19 ADC1 channel 6 J3.23 MKII Accelerometer X
// PA21 ADC1 channel 7 J17.8 (insert R20, remove R3)

// Assumes 40 MHz bus
void ADC0_Init(uint32_t channel, uint32_t reference){
    // Reset ADC and VREF
    // RSTCLR
    //   bits 31-24 unlock key 0xB1
    //   bit 1 is Clear reset sticky bit
    //   bit 0 is reset ADC
  ADC0->ULLMEM.GPRCM.RSTCTL = 0xB1000003;
  if(reference == ADCVREF_INT){
    VREF->GPRCM.RSTCTL = 0xB1000003;
  }
    // Enable power ADC and VREF
    // PWREN
    //   bits 31-24 unlock key 0x26
    //   bit 0 is Enable Power
  ADC0->ULLMEM.GPRCM.PWREN = 0x26000001;
  if(reference == ADCVREF_INT){
    VREF->GPRCM.PWREN = 0x26000001;
  }
  Clock_Delay(24); // time for ADC and VREF to power up
  ADC0->ULLMEM.GPRCM.CLKCFG = 0xA9000000; // ULPCLK
  // bits 31-24 key=0xA9
  // bit 5 CCONSTOP= 0 not continuous clock in stop mode
  // bit 4 CCORUN= 0 not continuous clock in run mode
  // bit 1-0 0=ULPCLK,1=SYSOSC,2=HFCLK
  ADC0->ULLMEM.CLKFREQ = 7; // 40 to 48 MHz
  ADC0->ULLMEM.CTL0 = 0x03010000;
  // bits 26-24 = 011 divide by 8
  // bit 16 PWRDN=1 for manual, =0 power down on completion, if no pending trigger
  // bit 0 ENC=0 disable (1 to 0 will end conversion)
  ADC0->ULLMEM.CTL1 = 0x00000000;
  // bits 30-28 =0  no shift
  // bits 26-24 =0  no averaging
  // bit 20 SAMPMODE=1 software triggers
  // bits 17-16 CONSEQ=0 ADC at start will be sampled once, 10 for repeated sampling
  // bit 8 SC=0 for stop, =1 to software start
  // bit 0 TRIGSRC=0 software trigger
  ADC0->ULLMEM.CTL2 = 0x00000000;
  // bits 28-24 ENDADD (which  MEMCTL to end)
  // bits 20-16 STARTADD (which  MEMCTL to start)
  // bits 15-11 SAMPCNT (for DMA)
  // bit 10 FIFOEN=0 disable FIFO
  // bit 8  DMAEN=0 disable DMA
  // bits 2-1 RES=0 for 12 bit (=1 for 10bit,=2for 8-bit)
  // bit 0 DF=0 unsigned formant (1 for signed, left aligned)
  ADC0->ULLMEM.MEMCTL[0] = reference+channel;
  // bit 28 WINCOMP=0 disable window comparator
  // bit 24 TRIG trigger policy, =0 for auto next, =1 for next requires trigger
  // bit 20 BCSEN=0 disable burn out current
  // bit 16 = AVGEN =0 for no averaging
  // bit 12 = STIME=0 for SCOMP0
  // bits 9-8 VRSEL = 10 for internal VREF,(00 for VDDA)
  // bits 4-0 channel = 0 to 7 available
  ADC0->ULLMEM.SCOMP0 = 0; // 8 sample clocks
//  ADC0->ULLMEM.GEN_EVENT.ICLR |= 0x0100; // clear flag MEMCTL[0]
//  ADC0->ULLMEM.GEN_EVENT.IMASK = 0; // no interrupt
  if(reference == ADCVREF_INT){
    VREF->CLKSEL = 0x00000008; // bus clock
    VREF->CLKDIV = 0; // divide by 1
    VREF->CTL0 = 0x0001;
  // bit 8 SHMODE = off
  // bit 7 BUFCONFIG=0 for 2.4 (=1 for 1.4)
  // bit 0 is enable
    VREF->CTL2 = 0;
  // bits 31-16 HCYCLE=0
    // bits 15-0 SHCYCLE=0
    while((VREF->CTL1&0x01)==0){}; // wait for VREF to be ready
  }
}
// sample 12-bit ADC
uint32_t ADC0_In(void){
  ADC0->ULLMEM.CTL0 |= 0x00000001; // enable conversions
  ADC0->ULLMEM.CTL1 |= 0x00000100; // start ADC
  uint32_t volatile delay=ADC0->ULLMEM.STATUS; // time to let ADC start
  while((ADC0->ULLMEM.STATUS&0x01)==0x01){} // wait for completion
  return ADC0->ULLMEM.MEMRES[0];
}


// Assumes 40 MHz bus
void ADC1_Init(uint32_t channel, uint32_t reference){
    // Reset ADC and VREF
    // RSTCLR
    //   bits 31-24 unlock key 0xB1
    //   bit 1 is Clear reset sticky bit
    //   bit 0 is reset ADC
  ADC1->ULLMEM.GPRCM.RSTCTL = (uint32_t)0xB1000003;
  if(reference == ADCVREF_INT){
    VREF->GPRCM.RSTCTL = (uint32_t)0xB1000003;
  }
    // Enable power ADC and VREF
    // PWREN
    //   bits 31-24 unlock key 0x26
    //   bit 0 is Enable Power
  ADC1->ULLMEM.GPRCM.PWREN = (uint32_t)0x26000001;
  if(reference == ADCVREF_INT){
    VREF->GPRCM.PWREN = (uint32_t)0x26000001;
  }
  Clock_Delay(24); // time for ADC and VREF to power up
  ADC1->ULLMEM.GPRCM.CLKCFG = 0xA9000000; // ULPCLK
  // bits 31-24 key=0xA9
  // bit 5 CCONSTOP= 0 not continuous clock in stop mode
  // bit 4 CCORUN= 0 not continuous clock in run mode
  // bit 1-0 0=ULPCLK,1=SYSOSC,2=HFCLK
  ADC1->ULLMEM.CLKFREQ = 7; // 40 to 48 MHz
  ADC1->ULLMEM.CTL0 = 0x03010000;
  // bits 26-24 = 011 divide by 8
  // bit 16 PWRDN=1 for manual, =0 power down on completion, if no pending trigger
  // bit 0 ENC=0 disable (1 to 0 will end conversion)
  ADC1->ULLMEM.CTL1 = 0x00000000;
  // bits 30-28 =0  no shift
  // bits 26-24 =0  no averaging
  // bit 20 SAMPMODE=0 high phase
  // bits 17-16 CONSEQ=0 ADC at start will be sampled once, 10 for repeated sampling
  // bit 8 SC=0 for stop, =1 to software start
  // bit 0 TRIGSRC=0 software trigger
  ADC1->ULLMEM.CTL2 = 0x00000000;
  // bits 28-24 ENDADD (which  MEMCTL to end)
  // bits 20-16 STARTADD (which  MEMCTL to start)
  // bits 15-11 SAMPCNT (for DMA)
  // bit 10 FIFOEN=0 disable FIFO
  // bit 8  DMAEN=0 disable DMA
  // bits 2-1 RES=0 for 12 bit (=1 for 10bit,=2for 8-bit)
  // bit 0 DF=0 unsigned formant (1 for signed, left aligned)
  ADC1->ULLMEM.MEMCTL[0] = reference+channel;
  // bit 28 WINCOMP=0 disable window comparator
  // bit 24 TRIG trigger policy, =0 for auto next, =1 for next requires trigger
  // bit 20 BCSEN=0 disable burn out current
  // bit 16 = AVGEN =0 for no averaging
  // bit 12 = STIME=0 for SCOMP0
  // bits 9-8 VRSEL = 10 for internal VREF,(00 for VDDA)
  // bits 4-0 channel = 0 to 7 available
  ADC1->ULLMEM.SCOMP0 = 0; // 8 sample clocks
 // ADC1->ULLMEM.GEN_EVENT.ICLR |= 0x0100; // clear flag MEMCTL[0]
  ADC1->ULLMEM.GEN_EVENT.IMASK = 0; // no interrupt
  if(reference == ADCVREF_INT){
    VREF->CLKSEL = 0x00000008; // bus clock
    VREF->CLKDIV = 0; // divide by 1
    VREF->CTL0 = 0x0001;
    // bit 8 SHMODE = off
    // bit 7 BUFCONFIG=0 for 2.4 (=1 for 1.4)
    // bit 0 is enable
    VREF->CTL2 = 0;
    // bits 31-16 HCYCLE=0
    // bits 15-0 SHCYCLE=0
    while((VREF->CTL1&0x01)==0){}; // wait for VREF to be ready
  }
}

// sample 12-bit ADC
uint32_t ADC1_In(void){
  ADC1->ULLMEM.CTL0 |= 0x00000001; // enable conversions
  ADC1->ULLMEM.CTL1 |= 0x00000100; // start ADC
  uint32_t volatile delay=ADC1->ULLMEM.STATUS; // time to let ADC start
  while((ADC1->ULLMEM.STATUS&0x01)==0x01){}; // wait for completion
  return ADC1->ULLMEM.MEMRES[0];

}


// Assumes 40 MHz bus
void ADC_Init(ADC12_Regs *adc12,uint32_t channel, uint32_t reference){
    // Reset ADC and VREF
    // RSTCLR
    //   bits 31-24 unlock key 0xB1
    //   bit 1 is Clear reset sticky bit
    //   bit 0 is reset ADC
  adc12->ULLMEM.GPRCM.RSTCTL = (uint32_t)0xB1000003;
  if(reference == ADCVREF_INT){
    VREF->GPRCM.RSTCTL = (uint32_t)0xB1000003;
  }
    // Enable power ADC and VREF
    // PWREN
    //   bits 31-24 unlock key 0x26
    //   bit 0 is Enable Power
  adc12->ULLMEM.GPRCM.PWREN = (uint32_t)0x26000001;
  if(reference == ADCVREF_INT){
    VREF->GPRCM.PWREN = (uint32_t)0x26000001;
  }
  Clock_Delay(24); // time for ADC and VREF to power up
  adc12->ULLMEM.GPRCM.CLKCFG = 0xA9000000; // ULPCLK
  // bits 31-24 key=0xA9
  // bit 5 CCONSTOP= 0 not continuous clock in stop mode
  // bit 4 CCORUN= 0 not continuous clock in run mode
  // bit 1-0 0=ULPCLK,1=SYSOSC,2=HFCLK
  adc12->ULLMEM.CLKFREQ = 7; // 40 to 48 MHz
  adc12->ULLMEM.CTL0 = 0x03010000;
  // bits 26-24 = 011 divide by 8
  // bit 16 PWRDN=1 for manual, =0 power down on completion, if no pending trigger
  // bit 0 ENC=0 disable (1 to 0 will end conversion)
  adc12->ULLMEM.CTL1 = 0x00000000;
  // bits 30-28 =0  no shift
  // bits 26-24 =0  no averaging
  // bit 20 SAMPMODE=0 high phase
  // bits 17-16 CONSEQ=0 ADC at start will be sampled once, 10 for repeated sampling
  // bit 8 SC=0 for stop, =1 to software start
  // bit 0 TRIGSRC=0 software trigger
  adc12->ULLMEM.CTL2 = 0x00000000;
  // bits 28-24 ENDADD (which  MEMCTL to end)
  // bits 20-16 STARTADD (which  MEMCTL to start)
  // bits 15-11 SAMPCNT (for DMA)
  // bit 10 FIFOEN=0 disable FIFO
  // bit 8  DMAEN=0 disable DMA
  // bits 2-1 RES=0 for 12 bit (=1 for 10bit,=2for 8-bit)
  // bit 0 DF=0 unsigned formant (1 for signed, left aligned)
  adc12->ULLMEM.MEMCTL[0] = reference|channel;
  // bit 28 WINCOMP=0 disable window comparator
  // bit 24 TRIG trigger policy, =0 for auto next, =1 for next requires trigger
  // bit 20 BCSEN=0 disable burn out current
  // bit 16 = AVGEN =0 for no averaging
  // bit 12 = STIME=0 for SCOMP0
  // bits 9-8 VRSEL = 10 for internal VREF,(00 for VDDA)
  // bits 4-0 channel = 0 to 7 available
  adc12->ULLMEM.SCOMP0 = 0; // 8 sample clocks
//  adc12->ULLMEM.INT_EVENT0.ICLR |= 0x0100; // clear flag MEMCTL[0]
  adc12->ULLMEM.GEN_EVENT.IMASK = 0; // no interrupt
  if(reference == ADCVREF_INT){
    VREF->CLKSEL = 0x00000008; // bus clock
    VREF->CLKDIV = 0; // divide by 1
    VREF->CTL0 = 0x0001;
  // bit 8 SHMODE = off
  // bit 7 BUFCONFIG=0 for 2.4 (=1 for 1.4)
  // bit 0 is enable
    VREF->CTL2 = 0;
  // bits 31-16 HCYCLE=0
    // bits 15-0 SHCYCLE=0
    while((VREF->CTL1&0x01)==0){}; // wait for VREF to be ready
  }
}

// sample 12-bit ADC
uint32_t ADC_In(ADC12_Regs *adc12){
  adc12->ULLMEM.CTL0 |= 0x00000001; // enable conversions
  adc12->ULLMEM.CTL1 |= 0x00000100; // start ADC
  uint32_t volatile delay=adc12->ULLMEM.STATUS; // time to let ADC start
  while((adc12->ULLMEM.STATUS&0x01)==0x01){}; // wait for completion
  return adc12->ULLMEM.MEMRES[0];
}


// Assumes 40 MHz bus
void ADC_InitDual(ADC12_Regs *adc12,uint32_t channel1,uint32_t channel2, uint32_t reference){
    // Reset ADC and VREF
    // RSTCLR
    //   bits 31-24 unlock key 0xB1
    //   bit 1 is Clear reset sticky bit
    //   bit 0 is reset ADC
  adc12->ULLMEM.GPRCM.RSTCTL = (uint32_t)0xB1000003;
  if(reference == ADCVREF_INT){
    VREF->GPRCM.RSTCTL = (uint32_t)0xB1000003;
  }
    // Enable power ADC and VREF
    // PWREN
    //   bits 31-24 unlock key 0x26
    //   bit 0 is Enable Power
  adc12->ULLMEM.GPRCM.PWREN = (uint32_t)0x26000001;
  if(reference == ADCVREF_INT){
    VREF->GPRCM.PWREN = (uint32_t)0x26000001;
  }
  Clock_Delay(24); // time for ADC and VREF to power up
  adc12->ULLMEM.GPRCM.CLKCFG = 0xA9000000; // ULPCLK
  // bits 31-24 key=0xA9
  // bit 5 CCONSTOP= 0 not continuous clock in stop mode
  // bit 4 CCORUN= 0 not continuous clock in run mode
  // bit 1-0 0=ULPCLK,1=SYSOSC,2=HFCLK
  adc12->ULLMEM.CLKFREQ = 7; // 40 to 48 MHz
  adc12->ULLMEM.CTL0 = 0x03010000;
  // bits 26-24 = 011 divide by 8
  // bit 16 PWRDN=1 for manual, =0 power down on completion, if no pending trigger
  // bit 0 ENC=0 disable (1 to 0 will end conversion)
  adc12->ULLMEM.CTL1 = 0x00010000;
  // bits 30-28 =0  no shift
  // bits 26-24 =0  no averaging
  // bit 20 SAMPMODE=0 high phase
  // bits 17-16 CONSEQ=01 ADC at start to end
  // bit 8 SC=0 for stop, =1 to software start
  // bit 0 TRIGSRC=0 software trigger
  adc12->ULLMEM.CTL2 = 0x02010000;
  // bits 28-24 ENDADD=2 (which  MEMCTL to end)
  // bits 20-16 STARTADD=1 (which  MEMCTL to start)
  // bits 15-11 SAMPCNT (for DMA)
  // bit 10 FIFOEN=0 disable FIFO
  // bit 8  DMAEN=0 disable DMA
  // bits 2-1 RES=0 for 12 bit (=1 for 10bit,=2for 8-bit)
  // bit 0 DF=0 unsigned formant (1 for signed, left aligned)
  adc12->ULLMEM.MEMCTL[1] = reference+channel1;
  adc12->ULLMEM.MEMCTL[2] = reference+channel2;
  // bit 28 WINCOMP=0 disable window comparator
  // bit 24 TRIG trigger policy, =0 for auto next, =1 for next requires trigger
  // bit 20 BCSEN=0 disable burn out current
  // bit 16 = AVGEN =0 for no averaging
  // bit 12 = STIME=0 for SCOMP0
  // bits 9-8 VRSEL = 10 for internal VREF,(00 for VDDA)
  // bits 4-0 channel = 0 to 7 available
  adc12->ULLMEM.SCOMP0 = 0; // 8 sample clocks
//  adc12->ULLMEM.GEN_EVENT.ICLR |= 0x0100; // clear flag MEMCTL[1] ??
  adc12->ULLMEM.GEN_EVENT.IMASK = 0; // no interrupt
  if(reference == ADCVREF_INT){
    VREF->CLKSEL = 0x00000008; // bus clock
    VREF->CLKDIV = 0; // divide by 1
    VREF->CTL0 = 0x0001;
  // bit 8 SHMODE = off
  // bit 7 BUFCONFIG=0 for 2.4 (=1 for 1.4)
  // bit 0 is enable
    VREF->CTL2 = 0;
  // bits 31-16 HCYCLE=0
    // bits 15-0 SHCYCLE=0
    while((VREF->CTL1&0x01)==0){}; // wait for VREF to be ready
  }
}

// sample 12-bit ADC
void ADC_InDual(ADC12_Regs *adc12,uint32_t *d1, uint32_t *d2){
  adc12->ULLMEM.CTL0 |= 0x00000001; // enable conversions
  adc12->ULLMEM.CTL1 |= 0x00000100; // start ADC
  uint32_t volatile delay=adc12->ULLMEM.STATUS; // time to let ADC start
  while((adc12->ULLMEM.STATUS&0x01)==0x01){}; // wait for completion
  *d1 = adc12->ULLMEM.MEMRES[1];
  *d2 = adc12->ULLMEM.MEMRES[2];
}
// n means 2^n samples in average, n=0 to 7
void ADC0_InitAve(uint32_t channel, uint32_t n){
  ADC0->ULLMEM.GPRCM.RSTCTL = 0xB1000003;  // 1) reset
  ADC0->ULLMEM.GPRCM.PWREN = 0x26000001;   // 2) activate
  Clock_Delay(24);                         // 3) wait
  ADC0->ULLMEM.GPRCM.CLKCFG = 0xA9000000;  // 4) ULPCLK
  ADC0->ULLMEM.CLKFREQ = 7;                // 5) 40-48 MHz
  ADC0->ULLMEM.CTL0 = 0x03010000;          // 6) divide by 8
  ADC0->ULLMEM.CTL1 = 0x00000000|(n<<28)|(n<<24);// 7) mode
  // AVGD = AVEN = n
  ADC0->ULLMEM.CTL2 = 0x00000000;          // 8) MEMRES
  if(n){
    ADC0->ULLMEM.MEMCTL[0] = (1<<16)|channel;  // 9) channel
  }else{
    ADC0->ULLMEM.MEMCTL[0] = channel;      // 9) channel
  }
  ADC0->ULLMEM.SCOMP0 = 0;                 // 10) 8 sample clocks
  ADC0->ULLMEM.GEN_EVENT.IMASK = 0;       // 11) no interrupt
}

