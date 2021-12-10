/*
 * bB2xx_flash.c
 *
 *  Created on: Oct 29, 2021
 *      Author: fil
 */

#include "main.h"
#include <string.h>
#include <stdio.h>

FlashConfig_TypeDef		FlashConfig,ReadFlashConfig;
//SamplesConfig_TypeDef	SamplesConfig[NUMSAMPLES_F256];
uint32_t	flash_id;

uint8_t bB2xx_flash_identify(void)
{
	flash_id = w25qxx_ReadID();
	flash_id = w25qxx_ReadID();
	flash_id = w25qxx_ReadID();
	SystemParameters.flash_capacity = (uint8_t )(flash_id & 0xff);
	if ( SystemParameters.flash_capacity != F128 )
		return 1;

	FlashConfig.params_pointer = PARAMS_PTR_F128;
	FlashConfig.usrparams_pointer = USRPARAMS_PTR_F128;
	FlashConfig.sequence_pointer = USRSEQ_PTR_F128;
	FlashConfig.sample_pointer = SAMPLE_PTR_F128;
	FlashConfig.digitblue_pointer = BLUE_DIGIT_FPTR;
	FlashConfig.digitred_pointer = RED_DIGIT_FPTR;
	FlashConfig.digitgreen_pointer = GREEN_DIGIT_FPTR;
	FlashConfig.digityellow_pointer = YELLOW_DIGIT_FPTR;
	FlashConfig.digitorange_pointer = ORANGE_DIGIT_FPTR;
	FlashConfig.splashimage_pointer = LOGO_FPTR;

	return 0;
}

uint8_t bB2xx_flash_init(void)
{
	return bB2xx_flash_identify();
}

void bB2xx_flash_store_sysparams(void)
{
	w25qxx_EraseSector(w25qxx_AddressToSector(FlashConfig.params_pointer));
	w25qxx_WriteBytes((uint8_t *)&SystemParameters, FlashConfig.params_pointer, sizeof(SystemParameters));
}

void bB2xx_flash_store_usrparams(void)
{
	w25qxx_EraseSector(w25qxx_AddressToSector(FlashConfig.usrparams_pointer));
	w25qxx_WriteBytes((uint8_t *)&UserParameters, FlashConfig.usrparams_pointer, sizeof(UserParameters));
}

void bB2xx_flash_store_digits( void )
{
uint8_t		*digit_ptr;
uint32_t	digits_address,digits_sector,digit,i;

	for(digit = DIGIT_BLUE_IDX; digit <= DIGIT_ORANGE_IDX; digit++)
	{
		switch ( digit )
		{
		case	DIGIT_BLUE_IDX :
			digits_sector = w25qxx_AddressToSector(FlashConfig.digitblue_pointer);
			digits_address = FlashConfig.digitblue_pointer;
			digit_ptr = (uint8_t *)&ram_blue_digits[0];
			break;
		case	DIGIT_RED_IDX :
			digits_sector = w25qxx_AddressToSector(FlashConfig.digitred_pointer);
			digits_address = FlashConfig.digitred_pointer;
			digit_ptr = (uint8_t *)&ram_red_digits[0];
			break;
		case	DIGIT_GREEN_IDX :
			digits_sector = w25qxx_AddressToSector(FlashConfig.digitgreen_pointer);
			digits_address = FlashConfig.digitgreen_pointer;
			digit_ptr = (uint8_t *)&ram_green_digits[0];
			break;
		case	DIGIT_YELLOW_IDX :
			digits_sector = w25qxx_AddressToSector(FlashConfig.digityellow_pointer);
			digits_address = FlashConfig.digityellow_pointer;
			digit_ptr = (uint8_t *)&ram_yellow_digits[0];
			break;
		case	DIGIT_ORANGE_IDX :
			digits_sector = w25qxx_AddressToSector(FlashConfig.digitorange_pointer);
			digits_address = FlashConfig.digitorange_pointer;
			digit_ptr = (uint8_t *)&ram_orange_digits[0];
			break;
		default : return;
		}
		for(i=0;i<10;i++,digits_sector++)
		{
			w25qxx_EraseSector(digits_sector);
			HAL_Delay(1);
		}
		for(i=0;i<10;i++)
		{
			w25qxx_WriteBytes(digit_ptr, digits_address,SECTOR_SIZE );
			digit_ptr += DIGITS_SIZE;
			digits_address += SECTOR_SIZE;
			HAL_Delay(1);
		}
	}
}

