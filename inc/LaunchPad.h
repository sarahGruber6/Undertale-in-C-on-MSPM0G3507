/*!
 * @defgroup LaunchPad
 * @brief LaunchPad input/output
  <table>
<caption id="LaunchPadpins">Pins on the MSPM0G3507 LaunchPad</caption>
<tr><th>Pin <th>GPIO<th>Hardware
<tr><td>PA0 <td>output<td>red LED1,   index 0 in IOMUX PINCM table, negative logic 
<tr><td>PB22<td>output<td>BLUE LED2,  index 49 in IOMUX PINCM table
<tr><td>PB26<td>output<td>RED LED2,   index 56 in IOMUX PINCM table
<tr><td>PB27<td>output<td>GREEN LED2, index 57 in IOMUX PINCM table
<tr><td>PA18<td>input <td>S2 positive logic switch,  index 39 in IOMUX PINCM table
<tr><td>PB21<td>input <td>S3 negative logic switch,  index 48 in IOMUX PINCM table

</table>
 * @{*/
/**
 * @file      LaunchPad.h
 * @brief     Initialize LaunchPad switches and LEDs
 * @details   MSPM0G3507 LaunchPad Development Kit (LP-MSPM0G3507)<br>
 * For more information see<br> https://www.ti.com/product/LP-MSPM0G3507/part-details/LP-MSPM0G3507<br> 
 * \image html Fg01_07_02_LaunchPad.png width=500px
 * \image latex Fg01_07_02_LaunchPad.png "TI MSPM0 LaunchPad" width=10cm
 * The following is a simplified circuit diagram<br>
 * \image html LaunchPadCircuit.png width=500px
 * \image latex LaunchPadCircuit.png "LP-MSPM0G3507 Circuit Diagram" width=10cm<br>
 * - LaunchPad Jumpers
 *    - J4 Connects PA0 to red LED1
 *    - J5: Connects PB22 to blue LED2
 *    - J6: Connects PB26 to red LED2
 *    - J7: Connects PB27 to green LED2

 * @version   ECE319K v1.0
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2023 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      August 13, 2023
  <table>
<caption id="LaunchPadpins2">Pins on the MSPM0G3507 LaunchPad</caption>
<tr><th>Pin <th>GPIO<th>Hardware
<tr><td>PA0 <td>output<td>RED LED1,   index 0 in IOMUX PINCM table, negative logic 
<tr><td>PB22<td>output<td>BLUE LED2,  index 49 in IOMUX PINCM table
<tr><td>PB26<td>output<td>RED LED2,   index 56 in IOMUX PINCM table
<tr><td>PB27<td>output<td>GREEN LED2, index 57 in IOMUX PINCM table
<tr><td>PA18<td>input <td>S2 positive logic switch,  index 39 in IOMUX PINCM table
<tr><td>PB21<td>input <td>S3 negative logic switch,  index 48 in IOMUX PINCM table


</table>
Mode=0 selects the GPIO pin<br>
  <table>
<caption id="PINCM">PINCM Mode values for the MSPM0G3507</caption>
<tr><th>Name<th>Value<th>Mode=2<th>Mode=3<th>Mode=4<th>Mode=5<th>Mode=6<th>Mode=7<th>Mode=8<th>Mode=9
<tr><td>PA0INDEX<td>0<td>UART0_TX<td>I2C0_SDA<td>TIMA0_C0<td>TIMA_FAL1<td>TIMG8_C1<td>FCC_IN<td> <td> 
<tr><td>PA1INDEX<td>1<td>UART0_RX<td>I2C0_SCL<td>TIMA0_C1<td>TIMA_FAL2<td>TIMG8_IDX<td>TIMG8_C0<td> <td> 
<tr><td>PA2INDEX<td>6<td>TIMG8_C1<td>SPI0_CS0<td>TIMG7_C1<td>SPI1_CS0<td> <td> <td> <td> 
<tr><td>PA3INDEX<td>7<td>TIMG8_C0<td>SPI0_CS1<td>UART2_CTS<td>TIMA0_C2<td>COMP1_OUT<td>TIMG7_C0<td>TIMA0_C1<td>I2C1_SDA
<tr><td>PA4INDEX<td>8<td>TIMG8_C1<td>SPI0_POCI<td>UART2_RTS<td>TIMA0_C3<td>LFCLK_IN<td>TIMG7_C1<td>TIMA0_C1N<td>I2C1_SCL
<tr><td>PA5INDEX<td>9<td>TIMG8_C0<td>SPI0_PICO<td>TIMA_FAL1<td>TIMG0_C0<td>TIMG6_C0<td>FCC_IN<td> <td> 
<tr><td>PA6INDEX<td>10<td>TTIMG8_C1<td>SPI0_SCK<td>TIMA_FAL0<td>TIMG0_C1<td>HFCLK_IN<td>TIMG6_C1<td>TIMA0_C2N<td> 
<tr><td>PA7INDEX<td>13<td>COMP0_OUT<td>CLK_OUT<td>TIMG8_C0<td>TIMA0_C2<td>TIMG8_IDX<td>TIMG7_C1<td>TIMA0_C1<td> 
<tr><td>PA8INDEX<td>18<td>UART1_TX<td>SPI0_CS0<td>UART0_RTS<td>TIMA0_C0<td>TIMA1_C0N<td> <td> <td> 
<tr><td>PA9INDEX<td>19<td>UART1_RX<td>SPI0_PICO<td>UART0_CTS<td>TIMA0_C1<td>RTC_OUT<td>TIMA0_C0N<td>TIMA1_C1N<td>CLK_OUT
<tr><td>PA10INDEX<td>20<td>UART0_TX<td>SPI0_POCI<td>I2C0_SDA<td>TIMA1_C0<td>TIMG12_C0<td>TIMA0_C2<td>I2C1_SDA<td>CLK_OUT
<tr><td>PA11INDEX<td>21<td>UART0_RX<td>SPI0_SCK<td>I2C0_SCL<td>TIMA1_C1<td>COMP0_OUT<td>TIMA0_C2N<td>I2C1_SCL<td> 
<tr><td>PA12INDEX<td>33<td>UART3_CTS<td>SPI0_SCK<td>TIMG0_C0<td>CAN_TX<td>TIMA0_C3<td>FCC_IN<td> <td> 
<tr><td>PA13INDEX<td>34<td>UART3_RTS<td>SPI0_POCI<td>UART3_RX<td>TIMG0_C1<td>CAN_RX<td>TIMA0_C3N<td> <td> 
<tr><td>PA14INDEX<td>35<td>UART0_CTS<td>SPI0_PICO<td>UART3_TX<td>TIMG12_C0<td>CLK_OUT<td> <td> <td> 
<tr><td>PA15INDEX<td>36<td>UART0_RTS<td>SPI1_CS2<td>I2C1_SCL<td>TIMA1_C0<td>TIMG8_IDX<td>TIMA1_C0N<td>TIMA0_C2<td> 
<tr><td>PA16INDEX<td>37<td>COMP2_OUT<td>SPI1_POCI<td>I2C1_SDA<td>TIMA1_C1<td>TIMA1_C1N<td>TIMA0_C2N<td>FCC_IN<td> 
<tr><td>PA17INDEX<td>38<td>UART1_TX<td>SPI1_SCK<td>I2C1_SCL<td>TIMA0_C3<td>TIMG7_C0<td>TIMA1_C0<td> <td> 
<tr><td>PA18INDEX<td>39<td>UART1_RX<td>SPI1_PICO<td>I2C1_SDA<td>TIMA0_C3N<td>TIMG7_C1<td>TIMA1_C1<td> <td> 
<tr><td>PA19INDEX<td>40<td>SWDIO<td> <td> <td> <td> <td> <td> <td> 
<tr><td>PA20INDEX<td>41<td>SWCLK<td> <td> <td> <td> <td> <td> <td> 
<tr><td>PA21INDEX<td>45<td>UART2_TX<td>TIMG8_C0<td>UART1_CTS<td>TIMA0_C0<td>TIMG6_C0<td> <td> <td> 
<tr><td>PA22INDEX<td>46<td>UART2_RX<td>TIMG8_C1<td>UART1_RTS<td>TIMA0_C1<td>CLK_OUT<td>TIMA0_C0N<td>TIMG6_C1<td> 
<tr><td>PA23INDEX<td>52<td>UART2_TX<td>SPI0_CS3<td>TIMA0_C3<td>TIMG0_C0<td>UART3_CTS<td>TIMG7_C0<td>TIMG8_C0<td> 
<tr><td>PA24INDEX<td>53<td>UART2_RX<td>SPI0_CS2<td>TIMA0_C3N<td>TIMG0_C1<td>UART3_RTS<td>TIMG7_C1<td>TIMA1_C1<td> 
<tr><td>PA25INDEX<td>54<td>UART3_RX<td>SPI1_CS3<td>TIMG12_C1<td>TIMA0_C3<td>TIMA0_C1N<td> <td> <td> 
<tr><td>PA26INDEX<td>58<td>UART3_TX<td>SPI1_CS0<td>TIMG8_C0<td>TIMA_FAL0<td>CAN_TX<td>TIMG7_C0<td> <td> 
<tr><td>PA27INDEX<td>59<td>RTC_OUT<td>SPI1_CS1<td>TIMG8_C1<td>TIMA_FAL2<td>CAN_RX<td>TIMG7_C1<td> <td> 
<tr><td>PA28INDEX<td>2<td>UART0_TX<td>I2C0_SDA<td>TIMA0_C3<td>TIMA_FAL0<td>TIMG7_C0<td>TIMA1_C0<td> <td> 
<tr><td>PA29INDEX<td>3<td>I2C1_SCL<td>UART2_RTS<td>TIMG8_C0<td>TIMG6_C0<td> <td> <td> <td> 
<tr><td>PA30INDEX<td>4<td>I2C1_SDA<td>UART2_CTS<td>TIMG8_C1<td>TIMG6_C1<td> <td> <td> <td> 
<tr><td>PA31INDEX<td>5<td>UART0_RX<td>I2C0_SCL<td>TIMA0_C3N<td>TIMG12_C1<td>CLK_OUT<td>TIMG7_C1<td>TIMA1_C1<td> 
<tr><td>PB0INDEX<td>11<td>UART0_TX<td>SPI1_CS2<td>TIMA1_C0<td>TIMA0_C2<td> <td> <td> <td> 
<tr><td>PB1INDEX<td>12<td>UART0_RX<td>SPI1_CS3<td>TIMA1_C1<td>TIMA0_C2N<td> <td> <td> <td> 
<tr><td>PB2INDEX<td>14<td>UART3_TX<td>UART2_CTS<td>I2C1_SCL<td>TIMA0_C3<td>UART1_CTS<td>TIMG6_C0<td>TIMA1_C0<td> 
<tr><td>PB3INDEX<td>15<td>UART3_RX<td>UART2_RTS<td>I2C1_SDA<td>TIMA0_C3N<td>UART1_RTS<td>TIMG6_C1<td>TIMA1_C1<td> 
<tr><td>PB4INDEX<td>16<td>UART1_TX<td>UART3_CTS<td>TIMA1_C0<td>TIMA0_C2<td>TIMA1_C0N<td> <td> <td> 
<tr><td>PB5INDEX<td>17<td>UART1_RX<td>UART3_RTS<td>TIMA1_C1<td>TIMA0_C2N<td>TIMA1_C1N<td> <td> <td> 
<tr><td>PB6INDEX<td>22<td>UART1_TX<td>SPI1_CS0<td>SPI0_CS1<td>TIMG8_C0<td>UART2_CTS<td>TIMG6_C0<td>TIMA1_C0N<td> 
<tr><td>PB7INDEX<td>23<td>UART1_RX<td>SPI1_POCI<td>SPI0_CS2<td>TIMG8_C1<td>UART2_RTS<td>TIMG6_C1<td>TIMA1_C1N<td> 
<tr><td>PB8INDEX<td>24<td>UART1_CTS<td>SPI1_PICO<td>TIMA0_C0<td>COMP1_OUT<td> <td> <td> <td> 
<tr><td>PB9INDEX<td>25<td>UART1_RTS<td>SPI1_SCK<td>TIMA0_C1<td>TIMA0_C0N<td> <td> <td> <td> 
<tr><td>PB10INDEX<td>26<td>TIMG0_C0<td>TIMG8_C0<td>COMP1_OUT<td>TIMG6_C0<td> <td> <td> <td> 
<tr><td>PB11INDEX<td>27<td>TIMG0_C1<td>TIMG8_C1<td>CLK_OUT<td>TIMG6_C1<td> <td> <td> <td> 
<tr><td>PB12INDEX<td>28<td>UART3_TX<td>TIMA0_C2<td>TIMA_FAL1<td>TIMA0_C1<td> <td> <td> <td> 
<tr><td>PB13INDEX<td>29<td>UART3_RX<td>TIMA0_C3<td>TIMG12_C0<td>TIMA0_C1N<td> <td> <td> <td> 
<tr><td>PB14INDEX<td>30<td>SPI1_CS3<td>SPI1_POCI<td>SPI0_CS3<td>TIMG12_C1<td>TIMG8_IDX<td>TIMA0_C0<td> <td> 
<tr><td>PB15INDEX<td>31<td>UART2_TX<td>SPI1_PICO<td>UART3_CTS<td>TIMG8_C0<td>TIMG7_C0<td> <td> <td> 
<tr><td>PB16INDEX<td>32<td>UART2_RX<td>SPI1_SCK<td>UART3_RTS<td>TIMG8_C1<td>TIMG7_C1<td> <td> <td> 
<tr><td>PB17INDEX<td>42<td>UART2_TX<td>SPI0_PICO<td>SPI1_CS1<td>TIMA1_C0<td>TIMA0_C2<td> <td> <td> 
<tr><td>PB18INDEX<td>43<td>UART2_RX<td>SPI0_SCK<td>SPI1_CS2<td>TIMA1_C1<td>TIMA0_C2N<td> <td> <td> 
<tr><td>PB19INDEX<td>44<td>COMP2_OUT<td>SPI0_POCI<td>TIMG8_C1<td>UART0_CTS<td>TIMG7_C1<td> <td> <td> 
<tr><td>PB20INDEX<td>47<td>SPI0_CS2<td>SPI1_CS0<td>TIMA0_C2<td>TIMG12_C0<td>TIMA_FAL1<td>TIMA0_C1<td>TIMA1_C1N<td> 
<tr><td>PB21INDEX<td>48<td>SPI1_POCI<td>TIMG8_C0<td> <td> <td> <td> <td> <td> 
<tr><td>PB22INDEX<td>49<td>SPI1_PICO<td>TIMG8_C1<td> <td> <td> <td> <td> <td> 
<tr><td>PB23INDEX<td>50<td>SPI1_SCK<td>COMP0_OUT<td>TIMA_FAL0<td> <td> <td> <td> <td> 
<tr><td>PB24INDEX<td>51<td>SPI0_CS3<td>SPI0_CS1<td>TIMA0_C3<td>TIMG12_C1<td>TIMA0_C1N<td>TIMA1_C0N<td> <td> 
<tr><td>PB25INDEX<td>55<td>UART0_CTS<td>SPI0_CS0<td>TIMA_FAL2<td> <td> <td> <td> <td> 
<tr><td>PB26INDEX<td>56<td>UART0_RTS<td>SPI0_CS1<td>TIMA0_C3<td>TIMG6_C0<td>TIMA1_C0<td> <td> <td> 
<tr><td>PB27INDEX<td>57<td>COMP2_OUT<td>SPI1_CS1<td>TIMA0_C3N<td>TIMG6_C1<td>TIMA1_C1<td> <td> <td> 
</table>
  ******************************************************************************/



