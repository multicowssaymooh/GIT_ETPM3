/** ***************************************************************************
 * @file
 * @brief Measurements from the ADC
 *
 * Initializes and displays the menu.  ANPASSEN TBD
 * @n Provides the function MENU_check_transition() for polling user actions.
 * The variable MENU_transition is set to the touched menu item.
 * If no touch has occurred the variable MENU_transition is set to MENU_NONE
 * @n If the interrupt handler is enabled by calling BSP_TS_ITConfig();
 * the variable MENU_transition is set to the touched menu entry as above.
 * @n Either call once BSP_TS_ITConfig() to enable the interrupt
 * or MENU_check_transition() in the main while loop for polling.
 * @n The function MENU_get_transition() returns the new menu item.
 *
 * @author  Berger Dominic bergedo1@zhaw.students.ch
 * @date	30.04.2020
**/


#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"

#include "gpio.h"
#include "adc.h"
#include "usart.h"

ADC_ChannelConfTypeDef sConfig = {0};
GPIO_InitTypeDef GPIO_InitStruct = {0};



/**
 * @brief This functions is used for delays in us. Example wait for ADC to stabilize.
 * @param delay Specifies the time of delay in us
 * @retval none
 */
void Delay_us (uint32_t delay)
{
	uint32_t i=0;
	for(i;i<(delay);i++){}

}



/**
 * @brief Reads value from ADC of PAD1
 * @param none
 * @retval result pointer to uint16_t array with 3x 12bit values. result[0] = PC1/hall/coil1, result[1] = PA5/PAD3 , result[2] = PC3/Coil2
 * @todo Überprüfen reihenfolge der AD Wandlungen. Pad1 in res0 etc.
 *
 */
uint16_t *Get_ADC1_Values(void)
{
	static uint16_t result[]={0,0,0};

	GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	//ADC1
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 5);
	result[0] = HAL_ADC_GetValue(&hadc1);  	//PC1
	result[1] = HAL_ADC_GetValue(&hadc1);	//PA5
	result[2] = HAL_ADC_GetValue(&hadc1);	//PC3

	HAL_ADC_Stop(&hadc1);

	gyro_disable();

	/*
	char text[11];
	snprintf(text, 10, "PC1=%4d\n", (uint16_t)(result[0] & 0xffff));
	  HAL_UART_Transmit(&huart1, text, 9, 500);
	  snprintf(text, 10, "PA5=%4d\n", (uint16_t)(result[1] & 0xffff));
	  HAL_UART_Transmit(&huart1, text, 9, 500);
	  snprintf(text, 10, "PC3=%4d\n", (uint16_t)(result[2] & 0xffff));
	  HAL_UART_Transmit(&huart1, text, 9, 500);
*/
	return result;
}


/**
 * @brief Reads value from ADC of PAD2
 * @param none
 * @retval result 12bit value from ADC. result[0] = PF8/PAD1, result[1] = PF6/PAD2
 * @todo Überprüfen reihenfolge der AD Wandlungen. Coil1 in res0 etc.
 */
uint16_t *Get_ADC3_Values(void)
{
	static uint16_t result[]={0,0};

	gyro_disable();
	/*
	sConfig.Channel = ADC_CHANNEL_6;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
	{
	  Error_Handler();
	}
	//PF8
	HAL_ADC_Start(&hadc3);
	HAL_ADC_PollForConversion(&hadc3, 10);
	result[0] = HAL_ADC_GetValue(&hadc3);
	HAL_ADC_Stop(&hadc3);


	sConfig.Channel = ADC_CHANNEL_4;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
	{
	   Error_Handler();
	}
	//PF6
	HAL_ADC_Start(&hadc3);
	HAL_ADC_PollForConversion(&hadc3, 10);
	result[1] = HAL_ADC_GetValue(&hadc3);
	HAL_ADC_Stop(&hadc3);*/

	HAL_ADC_Start(&hadc3);
	HAL_ADC_PollForConversion(&hadc3, 10);
	result[1] = HAL_ADC_GetValue(&hadc3);
	result[0] = HAL_ADC_GetValue(&hadc3);
	HAL_ADC_Stop(&hadc3);

	/*
	char text[10];

	snprintf(text, 10, "PF8=%4d\n", (uint16_t)(result[0] & 0xffff));
	HAL_UART_Transmit(&huart1, text, 9, 500);
	snprintf(text, 10, "PF6=%4d\n", (uint16_t)(result[1] & 0xffff));
	HAL_UART_Transmit(&huart1, text, 9, 500);
	*/



	return result;
}









// ------------------------------------- delete ---------------------------------------------//
//
// 	  	/*
//	    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_3;
//	    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//	    GPIO_InitStruct.Pull = GPIO_NOPULL;
//	    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//
//
//	    GPIO_InitStruct.Pin = GPIO_PIN_5;
//	    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//	    GPIO_InitStruct.Pull = GPIO_NOPULL;
//	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//		*/
//
//	    //Delay_us(10);
//
//	    //ADC1
//	  	HAL_ADC_Start(&hadc1);
//	  	HAL_ADC_PollForConversion(&hadc1, 50);
//	  	adc_PC1 = HAL_ADC_GetValue(&hadc1);
//	  	adc_PA5 = HAL_ADC_GetValue(&hadc1);
//	  	adc_PC3 = HAL_ADC_GetValue(&hadc1);
//
//	  	HAL_ADC_Stop(&hadc1);
//
//	    gyro_disable();
//
//	    sConfig.Channel = ADC_CHANNEL_6;
//	    sConfig.Rank = 1;
//	    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
//	    if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
//	    {
//	      Error_Handler();
//	    }
//
//	    HAL_ADC_Start(&hadc3);
//	    HAL_ADC_PollForConversion(&hadc3, 10);
//	    adc_PF8 = HAL_ADC_GetValue(&hadc3);
//	    HAL_ADC_Stop(&hadc3);
//
//
//	    sConfig.Channel = ADC_CHANNEL_4;
//	    sConfig.Rank = 1;
//	    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
//	    if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
//	    {
//	       Error_Handler();
//	    }
//
//	    HAL_ADC_Start(&hadc3);
//	    HAL_ADC_PollForConversion(&hadc3, 10);
//	    adc_PF6 = HAL_ADC_GetValue(&hadc3);
//	    HAL_ADC_Stop(&hadc3);
//
//	    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin,0);
//	    //while(1){}
//
//	    //testasldkfjasdlkfj
//
//	    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
//	    */
//
//
//	    //ADC1
//
//	    snprintf(text, 10, "PC1=%4d\n", (int)(adc_PC1 & 0xffff));
//	    HAL_UART_Transmit(&huart1, text, 9, 500);
//	    snprintf(text, 10, "PA5=%4d\n", (int)(adc_PA5 & 0xffff));
//	    HAL_UART_Transmit(&huart1, text, 9, 500);
//	    snprintf(text, 10, "PC3=%4d\n", (int)(adc_PC3 & 0xffff));
//	    HAL_UART_Transmit(&huart1, text, 9, 500);
//
//	    //ADC3
//	    snprintf(text, 10, "PF8=%4d\n", (int)(adc_PF8 & 0xffff));
//	   	HAL_UART_Transmit(&huart1, text, 9, 500);
//	   	snprintf(text, 10, "PF6=%4d\n", (int)(adc_PF6 & 0xffff));
//	   	HAL_UART_Transmit(&huart1, text, 9, 500);
//
//



