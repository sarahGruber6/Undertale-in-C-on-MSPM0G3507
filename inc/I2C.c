/* I2C.c
 * Jonathan Valvano
 * November 23, 2022
 * Derived from i2c_controller_rw_multibyte_fifo_poll_LP_MSPM0G3507_nortos_ticlang
light and temperature sensors (I2C)
J1.8 ambient light (OPT3001) interrupt (digital)  PA27
J1.9 ambient light (OPT3001) and temperature sensor (TMP006) I2C SCL (I2C)  PB2_I2C1-SCL
J1.10 ambient light (OPT3001) and temperature sensor (TMP006) I2C SDA (I2C) PB3_I2C1-SDA
J2.11 temperature sensor (TMP006) interrupt (digital) PB16
The TMP006 was discontinued in 2017. Therefore, it is no longer populated on the board.
Documentation of the TMP006 is unchanged in this user's guide to accommodate users who still
have the device populated on the board
 */
//#define CONFIG_MSPM0G350X

#include <ti/devices/msp/msp.h>
#include "../inc/I2C.h"
#include "../inc/Clock.h"
#define PA27INDEX 59
#define PB2INDEX 14
#define PB3INDEX 15
#define PB16INDEX 32

// assume 80MHz bus clock
// I2C in power domain PD0, so
// for 32MHz bus clock, SYSCLK clock is 32MHz
// for 40MHz bus clock, SYSCLK clock is ULPCLK 20MHz
// for 80MHz bus clock, SYSCLK clock is ULPCLK 40MHz
// initialize I2C for 400kHz clock
// busy-wait synchronization
void I2C_Init(void){
    // assumes GPIOA and GPIOB are reset and powered previously
    // RSTCLR to I2C1 peripheral
    //   bits 31-24 unlock key 0xB1
    //   bit 1 is Clear reset sticky bit
    //   bit 0 is reset gpio port
  I2C1->GPRCM.RSTCTL = 0xB1000003;
    // Enable power to I2C1 peripheral
    // PWREN
    //   bits 31-24 unlock key 0x26
    //   bit 0 is Enable Power
  I2C1->GPRCM.PWREN = 0x26000001;
  // configure PB3 and PB2 as alternate IC2 function
  // configure PB16 PA27 negative logic, with external pullup
  // bit 18 INENA
  // bit 25 hiZ
  // vi 7 PC peripheral connect
  IOMUX->SECCFG.PINCM[PB3INDEX]  = 0x02040084;  // I2C SDA
  IOMUX->SECCFG.PINCM[PB2INDEX]  = 0x02040084;  // I2C SCL


  Clock_Delay(24); // time for gpio to power up
  I2C1->CLKSEL = 8; // SYSCLK
// bit 3 SYSCLK
// bit 2 MFCLK
   I2C1->CLKDIV = 0; // divide by 1
   I2C1->MASTER.MCTR = 0x00;
   // period=clockPeriod*(1+MTPR)*10
   // bus clock=32MHz, make MTPR=7, frequency = 32MHz/80 = 400kHz
   // bus clock=40MHz, make MTPR=4, frequency = 20MHz/50 = 400kHz
   // bus clock=80MHz, make MTPR=9, frequency = 40MHz/100 = 400kHz
   I2C1->MASTER.MTPR = 9; /* Set frequency to 400000 Hz*/
// bits 6-0 TPR (0 to 63), divide by TPR+1
   I2C1->MASTER.MCR = 4;
   // bit 8 LPBK=0 no loop back
   // bit 2 CLKSTRETCH=1 allow stretch
   // bit 1 MMST=0 disable multicontroller mode
   // bit 0 ACTIVE 0 for disable, 1 for enable
   // not using interrupts, FIFO triggers not used
   I2C1->MASTER.MFIFOCTL = 0;
   I2C1->MASTER.MCR = 5;
}



// receives two bytes from specified slave
uint32_t I2C_error;

