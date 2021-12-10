/*
 * fonts.h
 *
 *  Created on: Oct 21, 2021
 *      Author: fil
 */

#ifndef SRC_BB2XX_BSP_COMPONENTS_FONTS_H_
#define SRC_BB2XX_BSP_COMPONENTS_FONTS_H_

#include <stdint.h>

typedef struct {
    const uint8_t width;
    uint8_t height;
    const uint16_t *data;
} FontDef;


extern FontDef Font_7x10;
extern FontDef Font_11x18;
extern FontDef Font_16x26;

#endif /* SRC_BB2XX_BSP_COMPONENTS_FONTS_H_ */
