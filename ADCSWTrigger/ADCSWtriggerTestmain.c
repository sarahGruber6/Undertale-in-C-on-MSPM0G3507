/* ADCSWtriggerTestmain.c
 * Jonathan Valvano
 * November 21, 2022
 * Derived from adc12_single_conversion_vref_internal_LP_MSPM0G3507_nortos_ticlang
 *              adc12_single_conversion_LP_MSPM0G3507_nortos_ticlang
 */
// Educational BoosterPack MKII (BOOSTXL-EDUMKII)
// Analog MKII  Joystick
// J1.5 joystick Select button (digital) PA26
// J1.2 joystick horizontal (X) (analog) PA25_ADC0.2
// J3.26 joystick vertical (Y) (analog)  PA18_ADC1.3

// MKII accelerometer
// J3.23 accelerometer X (analog)    PB19__ADC1.6
// J3.24 accelerometer Y (analog)    PA22__ADC0.7
// J3.25 accelerometer Z (analog)    PB18__ADC1.5

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/JoyStick.h"
#include "../inc/ADC.h"
//  PA0 is red LED1,   index 0 in IOMUX PINCM table
// PB22 is BLUE LED2,  index 49 in IOMUX PINCM table
// PB26 is RED LED2,   index 56 in IOMUX PINCM table
// PB27 is GREEN LED2, index 57 in IOMUX PINCM table
// PA18 is S2 positive logic switch,  index 39 in IOMUX PINCM table
// PB21 is S3 negative logic switch,  index 48 in IOMUX PINCM table
uint32_t XData,YData,ZData, Button,Button1;
uint16_t volt; // voltage in mV
int main(void){
  Clock_Init40MHz();
  LaunchPad_Init();
  ADC0_Init(7,ADCVREF_VDDA); //accelerometer Y (analog)
  while(1){    /* toggle on sample */
    Clock_Delay(10000000);
    YData = ADC0_In();
    volt = (YData*3300)>>12;
    GPIOA->DOUT31_0 ^= RED1;
  }
}
int main2(void){
  Clock_Init40MHz();
  LaunchPad_Init();
  ADC_InitDual(ADC1,6,5,ADCVREF_VDDA); //accelerometer X,Z (analog)
  while(1){    /* toggle on sample */
    Clock_Delay(10000000);
    ADC_InDual(ADC1,&XData,&ZData);

    GPIOA->DOUT31_0 ^= RED1;
  }
}

int main3(void){
  Clock_Init40MHz();
  LaunchPad_Init();
  JoyStick_Init();

  while(1){    /* toggle on sample */
    Clock_Delay(10000000);
    JoyStick_In(&XData,&YData);
    GPIOA->DOUT31_0 ^= RED1;
    Button = JoyStick_InButton();
    Button1 = JoyStick_InButton1();
    if(Button == 0){ // negative logic
      GPIOB->DOUT31_0 |= BLUE;
    }else{
      GPIOB->DOUT31_0 &= ~BLUE;
    }
  }
}


uint32_t elapsed,n;
#define SIZE 16
uint32_t Histogram[SIZE]; // probability mass function
int main4(void){uint32_t start,end,i;
  int32_t d,Center,Data;
  Clock_Init40MHz();

  LaunchPad_Init();
//  ADC0_Init(7,ADCVREF_VDDA); //accelerometer Y (analog)
  ADC0_InitAve(7,0); //accelerometer Y (analog)
  SysTick->CTRL = 0;           // 1) disable SysTick during setup
  SysTick->LOAD = 0xFFFFFF;    // 2) max
  SysTick->VAL = 0;            // 3) any write to current clears it
  SysTick->CTRL = 0x00000005;  // 4) enable SysTick with core clock
  while(1){    /* toggle on sample */
    for(n=0; n<=7; n++){
      ADC0_InitAve(7,n); //accelerometer Y (analog)
      Center = ADC0_In();
      Clock_Delay(1000);
      start = SysTick->VAL;
      Center = ADC0_In();
      end = SysTick->VAL;
      elapsed = ((start-end)&0xFFFFFF)-13;
      for(i=0; i<SIZE; i++) Histogram[i] = 0; // clear
      Center = 0;
      for(i=0; i<1000; i++){
        Clock_Delay(1000);
        Center += ADC0_In();
      }
      Center = Center/1000;
      for(i=0; i<1000; i++){
        Clock_Delay(1000);
        Data = ADC0_In();
        if(Data<Center-SIZE/2){
          Histogram[0]++;
        }else if(Data>=Center+SIZE/2){
          Histogram[SIZE-1]++;
        }else{
          d = Data-Center+SIZE/2;
          Histogram[d]++;
        }
      }
    GPIOA->DOUT31_0 ^= RED1;
    }
  }
}
