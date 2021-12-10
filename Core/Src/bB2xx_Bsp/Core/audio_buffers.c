/*
 * audio_buffers.c
 *
 *  Created on: Nov 10, 2021
 *      Author: fil
 */

#include "main.h"
#include "audio_buffers.h"

RAMD3_RAM_AREA	__attribute__ ((aligned (16)))	int16_t		signal_out_dac1[DAC_SIGNAL_LEN];
RAMD3_RAM_AREA	__attribute__ ((aligned (16)))	int16_t		signal_out_dac2[DAC_SIGNAL_LEN];
RAMD3_RAM_AREA	__attribute__ ((aligned (16)))	int16_t		signal_out_i2s[I2S_SIGNAL_LEN];
RAMD3_RAM_AREA	__attribute__ ((aligned (16)))	int16_t		i2s_buffer[I2S_SIGNAL_LEN];
RAMD3_RAM_AREA	__attribute__ ((aligned (16)))	int16_t		signal_out_spdif[SPDIF_SIGNAL_LEN];
RAMD3_RAM_AREA	__attribute__ ((aligned (16)))	int16_t		spdif_buffer[SPDIF_SIGNAL_LEN];

