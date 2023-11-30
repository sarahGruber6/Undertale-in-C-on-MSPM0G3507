// FIFO1.c
// Runs on any microcontroller
// Provide functions that implement the Software FiFo Buffer
// Last Modified: 10/29/2023
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly
#include <stdint.h>


// Declare state variables for FiFo
//        size, buffer, put and get indexes
#define FIFO_SIZE 64
uint32_t volatile PutI;
uint32_t volatile GetI;
char static FIFO1[FIFO_SIZE];

// *********** Fifo1_Init**********
// Initializes a software FIFO1 of a
// fixed size and sets up indexes for
// put and get operations
void Fifo1_Init(){
    PutI = GetI = 0;
}

// *********** Fifo1_Put**********
// Adds an element to the FIFO1
// Input: data is character to be inserted
// Output: 1 for success, data properly saved
//         0 for failure, FIFO1 is full
uint32_t Fifo1_Put(char data){
    uint32_t newPutI = (PutI+1)&(FIFO_SIZE-1);
    if(newPutI == GetI) return 0;
    FIFO1[PutI] = data;
    PutI = newPutI;
    return 1;
}

// *********** Fifo1_Get**********
// Gets an element from the FIFO1
// Input: none
// Output: If the FIFO1 is empty return 0
//         If the FIFO1 has data, remove it, and return it
char Fifo1_Get(void){
    char data;
    if(PutI == GetI) return 0;
    data = FIFO1[GetI];
    GetI = (GetI+1)&(FIFO_SIZE-1);
    return data;
}



