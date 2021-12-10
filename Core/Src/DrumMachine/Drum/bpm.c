/*
 * bpm.c
 *
 *  Created on: Nov 17, 2021
 *      Author: fil
 */


#include "main.h"
#include "bpm.h"
#include "../Hmi/menus.h"

void BPM_Value_Draw(uint8_t hilight)
{
uint32_t 	h,t,u;  // hundreds,tens,units

	u = UserParameters.bpm%10;
	t = (UserParameters.bpm/10)%10;
	h = (UserParameters.bpm/100)%10;

	if ( hilight == 0 )
	{
		ILI9341_DrawBitmap(BPM_DIGIT_XPOSH,BPM_DIGIT_YPOS,(uint8_t *)&ram_blue_digits[h]);
		ILI9341_DrawBitmap(BPM_DIGIT_XPOST,BPM_DIGIT_YPOS,(uint8_t *)&ram_blue_digits[t]);
		ILI9341_DrawBitmap(BPM_DIGIT_XPOSU,BPM_DIGIT_YPOS,(uint8_t *)&ram_blue_digits[u]);
	}
	else
	{
		ILI9341_DrawBitmap(BPM_DIGIT_XPOSH,BPM_DIGIT_YPOS,(uint8_t *)&ram_red_digits[h]);
		ILI9341_DrawBitmap(BPM_DIGIT_XPOST,BPM_DIGIT_YPOS,(uint8_t *)&ram_red_digits[t]);
		ILI9341_DrawBitmap(BPM_DIGIT_XPOSU,BPM_DIGIT_YPOS,(uint8_t *)&ram_red_digits[u]);
	}
}

void BPM_Value_Change(void)
{
	if (( SystemVar.encoder_flags & ENCODER_ROTATION_FLAG) == ENCODER_ROTATION_FLAG)
	{
		if (( SystemVar.encoder_flags & ENCODER_INCREMENT_FLAG) == ENCODER_INCREMENT_FLAG)
		{
			UserParameters.bpm++;
			if ( UserParameters.bpm > 180 )
				UserParameters.bpm = 180;
		}
		if (( SystemVar.encoder_flags & ENCODER_DECREMENT_FLAG) == ENCODER_DECREMENT_FLAG)
		{
			UserParameters.bpm--;
			if ( UserParameters.bpm < 10 )
				UserParameters.bpm = 10;
		}
	}
	BPM_Value_Draw(1);
	SystemVar.uservalue_changed = STORE_USERVALUES_TIMEOUT;
}

