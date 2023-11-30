/*
 * Switch.c
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"

#define Button1 (1<<12)
#define Button2 (1<<17)
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){
    IOMUX->SECCFG.PINCM[PB12INDEX] = 0x00040081;    // button 1
    IOMUX->SECCFG.PINCM[PB17INDEX] = 0x00040081;    // button 2
    IOMUX->SECCFG.PINCM[PB18INDEX] = 0x00040081;    // button 2
}
// return current state of switches
uint32_t Switch_In(void){
    uint32_t sensors = 0;
    if((GPIOB->DIN31_0 & (1U << 12)) != 0) { // button 1
        sensors |= 0x01;  // set bit 0
    }
    if((GPIOB->DIN31_0 & (1U << 17)) != 0) { // button 2
        sensors |= 0x02;  // set bit 1
    }
    if((GPIOB->DIN31_0 & (1U << 18)) != 0) { // button 2
        sensors |= 0x04;  // set bit 2
    }
    return sensors;
}
