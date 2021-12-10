/*
 * menus.c
 *
 *  Created on: Sep 20, 2021
 *      Author: fil
 */

#include "main.h"
#include "menus.h"

MenuVars_TypeDef	MenuVars;

Menu_TypeDef	MenuTop[] =
{
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y,
			"Samples & Sequence",
			MENU_ACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+1*MENU_FONT_HEIGHT,
			"Echo - Reverb",
			MENU_INACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+2*MENU_FONT_HEIGHT,
			"Global",
			MENU_INACTIVE_COLOR,
		},
		{
			0,0,
		},
};

Menu_TypeDef	MenuSamplesSequence[] =
{
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y,
			"Copy Samples from SD",
			MENU_ACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+1*MENU_FONT_HEIGHT,
			"Copy Sequence from SD",
			MENU_ACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+2*MENU_FONT_HEIGHT,
			"Return",
			MENU_INACTIVE_COLOR,
		},
		{
			0,0,
		},
};

Menu_TypeDef	MenuDelay[] =
{
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y,
			"Set Delay",
			MENU_ACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+MENU_FONT_HEIGHT,
			"Set Weight",
			MENU_INACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+2*MENU_FONT_HEIGHT,
			"Set Type",
			MENU_INACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+3*MENU_FONT_HEIGHT,
			"Return",
			MENU_INACTIVE_COLOR,
		},
		{
			0,0,
		},
};

Menu_TypeDef	MenuSettings[] =
{
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y,
			"Int Loop Mode",
			MENU_ACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+MENU_FONT_HEIGHT,
			"Ext Sequencer mode",
			MENU_INACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+2*MENU_FONT_HEIGHT,
			"BPM",
			MENU_INACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+3*MENU_FONT_HEIGHT,
			"Clear FLASH",
			MENU_INACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+4*MENU_FONT_HEIGHT,
			"Return",
			MENU_INACTIVE_COLOR,
		},
		{
			0,0,
		},
};

static void DisplayStringAt(uint16_t x , uint16_t y, uint8_t *text,uint8_t normal_color)
{
	if ( normal_color == 0 )
		ILI9341_WriteString(x,y,(char *)text,Font_11x18,ILI9341_BLACK,MENU_ACTIVE_COLOR);
	else
		ILI9341_WriteString(x,y,(char *)text,Font_11x18,MENU_ACTIVE_COLOR,ILI9341_BLACK);
}

static void EraseStringAt(uint16_t x , uint16_t y, uint8_t *text)
{
	ILI9341_WriteString(x,y,(char *)text,Font_11x18,ILI9341_BLACK,ILI9341_BLACK);
}

void MenuHilightItem(Menu_TypeDef *menu)
{
	if ( MenuVars.next_menu_item > menu[0].items)
		MenuVars.next_menu_item = 0;
	DisplayStringAt(menu[MenuVars.selected_menu_item].linex,menu[MenuVars.selected_menu_item].liney,menu[MenuVars.selected_menu_item].text,MENU_NORMAL_COLOR);
	MenuVars.selected_menu_item = MenuVars.next_menu_item;
	DisplayStringAt(menu[MenuVars.selected_menu_item].linex,menu[MenuVars.selected_menu_item].liney,menu[MenuVars.selected_menu_item].text,MENU_HILIGHT_COLOR);
}

void MenuDisplayMenu(Menu_TypeDef *menu )
{
uint8_t	i=0;
	for(i=0;i<=MenuVars.current_menu[0].items;i++)
		EraseStringAt(MenuVars.current_menu[i].linex,MenuVars.current_menu[i].liney, MenuVars.current_menu[i].text);
	for(i=0;i<=menu[0].items;i++)
		DisplayStringAt(menu[i].linex,menu[i].liney, menu[i].text,i);
	MenuVars.next_menu_item = 0;
	MenuVars.selected_menu_item = 0;
	MenuVars.current_menu = menu;
}

void MenuDisplayInit(void)
{
uint8_t	i=0;
	ILI9341_WriteString(STATUS_BAR_X,STATUS_BAR_Y,"bB208 Drum Machine",Font_11x18,ILI9341_RED,ILI9341_BLACK);
	ILI9341_SetBrightness(50);

	for(i=0;i< MENU_MAXLINENUM;i++)
	{
		if(MenuTop[i].liney != 0 )
		{
			MenuVars.current_menu = (Menu_TypeDef * )&MenuTop;
			DisplayStringAt(MenuTop[i].linex,MenuTop[i].liney,MenuTop[i].text,i);
			MenuTop[0].items = i;
		}
		else
			break;
	}
	for(i=0;i< MENU_MAXLINENUM;i++)
	{
		if(MenuSamplesSequence[i].liney != 0 )
			MenuSamplesSequence[0].items = i;
		else
			break;
	}
	for(i=0;i< MENU_MAXLINENUM;i++)
	{
		if(MenuDelay[i].liney != 0 )
			MenuDelay[0].items = i;
		else
			break;
	}
	for(i=0;i< MENU_MAXLINENUM;i++)
	{
		if(MenuSettings[i].liney != 0 )
			MenuSettings[0].items = i;
		else
			break;
	}
	SystemVar.lcd_flags = 0;
	HAL_Delay(100);
	ILI9341_DrawBitmap(DLY_DIGIT_XPOSH,DLY_DIGIT_YPOS,(uint8_t *)&ram_blue_digits[0]);

	Delay_Value_Draw(0);
	Delay_Weight_Draw(0);
	BPM_Value_Draw(0);
}

