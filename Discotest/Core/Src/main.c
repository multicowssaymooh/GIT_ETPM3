/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "crc.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"
#include "string.h"

#include "menu.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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

	uint32_t	adc_PC1=0,adc_PA5=0,adc_PC3=0;
	uint32_t	adc_PF8=0, adc_PF6=0;



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
  MX_CRC_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_I2C3_Init();
  MX_LTDC_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_ADC3_Init();
  MX_SPI3_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //BSP_LCD_Init();

  // -----------

  BSP_LCD_Init();						// Initialize the LCD display
  BSP_LCD_LayerDefaultInit(LCD_FOREGROUND_LAYER, LCD_FRAME_BUFFER);
  BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
  BSP_LCD_DisplayOn();
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

  MENU_draw();

  MENU_hint();

  gyro_disable();

  //HAL_ADC_Start(&hadc1);
  //HAL_ADC_Start(&hadc3);



  char text[10];

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
	  //HAL_UART_Transmit(&huart1, "Hello world\n", 12, 500);
	  HAL_Delay(200);

	  //ADC1
	    HAL_ADC_Start(&hadc1);
	    HAL_ADC_PollForConversion(&hadc1, 5);
	    adc_PC1 = HAL_ADC_GetValue(&hadc1);
	    adc_PA5 = HAL_ADC_GetValue(&hadc1);
	    adc_PC3 = HAL_ADC_GetValue(&hadc1);
	    HAL_ADC_Stop(&hadc1);

	    //ADC1
	    /*
	    snprintf(text, 10, "PC1=%4d\n", (int)(adc_PC1 & 0xffff));
	    HAL_UART_Transmit(&huart1, text, 9, 500);
	    snprintf(text, 10, "PA5=%4d\n", (int)(adc_PA5 & 0xffff));
	    HAL_UART_Transmit(&huart1, text, 9, 500);
	    snprintf(text, 10, "PC3=%4d\n", (int)(adc_PC3 & 0xffff));
	    HAL_UART_Transmit(&huart1, text, 9, 500);

*/
	    HAL_ADC_Start(&hadc3);
	    HAL_ADC_PollForConversion(&hadc3, 5);
	    adc_PF8 = HAL_ADC_GetValue(&hadc3);
	    adc_PF6 = HAL_ADC_GetValue(&hadc3);
	    HAL_ADC_Stop(&hadc3);

	    //hello asldkfjsölkdfj



	    //ADC3
	    snprintf(text, 10, "PF8=%4d\n", (int)(adc_PF8 & 0xffff));
	   	HAL_UART_Transmit(&huart1, text, 9, 500);
	   	snprintf(text, 10, "PF6=%4d\n", (int)(adc_PF6 & 0xffff));
	   	HAL_UART_Transmit(&huart1, text, 9, 500);



	  MENU_check_transition();

	  switch (MENU_get_transition()) {
	  		case MENU_NONE:					// No transition => do nothing
	  			break;
	  		case MENU_ZERO:
	  			HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
	  			//ADC_single_demo();
	  			break;
	  		case MENU_ONE:
	  			HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
	  			//ADC_timer_demo();
	  			break;
	  		case MENU_TWO:
	  			HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
	  			//ADC_DMA_demo();
	  			break;
	  		case MENU_THREE:
	  			//ADC_DMA_dual_demo();
	  			break;
	  		case MENU_FOUR:
	  			//ADC_DMA_scan_demo();
	  			break;
	  		case MENU_FIVE:
	  			//DAC_demo();
	  			//ADC_DMA_scan_demo();
	  			break;
	  		default:						// Should never occur
	  			break;
	  		}
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 50;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void gyro_disable(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();		// Enable Clock for GPIO port C
	/* Disable PC1 and PF8 first */
	GPIOC->MODER &= ~GPIO_MODER_MODER1; // Reset mode for PC1
	GPIOC->MODER |= GPIO_MODER_MODER1_0;	// Set PC1 as output
	GPIOC->BSRR |= GPIO_BSRR_BR1;		// Set GYRO (CS) to 0 for a short time
	HAL_Delay(10);						// Wait some time
	GPIOC->MODER |= GPIO_MODER_MODER1_Msk; // Analog mode PC1 = ADC123_IN11
	__HAL_RCC_GPIOF_CLK_ENABLE();		// Enable Clock for GPIO port F
	GPIOF->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED8;	// Reset speed of PF8
	GPIOF->AFR[1] &= ~GPIO_AFRH_AFSEL8;			// Reset alternate func. of PF8
	GPIOF->PUPDR &= ~GPIO_PUPDR_PUPD8;			// Reset pulup/down of PF8
	HAL_Delay(10);						// Wait some time
	GPIOF->MODER |= GPIO_MODER_MODER8_Msk; // Analog mode for PF6 = ADC3_IN4
}




/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/