// Dump.c
// Your solution to ECE319K Lab 3
// Author: Your name
// Last Modified: Your date

#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/Timer.h"
#define MAXBUF 50
uint32_t DataBuffer[MAXBUF];
uint32_t TimeBuffer[MAXBUF];
uint32_t DebugCnt; // 0 to 50 (0 is empty, 50 is full)

// *****Debug_Init******
// Initializes your index or pointer.
// Input: none
// Output:none
void Debug_Init(void){
// This function should also initialize Timer G12, call TimerG12_Init.
  DebugCnt = 0; // 0 is empty
  TimerG12_Init();
}

// *****Debug_Dump******
// Records one data and one time into the two arrays.
// Input: data is value to store in DataBuffer
// Output: 1 for success, 0 for failure (buffers full)
uint32_t Debug_Dump(uint32_t data){
// The software simply reads TIMG12->COUNTERREGS.CTR to get the current time in bus cycles.
  if(DebugCnt >= MAXBUF) return 0; // failure
  DataBuffer[DebugCnt] = data;
  TimeBuffer[DebugCnt] = TIMG12->COUNTERREGS.CTR;
  DebugCnt++;
  return 1; // success
}

// *****Debug_Period******
// Calculate period of the recorded data using mask
// Input: mask specifies which bit(s) to observe
// Output: period in bus cycles
// Period is defined as rising edge (low to high) to the next rising edge.
// Return 0 if there is not enough collected data to calculate period .
uint32_t Debug_Period(uint32_t mask){
// This function should not alter the recorded data.
// AND each recorded data with mask,
//    if nonzero the signal is considered high.
//    if zero, the signal is considered low.
  uint32_t num=0; // number of period measurements (need at least 1)
  uint32_t i=0;   // search index
  uint32_t lastTime=0;
  uint32_t p;
  uint32_t sum = 0;
  uint32_t first = 0; // have not found any rising edges yet
  uint32_t flag=0;  // search for 0
  while(i<DebugCnt){
    if(flag == 0){  // searching for 0
      if((DataBuffer[i]&mask) == 0){
        flag = 1; // look for 1
      }
    }else{ // searching for 1
      if((DataBuffer[i]&mask) != 0){
        if(first){ // not first edge
          p = lastTime - TimeBuffer[i];
          sum = sum+p;
          num++;
        }
        first = 1; // found at least one rising edge
        lastTime =  TimeBuffer[i];
        flag = 0; // look for 0
      }
    }
    i++;
  }
  if(num == 0){
    return 0;
  }
  return sum/num; // average period
}

// Lab 3 extra credit
// *****Debug_Duty******
// Calculate duty cycle of the recorded data using mask
// Input: mask specifies which bit(s) to observe
// Output: period in percent (0 to 100)
// Period is defined as rising edge (low to high) to the next rising edge.
// High is defined as rising edge (low to high) to the next falling edge.
// Duty cycle is (100*High)/Period
// Return 0 if there is not enough collected data to calculate duty cycle.
uint32_t Debug_Duty(uint32_t mask){
  return 0;
}

// Lab2 specific debugging code
uint32_t Theperiod;
void Dump(void){
  uint32_t out = GPIOB->DOUT31_0&0x0070000; // PB18-PB16 outputs
  uint32_t in = GPIOB->DIN31_0&0x0F;        // PB3-PB0 inputs
  uint32_t data = out|in;                   // PB18-PB16, PB3-PB0
  uint32_t result = Debug_Dump(data);       // calls your Lab3 function
  if(result == 0){ // 0 means full
    Theperiod = Debug_Period(1<<18);        // calls your Lab3 function
   __asm volatile("bkpt; \n"); // breakpoint here
// observe Theperiod
  }
}




