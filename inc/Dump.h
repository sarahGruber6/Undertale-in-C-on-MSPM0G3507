/**
 * @file      Dump.h
 * @brief     Debugging instruments designed in Lab 3
 * @details   Your Solution to ECE319K Lab 3<br>

 * @version   ECE319K v1.0
 * @author    Put your name here
 * @copyright Put your copyright here,
 * @warning   AS-IS
 * @date      August 13, 2023
  ******************************************************************************/
  /*!
 * @defgroup Debugging
 * @brief Debugging tools
  * @{*/
#ifndef __DUMP_H__
#define __DUMP_H__
#include <stdint.h>


/**
 * Initialize the debugging dump buffers
 * @param none
 * @return none 
 * @note  Initializes your index or pointer
 * @see Debug_Dump() Debug_Period()
 * @brief  Initialize arrays
 */
void Debug_Init(void);


/**
 * Records one data and one time into the two arrays.
 * @param data is value to store in DataBuffer
 * @return 1 for success, 0 for failure (buffers full)
 * @note  Initializes your index or pointer
 * @see Debug_Init() Debug_Period()
 * @brief  Initialize arrays
 */
uint32_t Debug_Dump(uint32_t data);


/**
 * Calculate period of the recorded data using mask
 * @param mask specifies which bit(s) to observe
 * @return period in bus cycles, 0 if there is not enough collected data to calculate period
 * @note  Period is defined as rising edge (low to high) to the next rising edge
 * @see Debug_Init() Debug_Period()
 * @brief  Initialize arrays
 */
uint32_t Debug_Period(uint32_t mask);


/**
 * Calculate duty cycle of the recorded data using mask.<br>
 * Period is defined as rising edge (low to high) to the next rising edge.<br>
 * High is defined as rising edge (low to high) to the next falling edge.<br>
 * Duty cycle is (100*High)/Period 
 * @param mask specifies which bit(s) to observe
 * @return period in bus cycles, 0 if there is not enough collected data to calculate period
 * @note  Lab 3 extra credit
 * @see Debug_Init() Debug_Period()
 * @brief  Initialize arrays
 */
uint32_t Debug_Duty(uint32_t mask);

#endif
/** @}*/

