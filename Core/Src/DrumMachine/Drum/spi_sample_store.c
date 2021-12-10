/*
 * spi_sample_store.c
 *
 *  Created on: Sep 17, 2021
 *      Author: fil
 */

#include "main.h"
#include <stdio.h>
#include <string.h>
#include "fatfs.h"
#include "spi_sample_store.h"
#include "delay.h"

extern	FlashConfig_TypeDef		FlashConfig;

uint8_t	line[256];
FIL								WavFile,ConfFile;
SPISample_DescriptorTypeDef		SPISample_Descriptor;
SampleWAV_FormatTypeDef 		wav_info;
uint8_t							spi_buf[SECTOR_SIZE];
uint16_t						spi_sec_buf[SECTOR_SIZE/2];


extern	SPISample_HeaderTypeDef SPISample_Header[NUM_INSTRUMENT];

uint32_t SPI_WriteSeqFromSD(void)
{
	w25qxx_EraseSector(w25qxx_AddressToSector(FlashConfig.sequence_pointer));
	w25qxx_WriteBytes((uint8_t *)&Sequencer_Descriptor, FlashConfig.sequence_pointer, sizeof(Sequencer_Descriptor));
	return 1;
}


uint32_t SPI_WriteWavFromSD(uint8_t instrument_number , uint8_t *filename , uint8_t midi_key)
{
uint32_t 	bytesread , sector_number=0;
uint32_t	instrument_address;

	if(f_open(&WavFile, (char *)filename, FA_OPEN_EXISTING | FA_READ) == FR_OK)
	{
		if(f_read(&WavFile, &wav_info, sizeof(wav_info), (void *)&bytesread) == FR_OK)
		{
			sprintf((char *)SPISample_Header[instrument_number].wav_name,(char *)filename);
			SPISample_Header[instrument_number].midi_key = midi_key;
			SPISample_Header[instrument_number].wav_len = wav_info.FileSize;
			SPISample_Header[instrument_number].sample_rate = wav_info.SampleRate;

			for(sector_number=0;sector_number < NUMSECTOR4K_PER_INSTRUMENT;sector_number++)
			{
				if(f_read(&WavFile,	(uint8_t *)spi_buf,SECTOR_SIZE,(void *)&bytesread) != FR_OK)
					return 1;
				if ( bytesread != 0 )
				{
					instrument_address = (instrument_number * INSTRUMENT_SIZE) + (sector_number * SECTOR_SIZE);
					w25qxx_EraseSector(instrument_address);
					w25qxx_WriteBytes(spi_buf, instrument_address, bytesread);
				}
				HAL_Delay(1);
			}
			f_close(&WavFile);
			return 0;
		}
	}
	return 1;
}

void SPI_ReadAndDumpDescriptor(void)
{
uint8_t 	i;
uint8_t		lcdstring[32];
uint32_t	instrument_address;
uint32_t	lcdline=90;

	for(i=0;i<NUM_INSTRUMENT;i++,lcdline+=10)
	{
		instrument_address = i * INSTRUMENT_SIZE;
		w25qxx_ReadBytesDMA((uint8_t *)&SPISample_Header[i], instrument_address, HEADER_SIZE);
		sprintf((char *)lcdstring,"%d %d 0x%02x %s",
				i,
				(int )  SPISample_Header[i].sample_rate,
				(int )  SPISample_Header[i].midi_key,
				(char *)SPISample_Header[i].wav_name);
		SPISample_Descriptor.midi_key[i] = SPISample_Header[i].midi_key;
		SPISample_Descriptor.sample_len[i] = SPISample_Header[i].wav_len;
		SPISample_Descriptor.qspi_ptr[i] = 0;
	}
}

