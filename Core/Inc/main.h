/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bB2xx_BspInclude.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SYSTEM_LED_Pin GPIO_PIN_5
#define SYSTEM_LED_GPIO_Port GPIOE
#define SW1_Pin GPIO_PIN_4
#define SW1_GPIO_Port GPIOC
#define SW2_Pin GPIO_PIN_5
#define SW2_GPIO_Port GPIOC
#define TOUCH_CS_Pin GPIO_PIN_8
#define TOUCH_CS_GPIO_Port GPIOE
#define TOUCH_IRQ_Pin GPIO_PIN_9
#define TOUCH_IRQ_GPIO_Port GPIOE
#define TOUCH_IRQ_EXTI_IRQn EXTI9_5_IRQn
#define FLASH_SS_Pin GPIO_PIN_11
#define FLASH_SS_GPIO_Port GPIOE
#define BKL_TIM2_CH4_Pin GPIO_PIN_11
#define BKL_TIM2_CH4_GPIO_Port GPIOB
#define FLAG_Pin GPIO_PIN_14
#define FLAG_GPIO_Port GPIOB
#define LD1_Pin GPIO_PIN_6
#define LD1_GPIO_Port GPIOC
#define LCDDC_Pin GPIO_PIN_0
#define LCDDC_GPIO_Port GPIOD
#define MICROSD_PRESENT_Pin GPIO_PIN_5
#define MICROSD_PRESENT_GPIO_Port GPIOD
#define SPDIF_OUT_SAI1_SD_A_Pin GPIO_PIN_6
#define SPDIF_OUT_SAI1_SD_A_GPIO_Port GPIOD
#define ENCODER_BTN_Pin GPIO_PIN_5
#define ENCODER_BTN_GPIO_Port GPIOB
#define ENCODER_BTN_EXTI_IRQn EXTI9_5_IRQn
#define TIM4_ENCODERDATA_Pin GPIO_PIN_6
#define TIM4_ENCODERDATA_GPIO_Port GPIOB
#define TIM4_ENCODERCLOCK_Pin GPIO_PIN_7
#define TIM4_ENCODERCLOCK_GPIO_Port GPIOB
#define LD8_Pin GPIO_PIN_8
#define LD8_GPIO_Port GPIOB
#define LD7_Pin GPIO_PIN_9
#define LD7_GPIO_Port GPIOB
#define LCDRESET_Pin GPIO_PIN_0
#define LCDRESET_GPIO_Port GPIOE
#define LCDCS_Pin GPIO_PIN_1
#define LCDCS_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
