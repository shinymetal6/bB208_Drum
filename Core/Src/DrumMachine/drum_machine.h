/*
 * drum_machine.h
 *
 *  Created on: Dec 10, 2021
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_DRUM_MACHINE_H_
#define SRC_DRUMMACHINE_DRUM_MACHINE_H_

#include "Drum/sequencer.h"
#include "Images/digits.h"
#include "Images/digits_memory.h"

#define	NUM_INSTRUMENT				16
#define	SAMPLE_NAME_MAX_LEN			20
#define	NUMSECTOR4K_PER_INSTRUMENT	(SAMPLES_NUMSECTORS / NUM_INSTRUMENT)
#define	INSTRUMENT_SIZE				(NUMSECTOR4K_PER_INSTRUMENT*SECTOR_SIZE)
#define	HEADER_SIZE					32
#define MAX_BEAT					175

typedef struct {
	uint32_t		spi_ptr[NUM_INSTRUMENT];
	uint32_t		sample_len[NUM_INSTRUMENT];
	uint8_t			sample_name[NUM_INSTRUMENT][SAMPLE_NAME_MAX_LEN];	// max file len is SAMPLE_NAME_MAX_LEN
	uint8_t			midi_key[NUM_INSTRUMENT];
	uint32_t 		sample_rate;
	uint16_t		sample_active_flag;
	uint8_t			flag;
}Instrument_TypeDef;


typedef struct {
	uint8_t		wav_name[SAMPLE_NAME_MAX_LEN];	// max file len is SAMPLE_NAME_MAX_LEN
	uint32_t	sample_rate;
	uint32_t	wav_len;
	uint8_t		empty0;
	uint8_t		empty1;
	uint8_t		empty2;
	uint8_t		midi_key;
}SPISample_HeaderTypeDef;

extern	Sequencer_DescriptorTypeDef	Sequencer_Descriptor;

extern	void MenuDisplayInit(void);
extern	void MenuEncoderNavigate(void);
extern	void MeuEncoderChangeMenu(void);

extern	void Delay_Value_Draw(uint8_t hilight);
extern	void Delay_Weight_Draw(uint8_t hilight);
extern	void Delay_Value_Change(void);
extern	void Delay_Weight_Change(void);
extern	void BPM_Value_Draw(uint8_t hilight);
extern	void BPM_Value_Change(void);

#endif /* SRC_APPLICATION_DRUM_MACHINE_H_ */
