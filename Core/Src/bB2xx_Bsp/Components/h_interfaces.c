/*
 * h_interfaces.c
 *
 *  Created on: Oct 27, 2021
 *      Author: fil
 */

#include "main.h"
#include "h_interfaces.h"

void encoder_rotation_callback(void)
{
	if ( SystemVar.encoder_rotation_debounce == 0 )
	{
		SystemVar.last_encval = SystemVar.encval;
		SystemVar.encval = TIM4->CNT;
		if ( SystemVar.last_encval != SystemVar.encval)
		{
			SystemVar.encoder_flags &= ~(ENCODER_INCREMENT_FLAG | ENCODER_DECREMENT_FLAG);
			if ( SystemVar.last_encval < SystemVar.encval)
				SystemVar.encoder_flags |= ENCODER_INCREMENT_FLAG;
			else
				SystemVar.encoder_flags |= ENCODER_DECREMENT_FLAG;
			SystemVar.encoder_flags |= ENCODER_ROTATION_FLAG;
		}
		SystemVar.encoder_rotation_debounce = 1;
	}
}

void encoder_switch_callback(void)
{
	if ( HAL_GPIO_ReadPin(ENCODER_BTN_GPIO_Port, ENCODER_BTN_Pin) == 0 )
		SystemVar.encoder_flags |= ENCODER_SW_FLAG;
}

uint8_t encoder_get(void)
{
	if (( SystemVar.encoder_flags & ENCODER_INCREMENT_FLAG) == ENCODER_INCREMENT_FLAG)
	{
		SystemVar.encoder_flags &= ~(ENCODER_ROTATION_FLAG | ENCODER_INCREMENT_FLAG);
		return ENCODER_PLUS;
	}
	if (( SystemVar.encoder_flags & ENCODER_DECREMENT_FLAG) == ENCODER_DECREMENT_FLAG)
	{
		SystemVar.encoder_flags &= ~(ENCODER_ROTATION_FLAG | ENCODER_DECREMENT_FLAG);
		return ENCODER_MINUS;
	}
	SystemVar.encoder_flags &= ~ENCODER_ROTATION_FLAG;
	return ENCODER_NONE;
}

uint8_t encoder_get_btn(void)
{
	if (( SystemVar.encoder_flags & ENCODER_SW_FLAG) == ENCODER_SW_FLAG)
		return 1;
	else
		return 0;
}

void LD1_OnOff(uint8_t value)
{
	  HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, value);
}

void LD7_OnOff(uint8_t value)
{
	  HAL_GPIO_WritePin(LD7_GPIO_Port, LD7_Pin, value);
}

void LD8_OnOff(uint8_t value)
{
	  HAL_GPIO_WritePin(LD8_GPIO_Port, LD8_Pin, value);
}

void check_buttons(void)
{
	if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == 0 )
	{
		if ( SystemVar.sw1_debounce == 0 )
		{
			SystemVar.buttons_flags |= SW1_PRESSED;
			SystemVar.sw1_debounce = 3;
		}
	}
	else
	{
		SystemVar.buttons_flags &= ~SW1_PRESSED;
	}

	if (HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == 0 )
	{
		if ( SystemVar.sw2_debounce == 0 )
		{
			SystemVar.buttons_flags |= SW2_PRESSED;
			SystemVar.sw2_debounce = 3;
		}
	}
	else
	{
		SystemVar.buttons_flags &= ~SW2_PRESSED;
	}
}

void InitControls(void)
{
	HAL_ADC_Start_DMA(&CONTROL_DMA, (uint32_t *)SystemVar.rvar, 4);
}