void MenuEncoderNavigate(void)
{
	if (( SystemVar.encoder_flags & ENCODER_ROTATION_FLAG) == ENCODER_ROTATION_FLAG)
	{
		if (( SystemVar.encoder_flags & ENCODER_INCREMENT_FLAG) == ENCODER_INCREMENT_FLAG)
		{
			if ( MenuVars.next_menu_item == 0 )
				MenuVars.next_menu_item = MenuVars.current_menu[0].items;
			else
				MenuVars.next_menu_item--;
			SystemVar.encoder_flags &= ~ENCODER_INCREMENT_FLAG;
		}
		if (( SystemVar.encoder_flags & ENCODER_DECREMENT_FLAG) == ENCODER_DECREMENT_FLAG)
		{
			MenuVars.next_menu_item++;
			if ( MenuVars.next_menu_item > MenuVars.current_menu[0].items )
				MenuVars.next_menu_item = 0;
			SystemVar.encoder_flags &= ~ENCODER_DECREMENT_FLAG;
		}
		SystemVar.encoder_flags &= ~ENCODER_ROTATION_FLAG;
	}
	MenuHilightItem(MenuVars.current_menu);
}

void MeuEncoderChangeMenu(void)
{
	switch(MenuVars.menu_state)
	{
	case	MENU_TOP	:
		if ( MenuVars.next_menu_item == 0 )
		{
			MenuDisplayMenu(MenuSamplesSequence);
			MenuVars.menu_state = MENU_SAMPLES;
		}
		if ( MenuVars.next_menu_item == 1 )
		{
			MenuDisplayMenu(MenuDelay);
			MenuVars.menu_state = MENU_DELAY;
		}
		if ( MenuVars.next_menu_item == 2 )
		{
			MenuDisplayMenu(MenuSettings);
			MenuVars.menu_state = MENU_SETTINGS;
		}
		break;
	case	MENU_SAMPLES	:
		/*
		if ( SystemVar.next_menu_item == 0 )
			ReadDescriptorFileFromUSB(MENU_LINE_0_Y+MENU_FONT_HEIGHT+MenuSamples[0].items*MENU_FONT_HEIGHT);
		if ( SystemVar.next_menu_item == 2 )
			QSPI_ParseWavUSB_AndWrite();
			*/
		if ( MenuVars.next_menu_item == MenuSamplesSequence[0].items)
		{
			MenuVars.menu_state = MENU_TOP;
			MenuDisplayMenu(MenuTop);
		}
		break;
	case	MENU_DELAY	:
		if ( MenuVars.next_menu_item == 0)
		{
			SystemVar.system |= SYSTEM_DELAYVAL_INCDEC;
			SystemVar.system &= ~SYSTEM_MENU_INCDEC;
			Delay_Value_Draw(1);
			return;
		}
		if ( MenuVars.next_menu_item == 1)
		{
			SystemVar.system |= SYSTEM_DELAYWEIGHT_INCDEC;
			SystemVar.system &= ~SYSTEM_MENU_INCDEC;
			Delay_Weight_Draw(1);
			return;
		}
		/*
		if ( SystemVar.next_menu_item == 2)
		{
			if (( SystemVar.delay_type & DELAY_TYPE_FLANGER) == DELAY_TYPE_FLANGER)
			{
				SystemVar.delay_type |= DELAY_TYPE_ECHO;
				SystemVar.delay_type &= ~DELAY_TYPE_FLANGER;
			}
			else if (( SystemVar.delay_type & DELAY_TYPE_ECHO) == DELAY_TYPE_ECHO)
			{
				SystemVar.delay_type &= ~DELAY_TYPE_FLANGER;
				SystemVar.delay_type &= ~DELAY_TYPE_ECHO;
			}
			else
			{
				SystemVar.delay_type |= DELAY_TYPE_FLANGER;
			}
			DelayTypeDisplay();
			return;
		}
		*/
		if ( MenuVars.next_menu_item == MenuDelay[0].items)
		{
			MenuVars.menu_state = MENU_TOP;
			MenuDisplayMenu(MenuTop);
		}
		break;
	case	MENU_SETTINGS	:
		/*
		if ( SystemVar.next_menu_item == 0)
		{
			SystemVar.system |= SYSTEM_INTEXT_SEQUENCER;
			return;
		}
		if ( SystemVar.next_menu_item == 1)
		{
			SystemVar.system &= ~SYSTEM_INTEXT_SEQUENCER;
			return;
		}
		*/
		if ( MenuVars.next_menu_item == 2)
		{
			SystemVar.system |= SYSTEM_BPM_INCDEC;
			SystemVar.system &= ~SYSTEM_MENU_INCDEC;
			BPM_Value_Draw(1);
			return;
		}
		/*
		if ( SystemVar.next_menu_item == 3)
		{

			SystemVar.system |= SYSTEM_ERASE_IN_PROGRESS;
			BSP_QSPI_Erase_Chip();
			SystemVar.system &= ~SYSTEM_ERASE_IN_PROGRESS;
			BSP_LCD_DisplayStringAt(0, DLY_TITLE_TEXT_Y-40, (uint8_t *)"                 ", CENTER_MODE);
		}
		*/
		if ( MenuVars.next_menu_item == MenuSettings[0].items)
		{
			MenuVars.menu_state = MENU_TOP;
			MenuDisplayMenu(MenuTop);
		}
		break;
	}
}



