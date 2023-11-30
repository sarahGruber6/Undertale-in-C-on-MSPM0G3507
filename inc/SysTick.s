//****************** SysTick.s ***************
// Busywait time delay
// Author: Jonathan Valvano
// Last Modified: May 12, 2023
// Brief description of the program:
// Wait functions based on SysTick.
       .data
       .align 2
// no global variables
       .section .text
       .thumb
       .align 2
// SysTick addresses
   .equ SysTick_CTRL,  0xE000E010
   .equ SysTick_LOAD,  0xE000E014
   .equ SysTick_VAL,   0xE000E018
    .global SysTickInit
    .global SysTickWait
    .global SysTickWait10ms
// Initialize
SysTickInit:
    LDR  R2, =SysTick_CTRL
    MOVS R0, #0
    STR  R0, [R2]   // disable SysTick during setup
    LDR  R1, =SysTick_LOAD
    STR  R0, [R1]
    LDR  R1, =SysTick_VAL
    STR  R0, [R1]    // any write to VAL clears it
    MOVS R0, #0x05
    STR  R0, [R2]    // ENABLE and CLK_SRC bits set
    BX   LR
//------------SysTickWait------------
// Time delay using busy wait.
// Input: R0  delay parameter in units of the core clock (12.5 nsec)
// Output: none
SysTickWait:
    LDR  R1, =SysTick_LOAD
    SUBS R0, #1
    STR  R0, [R1]        // delay-1, number of counts to wait
    LDR  R1, =SysTick_VAL
    STR  R0, [R1]        // any write clears counter
    LDR  R1, =SysTick_CTRL
    LDR  R2, =0x00010000
SysTickWaitloop:
    LDR  R3, [R1]
    ANDS R3, R3, R2      // Count set?
    BEQ  SysTickWaitloop
    BX   LR
//------------SysTickWait10ms------------
// Time delay using busy wait. This assumes 32 MHz clock
// Input: R0  number of times to wait 10 ms before returning
// Output: none		 ; Modifies: R0
    .equ DELAY10MS,320000    // 10ms (assumes 32 MHz clock)
SysTickWait10ms:
    PUSH {R4, LR}            // save current value of R4 and LR
    MOVS R4, R0              // R4 = R0 = remainingWaits
    BEQ SysTickWait10msdone  // R4 == 0, done
SysTickWait10msloop:
    LDR R0, =DELAY10MS       // R0 = DELAY10MS
    BL  SysTickWait          // wait 10 ms
    SUBS R4, R4, #1          // R4 = R4 - 1; remainingWaits--
    BHI SysTickWait10msloop  // if(R4 > 0), wait another 10 ms
SysTickWait10msdone:
    POP {R4, PC}