void bB2xx_flash_get_digits( void )
{
uint8_t		*digit_ptr;
uint32_t	digits_address,digit,i;

	for(digit = DIGIT_BLUE_IDX; digit <= DIGIT_ORANGE_IDX; digit++)
	{
		switch ( digit )
		{
		case	DIGIT_BLUE_IDX :
			digits_address = FlashConfig.digitblue_pointer;
			digit_ptr = (uint8_t *)&ram_blue_digits[0];
			break;
		case	DIGIT_RED_IDX :
			digits_address = FlashConfig.digitred_pointer;
			digit_ptr = (uint8_t *)&ram_red_digits[0];
			break;
		case	DIGIT_GREEN_IDX :
			digits_address = FlashConfig.digitgreen_pointer;
			digit_ptr = (uint8_t *)&ram_green_digits[0];
			break;
		case	DIGIT_YELLOW_IDX :
			digits_address = FlashConfig.digityellow_pointer;
			digit_ptr = (uint8_t *)&ram_yellow_digits[0];
			break;
		case	DIGIT_ORANGE_IDX :
			digits_address = FlashConfig.digitorange_pointer;
			digit_ptr = (uint8_t *)&ram_orange_digits[0];
			break;
		default : return;
		}
		for(i=0;i<10;i++)
		{
			w25qxx_ReadBytesDMA(digit_ptr, digits_address, DIGITS_SIZE);
			digit_ptr += DIGITS_SIZE;
			digits_address += SECTOR_SIZE;
		}
	}
}

void bB2xx_flash_get_usrsparams(void)
{
	w25qxx_ReadBytes((uint8_t *)&UserParameters, FlashConfig.usrparams_pointer, sizeof(UserParameters));
}

void bB2xx_flash_get_usrsequence(void)
{
}

void bB2xx_flash_get_sysparams(void)
{
	w25qxx_ReadBytes((uint8_t *)&SystemParameters, FlashConfig.params_pointer, sizeof(SystemParameters));
	if (( SystemParameters.id[0] == 'b') && ( SystemParameters.id[1] == 'B') && ( SystemParameters.id[2] == '2')&& ( SystemParameters.flash_capacity == F128))
	{
		FlashConfig.params_pointer = PARAMS_PTR_F128;
		FlashConfig.sequence_pointer = USRSEQ_PTR_F128;
		FlashConfig.sample_pointer = SAMPLE_PTR_F128;
		FlashConfig.digitblue_pointer = BLUE_DIGIT_FPTR;
		FlashConfig.digitred_pointer = RED_DIGIT_FPTR;
		FlashConfig.digitgreen_pointer = GREEN_DIGIT_FPTR;
		FlashConfig.digityellow_pointer = YELLOW_DIGIT_FPTR;
		FlashConfig.digitorange_pointer = ORANGE_DIGIT_FPTR;
		FlashConfig.splashimage_pointer = LOGO_FPTR;
		bB2xx_flash_get_digits();
		bB2xx_flash_get_usrsparams();
		bB2xx_flash_get_usrsequence();
		return;
	}
	else
	{
		SystemParameters.id[0] = 'b';
		SystemParameters.id[1] = 'B';
		SystemParameters.id[2] = '2';
		SystemParameters.id[3] = 'x';
		SystemParameters.touch_x_minimum = 0;
		SystemParameters.touch_y_minimum = 0;
		SystemParameters.flash_capacity = F128;
		bB2xx_flash_store_sysparams();
		w25qxx_ReadBytes((uint8_t *)&SystemParameters, FlashConfig.params_pointer, sizeof(SystemParameters));
	}
}
