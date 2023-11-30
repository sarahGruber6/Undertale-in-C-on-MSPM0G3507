/*!
 * @defgroup MKII
 * @brief I2C and MKII boosterpack sensors
 <table>
<caption id="MKIIsensorpins">MKII sensor pins </caption>
<tr><th>Pin <th>Function<th>Description
<tr><td>PA27<td>GPIO    <td>J1.8 ambient light (OPT3001) interrupt (digital)  
<tr><td>PB2 <td>I2C1-SCL<td>J1.9 ambient light (OPT3001) and temperature sensor (TMP006)    
<tr><td>PB3 <td>I2C1-SDA<td>J1.10 ambient light (OPT3001) and temperature sensor (TMP006)    
<tr><td>PB16<td>GPIO    <td>J2.11 temperature sensor (TMP006) interrupt (digital)   
</table>
 * @{*/
/**
 * @file      I2C.h
 * @brief     Software driver functions for I2C and MKII boosterpack
 * @details   Light and temperature sensors on MKII<br>
 * https://www.ti.com/tool/BOOSTXL-EDUMKII <br>
 * The TMP006 was discontinued in 2017. Therefore, it is no longer populated on the board.
 * Documentation of the TMP006 is unchanged in this user's guide to accommodate users who still
 * have the device populated on the board<br>

 * @version   ECE319K v1.0
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2023 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      August 14, 2023
 <table>
<caption id="MKIIsensorpins2">MKII sensor pins </caption>
<tr><th>Pin <th>Function<th>Description
<tr><td>PA27<td>GPIO    <td>J1.8 ambient light (OPT3001) interrupt (digital)  
<tr><td>PB2 <td>I2C1-SCL<td>J1.9 ambient light (OPT3001) and temperature sensor (TMP006)    
<tr><td>PB3 <td>I2C1-SDA<td>J1.10 ambient light (OPT3001) and temperature sensor (TMP006)    
<tr><td>PB16<td>GPIO    <td>J2.11 temperature sensor (TMP006) interrupt (digital)   
</table>
  ******************************************************************************/


#ifndef __I2C_H__
#define __I2C_H__


/**
 * Initialize PB2 and PB3 for I2C for 400kHz clock.
 * Uses busy-wait synchronization 
 * @param none
 * @return none 
 * @brief  Initialize I2C
 * @note Assumes 80MHz bus clock
 * @note Assumes GPIOB have been reset and powered previously
 */
void I2C_Init(void);


/**
 * Receive one byte from specified slave.
 * Uses busy-wait synchronization <br>
 * <b>[send address][read data]</b>
 * @param slave is slave address
 * @return 8-bit data received 
 * @brief  Receive one byte
 * @note Does not check for errors
 */
uint8_t I2C_Recv1(int8_t slave);


/**
 * Receive two bytes from specified slave.
 * Uses busy-wait synchronization <br>
 * <b>[send address][read data][read data]</b>
 * @param slave is slave address
 * @return 16-bit data received 
 * @brief  Receive two bytes
 * @note Does not check for errors
 */
uint16_t I2C_Recv2(int8_t slave);



/**
 * Send one byte to specified slave.
 * Uses busy-wait synchronization <br>
 * <b>[send address][send data]</b>
 * @param slave is slave address
 * @param data is byte to be sent
 * @return 0 if successful, nonzero if error 
 * @brief  Send one byte
 * @note Does check for errors
 */
int I2C_Send1(int8_t slave, uint8_t data);


/**
 * Send two bytes to specified slave.
 * Uses busy-wait synchronization <br>
 * <b>[send address][send data1][send data2]</b>
 * @param slave is slave address
 * @param data1 is first byte to be sent
 * @param data2 is second byte to be sent
 * @return 0 if successful, nonzero if error 
 * @brief  Send two bytes
 * @note Does check for errors
 */
int I2C_Send2(int8_t slave, uint8_t data1, uint8_t data2);

// sends two bytes to specified slave
// Returns 0 if successful, nonzero if error
/**
 * Send three bytes to specified slave.
 * Uses busy-wait synchronization <br>
 * <b>[send address][send data1][send data2][send data3]</b>
 * @param slave is slave address
 * @param data1 is first byte to be sent
 * @param data2 is second byte to be sent
 * @param data3 is third byte to be sent
 * @return 0 if successful, nonzero if error 
 * @brief  Send three bytes
 * @note Does check for errors
 */
int I2C_Send3(int8_t slave, uint8_t data1, uint8_t data2, uint8_t data3);