void SPI_ParseWavSD_AndWrite(void)
{
uint32_t	i;
int			instrument_number,midi_key;
uint8_t 	filename[32],len;

	if(f_open(&ConfFile, "bb_sam.txt", FA_OPEN_EXISTING | FA_READ) == FR_OK)
	{
		for(i=0;i<NUM_INSTRUMENT;i++)
		{
			f_gets((char * )line,256,&ConfFile);
			if ( (len = strlen((char * )line)) != 0 )
			{
				if ( sscanf((char * )line,"%d %s %d",&instrument_number,filename,&midi_key) == 3 )
				{
					SPI_WriteWavFromSD(instrument_number,filename,midi_key);
				}
			}
		}
		f_close(&ConfFile);
	}
}

void SPI_ParseSeqSD_AndWrite(void)
{
uint32_t	params=0,j,val,line_idx=0,end=0;
uint8_t 	len;
uint8_t 	delay_type[16];
int		 	seq[16];

	if(f_open(&ConfFile, "bb_seq.txt", FA_OPEN_EXISTING | FA_READ) == FR_OK)
	{
		while(end == 0)
		{
			f_gets((char * )line,256,&ConfFile);
			if ( (len = strlen((char * )line)) != 0 )
			{
				if ( params == 0 )
				{
					if ( sscanf((char * )line,"BEAT %d",(int *)&seq[0]) == 1 )
					{
						if ( seq[0] <= MAX_BEAT)
							Sequencer_Descriptor.sequencer_beat = seq[0];
					}
					else if ( sscanf((char * )line,"DELAY %s %d %d",delay_type,(int *)&seq[0],(int *)&seq[1]) == 3 )
					{
						if ( strcmp((char * )delay_type,"ECHO") == 0 )
							Sequencer_Descriptor.delay_type = DELAY_TYPE_ECHO;
						else if ( strcmp((char * )delay_type,"FLANGER") == 0 )
							Sequencer_Descriptor.delay_type = DELAY_TYPE_FLANGER;
						else
							Sequencer_Descriptor.delay_type = DELAY_TYPE_NONE;
						if ( seq[0] <= MAX_DELAY)
							Sequencer_Descriptor.delay = seq[0];
						else
							Sequencer_Descriptor.delay = 0;
						if (( seq[1] <= MAX_DELAYW) && ( seq[1] >= MIN_DELAYW))
							Sequencer_Descriptor.delay_weight = seq[1];
						else
							Sequencer_Descriptor.delay_weight = (MAX_DELAYW+MIN_DELAYW)/2;
					}
					else if ( sscanf((char * )line,"BB808_SEQUENCE %d",(int *)&seq[0]) == 1 )
					{
						Sequencer_Descriptor.sequencer_length = seq[0];
						params = 1;
					}
				}
				else
				{
					if ( sscanf((char * )line,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",&seq[0],&seq[1],&seq[2],&seq[3],&seq[4],&seq[5],&seq[6],&seq[7],&seq[8],&seq[9],&seq[10],&seq[11],&seq[12],&seq[13],&seq[14],&seq[15]) == 16 )
					{
						val = 0;
						for(j=0;j<16;j++)
						{
							if ( seq[j] != 0 )
								val |= (1 << j);
						}
						Sequencer_Descriptor.sequencer_steps[line_idx] = val;
						line_idx++;
						if ( line_idx >= Sequencer_Descriptor.sequencer_length )
							end = 1;
					}
				}
			}
		}
		Sequencer_Descriptor.flag = 0xdead;
		sprintf((char *)Sequencer_Descriptor.header,"Seq_8xx");
		SPI_WriteSeqFromSD();
		f_close(&ConfFile);
	}
	UserParameters.bpm = Sequencer_Descriptor.sequencer_beat;
	SystemVar.delay_flags = Sequencer_Descriptor.delay_type;
	UserParameters.delay_value = Sequencer_Descriptor.delay;
	UserParameters.delay_weight = (float )Sequencer_Descriptor.delay_weight / 100.0F;;
	UserParameters.sequencer_length = Sequencer_Descriptor.sequencer_length;

	BPM_Value_Draw(0);
	Delay_Weight_Draw(0);
	Delay_Value_Draw(0);
}
