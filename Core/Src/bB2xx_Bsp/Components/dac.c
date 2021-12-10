/*
 * dac.c
 *
 *  Created on: Nov 9, 2021
 *      Author: fil
 */

#include "main.h"
#include "dac.h"

#define	DAC_WAVETABLE_SIZE					256

FLASH_AREA const int16_t        dac_sinetab[DAC_WAVETABLE_SIZE] = {
		0x8000,0x8324,0x8647,0x896a,0x8c8b,0x8fab,0x92c7,0x95e1,
		0x98f8,0x9c0b,0x9f19,0xa223,0xa527,0xa826,0xab1f,0xae10,
		0xb0fb,0xb3de,0xb6b9,0xb98c,0xbc56,0xbf17,0xc1cd,0xc47a,
		0xc71c,0xc9b3,0xcc3f,0xcebf,0xd133,0xd39a,0xd5f5,0xd842,
		0xda82,0xdcb3,0xded7,0xe0eb,0xe2f1,0xe4e8,0xe6cf,0xe8a6,
		0xea6d,0xec23,0xedc9,0xef5e,0xf0e2,0xf254,0xf3b5,0xf504,
		0xf641,0xf76b,0xf884,0xf989,0xfa7c,0xfb5c,0xfc29,0xfce3,
		0xfd89,0xfe1d,0xfe9c,0xff09,0xff61,0xffa6,0xffd8,0xfff5,
		0xffff,0xfff5,0xffd8,0xffa6,0xff61,0xff09,0xfe9c,0xfe1d,
		0xfd89,0xfce3,0xfc29,0xfb5c,0xfa7c,0xf989,0xf884,0xf76b,
		0xf641,0xf504,0xf3b5,0xf254,0xf0e2,0xef5e,0xedc9,0xec23,
		0xea6d,0xe8a6,0xe6cf,0xe4e8,0xe2f1,0xe0eb,0xded7,0xdcb3,
		0xda82,0xd842,0xd5f5,0xd39a,0xd133,0xcebf,0xcc3f,0xc9b3,
		0xc71c,0xc47a,0xc1cd,0xbf17,0xbc56,0xb98c,0xb6b9,0xb3de,
		0xb0fb,0xae10,0xab1f,0xa826,0xa527,0xa223,0x9f19,0x9c0b,
		0x98f8,0x95e1,0x92c7,0x8fab,0x8c8b,0x896a,0x8647,0x8324,
		0x8000,0x7cdb,0x79b8,0x7695,0x7374,0x7054,0x6d38,0x6a1e,
		0x6707,0x63f4,0x60e6,0x5ddc,0x5ad8,0x57d9,0x54e0,0x51ef,
		0x4f04,0x4c21,0x4946,0x4673,0x43a9,0x40e8,0x3e32,0x3b85,
		0x38e3,0x364c,0x33c0,0x3140,0x2ecc,0x2c65,0x2a0a,0x27bd,
		0x257d,0x234c,0x2128,0x1f14,0x1d0e,0x1b17,0x1930,0x1759,
		0x1592,0x13dc,0x1236,0x10a1,0xf1d,0xdab,0xc4a,0xafb,
		0x9be,0x894,0x77b,0x676,0x583,0x4a3,0x3d6,0x31c,
		0x276,0x1e2,0x163,0xf6,0x9e,0x59,0x27,0xa,
		0x0,0xa,0x27,0x59,0x9e,0xf6,0x163,0x1e2,
		0x276,0x31c,0x3d6,0x4a3,0x583,0x676,0x77b,0x894,
		0x9be,0xafb,0xc4a,0xdab,0xf1d,0x10a1,0x1236,0x13dc,
		0x1592,0x1759,0x1930,0x1b17,0x1d0e,0x1f14,0x2128,0x234c,
		0x257d,0x27bd,0x2a0a,0x2c65,0x2ecc,0x3140,0x33c0,0x364c,
		0x38e3,0x3b85,0x3e32,0x40e8,0x43a9,0x4673,0x4946,0x4c21,
		0x4f04,0x51ef,0x54e0,0x57d9,0x5ad8,0x5ddc,0x60e6,0x63f4,
		0x6707,0x6a1e,0x6d38,0x7054,0x7374,0x7695,0x79b8,0x7cdb,
};

void InitDac(void)
{
	HAL_DAC_Start_DMA(&DAC_DMA, DAC_CHANNEL_1, (uint32_t *)signal_out_dac1, DAC_WAVETABLE_SIZE, DAC_ALIGN_12B_L);
	HAL_DAC_Start_DMA(&DAC_DMA, DAC_CHANNEL_2, (uint32_t *)signal_out_dac2, DAC_WAVETABLE_SIZE, DAC_ALIGN_12B_L);
	HAL_TIM_Base_Start(&SAMPLE_TIMER);
}

static void DAC_IrqProcessSamples(void)
{
uint32_t	i,limitlow,limithi;
	if (( SystemVar.audio_flags & AUDIO_DAC_READY_FLAG) == AUDIO_DAC_READY_FLAG)
	{
		if (( SystemVar.audio_flags & AUDIO_DAC_HALF_FLAG) == AUDIO_DAC_HALF_FLAG)
			limitlow = 0;
		else
			limitlow = DAC_WAVETABLE_SIZE/2;
		limithi = limitlow + DAC_WAVETABLE_SIZE/2;;

		for(i=limitlow;i<limithi;i++)
			signal_out_dac1[i] = signal_out_dac2[i] = dac_sinetab[i];
		SystemVar.audio_flags &= ~AUDIO_DAC_READY_FLAG;
	}
}

void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
	SystemVar.audio_flags |= (AUDIO_DAC_HALF_FLAG | AUDIO_DAC_READY_FLAG);
	DAC_IrqProcessSamples();
}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
	SystemVar.audio_flags &= ~AUDIO_DAC_HALF_FLAG;
	SystemVar.audio_flags |= AUDIO_DAC_READY_FLAG;
	DAC_IrqProcessSamples();
}

