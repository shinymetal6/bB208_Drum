/*
 * irq_manager.c
 *
 *  Created on: Nov 9, 2021
 *      Author: fil
 */

#include "main.h"
#include "irq_manager.h"

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	if ( hspi == &ILI9341_SPI_PORT )
	{
		ILI9341_SPI_TxEnd_Callback();
	}
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if ( hspi == &ILI9341_SPI_PORT )
	{
		ILI9341_SPI_TxEnd_Callback();
	}
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if ( hspi == &FLASH_SPI_PORT )
	{
		w25qxx_SPI_TxEnd_Callback();
	}
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
	SPDIF_TxHalfCpltCallback();
}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
	SPDIF_TxCpltCallback();
}
