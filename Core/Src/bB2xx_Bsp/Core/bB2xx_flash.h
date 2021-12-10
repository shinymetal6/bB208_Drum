/*
 * bB2xx_flash.h
 *
 *  Created on: Oct 29, 2021
 *      Author: fil
 */

#ifndef SRC_BB2XX_BSP_CORE_BB2XX_FLASH_H_
#define SRC_BB2XX_BSP_CORE_BB2XX_FLASH_H_

typedef struct {
	uint32_t		sample_pointer;
}FlashStruct_TypeDef;

typedef struct {
	uint32_t					params_pointer;
	uint32_t					usrparams_pointer;
	uint32_t					sequence_pointer;
	uint32_t					sample_pointer;
	uint32_t					digitblue_pointer;
	uint32_t					digitred_pointer;
	uint32_t					digitgreen_pointer;
	uint32_t					digityellow_pointer;
	uint32_t					digitorange_pointer;
	uint32_t					splashimage_pointer;
	SystemParameters_TypeDef	SystemParameters;
}FlashConfig_TypeDef;


typedef struct {
	uint8_t					sample_name[14];
	uint8_t					sample_index;
	uint8_t					sample_flags;
}SamplesConfig_TypeDef;



/*
 * memory structure for 128mBit case , 16 mBytes total
 * 0xfff000 - 0xffffff : parameters , 1 sector ,4096 bytes
 * 0xffe000 - 0xffefff : user parameters , 1 sector , 4096 bytes
 * 0xffd000 - 0xffdfff : user sequence , 1 sector , 4096 bytes
 * 0xffcfff - 0xf00000 : images , 253 sectors
 * 						0xf00000 - 0xf09fff : blue digits
 * 						0xf14000 - 0xf1dfff : red digits
 * 						0xf1e000 - 0xf27fff : green digits
 * 						0xf28000 - 0xf31fff : yellow digits
 * 						0xf32000 - 0xf31fff : orange digits
 * 						0xf3c000 -          : logo
 * 0x000000 - 0xefffff : samples , 3840 sectors
 */
#define		PARAM_NUMSECTORS	1
#define		USRPARAM_NUMSECTORS	1
#define		USRSEQ_NUMSECTORS	1
#define		IMAGE_NUMSECTORS	253
#define		PARAMS_PTR_F128		(F128_SIZE - (PARAM_NUMSECTORS*SECTOR_SIZE))
#define		USRPARAMS_PTR_F128	(F128_SIZE - ((PARAM_NUMSECTORS + USRPARAM_NUMSECTORS)*SECTOR_SIZE))
#define		USRSEQ_PTR_F128		(F128_SIZE - ((PARAM_NUMSECTORS + USRPARAM_NUMSECTORS + USRSEQ_NUMSECTORS)*SECTOR_SIZE))
#define		IMAGE_PTR_F128		(F128_SIZE - ((PARAM_NUMSECTORS + USRPARAM_NUMSECTORS + USRSEQ_NUMSECTORS + IMAGE_NUMSECTORS)*SECTOR_SIZE))
#define		BLUE_DIGIT_FPTR		IMAGE_PTR_F128
#define		RED_DIGIT_FPTR		(BLUE_DIGIT_FPTR+SECTOR_SIZE*10)
#define		GREEN_DIGIT_FPTR	(RED_DIGIT_FPTR+SECTOR_SIZE*10)
#define		YELLOW_DIGIT_FPTR	(GREEN_DIGIT_FPTR+SECTOR_SIZE*10)
#define		ORANGE_DIGIT_FPTR	(YELLOW_DIGIT_FPTR+SECTOR_SIZE*10)
#define		LOGO_FPTR			(ORANGE_DIGIT_FPTR+SECTOR_SIZE*10)
#define		SAMPLES_NUMSECTORS	((F128_SIZE-IMAGE_PTR_F128)/SECTOR_SIZE)

#define		DIGIT_BLUE_IDX		0
#define		DIGIT_RED_IDX		1
#define		DIGIT_GREEN_IDX		2
#define		DIGIT_YELLOW_IDX	3
#define		DIGIT_ORANGE_IDX	4

#define		SAMPLE_PTR_F128		0

extern	uint8_t bB2xx_flash_init(void);
extern	void bB2xx_flash_get_sysparams(void);
extern	void bB2xx_flash_store_sysparams(void);
extern	void bB2xx_flash_store_digits( void );
extern	void bB2xx_flash_get_digits( void );
extern	void bB2xx_flash_store_usrparams(void);
extern	void bB2xx_flash_get_usrsparams(void);

#endif /* SRC_BB2XX_BSP_CORE_BB2XX_FLASH_H_ */
