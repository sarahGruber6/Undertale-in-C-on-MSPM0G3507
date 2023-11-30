/**
 * @file      mainPage.h
 * @brief     Doxygen text for main page.
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2023 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      August 13, 2023
 * */

/*!
 * \mainpage ECE319K Introduction to Embedded Systems
 * August 13, 2023
 * \author Jonathan Valvano, PhD
 * \author Daniel Valvano, BS
 *
 * \section sec1 Overview
\par
An embedded system is a system that performs a specific task and has a computer embedded inside. A system is comprised of components and interfaces connected for a common purpose. This book is an introduction to embedded systems. Specific topics include the MSPM0+ microcontroller, finite-state machines, debugging, fixed-point numbers, the design of software in assembly language and C, elementary data structures, programming input/output, interrupts, measurements with analog to digital conversion, graphics, sound production with digital to analog conversion, introduction to networks using serial communication, and real-time systems.
\par
There is a web site accompanying this book http://users.ece.utexas.edu/~valvano/mspm0. Posted here are projects for each of the example programs in the book

 * \section sec2 Hardware
\par
The hardware used in these projects include
 \li MSPM0G3507 LaunchPad (LP-MSPM0G3507)
 \li Educational BoosterPack MKII (BOOSTXL-EDUMKII)
 \li LEDs, Switches, slide pot, speaker
 \li ST7735R or SSD1306 display
<br>

These project utilize the MSPM0G3507 LaunchPad.
For more information see<br> https://www.ti.com/product/LP-MSPM0G3507/part-details/LP-MSPM0G3507<br> 
 * \image html Fg01_07_02_LaunchPad.png width=500px
 * \image latex Fg01_07_02_LaunchPad.png "TI MSPM0 LaunchPad" width=10cm
 *
 *

The Educational BoosterPack MKII is simple platform with lots of I/O, shown here with the CC2650 BoosterPack.
For more information see<br>
https://www.ti.com/tool/BOOSTXL-EDUMKII<br>
 * \image html MKIIBoosterPack.png width=500px
 * \image latex MKIIBoosterPack.png "TI Educational BoosterPack MKII" width=10cm
 *


 * \section sec3 Modules
 * \par
The documentation is divided into modules.
 \li ADC contains the analog to digital converter
 \li Clock contains the MSPM0G3507 clock and timers
 \li DAC contains the digital to analog converter
 \li Debugging contains dump, TExaS scope, and TExaS logic analyzer
 \li Display contains the ST7735R and SSD1306 displays
 \li EdgeTriggered contains edge-triggered interrupts 
 \li FIFO contains software a first in first out queue
 \li MKII contains software for I2C module, plus devices on MKII

*/


