/***************************************************************************//**
 * @file
 * @brief Internal temperature sensor demo for SLSTK3401A_EFM32PG
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
#include "em_device.h"
#include "em_adc.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_system.h"
#include "em_emu.h"
#include "em_timer.h"
#include "rtcdriver.h"
#include "graphics.h"
#include "bspconfig.h"
#include <stddef.h>
#include <time.h>
#include <stdio.h>
#include "rtcdrv_config.h"
#include "retargetswo.h"
#include "bsp_trace.h"
#include "Yin.h"

/***************************************************************************//**
 * Local defines
 ******************************************************************************/

/** Time (in ms) between periodic updates of the measurements. */
#define PERIODIC_UPDATE_MS      1000
/***************************************************************************//**
 * Local variables
 ******************************************************************************/
/* RTC callback parameters. */
static void (*rtcCallback)(void*) = NULL;
static void * rtcCallbackArg = 0;
volatile uint32_t rtcCallbacks = 0;

/** This flag tracks if we need to update the display. */
static volatile bool updateDisplay = true;
static volatile bool sample = true;
static volatile int count_global = 0;
int16_t buffer_size = 512;
int16_t buffer[512];
/** Timer used for periodic update of the measurements. */
RTCDRV_TimerID_t periodicUpdateTimerId;
/***************************************************************************//**
 * Local prototypes
 ******************************************************************************/
static void periodicUpdateCallback(RTCDRV_TimerID_t id, void *user);
static void memLcdCallback(RTCDRV_TimerID_t id, void *user);

/**************************************************************************//**
 * @brief TIMER0_IRQHandler
 * Interrupt Service Routine for TIMER0
 * set the flag for sampling
 *****************************************************************************/
void TIMER0_IRQHandler(void) {
	TIMER_IntClear(TIMER0, TIMER_IF_OF);
	sample = true;
	count_global++;
}

/***************************************************************************//**
* @brief
*   Configure Timer for this application.
*******************************************************************************/
static void TimerConfig(void)
{
	/** TIMER CALCULATIONS:
		Desired Trigger Freq: 16kHz trigger
		Prescale: 1
		Cycles per second: 40MHz / Prescale
		Timer refresh rate: (Cycles per second) / Trigger Freq = 40MHz / 16kHz = 2500
	*/

	/* Use default timer settings */
	TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;

	/* Change prescaler  */
	timerInit.prescale = timerPrescale1;

	/* Enable timer interrupts */
	TIMER_IntEnable(TIMER0, TIMER_IF_OF);
	NVIC_EnableIRQ(TIMER0_IRQn);

	TIMER_TopSet(TIMER0, 2500);
	TIMER_Init(TIMER0, &timerInit);
}

/***************************************************************************//**
 * @brief Initialize ADC for temperature sensor readings in single poin
 ******************************************************************************/
static void AdcSetup(void)
{
  /* Enable ADC clock */
  CMU_ClockEnable(cmuClock_ADC0, true);

  /* Base the ADC configuration on the default setup. */
  ADC_Init_TypeDef       init  = ADC_INIT_DEFAULT;
  ADC_InitSingle_TypeDef sInit = ADC_INITSINGLE_DEFAULT;

  /* Initialize timebases */
  init.timebase = ADC_TimebaseCalc(0);
  init.prescale = ADC_PrescaleCalc(16000000, 0);
  init.warmUpMode = adcWarmupKeepADCWarm;
//  init.prescale = ADC_PrescaleCalc(100,0);
  ADC_Init(ADC0, &init);

  /* Set input to temperature sensor. Reference must be 1.25V */
  //sInit.reference   = adcRef1V25;
  sInit.reference 	= adcRef5V;
  sInit.acqTime     = adcAcqTime1; /* Minimum time for temperature sensor */
//  sInit.posSel      = adcPosSelTEMP;
  sInit.posSel		= adcPosSelAPORT3XCH8;
  sInit.rep 		= 1;
  ADC_InitSingle(ADC0, &sInit);
}

/***************************************************************************//**
 * @brief  Do one ADC conversion
 * @return ADC conversion result
 ******************************************************************************/
static uint32_t AdcRead(void)
{
  ADC_Start(ADC0, adcStartSingle);
  while ( (ADC0->STATUS & ADC_STATUS_SINGLEDV) == 0 ) {
  }
  return ADC_DataSingleGet(ADC0);
}


