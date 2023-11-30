/* TExaS.c
 * Jonathan Valvano
 * August 8, 2023
 * Derived from adc12_single_conversion_vref_internal_LP_MSPM0G3507_nortos_ticlang
 *              adc12_single_conversion_LP_MSPM0G3507_nortos_ticlang

 */

#include <ti/devices/msp/msp.h>
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
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
// for logic analyzer, create a TEXAS_LOGIC macro to read any 7bit digital measurement, add bit7=1
#define PAoutPin0Bit0 (GPIOA->DOUT31_0&1)
#define PBoutPin22Bit1 (((GPIOB->DOUT31_0&(1<<22))>>21))
#define PBoutPin26Bit2 (((GPIOB->DOUT31_0&(1<<26))>>24))
#define PBoutPin27Bit3 (((GPIOB->DOUT31_0&(1<<27))>>24))
#define PAinPin18Bit4 (((GPIOA->DIN31_0&(1<<18))>>14))
#define PBinPin21Bit5 (((GPIOB->DIN31_0&(1<<21))>>16))
// the following configures logic analyzer for PB21,PA18,PB27,PB26,PB22,PA0 (40 instructions, 2us, 2%overhead)
uint8_t TExaS_LaunchPadLogic(void){
  return (0x80|PBinPin21Bit5|PAinPin18Bit4|PBoutPin27Bit3|PBoutPin26Bit2|PBoutPin22Bit1|PAoutPin0Bit0);
}
// the following configures logic analyzer for PB22,PA0, (16 instructions, 1.5us, 1.5% overhead)
uint8_t TExaS_PB22PA0Logic(void){
  return (0x80|PBoutPin22Bit1|PAoutPin0Bit0);
}
// the following configures logic analyzer for PA6-PA0 outputs, (11 instructions, 1.5us, 1.5% overhead)
uint8_t TExaS_PA60Logic(void){
  return (0x80|(GPIOA->DOUT31_0&0x7F));
}
// the following configures logic analyzer for PB18-16 outputs, PB2-0 input
uint8_t TExaS_PB18PB17PB16PB2PB1PB0Logic(void){
  return (0x80|((GPIOB->DOUT31_0&0x70000)>>13)|(GPIOB->DIN31_0&0x07));
}

uint8_t (*TExaSLogic)(void);
#define ADCVREF_INT  0x200
// choose internal 2.5V reference for accuracy
#define ADCVREF_EXT  0x100
// external reference not tested
#define ADCVREF_VDDA 0x000
//choose power line 3.3V reference for 0 to 3.3V range

