/** ***************************************************************************
 * @file
 * @brief Calculations for the cable monitor
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
#include "measure.h"
#include "menu.h"
#include "stm32f4xx_it.h"

#define NO_SAMPLES 50


void UART_Transmit_Pad(uint16_t *pointer1,uint16_t *pointer2,uint16_t *pointer3);
uint16_t Find_Peakpeak(uint16_t *array);
uint8_t Get_Direction(uint16_t pad1, uint16_t pad2, uint16_t pad3);

/**
 * @brief Reads one period of samples, calculates the
 * @param none
 * @retval result 12bit value from ADC
 **/
void Single_Measurement_Pads(void)
{
	  uint16_t *Results_ADC1;
	  uint16_t *Results_ADC3;

	  uint16_t Result_PAD1[NO_SAMPLES];
	  uint16_t Result_PAD2[NO_SAMPLES];
	  uint16_t Result_PAD3[NO_SAMPLES];

	  uint16_t pp_Pad1=0,pp_Pad2=0,pp_Pad3=0;

	  uint8_t i=0;
	  char text[10];



	  while(i<NO_SAMPLES)
	  {
		  if(TIM2_ellapsed)
		  {
			  TIM2_ellapsed = 0;
			  HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

			  //HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
			  Results_ADC1 = Get_ADC1_Values();
			  Results_ADC3 = Get_ADC3_Values();
			  // PAD1 = PF8, PAD2=PF6, PAD3=PA5
			  Result_PAD1[i] = Results_ADC3[0];
			  Result_PAD2[i] = Results_ADC3[1];
			  Result_PAD3[i] = Results_ADC1[1];

			  i++;

			  			  //Delay_us(1600);

		  }

	  }


	  //UART_Transmit_Pad(Result_PAD1, Result_PAD2, Result_PAD3);

	  //blink_direction();
	  //HAL_Delay(1000);
	  //blink_direction();
	  HAL_UART_Transmit(&huart1, "Single measurement fertig\n", 26, 500);

	  pp_Pad1 = Find_Peakpeak(Result_PAD1);
	  pp_Pad2 = Find_Peakpeak(Result_PAD2);
	  pp_Pad3 = Find_Peakpeak(Result_PAD3);

	  Display_peak_peak(pp_Pad1,pp_Pad2,pp_Pad3);

	  snprintf(text, 10, "PP1=%4d\n", (uint16_t)(pp_Pad1 & 0xffff));
	  HAL_UART_Transmit(&huart1, text, 9, 500);
	  snprintf(text, 10, "PP2=%4d\n", (uint16_t)(pp_Pad2 & 0xffff));
	  HAL_UART_Transmit(&huart1, text, 9, 500);
	  snprintf(text, 10, "PP3=%4d\n", (uint16_t)(pp_Pad3 & 0xffff));
	  HAL_UART_Transmit(&huart1, text, 9, 500);


}



/**
 * @brief Prints Array via UART
 * @param *pointer pointer to uint_16t array
 * @retval result 12bit value from ADC
 * @todo calculate size of array
 **/
void UART_Transmit_Pad(uint16_t *pointer1,uint16_t *pointer2,uint16_t *pointer3)
{
	uint8_t k=0;
	char text[10];

	 for(k=0;k<NO_SAMPLES;k++)
	 {
		  snprintf(text, 10, "PF8=%4d\n", (uint16_t)(pointer1[k] & 0xffff));
		  HAL_UART_Transmit(&huart1, text, 9, 500);
		  snprintf(text, 10, "PF6=%4d\n", (uint16_t)(pointer2[k] & 0xffff));
		  HAL_UART_Transmit(&huart1, text, 9, 500);
		  snprintf(text, 10, "PA5=%4d\n", (uint16_t)(pointer3[k] & 0xffff));
		  HAL_UART_Transmit(&huart1, text, 9, 500);
	  }
}


/**
 * @brief Finds peakpeak value from given array
 * @param *array pointer to uint_16t array
 * @retval result peakpeak value
 * @todo find size of array
 **/
uint16_t Find_Peakpeak(uint16_t *array)
{
	//static uint16_t result=0;
	uint8_t k=0;
	uint16_t max=0, min=4000;
	char text[10];

	 for(k=0;k<NO_SAMPLES;k++)
	 {
		 if(max<array[k]) max=array[k];
		 if(min>array[k]) min=array[k];
	 }
	 snprintf(text, 10, "MAX=%4d\n", (uint16_t)(max & 0xffff));
	 HAL_UART_Transmit(&huart1, text, 9, 500);
	 snprintf(text, 10, "MIN=%4d\n", (uint16_t)(min & 0xffff));
	 HAL_UART_Transmit(&huart1, text, 9, 500);

	 return (max-min);

}


