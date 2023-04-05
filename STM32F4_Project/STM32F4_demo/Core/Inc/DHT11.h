#ifndef __DHT11_H__
#define __DHT11_H__

/* USER CODE BEGIN Includes */
#include "main.h"
#include "tim.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define DHT11_DQ_OUT_HIGH  		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_SET)
#define DHT11_DQ_OUT_LOW 			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_RESET)
#define DHT11_DQ_IN	      			HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_9)
/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */
uint8_t DHT11_Init(void);
void DHT11_IO_IN(void);
void DHT11_IO_OUT(void);
void DHT11_Rst(void);
uint8_t DHT11_Check(void);
uint8_t DHT11_Read_Bit(void);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Data(uint16_t *temp,uint16_t *humi);
/* USER CODE END Prototypes */

#endif