// Bus frequency set by main program
// continuous sampling, 8 bit mode
void ADC_Init(ADC12_Regs *adc12, uint32_t channel, uint32_t reference){
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
  adc12->ULLMEM.GPRCM.CLKCFG = 0xA9000000; // SYSOSC
  // bits 31-24 key=0xA9
  // bit 5 CCONSTOP= 0 not continuous clock in stop mode
  // bit 4 CCORUN= 0 not continuous clock in run mode
  // bit 1-0 0=SYSOSC,1=MCLK,2=HFCLK
  if(Clock_Freq() > 32000000){
    adc12->ULLMEM.CLKFREQ = 7; // 7 is 40 MHz
  }else{
    adc12->ULLMEM.CLKFREQ = 5; //5 is 32MHz
  }
  adc12->ULLMEM.CTL0 = 0x03010000;
  // bits 26-24 = 011 divide by 8
  // bit 16 PWRDN=1 for manual, =0 power down on completion, if no pending trigger
  // bit 0 ENC=0 disable (1 to 0 will end conversion), =1 for enable
  adc12->ULLMEM.CTL1 = 0x00020000;
  // bits 30-28 =0  no shift
  // bits 26-24 =0  no averaging
  // bit 20 SAMPMODE=0 high phase
  // bits 17-16 CONSEQ=00 ADC at start will be sampled once, 10 for repeated sampling
  // bit 8 SC=0 for stop, =1 to software start
  // bit 0 TRIGSRC=0 software trigger
  adc12->ULLMEM.CTL2 = 0x00000004;
  // bits 28-24 ENDADD (which  MEMCTL to end)
  // bits 20-16 STARTADD (which  MEMCTL to start)
  // bits 15-11 SAMPCNT (for DMA)
  // bit 10 FIFOEN=0 disable FIFO
  // bit 8  DMAEN=0 disable DMA
  // bits 2-1 RES=00 for 12 bit (=01 for 10bit,=10for 8-bit)
  // bit 0 DF=0 unsigned formant (1 for signed, left aligned)
  adc12->ULLMEM.MEMCTL[0] = reference+channel;
  // bit 28 WINCOMP=0 disable window comparator
  // bit 24 TRIG trigger policy, =0 for auto next, =1 for next requires trigger
  // bit 20 BCSEN=0 disable burn out current
  // bit 16 = AVGEN =0 for no averaging
  // bit 12 = STIME=0 for SCOMP0
  // bits 9-8 VRSEL = 10 for internal VREF,(00 for VDDA)
  // bits 4-0 channel = 0 to 7 available
  adc12->ULLMEM.SCOMP0 = 0; // 8 sample clocks
//  adc12->ULLMEM.GEN_EVENT.ICLR |= 0x0100; // clear flag MEMCTL[0]
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

// start continuous sampling 12-bit ADC
void ADC_Start(ADC12_Regs *adc12){
  adc12->ULLMEM.CTL0 |= 0x00000001; // enable conversions
  adc12->ULLMEM.CTL1 |= 0x00000100; // start ADC
}
// return last sample 12-bit ADC
uint32_t ADC_Data(ADC12_Regs *adc12){
  return adc12->ULLMEM.MEMRES[0];
}

#define PA10INDEX 20
#define PA11INDEX 21
// assume 32MHz or 40MHz or 80MHz bus clock
// initialize UART for 115200 baud rate
// blind output, no input (yet)
void UART_Init(void){
    // RSTCLR to GPIOA and UART0 peripherals
    //   bits 31-24 unlock key 0xB1
    //   bit 1 is Clear reset sticky bit
    //   bit 0 is reset gpio port
//  GPIOA->GPRCM.RSTCTL = 0xB1000003; // called previously
  UART0->GPRCM.RSTCTL = 0xB1000003;
    // Enable power to GPIOA and UART0 peripherals
    // PWREN
    //   bits 31-24 unlock key 0x26
    //   bit 0 is Enable Power
//  GPIOA->GPRCM.PWREN = (uint32_t)0x26000001;
  UART0->GPRCM.PWREN = 0x26000001;
  // configure PA11 PA10 as alternate UART0 function
  IOMUX->SECCFG.PINCM[PA10INDEX]  = 0x00000082;
  IOMUX->SECCFG.PINCM[PA11INDEX]  = 0x00040082;
  Clock_Delay(24); // time for gpio to power up
  UART0->CLKSEL = 0x08; // bus clock
  UART0->CLKDIV = 0x00; // no divide
  UART0->CTL0 &= ~0x01; // disable UART0
  UART0->CTL0 = 0x00020018;
   // bit  17    FEN=1    enable FIFO
   // bits 16-15 HSE=00   16x oversampling
   // bit  14    CTSEN=0  no CTS hardware
   // bit  13    RTSEN=0  no RTS hardware
   // bit  12    RTS=0    not RTS
   // bits 10-8  MODE=000 normal
   // bits 6-4   TXE=001  enable TxD
   // bit  3     RXE=1    enable TxD
   // bit  2     LBE=0    no loop back
   // bit  0     ENABLE   0 is disable, 1 to enable
  if(Clock_Freq() == 40000000){
      // 20000000/16 = 1,250,000 Hz
     // Baud = 115200
      //   1,250,000/115200 = 10.850694
      //   divider = 10+54/64 = 10.84375
      UART0->IBRD = 10;
      UART0->FBRD = 54; // baud =1,250,000/10.84375 = 115,274
  }else if (Clock_Freq() == 32000000){
    // 32000000/16 = 2,000,000
     // Baud = 115200
      //   2,000,000/115200 = 17.361
      //   divider = 17+23/64 = 17.359
    UART0->IBRD = 17;
    UART0->FBRD = 23;
  }else if (Clock_Freq() == 80000000){
     // 40000000/16 = 2,500,000 Hz
     // Baud = 115200
      //    2,500,000/115200 = 21.701388
      //   divider = 21+45/64 = 21.703125
      UART0->IBRD = 21;
      UART0->FBRD = 45; // baud =2,500,000/21.703125 = 115,191
  }else return;
  UART0->LCRH = 0x00000030;
   // bits 5-4 WLEN=11 8 bits
   // bit  3   STP2=0  1 stop
   // bit  2   EPS=0   parity select
   // bit  1   PEN=0   no parity
   // bit  0   BRK=0   no break
  UART0->CTL0 |= 0x01; // enable UART0
}

//------------UART_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
char UART_InChar(void){
  while((UART0->STAT&0x04) == 0x04){}; // wait for input
  return((char)(UART0->RXDATA));
}
//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(char data){
  while((UART0->STAT&0x80) == 0x80){}; // spin if TxFifo full
  UART0->TXDATA = data;
}
// blind
#define UART_Out(data) (UART0->TXDATA = data)

#define ADC_In(adc12) ((adc12)->ULLMEM.MEMRES[0])
ADC12_Regs *TExaSadc;
uint32_t test=0;
void TIMG7_IRQHandler(void){
  TIMG7->GEN_EVENT0.ICLR = 1; //acknowledge
// UART_Out(test); test =( test+1)&0xFF;
//  GPIOA->DOUTTGL31_0 = 1;
  if(TExaSadc){
    UART_Out(ADC_In(TExaSadc)); // 8-bit data at 10 kHz
  }else{
    UART_Out((*TExaSLogic)()); // 7-bit digital data at 10 kHz
  }
}
void TExaS_Init(ADC12_Regs *adc12, uint32_t channel, uint8_t (*logic)(void)){
  TExaSadc = adc12;
  TExaSLogic = logic;
  if(TExaSadc){
    ADC_Init(adc12,channel,ADCVREF_VDDA);
    ADC_Start(adc12);
  }
  UART_Init();
  TimerG7_IntArm(Clock_Freq()/10000,1,3); // G7 is in Power domain PD2
}

