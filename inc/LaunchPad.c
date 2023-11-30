/* LaunchPad.c
 * Jonathan Valvano
 * July 5, 2023
 * Derived from gpio_toggle_output_LP_MSPM0G3507_nortos_ticlang
 */


#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
//  PA0 is red LED1,   index 0 in IOMUX PINCM table, negative logic
// PB22 is BLUE LED2,  index 49 in IOMUX PINCM table
// PB26 is RED LED2,   index 56 in IOMUX PINCM table
// PB27 is GREEN LED2, index 57 in IOMUX PINCM table
// PA18 is S2 positive logic switch,  index 39 in IOMUX PINCM table
// PB21 is S3 negative logic switch,  index 48 in IOMUX PINCM table
void ActivatePortA_B(void){
    // Reset two GPIO peripherals
    // RSTCLR
    //   bits 31-24 unlock key 0xB1
    //   bit 1 is Clear reset sticky bit
    //   bit 0 is reset gpio port
  GPIOA->GPRCM.RSTCTL = (uint32_t)0xB1000003;
  GPIOB->GPRCM.RSTCTL = (uint32_t)0xB1000003;

    // Enable power to two GPIO peripherals
    // PWREN
    //   bits 31-24 unlock key 0x26
    //   bit 0 is Enable Power
  GPIOA->GPRCM.PWREN = (uint32_t)0x26000001;
  GPIOB->GPRCM.PWREN = (uint32_t)0x26000001;

  Clock_Delay(24); // time for gpio to power up
}
// Initialize LEDs and switches
// Input: none
// Output:none
void LaunchPad_Init(void){
  ActivatePortA_B();

// PINCM
//   bit 25 is HiZ
//   bit 20 is drive strength
//   bit 18 is input enable control
//   bit 17 is pull up control
//   bit 16 is pull down control
//   bit 7 is PC peripheral connected, enable transparent data flow
//   bit 0 selects GPIO function
  IOMUX->SECCFG.PINCM[PA0INDEX]  = (uint32_t) 0x00000081;
  IOMUX->SECCFG.PINCM[PA18INDEX] = (uint32_t) 0x00050081; // input, pull down
  IOMUX->SECCFG.PINCM[PB21INDEX] = (uint32_t) 0x00060081; // input, pull up
  IOMUX->SECCFG.PINCM[PB22INDEX] = (uint32_t) 0x00100081;
  IOMUX->SECCFG.PINCM[PB26INDEX] = (uint32_t) 0x00100081;
  IOMUX->SECCFG.PINCM[PB27INDEX] = (uint32_t) 0x00100081;
// DOE31_0 Data output enable
  GPIOA->DOE31_0 |= RED1;
  GPIOB->DOE31_0 |= BLUE | RED | GREEN; // enable outputs
// DOUT31_0 read/write data output pins
  GPIOA->DOUT31_0 &= ~RED1; // LED1 off
  GPIOB->DOUT31_0 &= ~(BLUE | RED | GREEN); // clear LED2
}
// **** LaunchPad_InS2 *****
// Read S2,  positive logic switch on PA18
// return 0 if S2 is not pressed
//        0x00040000 if S2 is pressed
uint32_t LaunchPad_InS2(void){ uint32_t data;
  data = GPIOA->DIN31_0; // read all of Port A
  data = data & S2;      // select bit 18
  return data;
}
// **** LaunchPad_InS3 *****
// Read S3,  negative logic switch on PB21
// return 0 if S3 is not pressed
//        0x00200000 if S3 is pressed
uint32_t LaunchPad_InS3(void){ uint32_t data;
  data = GPIOB->DIN31_0; // read all of Port B
  data = data ^ S3;      // convert to positive logic
  return data & S3;      // select bit 31
}
// **** LaunchPad_LED1 *****
// Set LED1,  negative logic LED on PA0
// Input led=0 to PA0=1, turn off LED1
//       led=1 to PA0=0, turn on LED1
void LaunchPad_LED1(uint32_t led){uint32_t data;
  data = GPIOA->DOUT31_0; // read all of previous values
  data = data & ~0x01;    // clear bit 0
  data = data | (led^1);  // new value for bit 0
  GPIOA->DOUT31_0 = data; // change bit 0
}
// **** LaunchPad_LED1on *****
// Set LED1 on PA0, negative logic
// Makes PA0=0 to turn on LED
void LaunchPad_LED1on(void){
  GPIOA->DOUTCLR31_0 = RED1;
}
// **** LaunchPad_LED1off *****
// Set LED1 on PA0, negative logic
// Makes PA0=1 to turn off LED
void LaunchPad_LED1off(void){
  GPIOA->DOUTSET31_0 = RED1;
}
// **** LaunchPad_LED *****
// Set LED, 3-color positive logic LED on PB22,PB26,PB27
// Input led=0 to turn off LED
//       led bit 22 sets blue color
//       led bit 26 sets red color
//       led bit 27 sets green color
void LaunchPad_LED(uint32_t led){uint32_t data;
  data = GPIOB->DOUT31_0; // read all of previous values
  data = data & ~(BLUE|RED|GREEN); // clear bits 22,26,27
  data = data | led;      // new values for bits 22,26,27
  GPIOB->DOUT31_0 = data; // change bits 22,26,27
}
// **** LaunchPad_LEDwhite *****
// Set LED to white, 3-color positive logic LED on PB22,PB26,PB27
void LaunchPad_LEDwhite(void){
  GPIOB->DOUTSET31_0 = BLUE|RED|GREEN;
}
// **** LEDoff *****
// turn off LED, 3-color positive logic LED on PB22,PB26,PB27
void LaunchPad_LEDoff(void){
  GPIOB->DOUTCLR31_0 = BLUE|RED|GREEN;
}

