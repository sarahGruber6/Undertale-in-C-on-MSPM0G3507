/* SPI.c
 * Jonathan Valvano
 * November 21, 2022
 * Derived from uart_rw_multibyte_fifo_poll_LP_MSPM0G3507_nortos_ticlang
SPI MKII LCD
J1.7  SPI1 SCLK  PB9_SPI1-SCK
J2.13 SPI1 CS0   PB6_SPI1-CS0
J2.15 SPI1 PICO  PB8_SPI1-PICO
J2.17 LCD !RST   PB15
J4.31 LCD RS     PA13
 */


#include <ti/devices/msp/msp.h>
#include "../inc/SPI.h"
#include "../inc/Clock.h"
#define PB9INDEX 25
#define PB6INDEX 22
#define PB8INDEX 24
#define PB15INDEX 31
#define PA13INDEX 34
// calls Clock_Freq to get bus clock
// initialize SPI for 8 MHz baud clock
// busy-wait synchronization
// SPI0,SPI1 in power domain PD1 SysClk equals bus CPU clock
void SPI_Init(void){uint32_t busfreq =  Clock_Freq();
    // assumes GPIOA and GPIOB are reset and powered previously
    // RSTCLR to SPI1 peripherals
    //   bits 31-24 unlock key 0xB1
    //   bit 1 is Clear reset sticky bit
    //   bit 0 is reset gpio port
  SPI1->GPRCM.RSTCTL = 0xB1000003;
    // Enable power to SPI1 peripherals
    // PWREN
    //   bits 31-24 unlock key 0x26
    //   bit 0 is Enable Power
  SPI1->GPRCM.PWREN = 0x26000001;
  // configure PB9 PB6 PB8 as alternate SPI1 function
  IOMUX->SECCFG.PINCM[PB9INDEX]  = 0x00000083;  // SPI1 SCLK
  IOMUX->SECCFG.PINCM[PB6INDEX]  = 0x00000083;  // SPI1 CS0
  IOMUX->SECCFG.PINCM[PB8INDEX]  = 0x00000083;  // SPI1 PICO
  IOMUX->SECCFG.PINCM[PB15INDEX] = 0x00000081;  // GPIO output, LCD !RST
  IOMUX->SECCFG.PINCM[PA13INDEX] = 0x00000081;  // GPIO output, LCD RS
  Clock_Delay(24); // time for gpio to power up
  GPIOA->DOE31_0 |= 1<<13;    // PA13 is LCD RS
  GPIOB->DOE31_0 |= 1<<15;    // PB15 is LCD !RST
  GPIOA->DOUTSET31_0 = 1<<13; // RS=1
  GPIOB->DOUTSET31_0 = 1<<15; // !RST = 1
  SPI1->CLKSEL = 8; // SYSCLK
// bit 3 SYSCLK
// bit 2 MFCLK
// bit 1 LFCLK
   SPI1->CLKDIV = 0; // divide by 1
// bits 2-0 n (0 to 7), divide by n+1
//Set the bit rate clock divider to generate the serial output clock
//     outputBitRate = (spiInputClock) / ((1 + SCR) * 2)
//     8,000,000 = (16,000,000)/((0 + 1) * 2)
//     8,000,000 = (32,000,000)/((1 + 1) * 2)
//     6,666,667 = (40,000,000)/((2 + 1) * 2)
//     8,000,000 = (80,000,000)/((4 + 1) * 2)
//     8,000,000 = (Clock_Freq)/((m + 1) * 2)
//     m = (Clock_Freq/16000000) - 1
   if(busfreq <= 16000000){
     SPI1->CLKCTL = 0; // frequency= busfreq/2
   }else if(busfreq == 4000000){
     SPI1->CLKCTL = 2; // frequency= 6.66MHz
   }else{
     SPI1->CLKCTL = busfreq/16000000 -1; // 8 MHz
   }
   SPI1->CTL0 = 0x0027;
// bit 14 CSCLR=0 not cleared
// bits 13-12 CSSEL=0 CS0
// bit 9 SPH = 0
// bit 8 SPO = 0
// bits 6-5 FRF = 01 (4 wire)
// bits 4-0 n=7, data size is n+1 (8bit data)
  SPI1->CTL1 = 0x0015;
// bits 29-24 RXTIMEOUT=0
// bits 23-16 REPEATX=0 disabled
// bits 15-12 CDMODE=0 manual
// bit 11 CDENABLE=0 CS3
// bit 7-5 =0 no parity
// bit 4=1 MSB first
// bit 3=0 POD (not used, not peripheral)
// bit 2=1 CP controller mode
// bit 1=0 LBM disable loop back
// bit 0=1 enable SPI
  SPI_Reset();
}
/* STAT register
 * 4 BUSY 0h = SPI is in idle mode. 1h = SPI is currently transmitting and/or receiving data, or transmit FIFO is not empty.
   3 RNF Receive FIFO not full 0h = Receive FIFO is full. 1h = Receive FIFO is not full.
   2 RFE Receive FIFO empty. 0h = Receive FIFO is not empty. 1h = Receive FIFO is empty.
   1 TNF Transmit FIFO not full 0h = Transmit FIFO is full. 1h = Transmit FIFO is not full.
   0 TFE Transmit FIFO empty. 0h = Transmit FIFO is not empty. 1h = Transmit FIFO is empty.*/

//---------SPI_OutData------------
// Output 8-bit data to SPI port
// Input: data is an 8-bit data to be transferred
// Output: none
void SPI_OutData(char data){
  while((SPI1->STAT&0x02) == 0x00){}; // spin if TxFifo full
  GPIOA->DOUTSET31_0 = 1<<13;         // RS=PA13=1 for data
  SPI1->TXDATA = data;
}
 //---------SPI_OutCommand------------
 // Output 8-bit command to SPI port
 // Input: data is an 8-bit data to be transferred
 // Output: none
 void SPI_OutCommand(char command){
   while((SPI1->STAT&0x10) == 0x10){}; // spin if SPI busy
   GPIOA->DOUTCLR31_0 = 1<<13;         // RS=PA13=0 for command
   SPI1->TXDATA = command;
   while((SPI1->STAT&0x10) == 0x10){}; // spin if SPI busy
 }

 //---------SPI_Reset------------
 // Reset LCD
 // Input: none
 // Output: none
 // at 48 MHz
 void SPI_Reset(void){
   GPIOB->DOUTSET31_0 = 1<<15; // PB15=!RST=1
   Clock_Delay1ms(500);        // 500ms (calibrated with logic analyzer)
   GPIOB->DOUTCLR31_0 = 1<<15; // PB15=!RST=0
   Clock_Delay1ms(500);        // 500ms
   GPIOB->DOUTSET31_0 = 1<<15; // PB15=!RST=1
   Clock_Delay1ms(500);        // 500ms
 }