#ifndef __LAUNCHPAD_H__
#define __LAUNCHPAD_H__
/**
 * \brief RED1 is a constant to select red LED1 on Port A, PA0
 */
#define RED1 1
/**
 * \brief BLUE is a constant to select blue LED2 on Port B, PB22
 */
#define BLUE (1<<22)
/**
 * \brief RED is a constant to select red LED2 on Port B, PB26
 */
#define RED (1<<26)
/**
 * \brief GREEN is a constant to select green LED2 on Port B, PB27
 */
#define GREEN (1<<27)
/**
 * \brief S2 is a constant to select switch 2 on Port A, PA18
 */
#define S2 (1<<18)
/**
 * \brief S3 is a constant to select switch 3 on Port B, PB21
 */
#define S3 (1<<21)

/**
 * Initialize LEDs and switches on MSPM0G3507 LaunchPad 
 *    - PA0 output  RED LED1,  negative logic 
 *    - PB22 output BLUE LED2, positive logic
 *    - PB26 output RED LED2,  positive logic
 *    - PB27 output GREEN LED2,positive logic
 *    - PA18 input  S2 switch, positive logic 
 *    - PB21 input  S3 switch, negative logic 
 *
 * @param none
 * @return none 
 * @brief  Initialize LaunchPad
 * @note In many ECE319K example code, this function is called first and will reset and enable power to Port A and Port B
 */
