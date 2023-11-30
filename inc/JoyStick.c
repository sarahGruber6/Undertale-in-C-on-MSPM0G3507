/* JoyStick.c
 * MKII BoosterPack
 * Jonathan Valvano
 * November 21, 2022
 * Derived from gpio_toggle_output_LP_MSPM0G3507_nortos_ticlang
 *              adc12_single_conversion_vref_internal_LP_MSPM0G3507_nortos_ticlang
 *              adc12_single_conversion_LP_MSPM0G3507_nortos_ticlang
 */



#include <ti/devices/msp/msp.h>
#include "../inc/JoyStick.h"
#include "../inc/Clock.h"
#include "../inc/ADC.h"
// Analog MKII  Joystick
// J1.5 joystick Select button (digital) PA26
// J1.2 joystick horizontal (X) (analog) PA25_ADC0.2
// J3.26 joystick vertical (Y) (analog)  PA18_ADC1.3
// remove J1 J2 J3 on LaunchPad to use PA26

// user buttons
// J4.33 user Button1 (bottom) (digital) PA12
// J4.32 user Button2 (top) (digital)    PA11 (conflicts with UART0 Rx from XDS Tx)

#define PA26INDEX 58
#define PA11INDEX 21
#define PA12INDEX 33
// Initialize MKII JoyStick and JoyStick button
void JoyStick_Init(void){
  ADC_Init(ADC0,2,ADCVREF_VDDA); // x position joystick
  ADC_Init(ADC1,3,ADCVREF_VDDA); // y position joystick
  // assume these are called from LaunchPad_Init
//  GPIOA->GPRCM.RSTCTL = (uint32_t)0xB1000003;  // Reset GPIOA
//  GPIOA->GPRCM.PWREN = (uint32_t)0x26000001;   // Enable power to GPIOA
  Clock_Delay(24); // time for gpio to power up

    // PINCM
    //   bit 25 is HiZ
    //   bit 20 is drive strength
    //   bit 18 is input enable control
    //   bit 17 is pull up control
    //   bit 16 is pull down control
    //   bit 7 is PC peripheral connected, enable transparent data flow
    //   bit 0 selects GPIO function
  IOMUX->SECCFG.PINCM[PA26INDEX]  = (uint32_t) 0x00040081;
  IOMUX->SECCFG.PINCM[PA11INDEX]  = (uint32_t) 0x00040081;
  IOMUX->SECCFG.PINCM[PA12INDEX]  = (uint32_t) 0x00040081;
}
#define JOYBUTTON (1<<26)
// Read JoyStick button
// Input: none
// Output: 0 if pressed, nonzero if not pressed
uint32_t JoyStick_InButton(void){
  return GPIOA->DIN31_0 & JOYBUTTON;
}

// Read JoyStick position
// Inputs: *x pointer to empty 32 bit unsigned variable
//         *y pointer to empty 32 bit unsigned variable
// Output: none
void JoyStick_In(uint32_t *x, uint32_t *y){
  *x = ADC_In(ADC0);
  *y = ADC_In(ADC1);
}
#define BUTT1 (1<<12)
// Read Button1 button, PA12
// Input: none
// Output: 0 if pressed, nonzero if not pressed
uint32_t JoyStick_InButton1(void){
  return GPIOA->DIN31_0 & BUTT1;
}
//#define BUTT2 (1<<11)
// Read Button2 button, PA11
// Input: none
// Output: 0 if pressed, nonzero if not pressed
//uint32_t JoyStick_InButton2(void){
//  return GPIOA->DIN31_0 & BUTT2;
//}
