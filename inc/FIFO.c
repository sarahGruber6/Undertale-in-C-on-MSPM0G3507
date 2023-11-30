// FIFO.c
// Runs on any Microcontroller
// Provide functions that initialize a FIFO, put data in, get data out,
// and return the current size.  The file includes a transmit FIFO
// using index implementation and a receive FIFO using pointer
// implementation.  Other index or pointer implementation FIFOs can be
// created using the macros supplied at the end of the file.
// Daniel Valvano
// November 18, 2022

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011
   Programs 3.7, 3.8., 3.9 and 3.10 in Section 3.7

 Copyright 2022 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
#include <ti/devices/msp/msp.h>
#include "../inc/FIFO.h"

// Two-index implementation of the transmit FIFO
// can hold 0 to TXFIFOSIZE-1 elements
uint32_t volatile TxPutI; // where to put next
uint32_t volatile TxGetI; // where to get next
char static TxFifo[TXFIFOSIZE];

void TxFifo_Init(void){
  TxPutI = TxGetI = 0; // empty
}
int TxFifo_Put(char data){
uint32_t newPutI = (TxPutI+1)&(TXFIFOSIZE-1);
  if(newPutI == TxGetI) return 0; // fail if full
  TxFifo[TxPutI] = data;          // save in Fifo
  TxPutI = newPutI;               // next place to put
  return 1;
}
char TxFifo_Get(void){char data;
  if(TxGetI == TxPutI) return 0;      // fail if empty
  data = TxFifo[TxGetI];              // retrieve data
  TxGetI = (TxGetI+1)&(TXFIFOSIZE-1); // next place to get
  return data;
}
uint32_t TxFifo_Size(void){
  return (TxPutI-TxGetI)&(TXFIFOSIZE-1);
}

// Two-index implementation of the receive FIFO
// can hold 0 to RXFIFOSIZE-1 elements
uint32_t volatile RxPutI; // where to put next
uint32_t volatile RxGetI; // where to get next
char static RxFifo[RXFIFOSIZE];

void RxFifo_Init(void){
  RxPutI = RxGetI = 0;  // empty
}
int RxFifo_Put(char data){
uint32_t newPutI = (RxPutI+1)&(RXFIFOSIZE-1);
  if(newPutI == RxGetI) return 0; // fail if full
  RxFifo[RxPutI] = data;          // save in Fifo
  RxPutI = newPutI;               // next place to put
  return 1;
}
char RxFifo_Get(void){char data;
  if(RxGetI == RxPutI) return 0;      // fail if empty
  data = RxFifo[RxGetI];              // retrieve data
  RxGetI = (RxGetI+1)&(RXFIFOSIZE-1); // next place to get
  return data;
}
uint32_t RxFifo_Size(void){
  return (RxPutI-RxGetI)&(RXFIFOSIZE-1);
}

