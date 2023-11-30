/* UARTints.c
 * Jonathan Valvano
 * November 18, 2022
 * Derived from uart_rw_multibyte_fifo_poll_LP_MSPM0G3507_nortos_ticlang
 *              uart_echo_interrupts_standby_LP_MSPM0G3507_nortos_ticlang
 * PA.10 UART0 Tx to XDS Rx
 * PA.11 UART0 Rx from XDS Tx
 * Insert jumper J25: Connects PA10 to XDS_UART
 * Insert jumper J26: Connects PA11 to XDS_UART
 */


#include <ti/devices/msp/msp.h>
#include "../inc/UART.h"
#include "../inc/Clock.h"
#include "../inc/FIFO.h"
#define PA10INDEX 20
#define PA11INDEX 21
// power Domain PD0
// for 32MHz bus clock, bus clock is 32MHz
// for 40MHz bus clock, bus clock is ULPCLK 20MHz
// for 80MHz bus clock, bus clock is ULPCLK 40MHz
// assume 40MHz bus clock, bus clock = 20MHz
// initialize UART for 115200 baud rate
// interrupt synchronization
void UART_Init(void){
    // RSTCLR to GPIOA and UART0 peripherals
    //   bits 31-24 unlock key 0xB1
    //   bit 1 is Clear reset sticky bit
    //   bit 0 is reset gpio port
 // GPIOA->GPRCM.RSTCTL = (uint32_t)0xB1000003; // called previously
  UART0->GPRCM.RSTCTL = 0xB1000003;
    // Enable power to GPIOA and UART0 peripherals
    // PWREN
    //   bits 31-24 unlock key 0x26
    //   bit 0 is Enable Power
 // GPIOA->GPRCM.PWREN = (uint32_t)0x26000001; // called previously
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
  TxFifo_Init();
  RxFifo_Init();
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
  // 20000000/16 = 1,250,000 Hz
 // Baud = 115200
  //   1,250,000/115200 = 10.850694
  //   divider = 10+54/64 = 10.84375
  UART0->IBRD = 10;
  UART0->FBRD = 54; // baud =1,250,000/10.84375 = 115,274 bps
  UART0->LCRH = 0x00000030;
   // bits 5-4 WLEN=11 8 bits
   // bit  3   STP2=0  1 stop
   // bit  2   EPS=0   parity select
   // bit  1   PEN=0   no parity
   // bit  0   BRK=0   no break
  UART0->CPU_INT.IMASK = 0x0C01;
  // bit 11 TXINT
  // bit 10 RXINT
  // bit 0  Receive timeout
  UART0->IFLS = 0x0422;
  // bits 11-8 RXTOSEL receiver timeout select 4 (0xF highest)
  // bits 6-4  RXIFLSEL 2 is greater than or equal to half
  // bits 2-0  TXIFLSEL 2 is less than or equal to half
  NVIC->ICPR[0] = 1<<15; // UART0 is IRQ 15
  NVIC->ISER[0] = 1<<15;
  NVIC->IP[3] = (NVIC->IP[3]&(~0xFF000000))|(2<<30);    // set priority (bits 31,30) IRQ 15
  UART0->CTL0 |= 0x01; // enable UART0
}
// copy from hardware RX FIFO to software RX FIFO
// stop when hardware RX FIFO is empty or software RX FIFO is full
void static copyHardwareToSoftware(void){
  char letter;
  while(((UART0->STAT&0x04) == 0) && (RxFifo_Size() < (RXFIFOSIZE - 1))){
    letter = UART0->RXDATA;
    RxFifo_Put(letter);
  }
}

//------------UART_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
char UART_InChar(void){
  char letter;
  do{
    letter = RxFifo_Get();
  }while(letter==0);
  return(letter);
}
// copy from software TX FIFO to hardware TX FIFO
// stop when software TX FIFO is empty or hardware TX FIFO is full
void static copySoftwareToHardware(void){
  char letter;
  while(((UART0->STAT&0x80) == 0) && (TxFifo_Size() > 0)){
    letter = TxFifo_Get();
    UART0->TXDATA = letter;
  }
}
//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(char data){
    while(TxFifo_Put(data) == 0){};
    UART0->CPU_INT.IMASK &= ~0x0800;   // disarm TX FIFO interrupt
    copySoftwareToHardware();
    UART0->CPU_INT.IMASK |= 0x0800;    // rearm TX FIFO interrupt
  }

void UART0_IRQHandler(void){ uint32_t status;
  status = UART0->CPU_INT.IIDX; // reading clears bit in RIS
  if(status == 0x01){   // 0x01 receive timeout
    copyHardwareToSoftware();
  }else if(status == 0x0B){ // 0x0B receive
    copyHardwareToSoftware();
  }else if(status == 0x0C){ // 0x0C transmit
    copySoftwareToHardware();
    if(TxFifo_Size() == 0){             // software TX FIFO is empty
      UART0->CPU_INT.IMASK &= ~0x0800;    // disable TX FIFO interrupt
    }
  }
}

