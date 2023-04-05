/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim6;

extern TIM_HandleTypeDef htim7;

/* USER CODE BEGIN Private defines */
typedef struct{
	uint16_t flag_1ms;
	uint16_t flag_10ms;
	uint16_t flag_100ms;
	uint16_t flag_200ms;
	uint16_t flag_300ms;
	uint16_t flag_500ms;
	uint16_t flag_1s;
	uint16_t flag_2s;
}TIM6_delayTypeDef;
/* USER CODE END Private defines */

void MX_TIM6_Init(void);
void MX_TIM7_Init(void);

/* USER CODE BEGIN Prototypes */
void TIM7_delay_us(uint16_t us);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

