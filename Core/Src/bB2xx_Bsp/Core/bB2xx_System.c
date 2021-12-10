/*
 * bB2xx_System.c
 *
 *  Created on: Oct 18, 2021
 *      Author: fil
 */

#include "main.h"
#include "bB2xx_System.h"
#include <strings.h>

RAMD3_RAM_AREA	__attribute__ ((aligned (16)))	SystemVar_TypeDef			SystemVar;
RAMD3_RAM_AREA	__attribute__ ((aligned (16)))	SystemParameters_TypeDef	SystemParameters;
RAMD3_RAM_AREA	__attribute__ ((aligned (16)))	UserParameters_TypeDef		UserParameters;


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if ( GPIO_Pin == ENCODER_BTN_Pin)
	{
		if ( SystemVar.encoder_sw_debounce == 0 )
		{
			encoder_switch_callback();
			SystemVar.encoder_sw_debounce = 3;
		}
	}
	if ( GPIO_Pin == TOUCH_IRQ_Pin)
	{
		ILI9341_TouchCallback();
	}
}

void SysLedActivities(uint8_t val)
{
	  HAL_GPIO_WritePin(SYSTEM_LED_GPIO_Port, SYSTEM_LED_Pin, val);
}

void SysTimer100msecCallback(void)
{
	if ( SystemVar.encoder_sw_debounce != 0 )
		SystemVar.encoder_sw_debounce--;
	if ( SystemVar.encoder_rotation_debounce != 0 )
		SystemVar.encoder_rotation_debounce--;
	if ( SystemVar.sw1_debounce != 0 )
		SystemVar.sw1_debounce--;
	if ( SystemVar.sw2_debounce != 0 )
		SystemVar.sw2_debounce--;

	SystemVar.timers_flag |= TIMER_100MS_FLAG;
	SystemVar.tim100msec_counter++;
	if ( SystemVar.tim100msec_counter >= 9 )
	{
		SystemVar.timers_flag |= TIMER_1SEC_FLAG;
		SysLedActivities(GPIO_PIN_RESET);
		SystemVar.tim100msec_counter = 0;
	}
	else
		SysLedActivities(GPIO_PIN_SET);
	check_buttons();
}

void InitSystemTimers(void)
{
	HAL_TIM_Base_Start_IT(&SYSTICK_100MSEC);
	HAL_TIM_PWM_Start(&BACKLIGHT_TIMER, BACKLIGHT_TIMER_CHANNEL);
	ILI9341_SetBrightness(0);
	HAL_TIM_Encoder_Start_IT(&ENCODER_TIMER, TIM_CHANNEL_ALL);
	HAL_TIM_Base_Start_IT(&CONTROL_TIMER);
}

void InitSystemVars(void)
{
	bzero((uint8_t *)&SystemVar,sizeof(SystemVar));
	SystemVar.system |= SYSTEM_MENU_INCDEC;
}


void bB2xx_Init(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	InitSystemVars();
	bB2xx_flash_init();
	bB2xx_flash_get_sysparams();

	InitSystemTimers();
	LD1_OnOff(LED_ON);
	LD7_OnOff(LED_ON);
	LD8_OnOff(LED_ON);

	ILI9341_Init();
	ILI9341_FillScreen(ILI9341_BLACK);
	MenuDisplayInit();
	sdcard_check_for_update();

	InitDac();
	InitSPDIF();
	InitI2S();
	InitControls();

	/*
	bB2xx_flash_store_sysparams();
	*/
	LD1_OnOff(LED_OFF);
	LD7_OnOff(LED_OFF);
	LD8_OnOff(LED_OFF);

}

void bB2xx_Set_NO_MicroSD_Flag(void)
{
	SystemVar.microsd_flags |= MICROSD_NOT_PRESENT_FLAG;
}

uint8_t	ld1=0,ld7=0;
extern	MDMA_HandleTypeDef hmdma_mdma_channel40_sw_0;

void bB2xx_Loop(void)
{
	HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON,PWR_SLEEPENTRY_WFI);
	if (( SystemVar.buttons_flags & SW1_PRESSED ) == SW1_PRESSED)
	{
		SystemVar.buttons_flags &= ~SW1_PRESSED;
		ld1++;
		ld1 &=1;
		LD1_OnOff(ld1);
	}
	if (( SystemVar.buttons_flags & SW2_PRESSED ) == SW2_PRESSED)
	{
		SystemVar.buttons_flags &= ~SW2_PRESSED;
		ld7++;
		ld7 &=1;
		LD7_OnOff(ld7);
	}

	if (( SystemVar.timers_flag & TIMER_100MS_FLAG ) == TIMER_100MS_FLAG)
	{
		SystemVar.timers_flag &= ~TIMER_100MS_FLAG;
		if ( SystemVar.uservalue_changed != 0 )
		{
			SystemVar.uservalue_changed--;
			if ( SystemVar.uservalue_changed == 0 )
			{
				bB2xx_flash_store_usrparams();
			}
		}
		I2S_GetAudioBuf();
	}

	if (( SystemVar.encoder_flags & ENCODER_ROTATION_FLAG ) == ENCODER_ROTATION_FLAG)
	{
		if (( SystemVar.system & SYSTEM_MENU_INCDEC) == SYSTEM_MENU_INCDEC)
			MenuEncoderNavigate();
		else
		{
			if (( SystemVar.system & SYSTEM_BPM_INCDEC) == SYSTEM_BPM_INCDEC)
			{
				BPM_Value_Change();
			}
			if (( SystemVar.system & SYSTEM_DELAYVAL_INCDEC) == SYSTEM_DELAYVAL_INCDEC)
			{
				Delay_Value_Change();
			}
			if (( SystemVar.system & SYSTEM_DELAYWEIGHT_INCDEC) == SYSTEM_DELAYWEIGHT_INCDEC)
			{
				Delay_Weight_Change();
			}
		}
		SystemVar.encoder_flags &= ~ENCODER_ROTATION_FLAG;
	}
	if (( SystemVar.encoder_flags & ENCODER_SW_FLAG ) == ENCODER_SW_FLAG)
	{
		if (( SystemVar.system & SYSTEM_MENU_INCDEC) == SYSTEM_MENU_INCDEC)
			MeuEncoderChangeMenu();
		else
		{
			if (( SystemVar.system & SYSTEM_BPM_INCDEC) == SYSTEM_BPM_INCDEC)
			{
				BPM_Value_Draw(0);
			}
			if (( SystemVar.system & SYSTEM_DELAYVAL_INCDEC) == SYSTEM_DELAYVAL_INCDEC)
			{
				Delay_Value_Draw(0);
			}
			if (( SystemVar.system & SYSTEM_DELAYWEIGHT_INCDEC) == SYSTEM_DELAYWEIGHT_INCDEC)
			{
				Delay_Weight_Draw(0);
			}
			SystemVar.system = SYSTEM_MENU_INCDEC;
		}
		SystemVar.encoder_flags = 0;
	}
}

