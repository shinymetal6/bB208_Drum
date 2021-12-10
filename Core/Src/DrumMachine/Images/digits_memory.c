/*
 * digits_memory.c
 *
 *  Created on: Nov 12, 2021
 *      Author: fil
 */

#include "main.h"
#include "digits_memory.h"

//RAMD2_RAM_AREA	__attribute__ ((aligned (16)))	uint8_t ram_blue_digits[10][3378];
__attribute__ ((aligned (16))) uint8_t ram_blue_digits[10][DIGITS_SIZE];
__attribute__ ((aligned (16))) uint8_t ram_red_digits[10][DIGITS_SIZE];
__attribute__ ((aligned (16))) uint8_t ram_green_digits[10][DIGITS_SIZE];
__attribute__ ((aligned (16))) uint8_t ram_yellow_digits[10][DIGITS_SIZE];
__attribute__ ((aligned (16))) uint8_t ram_orange_digits[10][DIGITS_SIZE];
