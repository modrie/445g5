/***************************************************************************//**
 * @file
 * @brief Relative humidity and temperature sensor demo for SLSTK3401A
 * @version 5.4.0
 *******************************************************************************
 * # License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
//#include "display.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "cpt112s_config.h"
#include "i2cspm.h"
#include "bspconfig.h"

#define SSD1306_I2C_ADDRESS   0x3C

/***************************************************************************//**
 * Local defines
 ******************************************************************************/


/***************************************************************************//**
 * Local prototypes
 ******************************************************************************/
static void gpioSetup(void);


/***************************************************************************//**
 * @brief  Helper function to perform data measurements.
 ******************************************************************************/
//static int updateDisplay(I2C_TypeDef *i2c)
//{
//  Si7013_MeasureRHAndTemp(i2c, SI7021_ADDR, rhData, tData);
//  return 0;
//}


bool ssd1306_Detect(I2C_TypeDef *i2c, uint8_t addr)
{
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t                    i2c_read_data[2];
  uint8_t                    i2c_write_data[2];

  seq.addr  = addr;
  seq.flags = I2C_FLAG_WRITE_READ;
  /* Select command to issue */
  i2c_write_data[0] = 0x00;
  i2c_write_data[1] = 0xA4;
  seq.buf[0].data   = i2c_write_data;
  seq.buf[0].len    = 2;
  /* Select location/length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 2;

  ret = I2CSPM_Transfer(i2c, &seq);
  if (ret != i2cTransferDone) {
    return false;
  }
  return true;
}


/***************************************************************************//**
 * @brief  Main function
 ******************************************************************************/
int main(void)
{
  I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;
  EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_STK_DEFAULT;
  CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;
  uint32_t         rhData = 0;
  bool             si7013_status;
  int32_t          tempData = 0;

  /* Chip errata */
  CHIP_Init();

  /* Initalize hardware */
  EMU_DCDCInit(&dcdcInit);
  CMU_HFXOInit(&hfxoInit);

  /* Switch HFCLK to HFXO and disable HFRCO */
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);

  /* Initalize other peripherals and drivers */
  gpioSetup();

  // configure ports
  i2cInit.sclPort = gpioPortD;       /**< SCL pin port number */
  i2cInit.sclPin  = 13;              /**< SCL pin number */
  i2cInit.sdaPort = gpioPortD;       /**< SDA pin port number */
  i2cInit.sdaPin  = 12;              /**< SDA pin number */


  I2CSPM_Init(&i2cInit);

  /* Get initial sensor status */
  si7013_status = ssd1306_Detect(i2cInit.port, SSD1306_I2C_ADDRESS);

  while(1);
  EMU_EnterEM2(false);

//
//  uint8_t command = 0x00 ; // some display command
//  while (true) {
//
//    performMeasurements(i2cInit.port, command);
//
//    EMU_EnterEM2(false);
//  }

}

/***************************************************************************//**
 * @brief Setup GPIO, enable sensor isolation switch
 ******************************************************************************/
static void gpioSetup(void)
{
  /* Enable GPIO clock */
  CMU_ClockEnable(cmuClock_GPIO, true);

  /* Enable Si7021 sensor isolation switch */
//  GPIO_PinModeSet(CS0_SENSOR_EN_PORT, CS0_SENSOR_EN_PIN, gpioModePushPull, 1);
//  GPIO_PinModeSet(gpioPortD, 12, gpioModePushPull, 1);
//  GPIO_PinModeSet(gpioPortD, 13, gpioModePushPull, 1);
}

