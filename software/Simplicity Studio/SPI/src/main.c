/**************************************************************************//**
 * @main_series1.c
 * @brief Demonstrates USART1 as SPI master.
 * @version 0.0.1
 ******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "retargetswo.h"
#include <stdint.h>
#include <stdio.h>


#define NUM_LEDS 		 24
const uint8_t two_bits_to_SPI[4] = {0x88 ,0xE8, 0x8E,0xEE};
// Lookup table for bytes
// Index - 0b(MSB)(LSB)
// MSB is sent to line first, so the values corresponding to the index are flipped

// e.g  Sending 0b10
//			0 -> 0b1000 = 0x8
//			1 -> 0b1110 = 0xE
// 		Index is 2 so lookup table returns 0x8E in correct order



volatile uint32_t msTicks; /* counts 1ms timeTicks */

void Delay(uint32_t dlyTicks);

/***************************************************************************//**
 * @brief SysTick_Handler
 * Interrupt Service Routine for system tick counter
 ******************************************************************************/
void SysTick_Handler(void)
{
  msTicks++;       /* increment counter necessary in Delay()*/
}

/***************************************************************************//**
 * @brief Delays number of msTick Systicks (typically 1 ms)
 * @param dlyTicks Number of ticks to delay
 ******************************************************************************/
void Delay(uint32_t dlyTicks)
{
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) ;
}


/**************************************************************************//**
 * @brief Initialize USART1
 *****************************************************************************/
void initUSART1 (void)
{
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_USART1, true);

	// Start with default config, then modify as necessary
	USART_InitSync_TypeDef config = USART_INITSYNC_DEFAULT;
	config.master       = true;            // master mode
	config.baudrate     = 3333333; //5000000;         // CLK freq is 1 MHz
	config.autoCsEnable = true;            // CS pin controlled by hardware, not firmware
	config.clockMode    = usartClockMode0; // clock idle low, sample on rising/first edge
	config.msbf         = true;            // send MSB first
    USART_InitSync(USART1, &config);

    // Set USART pin locations
    USART1->ROUTELOC0 = (USART_ROUTELOC0_CLKLOC_LOC15) | // US1_CLK† † † †on location 15 = PD9
                        (USART_ROUTELOC0_CSLOC_LOC15)  | // US1_CS† † † † on location 15 = PD10
                        (USART_ROUTELOC0_TXLOC_LOC15)  | // US1_TX (MOSI) on location 15 = PC10
                        (USART_ROUTELOC0_RXLOC_LOC15);   // US1_RX (MISO) on location 15 = PC11

    // Enable USART pins
    USART1->ROUTEPEN = USART_ROUTEPEN_CLKPEN | USART_ROUTEPEN_CSPEN | USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN;

    // Configure GPIO mode
    GPIO_PinModeSet(gpioPortD, 9, gpioModePushPull, 1); // US1_CLK is push pull
    GPIO_PinModeSet(gpioPortD, 10, gpioModePushPull, 1); // US1_CS is push pull
    GPIO_PinModeSet(gpioPortC, 10, gpioModePushPull, 1); // US1_TX (MOSI) is push pull
    GPIO_PinModeSet(gpioPortC, 11, gpioModeInput, 1);    // US1_RX (MISO) is input

    // Clear RX buffer and shift register
    USART1->CMD |= USART_CMD_CLEARRX;

    // Clear TX buffer and shift register
    USART1->CMD |= USART_CMD_CLEARTX;

    // Enable USART1
    USART_Enable(USART1, usartEnable);
}



void spi_illuminate(uint8_t r, uint8_t g, uint8_t b){

	// send green
	for (int i = 0; i <= 3 ; i++){
		g <<= i*2;
		USART_Tx(USART1, two_bits_to_SPI[(g & 192U) >> 6]);  // bit mask with 11000000 and shift, then index into bits_to_SPI array
	}

	// send red
	for (int i = 0; i <= 3 ; i++){
		r <<= i*2;
		USART_Tx(USART1, two_bits_to_SPI[(r & 192U)>>6]);   // bit mask with 11000000 and shift, then index into bits_to_SPI array
	}


	// send blue
	for (int i = 0; i <= 3 ; i++){
		b <<= i*2;
		USART_Tx(USART1, two_bits_to_SPI[(b & 192U) >> 6]); // bit mask with 11000000 and shift, then index into bits_to_SPI array
	}
}


void LEDs_latch(void){
	// Pull line down for LEDs to latch
	// 20 *  300 ns = ~ 6 microsecond line
	for(int i = 0; i < 20; i++){
		USART_Tx(USART1, 0x00);
	}
}


void light_LEDs(bool* LEDs){

	for (int i = 0; i < NUM_LEDS ; i++){
		if (LEDs[i] == true){
			spi_illuminate( 0, 3, 3);
		}
		else{
			spi_illuminate( 0, 0, 0);
		}
	}

	LEDs_latch();
}



/**************************************************************************//**
 * @brief Main function
 *****************************************************************************/


int main(void)
{
  // Initialize chip
  CHIP_Init();

  // Initialize USART1 as SPI slave
  initUSART1();


  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) {
    while (1) ;
  }


  bool LEDs[NUM_LEDS] = {true, false, true, false, true, false, true, false, true, false, true, false,
		  true, false, true, false, true, false, true, false, true, false, true, false};

  while(1){
	  light_LEDs(LEDs);
	  Delay(1000);
	  for (int i = 0 ; i <= NUM_LEDS; i++){
		  LEDs[i] = !LEDs[i];
	  }
  }

}