uint16_t I2C_Recv2(int8_t slave){
  uint8_t data1, data2;
  while((I2C1->MASTER.MSR &0x20 )==0){}; // wait until idle
  I2C1->MASTER.MSA = (slave<<1)|0x0001; 
  // bit 7-1 address
  // bit 0 direction=1 receive
  I2C1->MASTER.MCTR = 0x00020007;
  // bits 27-16 MBLEN =2 (length)
  // bit 3 ACK
  // bit 2 STOP=1
  // bit 1 START=1
  // bit 0 BURSTRUN=1
  while((I2C1->MASTER.MFIFOSR & 0x000F)==0){}; //wait for received data
  data1 = I2C1->MASTER.MRXDATA;
  while((I2C1->MASTER.MFIFOSR & 0x000F)==0){}; //wait for received data
  data2 = I2C1->MASTER.MRXDATA;
  // check for error
//  if(I2C1->MASTER.MSR &0x12){ // lost arbitration or no ack
//	I2C_error = I2C1->MASTER.MSR; // help debugging
//    return 0; // error
//  }

  return (data1<<8)+data2;
}
int static IC2FillTxFifo(uint8_t *buffer, uint16_t count){
  for(int i=0; i<count; i++){
    if((I2C1->MASTER.MFIFOSR & 0x0F00) == 0) return 0; // fail TxFifo full
	I2C1->MASTER.MTXDATA = buffer[i];
  }
  return 1;
}

// sends one byte to specified slave
// Returns 0 if successful, nonzero if error
int I2C_Send1(int8_t slave, uint8_t data1){
  if(IC2FillTxFifo(&data1,1) == 0) return 0;
  while((I2C1->MASTER.MSR &0x20 )==0){}; // wait until idle
  I2C1->MASTER.MSA = (slave<<1); 
  // bit 7-1 address
  // bit 0 direction=0 transmit
  I2C1->MASTER.MCTR = 0x00010007;
  // bits 27-16 MBLEN =1 (length)
  // bit 3 ACK
  // bit 2 STOP=1
  // bit 1 START=1
  // bit 0 BURSTRUN=1
  while((I2C1->MASTER.MSR&0x01 )==0x01){}; // wait until not busy
  
  // check for error
  if(I2C1->MASTER.MSR &0x12){ // lost arbitration or no ack
	I2C_error = I2C1->MASTER.MSR; // help debugging
    return 0; // error
  }
  while((I2C1->MASTER.MSR&0x20 )==0){}; // wait until idle
  return 1;
}
// sends two bytes to specified slave
// Returns 0 if successful, nonzero if error
int I2C_Send2(int8_t slave, uint8_t data1, uint8_t data2){
  uint8_t data[2];
  data[0] = data1;
  data[1] = data2;
  if(IC2FillTxFifo(data,2) == 0) return 0;
  while((I2C1->MASTER.MSR&0x20 )==0){}; // wait until idle
  I2C1->MASTER.MSA = (slave<<1); 
  // bit 7-1 address
  // bit 0 direction=0 transmit
  I2C1->MASTER.MCTR = 0x00020007;
  // bits 27-16 MBLEN =2 (length)
  // bit 3 ACK
  // bit 2 STOP=1
  // bit 1 START=1
  // bit 0 BURSTRUN=1
  while((I2C1->MASTER.MSR&0x01 )==0x01){}; // wait until not busy
  // check for error
  if(I2C1->MASTER.MSR &0x12){ // lost arbitration or no ack
	I2C_error = I2C1->MASTER.MSR; // help debugging
    return 0; // error
  }
  while((I2C1->MASTER.MSR &0x20 )==0){}; // wait until idle
  return 1;
}

// sends two bytes to specified slave
// Returns 0 if successful, nonzero if error
int I2C_Send3(int8_t slave, uint8_t data1, uint8_t data2, uint8_t data3){
  uint8_t data[3];
  data[0] = data1;
  data[1] = data2;
  data[2] = data3;
  if(IC2FillTxFifo(data,3) == 0) return 0;
  while((I2C1->MASTER.MSR &0x20 )==0){}; // wait until idle
  I2C1->MASTER.MSA = (slave<<1); 
  // bit 7-1 address
  // bit 0 direction=0 transmit
  I2C1->MASTER.MCTR = 0x00030007;
  // bits 27-16 MBLEN =3 (length)
  // bit 3 ACK
  // bit 2 STOP=1
  // bit 1 START=1
  // bit 0 BURSTRUN=1
  while((I2C1->MASTER.MSR&0x01 )==0x01){}; // wait until not busy
  // check for error
  if(I2C1->MASTER.MSR &0x12){ // lost arbitration or no ack
	I2C_error = I2C1->MASTER.MSR; // help debugging
    return 0; // error
  }
  while((I2C1->MASTER.MSR &0x20 )==0){}; // wait until idle
  return 1;
}


// ------------I2C_LightSensor_Init------------
// Initialize a GPIO pin for input, which corresponds
// with BoosterPack pins J1.8 PA27 (Light Sensor interrupt).
// Initialize two I2C pins, which correspond with
// BoosterPack pins J1.9 (SCL) and J1.10 (SDA).
// Input: none
// Output: none
void I2C_LightSensor_Init(void){
  I2C_Init();
  IOMUX->SECCFG.PINCM[PA27INDEX] = 0x00040081;  // GPIO input (OPT3001) interrupt (digital)
}

