/*!
 * @defgroup SPI
 * @brief Synchronous serial communication
 <table>
<caption id="SPIpins">SPI-LCD pins </caption>
<tr><th>Pin <th>Function  <th>Description
<tr><td>PB9 <td>SPI1 SCLK <td>J1.7 LCD SPI clock (SPI)   
<tr><td>PB6 <td>SPI1 CS0  <td>J2.13 LCD SPI CS (SPI)     
<tr><td>PB8 <td>SPI1 PICO <td>J2.15 LCD SPI data (SPI)    
<tr><td>PB15<td>GPIO      <td>J2.17 LCD !RST =1 for run, =0 for reset   
<tr><td>PA13<td>GPIO      <td>J4.31 LCD RS =1 for data, =0 for command   
</table>
 * @{*/
/**
 * @file      SPI.h
 * @brief     Synchronous serial communication
 * @details   SPI uses a chip select, clock, data out and data in. This interface does not use data in.
 * This interface can be used for MKII LCD and the ST7735R LCD
 * \image html SPIinterface.png  width=500px
 * @version   ECE319K v1.0
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2023 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      August 13, 2023
 <table>
<caption id="SPIpins2">SPI-LCD pins </caption>
<tr><th>Pin <th>Function  <th>Description
<tr><td>PB9 <td>SPI1 SCLK <td>J1.7 LCD SPI clock (SPI)   
<tr><td>PB6 <td>SPI1 CS0  <td>J2.13 LCD SPI CS (SPI)     
<tr><td>PB8 <td>SPI1 PICO <td>J2.15 LCD SPI data (SPI)    
<tr><td>PB15<td>GPIO      <td>J2.17 LCD !RST =1 for run, =0 for reset  
<tr><td>PA13<td>GPIO      <td>J4.31 LCD RS =1 for data, =0 for command   
</table>
  ******************************************************************************/
#ifndef __SPI_H__
#define __SPI_H__


/**
 * Initialize SPI for 8 MHz baud clock
 * using busy-wait synchronization.
 * Calls Clock_Freq to get bus clock
 * - PB9 SPI1 SCLK   
 * - PB6 SPI1 CS0       
 * - PB8 SPI1 PICO  
 * - PB15 GPIO !RST =1 for run, =0 for reset    
 * - PA13 GPIO RS =1 for data, =0 for command
 *
 * @note SPI0,SPI1 in power domain PD1 SysClk equals bus CPU clock
 * @param none
 * @return none 
 * @brief initialize SPI
 */
void SPI_Init(void);

/**
 * Output 8-bit data to SPI port.
 * RS=PA13=1 for data.
 * @param data is an 8-bit data to be transferred
 * @return none 
 * @brief Output data
 */
void SPI_OutData(char data);

/**
 * Output 8-bit command to SPI port.
 * RS=PA13=0 for command
 * @param  command is an 8-bit command to be transferred
 * @return none 
 * @brief Output command
 */
void SPI_OutCommand(char command);

/**
 *  Reset LCD 
 * -# drive RST high for 500ms
 * -# drive RST low for 500ms
 * -# drive RST high for 500ms
 * 
 * @param none
 * @return none 
 * @brief Reset LCD
 */
void SPI_Reset(void);
#endif // __SPI_H__
/** @}*/