void LaunchPad_Init(void);


/**
 * Read S2,  positive logic switch on PA18 
 * @param none
 * @return 0 if S2 is not pressed, 0x00040000 if S2 is pressed
 * @see LaunchPad_Init()
 * @brief  Input S2
 */
uint32_t LaunchPad_InS2(void);


/**
 * Read S3,  negative logic switch on PA18. The software converts to positive logic.
 * @param none
 * @return 0 if S3 is not pressed, 0x00200000 if S3 is pressed
 * @see LaunchPad_Init()
 * @brief  Input S2
 */
uint32_t LaunchPad_InS3(void);

/**
 * Set LED1,  negative logic LED on PA0
 *    - led=0 to PA0=1, turn off LED1
 *    - led=1 to PA0=0, turn on LED1
 *
 * @param led 1 to turn on, 0 to turn off
 * @return none
 * @brief  Output to LED1
 */
void LaunchPad_LED1(uint32_t led);


/**
 * Turn on LED1. Makes PA0=0 to turn on LED1
 * @param none
 * @return none
 * @brief  Turn on LED1
 */
void LaunchPad_LED1on(void);


/**
 * Turn off LED1. Makes PA0=1 to turn off LED
 * @param none
 * @return none
 * @brief  Turn off LED1
 */
void LaunchPad_LED1off(void);