float yin(Yin* yin,int16_t* audio)
{
	float pitch;
	//printf("?\n");
	pitch = Yin_getPitch(yin, audio);

	return pitch;
}

/***************************************************************************//**
 * @brief  Main function
 ******************************************************************************/
int main(void)
{
  EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_STK_DEFAULT;
  CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_STK_DEFAULT;
  int32_t tempRead;
  float celsius;

  /* Chip errata */
  CHIP_Init();

  /* Init DCDC regulator and HFXO with WSTK radio board specific parameters
     kits\SLSTK3401A_EFM32PG\config\bspconfig.h. */
  EMU_DCDCInit(&dcdcInit);
  CMU_HFXOInit(&hfxoInit);

  /* Switch HFCLK to HFXO and disable HFRCO */
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);

  CMU_ClockEnable(cmuClock_TIMER0, true);

  /* Setup ADC */
  AdcSetup();
  TimerConfig();
  RTCDRV_Init();
//  GRAPHICS_Init();
  RETARGET_SwoInit();

  /* Set up periodic update of the display. */
  RTCDRV_AllocateTimer(&periodicUpdateTimerId);
  RTCDRV_StartTimer(periodicUpdateTimerId, rtcdrvTimerTypePeriodic,
                    PERIODIC_UPDATE_MS, periodicUpdateCallback, NULL);

  updateDisplay = true;

  int i = 0, t_elapsed, ticks;



  RTCDRV_SetWallClock(0);

printf("\n");
printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  int idx = 0;
  float pitch;
//  while (count < 48000)


  Yin yin_obj;
  Yin_init(&yin_obj, buffer_size, 0.1);
  int ticks_1, ticks_2, sample_count;
  ticks = RTCDRV_GetWallClockTicks32();
  while (true)
  {
	  if (sample == true)
	  {
		  sample_count++;
		  sample = false;
		  tempRead = AdcRead();
//		  printf("%d ", tempRead);
		  buffer[idx] = (int16_t)tempRead;
		  idx++;

//		  if (sample_count % 512 == 0)
//		  {
//			  ticks_1 = RTCDRV_GetWallClockTicks32();
//			  t_elapsed = RTCDRV_TicksToMsec(ticks_1 - ticks);
//			  printf("Time elapsed: %d\n", t_elapsed);
//			  ticks = ticks_1;
//		  }

		  if (idx == buffer_size)
		  {

			  pitch = yin(&yin_obj, buffer);

			  //ticks_2 = RTCDRV_GetWallClockTicks32();
			  //t_elapsed = RTCDRV_TicksToMsec(ticks_2 - ticks_1);
			  //printf("Time elapsed: %d\n", t_elapsed);
			  printf("pitch: %f\n", pitch);

			  idx = 0;

		  }


	  }
  }


//  GRAPHICS_ShowTemp((float)(t_elapsed));
}

/***************************************************************************//**
 * @brief   The actual callback for Memory LCD toggling
 * @param[in] id
 *   The id of the RTC timer (not used)
 ******************************************************************************/
static void memLcdCallback(RTCDRV_TimerID_t id, void *user)
{
  (void) id;
  (void) user;
  rtcCallback(rtcCallbackArg);

  rtcCallbacks++;
}

/***************************************************************************//**
 * @brief   Register a callback function at the given frequency.
 *
 * @param[in] pFunction  Pointer to function that should be called at the
 *                       given frequency.
 * @param[in] argument   Argument to be given to the function.
 * @param[in] frequency  Frequency at which to call function at.
 *
 * @return  0 for successful or
 *         -1 if the requested frequency does not match the RTC frequency.
 ******************************************************************************/
int rtcIntCallbackRegister(void (*pFunction)(void*),
                           void* argument,
                           unsigned int frequency)
{
  RTCDRV_TimerID_t timerId;
  rtcCallback    = pFunction;
  rtcCallbackArg = argument;

  RTCDRV_AllocateTimer(&timerId);

  RTCDRV_StartTimer(timerId, rtcdrvTimerTypePeriodic, 1000 / frequency,
                    memLcdCallback, NULL);

  return 0;
}

/***************************************************************************//**
 * @brief Callback used to count between measurement updates
 ******************************************************************************/
static void periodicUpdateCallback(RTCDRV_TimerID_t id, void *user)
{
  (void) id;
  (void) user;
  updateDisplay = true;

}