// Send the appropriate codes to initiate a
// measurement with an OPT3001 light sensor at I2C
// slave address 'slaveAddress'.
// Assumes: I2C_LightSensor_Init() has been called
void lightsensorstart(uint8_t slaveAddress){
  // configure Low Limit Register (0x02) for:
  // INT pin active after each conversion completes
  I2C_Send3(slaveAddress, 0x02, 0xC0, 0x00);
  // configure Configuration Register (0x01) for:
  // 15-12 RN         range number         1100b = automatic full-scale setting mode
  // 11    CT         conversion time         1b = 800 ms
  // 10-9  M          mode of conversion     01b = single-shot
  // 8     OVF        overflow flag field     0b (read only)
  // 7     CRF        conversion ready field  0b (read only)
  // 6     FH         flag high field         0b (read only)
  // 5     FL         flag low field          0b (read only)
  // 4     L          latch                   1b = latch interrupt if measurement exceeds programmed ranges
  // 3     POL        polarity                0b = INT pin reports active low
  // 2     ME         mask exponent           0b = do not mask exponent (more math later)
  // 1-0   FC         fault count            00b = 1 fault triggers interrupt
  I2C_Send3(slaveAddress, 0x01, 0xCA, 0x10);
  I2C_Recv2(slaveAddress);         // read Configuration Register to reset conversion ready
}

// Send the appropriate codes to end a measurement
// with an OPT3001 light sensor at I2C slave address
// 'slaveAddress'.  Return results (units 100*lux).
// Assumes: I2C_LightSensor_Init() has been called and measurement is ready
int32_t static lightsensorend(uint8_t slaveAddress){
  uint16_t raw, config;
  I2C_Send1(slaveAddress, 0x00);   // pointer register 0x00 = Result Register
  raw = I2C_Recv2(slaveAddress);
  // force the INT pin to clear by clearing and resetting the latch bit of the Configuration Register (0x01)
  I2C_Send1(slaveAddress, 0x01);   // pointer register 0x01 = Configuration Register
  config = I2C_Recv2(slaveAddress);// current Configuration Register
  I2C_Send3(slaveAddress, 0x01, (config&0xFF00)>>8, (config&0x00FF)&~0x0010);
  I2C_Send3(slaveAddress, 0x01, (config&0xFF00)>>8, (config&0x00FF)|0x0010);
  return (1<<(raw>>12))*(raw&0x0FFF);
}

// ------------I2C_LightSensor_Input------------
// Query the OPT3001 light sensor for a measurement.
// Wait until the measurement is ready, which may
// take 800 ms.
// Input: none
// Output: light intensity (units 100*lux)
// Assumes: I2C_LightSensor_Init() has been called
#define LIGHTINT  (1<<27)  /* PA27 Input */
int LightBusy = 0;                 // 0 = idle; 1 = measuring
uint32_t I2C_LightSensor_Input(void){
  uint32_t light;
  LightBusy = 1;
  lightsensorstart(0x44);
  while((GPIOA->DIN31_0 & LIGHTINT) == LIGHTINT){};       // wait for conversion to complete
  light = lightsensorend(0x44);
  LightBusy = 0;
  return light;
}

// ------------I2C_LightSensor_Start------------
// Start a measurement using the OPT3001.
// If a measurement is currently in progress, return
// immediately.
// Input: none
// Output: none
// Assumes: I2C_LightSensor_Init() has been called
void I2C_LightSensor_Start(void){
  if(LightBusy == 0){
    // no measurement is in progress, so start one
    LightBusy = 1;
    lightsensorstart(0x44);
  }
}

// ------------I2C_LightSensor_End------------
// Query the OPT3001 light sensor for a measurement.
// If no measurement is currently in progress, start
// one and return zero immediately.  If the measurement
// is not yet complete, return zero immediately.  If
// the measurement is complete, store the result in the
// pointer provided and return one.
// Input: light is pointer to store light intensity (units 100*lux)
// Output: one if measurement is ready and pointer is valid
//         zero if measurement is not ready and pointer unchanged
// Assumes: I2C_LightSensor_Init() has been called
int I2C_LightSensor_End(uint32_t *light){
  uint32_t lightLocal;
  if(LightBusy == 0){
    // no measurement is in progress, so start one
    LightBusy = 1;
    lightsensorstart(0x44);
    return 0;                      // measurement needs more time to complete
  } else{
    // measurement is in progress
    if((GPIOA->DIN31_0 & LIGHTINT) == LIGHTINT){
      return 0;                    // measurement needs more time to complete
    } else{
      lightLocal = lightsensorend(0x44);
      *light = lightLocal;
      LightBusy = 0;
      return 1;                    // measurement is complete; pointer valid
    }
  }
}

