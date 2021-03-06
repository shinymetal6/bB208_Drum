/*
 * spi_sample_store.h
 *
 *  Created on: Sep 17, 2021
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_SPI_SAMPLE_STORE_H_
#define SRC_DRUMMACHINE_SPI_SAMPLE_STORE_H_

//#include	"sequencer.h"

#define	STORE_MSG_MESSAGE	120
#define	STORE_MSG_LINE		140
#define	STORE_MSG_ERRORLINE	160

typedef struct {
	uint32_t	qspi_ptr[NUM_INSTRUMENT];
	uint32_t	sample_len[NUM_INSTRUMENT];
	uint32_t	sample_flag[NUM_INSTRUMENT];
	uint8_t		sample_name[NUM_INSTRUMENT][SAMPLE_NAME_MAX_LEN];	// max file len is SAMPLE_NAME_MAX_LEN
	uint32_t 	sample_rate[NUM_INSTRUMENT];
	uint8_t		midi_key[NUM_INSTRUMENT];
	uint8_t		flag;
}SPISample_DescriptorTypeDef;

typedef struct {
  uint32_t ChunkID;       /* 0 */
  uint32_t FileSize;      /* 4 */
  uint32_t FileFormat;    /* 8 */
  uint32_t SubChunk1ID;   /* 12 */
  uint32_t SubChunk1Size; /* 16*/
  uint16_t AudioFormat;   /* 20 */
  uint16_t NbrChannels;   /* 22 */
  uint32_t SampleRate;    /* 24 */

  uint32_t ByteRate;      /* 28 */
  uint16_t BlockAlign;    /* 32 */
  uint16_t BitPerSample;  /* 34 */
  uint32_t SubChunk2ID;   /* 36 */
  uint32_t SubChunk2Size; /* 40 */
}SampleWAV_FormatTypeDef;

extern	void SPI_ParseWavUSB_AndWrite(void);
extern	void SPI_ParseSeqUSB_AndWrite(void);


#endif /* SRC_BB808_QSPI_SAMPLE_STORE_H_ */