/**
 * Initialize OPT3001 on MKII boosterpack. 
 * PA27 is GPIO pin for input, which corresponds
 * with BoosterPack pins J1.8 (Light Sensor interrupt).
 * Initialize PB2 and PB3 for I2C pins, which correspond with
 * BoosterPack pins J1.9 (SCL) and J1.10 (SDA) 
 * @param none
 * @return none 
 * @brief  Initialize OPT3001
 * @note Assumes 80MHz bus clock
 * @note Assumes GPIOB have been reset and powered previously
 */
void I2C_LightSensor_Init(void);



/**
 * Query the OPT3001 light sensor for a measurement.
 * Wait until the measurement is ready, which may take 800 ms 
 * @param none
 * @return light intensity (units 100*lux) 
 * @brief  Input from OPT3001
 * @see I2C_LightSensor_Start I2C_LightSensor_End
 * @note Assumes: I2C_LightSensor_Init() has been called
 */
uint32_t I2C_LightSensor_Input(void);


/**
 * Start a measurement using the OPT3001.
 * If a measurement is currently in progress, return
 * without starting another measurement 
 * @param none
 * @return none 
 * @brief  Start OPT3001
 * @see I2C_LightSensor_Init I2C_LightSensor_End
 * @note Assumes: I2C_LightSensor_Init() has been called
 */
void I2C_LightSensor_Start(void);


/**
 * Finish a measurement using the OPT3001.
 * Query the OPT3001 light sensor for a measurement.
 * If no measurement is currently in progress, start
 * one and return zero immediately.  If the measurement
 * is not yet complete, return zero immediately.  If
 * the measurement is complete, store the result in the
 * pointer provided and return one.
 * @param light pointer to a place to return data
 * @return 1 if measurement is ready and pointer is valid, 0 if measurement is not ready and pointer unchanged 
 * @brief  Finish OPT3001
 * @see I2C_LightSensor_Init I2C_LightSensor_End
 * @note Assumes: I2C_LightSensor_Start() has been called
 */
int I2C_LightSensor_End(uint32_t *light);


/**
 * Initialize TMP006 on MKII boosterpack. 
 * PB16 is GPIO pin for input, which corresponds
 * with BoosterPack pins J1.8 (Light Sensor interrupt).
 * Initialize PB2 and PB3 for I2C pins, which correspond with
 * BoosterPack pins J1.9 (SCL) and J1.10 (SDA) 
 * @param none
 * @return none 
 * @brief  Initialize TMP006
 * @note Assumes 80MHz bus clock
 * @note Assumes GPIOB have been reset and powered previously
 */
void I2C_TempSensor_Init(void);



/**
 * Query the TMP006 temperature sensor for a measurement.
 * Wait until the measurement is ready, which may take 4 seconds 
 * @param sensorV is signed pointer to store sensor voltage (units 100*nV)
 * @param localT is signed pointer to store local temperature (units 100,000*C) 
 * @return none 
 * @brief  Input from TMP006
 * @note TMP006 was discontinued and not populated on most MKII
 */
void I2C_TempSensor_Input(int32_t *sensorV, int32_t *localT);


/**
 * Start a measurement using the TMP006.
 * If a measurement is currently in progress, return
 * without starting another measurement 
 * @param none
 * @return none 
 * @brief  Start TMP006
 * @see I2C_LightSensor_Init I2C_LightSensor_End
 * @note Assumes: I2C_LightSensor_Init() has been called
 */
void I2C_TempSensor_Start(void);

// ------------I2C_TempSensor_End------------
// Query the TMP006 temperature sensor for a
// measurement.  If no measurement is currently in
// progress, start one and return zero immediately.
// If the measurement is not yet complete, return
// zero immediately.  If the measurement is complete,
// store the result in the pointers provided and
// return one.
// Input: sensorV is signed pointer to store sensor voltage (units 100*nV)
//        localT is signed pointer to store local temperature (units 100,000*C)
// Output: one if measurement is ready and pointers are valid
//         zero if measurement is not ready and pointers unchanged
// Assumes: I2C_TempSensor_Init() has been called
/**
 * Finish a measurement using the OPT3001.
 * Query the TMP006 temperature sensor for a measurement.
 * If no measurement is currently in progress, start
 * one and return zero immediately.  If the measurement
 * is not yet complete, return zero immediately.  If
 * the measurement is complete, store the result in the
 * pointers provided and return one.
 * @param sensorV is signed pointer to store sensor voltage (units 100*nV)
 * @param localT is signed pointer to store local temperature (units 100,000*C) 
 * @return 1 if measurement is ready and pointer is valid, 0 if measurement is not ready and pointer unchanged 
 * @brief  Finish TMP006
 * @see I2C_TempSensor_Start 
 * @note Assumes: I2C_TempSensor_Start has been called
 */
int I2C_TempSensor_End(int32_t *sensorV, int32_t *localT);

#endif // __I2C_H__
/** @}*/
