/*
 * w25qxx.c
 *
 *  Created on: Oct 18, 2021
 *      Author: fil
 */


#include "main.h"
#include "../Core/bB2xx_System.h"
#include "w25qxx.h"

static uint8_t	w25qxx_SpiTX(uint8_t	Data)
{
uint8_t	ret;
	HAL_SPI_TransmitReceive(&FLASH_SPI_PORT,&Data,&ret,1,100);
	return ret;
}

static void w25qxx_WaitForWriteEnd(void)
{
uint8_t		StatusRegister1;
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	w25qxx_SpiTX(GETSTATUS_COMMAND);
	do
	{
		StatusRegister1 = w25qxx_SpiTX(W25Q_DUMMY_BYTE);
		HAL_Delay(1);
	}
	while ((StatusRegister1 & 0x01) == 0x01);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
}

static void w25qxx_WriteEnable(void)
{
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	w25qxx_SpiTX(WRITE_ENABLE_COMMAND);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
}

void w25qxx_WriteDisable(void)
{
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	w25qxx_SpiTX(WRITE_DISABLE_COMMAND);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
}

uint32_t	w25qxx_SectorToAddress(uint32_t	Sector)
{
	return Sector * SECTOR_SIZE;
}

uint32_t	w25qxx_AddressToSector(uint32_t	Address)
{
	return Address / SECTOR_SIZE;
}

static uint32_t send_cmd_addr(uint8_t cmd,uint32_t Address)
{
uint32_t	ret = 0;
	w25qxx_SpiTX(cmd);
	if ( SystemParameters.flash_capacity == F256 )
		ret = w25qxx_SpiTX((Address & 0xFF000000) >> 24) << 24;
	ret |= w25qxx_SpiTX((Address & 0xFF0000) >> 16) << 16;
	ret |= w25qxx_SpiTX((Address & 0xFF00) >> 8) << 8;
	ret |= w25qxx_SpiTX(Address & 0xFF);
	return ret;
}

void w25qxx_EraseSector(uint32_t Sector)
{
uint32_t	SectorAddr;
	SectorAddr = Sector * SECTOR_SIZE;
	w25qxx_WriteEnable();
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	send_cmd_addr(SECTOR_ERASE_COMMAND,SectorAddr);
	HAL_Delay(1);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
	HAL_Delay(50);
	w25qxx_WaitForWriteEnd();
}

void w25qxx_ReadBytes(uint8_t* pBuffer, uint32_t Address, uint32_t size)
{
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	send_cmd_addr(READ_COMMAND,Address);
	HAL_SPI_Receive(&FLASH_SPI_PORT,pBuffer,size,2000);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
}

void w25qxx_SPI_TxEnd_Callback(void)
{
	SystemVar.flash_flags &= ~FLASH_DMARUNNING_FLAG;
}

void w25qxx_ReadBytesDMA(uint8_t* pBuffer, uint32_t Address, uint32_t size)
{
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	send_cmd_addr(READ_COMMAND,Address);
	SystemVar.flash_flags |= FLASH_DMARUNNING_FLAG;
	HAL_SPI_Receive_DMA(&FLASH_SPI_PORT,pBuffer,size);
	HAL_GPIO_WritePin(FLAG_GPIO_Port, FLAG_Pin, GPIO_PIN_SET);
	while ((SystemVar.flash_flags & FLASH_DMARUNNING_FLAG) == FLASH_DMARUNNING_FLAG);
	HAL_GPIO_WritePin(FLAG_GPIO_Port, FLAG_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
}

static void w25qxx_WritePage(uint8_t* pBuffer, uint32_t Address, uint32_t size)
{
	w25qxx_WriteEnable();
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	send_cmd_addr(WRITE_COMMAND,Address);
	HAL_SPI_Transmit(&FLASH_SPI_PORT,pBuffer,size,100);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
	HAL_Delay(3);
	w25qxx_WaitForWriteEnd();
}

void w25qxx_WriteBytes(uint8_t* pBuffer, uint32_t Address, uint32_t size)
{
	while(size > PAGE_SIZE)
	{
		w25qxx_WritePage(pBuffer, Address, PAGE_SIZE);
		size -= PAGE_SIZE;
		Address += PAGE_SIZE;
		pBuffer += PAGE_SIZE;
	}
	if ( size > 0 )
		w25qxx_WritePage(pBuffer, Address, size);
}

uint32_t w25qxx_ReadID(void)
{
uint32_t fake_addr, ret_val;
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	w25qxx_SpiTX(READ_ID_COMMAND);
	fake_addr = (W25Q_DUMMY_BYTE << 24 )|(W25Q_DUMMY_BYTE << 16 )|(W25Q_DUMMY_BYTE << 8 )|(W25Q_DUMMY_BYTE << 0 );
	ret_val = send_cmd_addr(READ_ID_COMMAND,fake_addr) >> 8;
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
	return ret_val;
}

uint8_t w25qxx_PollChipErase(void)
{
uint8_t		StatusRegister1; // 1 if operation in progress, 0 if finished
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	w25qxx_SpiTX(GETSTATUS_COMMAND);
	StatusRegister1 = w25qxx_SpiTX(W25Q_DUMMY_BYTE);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
	return StatusRegister1;
//	while ((StatusRegister1 & 0x01) == 0x01);
}

void w25qxx_ChipErase(void)
{
	w25qxx_WriteEnable();
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	w25qxx_SpiTX(CHIP_ERASE_COMMAND);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
	HAL_Delay(1);
}

