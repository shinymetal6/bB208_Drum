/*
 * sequencer.c
 *
 *  Created on: Sep 24, 2021
 *      Author: fil
 */

#include "main.h"
#include "bpm.h"
#include <string.h>

__attribute__ ((aligned (16)))  uint16_t sequencer_steps[SEQUENCER_MAX_SIZE];
__attribute__ ((aligned (16)))  Sequencer_DescriptorTypeDef	Sequencer_Descriptor;

extern	Instrument_TypeDef  Instrument;
extern	FlashConfig_TypeDef		FlashConfig;

uint32_t	bpm2irq[MAX_BEAT];

void SequencerInit(void)
{
	w25qxx_ReadBytesDMA((uint8_t *)&Sequencer_Descriptor, w25qxx_AddressToSector(FlashConfig.sequence_pointer), sizeof(Sequencer_Descriptor));
	if ( strncmp((char *)Sequencer_Descriptor.header,"Seq_8xx",HEADER_LEN-1) == 0 )
	{
		UserParameters.bpm = Sequencer_Descriptor.sequencer_beat;
		SystemVar.sequencer_preload = BPM_1_BPM - (UserParameters.bpm * BPM_UNIT);
		TIM7->ARR = SystemVar.sequencer_preload;
		SystemVar.delay_flags = Sequencer_Descriptor.delay_type;
		UserParameters.delay_value = Sequencer_Descriptor.delay;
		UserParameters.delay_weight = (float )Sequencer_Descriptor.delay_weight / 100.0F;;
		SystemVar.sequencer_length = Sequencer_Descriptor.sequencer_length;
		SystemVar.sequencer_counter = 0;
		SystemVar.sequencer_step = 0;
		BPM_Value_Draw(0);
		Delay_Weight_Draw(0);
		Delay_Value_Draw(0);
	}
}

void SequencerCallback(void)
{
uint16_t	i , val;

	if ((((SystemVar.system & SYSTEM_INTEXT_SEQUENCER ) == SYSTEM_INTEXT_SEQUENCER ) && (SystemVar.sequencer_length != 0 )) |
			((SystemVar.sequencer_flags & SEQUENCER_SINGLE ) == SEQUENCER_SINGLE ))
	{
		for(i=0;i<NUM_INSTRUMENT;i++)
		{
			val = 1 << i;
			if (( Sequencer_Descriptor.sequencer_steps[SystemVar.sequencer_step] & val) != 0 )
			{
				Instrument.spi_ptr[i] = HEADER_SIZE;
				Instrument.sample_active_flag |= (1 << i);
			}
		}
		SystemVar.sequencer_step ++;
		if ( SystemVar.sequencer_step >= SystemVar.sequencer_length )
		{
			SystemVar.sequencer_step = 0;
			if ( (SystemVar.sequencer_flags & SEQUENCER_SINGLE ) == SEQUENCER_SINGLE )
			{
				SystemVar.sequencer_flags &= ~SEQUENCER_SINGLE;
			}
		}
		if ((SystemVar.sequencer_flags & SEQUENCER_PRELOAD ) == SEQUENCER_PRELOAD)
		{
			TIM7->ARR = SystemVar.sequencer_preload;
			SystemVar.sequencer_flags &= ~SEQUENCER_PRELOAD;
		}
	}
	else
		SystemVar.sequencer_step = 0;

	SystemVar.sequencer_flags |= SEQUENCER_IRQ_FLAG;
}

