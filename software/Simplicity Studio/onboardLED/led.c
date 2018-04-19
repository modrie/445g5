#include "em_device.h"
#include "em_chip.h"
#include <stddef.h>
#include <time.h>
#include <stdio.h>
#include "em_timer.h"
#include "em_emu.h"
#include "em_cmu.h"
#include "em_system.h"
#include "em_gpio.h"
#include "em_adc.h"

static volatile uint32_t count = 0;

#define LED_PORT gpioPortF
#define LED_PIN 2

#define EMU_DCDCINIT_STK_DEFAULT                                                        \
  {                                                                                 \
    emuPowerConfig_DcdcToDvdd,   /* DCDC to DVDD */                                 \
    emuDcdcMode_LowNoise,        /* Low-niose mode in EM0 */                        \
    1800,                        /* Nominal output voltage for DVDD mode, 1.8V  */  \
    5,                           /* Nominal EM0/1 load current of less than 5mA */  \
    10,                          /* Nominal EM2/3/4 load current less than 10uA  */ \
    200,                         /* Maximum average current of 200mA
                                    (assume strong battery or other power source) */      \
    emuDcdcAnaPeripheralPower_DCDC,/* Select DCDC as analog power supply (lower power) */ \
    emuDcdcLnHighEfficiency,     /* Use high-efficiency mode */                           \
    emuDcdcLnCompCtrl_1u0F,      /* 1uF DCDC capacitor */                                 \
  }

#define CMU_HFXOINIT_STK_DEFAULT                                          \
  {                                                                       \
    true,       /* Low-power mode for EFM32 */                            \
    false,      /* Disable auto-start on EM0/1 entry */                   \
    false,      /* Disable auto-select on EM0/1 entry */                  \
    false,      /* Disable auto-start and select on RAC wakeup */         \
    _CMU_HFXOSTARTUPCTRL_CTUNE_DEFAULT,                                   \
    0x142,      /* Steady-state CTUNE for STK boards without load caps */ \
    _CMU_HFXOSTEADYSTATECTRL_REGISH_DEFAULT,                              \
    0x20,       /* Matching errata fix in CHIP_Init() */                  \
    0x7,        /* Recommended steady-state osc core bias current */      \
    0x6,        /* Recommended peak detection threshold */                \
    _CMU_HFXOTIMEOUTCTRL_SHUNTOPTTIMEOUT_DEFAULT,                         \
    0xA,        /* Recommended peak detection timeout  */                 \
    0x4,        /* Recommended steady timeout */                          \
    _CMU_HFXOTIMEOUTCTRL_STARTUPTIMEOUT_DEFAULT,                          \
    cmuOscMode_Crystal,                                                   \
  }

/**************************************************************************//**
 * @brief TIMER0_IRQHandler
 * Interrupt Service Routine for TIMER0
 * set the flag for sampling
 *****************************************************************************/
void TIMER0_IRQHandler(void) {
	TIMER_IntClear(TIMER0, TIMER_IF_OF);
	count++;
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
		Timer refresh rate: (Cycles per second) / Trigger Freq = 40MHz / 800KHz  = 50
	*/

	/* Use default timer settings */
	TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;

	/* Change prescaler  */
	timerInit.prescale = timerPrescale1;

	/* Enable timer interrupts */
	TIMER_IntClear(TIMER0, TIMER_IF_OF);
	 NVIC_DisableIRQ(TIMER0_IRQn);
	TIMER_IntEnable(TIMER0, TIMER_IF_OF);
	NVIC_EnableIRQ(TIMER0_IRQn);

	TIMER_TopSet(TIMER0, 38);
	TIMER_Init(TIMER0, &timerInit);
}



int main(void)
{
	EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_STK_DEFAULT;
	CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_STK_DEFAULT;
	/* Chip errata */
	CHIP_Init();


	/* Init DCDC regulator and HFXO with WSTK radio board specific parameters
       kits\SLSTK3401A_EFM32PG\config\bspconfig.h. */
    EMU_DCDCInit(&dcdcInit);
    CMU_HFXOInit(&hfxoInit);

    /* Switch HFCLK to HFXO and disable HFRCO */
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);

    CMU_ClockEnable(cmuClock_GPIO, true);
    GPIO_PinModeSet(LED_PORT, LED_PIN, gpioModePushPull, 0);
    GPIO_PinModeSet(LED_PORT, 3, gpioModePushPull, 0);

    CMU_ClockEnable(cmuClock_TIMER0, true);

    TimerConfig();

    /* Infinite loop */
    int count = 0;
    while (1)
    {
    	count++;
    	if (count % 2 == 0)
    	{
    		GPIO_PinOutClear(LED_PORT, 3);
    		GPIO_PinOutSet(LED_PORT, LED_PIN);
    	}
    	else
    	{
    		GPIO_PinOutClear(LED_PORT, LED_PIN);
    		GPIO_PinOutSet(LED_PORT, 3);
    	}
    	int delay = 1000000;
    	while(delay > 0)
    	{
    		delay -- ;
    	}
    }
}

