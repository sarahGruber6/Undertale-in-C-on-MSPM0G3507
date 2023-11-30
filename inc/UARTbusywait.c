/* UARTbusywait.c
 * Jonathan Valvano
 * November 18, 2022
 * Derived from uart_rw_multibyte_fifo_poll_LP_MSPM0G3507_nortos_ticlang
 * PA.10 UART0 Tx to XDS Rx
 * PA.11 UART0 Rx from XDS Tx
 * Insert jumper J25: Connects PA10 to XDS_UART
 * Insert jumper J26: Connects PA11 to XDS_UART
 */


#include <ti/devices/msp/msp.h>
#include "../inc/UART.h"
#include "../inc/Clock.h"
#define PA10INDEX 20
#define PA11INDEX 21
// power Domain PD0
// for 32MHz bus clock, bus clock is 32MHz
// for 40MHz bus clock, bus clock is ULPCLK 20MHz
// for 80MHz bus clock, bus clock is ULPCLK 40MHz
// calls Clock_Freq to get bus clock
// initialize UART for 115200 baud rate
// busy-wait synchronization
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
//  GPIOA->GPRCM.PWREN = 0x26000001; // called previously
  UART0->GPRCM.PWREN = 0x26000001;
  Clock_Delay(24); // time for uart to power up
  // configure PA11 PA10 as alternate UART0 function
  IOMUX->SECCFG.PINCM[PA10INDEX]  = 0x00000082;
  //bit 7  PC connected
  //bits 5-0=2 for UART0_Tx
  IOMUX->SECCFG.PINCM[PA11INDEX]  = 0x00040082;
  //bit 18 INENA input enable
  //bit 7  PC connected
  //bits 5-0=2 for UART0_Rx
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
  /* old
  // 20000000/16 = 1,250,000 Hz
 // Baud = 115200
  //   1,250,000/115200 = 10.850694
  //   divider = 10+54/64 = 10.84375
  UART0->IBRD = 10;
  UART0->FBRD = 54; // baud =1,250,000/10.84375 = 115,274
  */
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