/**
 * Set LED, 3-color positive logic LED on PB22,PB26,PB27
 *    - led=0 to turn off LED
 *    - led bit 22 sets blue color
 *    - led bit 26 sets red color
 *    - led bit 27 sets green color
 *
 * @param led sets the color of LED
 * @return none
 * @brief  Output to LED
*/
void LaunchPad_LED(uint32_t led);


/**
 * Set LED to white. Make PB22=1,PB26=1,PB27=1 to create white
 * @param none
 * @return none
 * @brief  Make LED white
*/
void LaunchPad_LEDwhite(void);


/**
 * Turn off LED. Make PB22=0,PB26=0,PB27=0 to turn off LED
 * @param none
 * @return none
 * @brief  Turn off LED
*/
void LaunchPad_LEDoff(void);

/**
 * \brief The following constants are used to index into the PINCM table
 */
//                      Mode2     Mode3     Mode4     Mode5     Mode6     Mode7     Mode8     Mode9
#define PA0INDEX   0 // UART0_TX  I2C0_SDA  TIMA0_C0  TIMA_FAL1 TIMG8_C1  FCC_IN
#define PA1INDEX   1 // UART0_RX  I2C0_SCL  TIMA0_C1  TIMA_FAL2 TIMG8_IDX TIMG8_C0
#define PA2INDEX   6 // TIMG8_C1  SPI0_CS0  TIMG7_C1  SPI1_CS0
#define PA3INDEX   7 // TIMG8_C0  SPI0_CS1  UART2_CTS TIMA0_C2  COMP1_OUT TIMG7_C0  TIMA0_C1  I2C1_SDA
#define PA4INDEX   8 // TIMG8_C1  SPI0_POCI UART2_RTS TIMA0_C3  LFCLK_IN  TIMG7_C1  TIMA0_C1N I2C1_SCL
#define PA5INDEX   9 // TIMG8_C0  SPI0_PICO TIMA_FAL1 TIMG0_C0  TIMG6_C0  FCC_IN
#define PA6INDEX  10 // TTIMG8_C1 SPI0_SCK  TIMA_FAL0 TIMG0_C1  HFCLK_IN  TIMG6_C1  TIMA0_C2N
#define PA7INDEX  13 // COMP0_OUT CLK_OUT   TIMG8_C0  TIMA0_C2  TIMG8_IDX TIMG7_C1  TIMA0_C1
#define PA8INDEX  18 // UART1_TX  SPI0_CS0  UART0_RTS TIMA0_C0  TIMA1_C0N
#define PA9INDEX  19 // UART1_RX  SPI0_PICO UART0_CTS TIMA0_C1  RTC_OUT   TIMA0_C0N TIMA1_C1N CLK_OUT
#define PA10INDEX 20 // UART0_TX  SPI0_POCI I2C0_SDA  TIMA1_C0  TIMG12_C0 TIMA0_C2  I2C1_SDA  CLK_OUT
#define PA11INDEX 21 // UART0_RX  SPI0_SCK  I2C0_SCL  TIMA1_C1  COMP0_OUT TIMA0_C2N I2C1_SCL
#define PA12INDEX 33 // UART3_CTS SPI0_SCK  TIMG0_C0  CAN_TX    TIMA0_C3  FCC_IN
#define PA13INDEX 34 // UART3_RTS SPI0_POCI UART3_RX  TIMG0_C1  CAN_RX    TIMA0_C3N
#define PA14INDEX 35 // UART0_CTS SPI0_PICO UART3_TX  TIMG12_C0 CLK_OUT
#define PA15INDEX 36 // UART0_RTS SPI1_CS2  I2C1_SCL  TIMA1_C0  TIMG8_IDX TIMA1_C0N TIMA0_C2
#define PA16INDEX 37 // COMP2_OUT SPI1_POCI I2C1_SDA  TIMA1_C1  TIMA1_C1N TIMA0_C2N FCC_IN
#define PA17INDEX 38 // UART1_TX  SPI1_SCK  I2C1_SCL  TIMA0_C3  TIMG7_C0  TIMA1_C0
#define PA18INDEX 39 // UART1_RX  SPI1_PICO I2C1_SDA  TIMA0_C3N TIMG7_C1  TIMA1_C1
#define PA19INDEX 40 // SWDIO
#define PA20INDEX 41 // SWCLK
#define PA21INDEX 45 // UART2_TX  TIMG8_C0  UART1_CTS TIMA0_C0  TIMG6_C0
#define PA22INDEX 46 // UART2_RX  TIMG8_C1  UART1_RTS TIMA0_C1  CLK_OUT   TIMA0_C0N TIMG6_C1
#define PA23INDEX 52 // UART2_TX  SPI0_CS3  TIMA0_C3  TIMG0_C0  UART3_CTS TIMG7_C0  TIMG8_C0
#define PA24INDEX 53 // UART2_RX  SPI0_CS2  TIMA0_C3N TIMG0_C1  UART3_RTS TIMG7_C1  TIMA1_C1
#define PA25INDEX 54 // UART3_RX  SPI1_CS3  TIMG12_C1 TIMA0_C3  TIMA0_C1N
#define PA26INDEX 58 // UART3_TX  SPI1_CS0  TIMG8_C0  TIMA_FAL0 CAN_TX    TIMG7_C0
#define PA27INDEX 59 // RTC_OUT   SPI1_CS1  TIMG8_C1  TIMA_FAL2 CAN_RX    TIMG7_C1
#define PA28INDEX  2 // UART0_TX  I2C0_SDA  TIMA0_C3  TIMA_FAL0 TIMG7_C0  TIMA1_C0
#define PA29INDEX  3 // I2C1_SCL  UART2_RTS TIMG8_C0  TIMG6_C0
#define PA30INDEX  4 // I2C1_SDA  UART2_CTS TIMG8_C1  TIMG6_C1
#define PA31INDEX  5 // UART0_RX  I2C0_SCL  TIMA0_C3N TIMG12_C1 CLK_OUT   TIMG7_C1  TIMA1_C1
#define PB0INDEX  11 // UART0_TX  SPI1_CS2  TIMA1_C0  TIMA0_C2
#define PB1INDEX  12 // UART0_RX  SPI1_CS3  TIMA1_C1  TIMA0_C2N
#define PB2INDEX  14 // UART3_TX  UART2_CTS I2C1_SCL  TIMA0_C3  UART1_CTS TIMG6_C0  TIMA1_C0
#define PB3INDEX  15 // UART3_RX  UART2_RTS I2C1_SDA  TIMA0_C3N UART1_RTS TIMG6_C1  TIMA1_C1
#define PB4INDEX  16 // UART1_TX  UART3_CTS TIMA1_C0  TIMA0_C2  TIMA1_C0N
#define PB5INDEX  17 // UART1_RX  UART3_RTS TIMA1_C1  TIMA0_C2N TIMA1_C1N
#define PB6INDEX  22 // UART1_TX  SPI1_CS0  SPI0_CS1  TIMG8_C0  UART2_CTS TIMG6_C0  TIMA1_C0N
#define PB7INDEX  23 // UART1_RX  SPI1_POCI SPI0_CS2  TIMG8_C1  UART2_RTS TIMG6_C1  TIMA1_C1N
#define PB8INDEX  24 // UART1_CTS SPI1_PICO TIMA0_C0  COMP1_OUT
#define PB9INDEX  25 // UART1_RTS SPI1_SCK  TIMA0_C1  TIMA0_C0N
#define PB10INDEX 26 // TIMG0_C0  TIMG8_C0  COMP1_OUT TIMG6_C0
#define PB11INDEX 27 // TIMG0_C1  TIMG8_C1  CLK_OUT   TIMG6_C1
#define PB12INDEX 28 // UART3_TX  TIMA0_C2  TIMA_FAL1 TIMA0_C1
#define PB13INDEX 29 // UART3_RX  TIMA0_C3  TIMG12_C0 TIMA0_C1N
#define PB14INDEX 30 // SPI1_CS3  SPI1_POCI SPI0_CS3  TIMG12_C1 TIMG8_IDX TIMA0_C0
#define PB15INDEX 31 // UART2_TX  SPI1_PICO UART3_CTS TIMG8_C0  TIMG7_C0
#define PB16INDEX 32 // UART2_RX  SPI1_SCK  UART3_RTS TIMG8_C1  TIMG7_C1
#define PB17INDEX 42 // UART2_TX  SPI0_PICO SPI1_CS1  TIMA1_C0  TIMA0_C2
#define PB18INDEX 43 // UART2_RX  SPI0_SCK  SPI1_CS2  TIMA1_C1  TIMA0_C2N
#define PB19INDEX 44 // COMP2_OUT SPI0_POCI TIMG8_C1  UART0_CTS TIMG7_C1
#define PB20INDEX 47 // SPI0_CS2  SPI1_CS0  TIMA0_C2  TIMG12_C0 TIMA_FAL1 TIMA0_C1  TIMA1_C1N
#define PB21INDEX 48 // SPI1_POCI TIMG8_C0
#define PB22INDEX 49 // SPI1_PICO TIMG8_C1
#define PB23INDEX 50 // SPI1_SCK  COMP0_OUT TIMA_FAL0
#define PB24INDEX 51 // SPI0_CS3  SPI0_CS1  TIMA0_C3  TIMG12_C1 TIMA0_C1N TIMA1_C0N
#define PB25INDEX 55 // UART0_CTS SPI0_CS0  TIMA_FAL2
#define PB26INDEX 56 // UART0_RTS SPI0_CS1  TIMA0_C3  TIMG6_C0  TIMA1_C0
#define PB27INDEX 57 // COMP2_OUT SPI1_CS1  TIMA0_C3N TIMG6_C1  TIMA1_C1



#endif // __LAUNCHPAD_H__

/** @}*/
