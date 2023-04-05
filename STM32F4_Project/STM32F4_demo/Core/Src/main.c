/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "DHT11.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//#define DEBUG_LOG_1  1 //寻找项目bug使用
//#define DEBUG_LOG_Tx  1
#define DEBUG_LOG_Rx  1
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
	extern TIM6_delayTypeDef TIM6_delay;
	extern USART_DataStruct UART2_Rx_Buffer;
	USART_DataStruct UART2_Tx_Buffer = {0xAA, 0x55, 0x00, 0x00, 0x66};
	
	HAL_TIM_Base_Start_IT(&htim6); // 启动定时器TIM6
	HAL_TIM_Base_Start_IT(&htim7); // 启动定时器TIM7
	HAL_UART_Receive_IT(&huart2,&UART2_Rx_Buffer.RxBuf,1); 		//打开串口接收中断
	
	uint16_t temperature,humidity;
  uint8_t LED_a,LED_b,LED_c,Slider_abc;
	
	while(DHT11_Init())//等待DHT11的回应,返回1:未检测到DHT11的存在,返回0:存在
	{
		#ifdef DEBUG_LOG_1
		printf("DHT11 Checked faile!!!\r\n");
		#endif
		HAL_Delay(500);
	}
	#ifdef DEBUG_LOG_1
	printf("DHT11 Checked Success!!!\r\n");
	#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		DHT11_Read_Data(&temperature,&humidity);
		#ifdef DEBUG_LOG_1
		printf("DHT11 Temperature = %d  摄氏度\r\n",temperature);
		printf("DHT11 Humidity = %d  %%\r\n",humidity);
		#endif
		UART2_Tx_Buffer.Data[2] = temperature;
		UART2_Tx_Buffer.Data[3] = humidity;
		if(TIM6_delay.flag_2s == 1)
		{
			#ifdef DEBUG_LOG_Tx
			for(uint8_t i = 0;i < 5;i++)
			{
				printf("UART2_Tx_Buffer.Data[%d] :  %d\r\n",i,UART2_Tx_Buffer.Data[i]);
			}
			#endif
			
			if(UART2_Rx_Buffer.Flag == 1)//接收完成标志
			{
				UART2_Rx_Buffer.Flag = 0;//接收完成标志清零
				/*获取接收到的数据包中的重要信息*/
				LED_a = UART2_Rx_Buffer.Data[2];
				LED_b = UART2_Rx_Buffer.Data[3];
				LED_c = UART2_Rx_Buffer.Data[4];
				Slider_abc = UART2_Rx_Buffer.Data[5];
				
				for(uint8_t i = 0;i < 7;i++)
				{
					#ifdef DEBUG_LOG_Rx
					printf("UART2_Tx_Buffer.Data[%d] :  %d\r\n",i,UART2_Rx_Buffer.Data[i]);
					#endif
					UART2_Rx_Buffer.Data[i] = 0;//清空数组，这很重要				
				}
			}		
			HAL_UART_Transmit_IT(&huart2,UART2_Tx_Buffer.Data,5);
			TIM6_delay.flag_2s = 0;
		}
		
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