// ------------I2C_TempSensor_Init------------
// Initialize a GPIO pin for input, which corresponds
// with BoosterPack pin J2.11 PB16 (Temperature Sensor
// interrupt).  Initialize two I2C pins, which
// correspond with BoosterPack pins J1.9 (SCL) and
// J1.10 (SDA).
// Input: none
// Output: none
void I2C_TempSensor_Init(void){
  I2C_Init();
  IOMUX->SECCFG.PINCM[PB16INDEX] = 0x00040081;  // GPIO input, (TMP006) interrupt (digital)
}

// Send the appropriate codes to initiate a
// measurement with a TMP006 temperature sensor at
// I2C slave address 'slaveAddress'.
// Assumes: I2C_TempSensor_Init() has been called
void static tempsensorstart(uint8_t slaveAddress){
  // configure Configuration Register (0x02) for:
  // 15    RST        software reset bit      0b = normal operation
  // 14-12 MOD        mode of operation     111b = sensor and die continuous conversion
  // 11-9  CR         ADC conversion rate   010b = 1 sample/sec
  // 8     EN         interrupt pin enable    1b = ~DRDY pin enabled (J2.11/P3.6)
  // 7     ~DRDY      data ready bit          0b (read only, automatic clear)
  // 6-0   reserved                      000000b (reserved)
  I2C_Send3(slaveAddress, 0x02, 0x75, 0x00);
}

// Send the appropriate codes to end a measurement
// with a TMP006 temperature sensor at I2C slave
// address 'slaveAddress'.  Store the results at the
// provided pointers.
// Assumes: I2C_TempSensor_Init() has been called and measurement is ready
void static tempsensorend(uint8_t slaveAddress, int32_t *sensorV, int32_t *localT){
  int16_t raw;
  I2C_Send1(slaveAddress, 0x00);   // pointer register 0x00 = Sensor Voltage Register
  raw = I2C_Recv2(slaveAddress);
  *sensorV = raw*15625;            // 156.25 nV per LSB
  I2C_Send1(slaveAddress, 0x01);   // pointer register 0x01 = Local Temperature Register
  raw = I2C_Recv2(slaveAddress);
  *localT = (raw>>2)*3125;         // 0.03125 C per LSB
}

// ------------I2C_TempSensor_Input------------
// Query the TMP006 temperature sensor for a
// measurement.  Wait until the measurement is ready,
// which may take 4 seconds.
// Input: sensorV is signed pointer to store sensor voltage (units 100*nV)
//        localT is signed pointer to store local temperature (units 100,000*C)
// Output: none
// Assumes: I2C_TempSensor_Init() has been called
#define TEMPINT   (1<<16)  /* Port PB16 Input */

int TempBusy = 0;                  // 0 = idle; 1 = measuring
void I2C_TempSensor_Input(int32_t *sensorV, int32_t *localT){
  int32_t volt, temp;
  TempBusy = 1;
  tempsensorstart(0x40);
  while((GPIOB->DIN31_0 & TEMPINT) == TEMPINT){};        // wait for conversion to complete
  tempsensorend(0x40, &volt, &temp);
  *sensorV = volt;
  *localT = temp;
  TempBusy = 0;
}

// ------------I2C_TempSensor_Start------------
// Start a measurement using the TMP006.
// If a measurement is currently in progress, return
// immediately.
// Input: none
// Output: none
// Assumes: I2C_TempSensor_Init() has been called
void I2C_TempSensor_Start(void){
  if(TempBusy == 0){
    // no measurement is in progress, so start one
    TempBusy = 1;
    tempsensorstart(0x40);
  }
}

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
int I2C_TempSensor_End(int32_t *sensorV, int32_t *localT){
  int32_t volt, temp;
  if(TempBusy == 0){
    // no measurement is in progress, so start one
    TempBusy = 1;
    tempsensorstart(0x40);
    return 0;                      // measurement needs more time to complete
  } else{
    // measurement is in progress
    if((GPIOB->DIN31_0 & TEMPINT) == TEMPINT){
      return 0;                    // measurement needs more time to complete
    } else{
      tempsensorend(0x40, &volt, &temp);
      *sensorV = volt;
      *localT = temp;
      TempBusy = 0;
      return 1;                    // measurement is complete; pointers valid
    }
  }
}
