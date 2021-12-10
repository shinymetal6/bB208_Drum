/*
 * sdcard.c
 *
 *  Created on: Nov 15, 2021
 *      Author: fil
 */

#include "main.h"
#include "sdcard.h"
#include "fatfs.h"
#include "Core/bB2xx_flash.h"
#include <stdio.h>

FATFS fileSystem;
FIL digitFile[10];
uint8_t retSD;    /* Return value for SD */
char SD_Path[4];  /* SD logical drive path */
uint8_t	tries[10];
extern	char SDPath[4];   /* SD logical drive path */
char			image_full_path[32];

uint8_t sdcard_get_digits( void )
{
uint8_t			*digit_ptr;
uint32_t		digits_address;
char 			image_path[32];
unsigned int	readbytes,digit,i;
FILINFO 		fno;

	for(i=0;i<10;i++,digits_address += SECTOR_SIZE)
	{
		for(digit = DIGIT_BLUE_IDX; digit <= DIGIT_ORANGE_IDX; digit++)
		{
			switch ( digit )
			{
			case	DIGIT_BLUE_IDX :
				digit_ptr = (uint8_t *)&ram_blue_digits[i];
				sprintf(image_path,"/system/images/blue%d.bmp",i);
				break;
			case	DIGIT_RED_IDX :
				digit_ptr = (uint8_t *)&ram_red_digits[i];
				sprintf(image_path,"/system/images/red%d.bmp",i);
				break;
			case	DIGIT_GREEN_IDX :
				digit_ptr = (uint8_t *)&ram_green_digits[i];
				sprintf(image_path,"/system/images/green%d.bmp",i);
				break;
			case	DIGIT_YELLOW_IDX :
				digit_ptr = (uint8_t *)&ram_yellow_digits[i];
				sprintf(image_path,"/system/images/yellow%d.bmp",i);
				break;
			case	DIGIT_ORANGE_IDX :
				digit_ptr = (uint8_t *)&ram_orange_digits[i];
				sprintf(image_path,"/system/images/orange%d.bmp",i);
				break;
			default : return 1;
			}

			f_stat(image_path, &fno);
			if(f_open(&digitFile[i], image_path, FA_OPEN_EXISTING | FA_READ) == FR_OK)
			{
				readbytes = 0;
				while ( readbytes != fno.fsize)
					f_read(&digitFile[i],digit_ptr,fno.fsize,&readbytes);
				f_close(&digitFile[i]);
			}
			else
				return 1;
		}
	}
	return 0;
}

uint8_t sdcard_check_for_update(void)
{
FIL 	updateFile;
#define	UPDATE_IF	1
	if(f_mount(&fileSystem, SDPath, 1) != FR_OK)
		return 1;
#ifdef UPDATE_IF
	if(f_open(&updateFile, "/system/update.txt", FA_OPEN_EXISTING | FA_READ) == FR_OK)
	{
		sdcard_get_digits();
		bB2xx_flash_store_digits();
		bB2xx_flash_get_digits();
		f_close(&updateFile);
		f_unlink("/system/update.txt");
	}
#endif
	return 0;

}

