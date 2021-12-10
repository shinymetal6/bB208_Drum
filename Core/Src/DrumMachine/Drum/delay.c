/*
 * delay.c
 *
 *  Created on: Nov 17, 2021
 *      Author: fil
 */


#include "main.h"
#include "delay.h"
#include "../Hmi/menus.h"

void Delay_Value_Draw(uint8_t hilight)
{
uint32_t 	h,t,u;  // hundreds,tens,units

	u = UserParameters.delay_value%10;
	t = (UserParameters.delay_value/10)%10;
	h = (UserParameters.delay_value/100)%10;

	if ( hilight == 0 )
	{
		ILI9341_DrawBitmap(DLY_DIGIT_XPOSH,DLY_DIGIT_YPOS,(uint8_t *)&ram_blue_digits[h]);
		ILI9341_DrawBitmap(DLY_DIGIT_XPOST,DLY_DIGIT_YPOS,(uint8_t *)&ram_blue_digits[t]);
		ILI9341_DrawBitmap(DLY_DIGIT_XPOSU,DLY_DIGIT_YPOS,(uint8_t *)&ram_blue_digits[u]);
	}
	else
	{
		ILI9341_DrawBitmap(DLY_DIGIT_XPOSH,DLY_DIGIT_YPOS,(uint8_t *)&ram_red_digits[h]);
		ILI9341_DrawBitmap(DLY_DIGIT_XPOST,DLY_DIGIT_YPOS,(uint8_t *)&ram_red_digits[t]);
		ILI9341_DrawBitmap(DLY_DIGIT_XPOSU,DLY_DIGIT_YPOS,(uint8_t *)&ram_red_digits[u]);
	}
}

void Delay_Weight_Draw(uint8_t hilight)
{
uint32_t 	t,u;  // hundreds,tens,units

	u = UserParameters.delay_weight_int%10;
	t = (UserParameters.delay_weight_int/10)%10;

	if ( hilight == 0 )
	{
		ILI9341_DrawBitmap(DLYW_DIGIT_XPOST,DLYW_DIGIT_YPOS,(uint8_t *)&ram_blue_digits[t]);
		ILI9341_DrawBitmap(DLYW_DIGIT_XPOSU,DLYW_DIGIT_YPOS,(uint8_t *)&ram_blue_digits[u]);
	}
	else
	{
		ILI9341_DrawBitmap(DLYW_DIGIT_XPOST,DLYW_DIGIT_YPOS,(uint8_t *)&ram_red_digits[t]);
		ILI9341_DrawBitmap(DLYW_DIGIT_XPOSU,DLYW_DIGIT_YPOS,(uint8_t *)&ram_red_digits[u]);
	}
}


void Delay_Value_Change(void)
{
	if (( SystemVar.encoder_flags & ENCODER_ROTATION_FLAG) == ENCODER_ROTATION_FLAG)
	{
		if (( SystemVar.encoder_flags & ENCODER_INCREMENT_FLAG) == ENCODER_INCREMENT_FLAG)
		{
			UserParameters.delay_value++;
			if ( UserParameters.delay_value > 900 )
				UserParameters.delay_value = 900;
		}
		if (( SystemVar.encoder_flags & ENCODER_DECREMENT_FLAG) == ENCODER_DECREMENT_FLAG)
		{
			UserParameters.delay_value--;
			if ( UserParameters.delay_value < 1 )
				UserParameters.delay_value = 1;
		}
	}
	SystemVar.encoder_flags = 0;
	Delay_Value_Draw(1);
	SystemVar.uservalue_changed = STORE_USERVALUES_TIMEOUT;
}

void Delay_Weight_Change(void)
{
	if (( SystemVar.encoder_flags & ENCODER_ROTATION_FLAG) == ENCODER_ROTATION_FLAG)
	{
		if (( SystemVar.encoder_flags & ENCODER_INCREMENT_FLAG) == ENCODER_INCREMENT_FLAG)
		{
			UserParameters.delay_weight_int++;
			if ( UserParameters.delay_weight_int > 90 )
				UserParameters.delay_weight_int = 90;
		}
		if (( SystemVar.encoder_flags & ENCODER_DECREMENT_FLAG) == ENCODER_DECREMENT_FLAG)
		{
			UserParameters.delay_weight_int--;
			if ( UserParameters.delay_weight_int < 10 )
				UserParameters.delay_weight_int = 10;
		}
	}
	SystemVar.encoder_flags = 0;
	Delay_Weight_Draw(1);
	SystemVar.uservalue_changed = STORE_USERVALUES_TIMEOUT;
}

