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
#include "calculations.h"

#define NO_SAMPLES 50
//#define DEBUG_UART



/*
void UART_Transmit_Pad(uint16_t *pointer1,uint16_t *pointer2,uint16_t *pointer3);
uint16_t Find_Peakpeak(uint16_t *array);
void Get_Direction(uint16_t PP1, uint16_t PP2, uint16_t PP3);
*/
/**
 * @brief Reads one period of samples from PAD1..3 and calculates peak peak values
 * @param none
 * @retval Str_ADC_Values Struct with PP values from pad1..3 and coil1..2. see struct
 * @todo Result_PADxy beinhaltelt auch Werte von SPulen --> Unschön Name neu vergeben
 **/
Struct_ADC_Values Get_Measurement_Data(type_of_measurement type)
{
	  uint16_t *Results_ADC1;
	  uint16_t *Results_ADC3;

	  uint16_t Result_PAD1[NO_SAMPLES];
	  uint16_t Result_PAD2[NO_SAMPLES];
	  uint16_t Result_PAD3[NO_SAMPLES];

	  uint16_t pp_Pad1=0,pp_Pad2=0,pp_Pad3=0;
	  uint16_t pp_Coil1=0, pp_Coil2=0;
	  uint8_t i=0;

	  static Struct_ADC_Values Str_ADC_Values;

	  while(i<NO_SAMPLES)
	  {
		  if(TIM2_ellapsed)
		  {
			  TIM2_ellapsed = 0;
			  HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

			  Results_ADC1 = Get_ADC1_Values();
			  Results_ADC3 = Get_ADC3_Values();
			  // PAD1 = PF8, PAD2=PF6, PAD3=PA5

			  // Fill struct
			  if(type == PADS)
			  {
				  Result_PAD1[i] = Results_ADC3[0];
				  Result_PAD2[i] = Results_ADC3[1];
				  Result_PAD3[i] = Results_ADC1[1];
				  Str_ADC_Values.array_pad1[i]=Results_ADC3[0];
				  Str_ADC_Values.array_pad2[i]=Results_ADC3[1];
				  Str_ADC_Values.array_pad3[i]=Results_ADC1[1];

				  pp_Pad1 = Find_Peakpeak(Result_PAD1);
				  pp_Pad2 = Find_Peakpeak(Result_PAD2);
				  pp_Pad3 = Find_Peakpeak(Result_PAD3);

				  //------------ Fill struct ------------- //
				  Str_ADC_Values.PP_Pad1 = pp_Pad1;
				  Str_ADC_Values.PP_Pad2 = pp_Pad2;
				  Str_ADC_Values.PP_Pad3 = pp_Pad3;


			  }
			  if(type == COILS)
			  {
				  Result_PAD1[i] = Results_ADC1[0];
				  Result_PAD2[i] = Results_ADC1[2];
				  Str_ADC_Values.array_pad1[i]=Results_ADC1[0];
				  Str_ADC_Values.array_pad2[i]=Results_ADC1[2];


				  pp_Coil1 = Find_Peakpeak(Result_PAD1);
				  pp_Coil2 = Find_Peakpeak(Result_PAD2);
				  //pp_Pad3 = Find_Peakpeak(Result_PAD3);

				  //------------ Fill struct ------------- //
				  Str_ADC_Values.PP_Coil1 = pp_Coil1;
				  Str_ADC_Values.PP_Coil2 = pp_Coil2;
				  //Str_ADC_Values.PP_Pad3 = pp_Pad3;

			  }

			  i++;

		  }
	  }

#ifdef DEBUG_UART
	  UART_Transmit_Pad(Result_PAD1, Result_PAD2, Result_PAD3);
#endif




	  //Display_Signal_Pads(Result_PAD1,Result_PAD2,Result_PAD3)
	  //Display_peak_peak(pp_Pad1,pp_Pad2,pp_Pad3);
	  //Get_Direction(pp_Pad1,pp_Pad2,pp_Pad3);

#ifdef DEBUG_UART
	  char text[10];
	  snprintf(text, 10, "PP1=%4d\n", (uint16_t)(pp_Pad1 & 0xffff));
	  HAL_UART_Transmit(&huart1, text, 9, 500);
	  snprintf(text, 10, "PP2=%4d\n", (uint16_t)(pp_Pad2 & 0xffff));
	  HAL_UART_Transmit(&huart1, text, 9, 500);
	  snprintf(text, 10, "PP3=%4d\n", (uint16_t)(pp_Pad3 & 0xffff));
	  HAL_UART_Transmit(&huart1, text, 9, 500);
#endif

	  return Str_ADC_Values;
}




/**
 * @brief Measures one period of Data
 * @param none
 * @retval none
 * @todo Pad/Spulen umschaltung
 **/
void Single_Measurement(type_of_measurement type)
{
	static Struct_ADC_Values Res;

	if(type == PADS)
	{

		 Res = Get_Measurement_Data(PADS);

		 Display_Signal_Pads(Res.array_pad1,Res.array_pad2,Res.array_pad3);
		 Display_peak_peak(Res.PP_Pad1,Res.PP_Pad2,Res.PP_Pad3);
		 Get_Direction(Res.PP_Pad1,Res.PP_Pad2,Res.PP_Pad3);
	}
	if(type == COILS)
	{

		 Res = Get_Measurement_Data(COILS);

		 Display_Signal_Pads(Res.array_pad1,Res.array_pad2,0);
		 Display_peak_peak(Res.PP_Coil1,Res.PP_Coil2,0);
		 //Get_Direction(Res.PP_Pad1,Res.PP_Pad2,Res.PP_Pad3);
	}
	Display_Type_of_Measurement(type);
}

/**
 * @brief Measures three periods of Data and averages them.
 * @param none
 * @retval none
 * @todo Anzahl messungen übergeben
 **/
void Continuous_Measurement(type_of_measurement type)
{
	static Struct_ADC_Values Res;
	uint16_t i=0;
	uint16_t PP1=0, PP2=0, PP3=0;

	if(type == PADS)
	{
		for(i=0;i<5;i++)
		{
			Res = Get_Measurement_Data(PADS);
			PP1 = PP1 + Res.PP_Pad1;
			PP2= PP2 + Res.PP_Pad2;
			PP3 = PP3 + Res.PP_Pad3;
		}
		PP1 = (uint16_t)(PP1/i);
		PP2 = (uint16_t)(PP2/i);
		PP3 = (uint16_t)(PP3/i);

		Display_Signal_Pads(Res.array_pad1,Res.array_pad2,Res.array_pad3);
		Display_peak_peak(PP1, PP2, PP3);
		Get_Direction(PP1, PP2, PP3);

	}

	if(type == COILS)
	{
		for(i=0;i<5;i++)
		{
			Res = Get_Measurement_Data(COILS);
			PP1 = PP1 + Res.PP_Coil1;
			PP2 = PP2 + Res.PP_Coil2;
		}
		PP1 = (uint16_t)(PP1/i);
		PP2 = (uint16_t)(PP2/i);
		Display_Signal_Pads(Res.array_pad1,Res.array_pad2,0);
		Display_peak_peak(PP1,PP2,0);
	}
	Display_Type_of_Measurement(type);
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

	 for(k=0;k<NO_SAMPLES-1;k++)
	 {
		 if(abs(array[k]-array[k+1])<200)
		 {
		 	 if(max<array[k]) max=array[k];
		 	 if(min>array[k]) min=array[k];
		 }
	 }
#ifdef DEBUG_UART
	 snprintf(text, 10, "MAX=%4d\n", (uint16_t)(max & 0xffff));
	 HAL_UART_Transmit(&huart1, text, 9, 500);
	 snprintf(text, 10, "MIN=%4d\n", (uint16_t)(min & 0xffff));
	 HAL_UART_Transmit(&huart1, text, 9, 500);
#endif

	 return (max-min);

}


/**
 * @brief Finds direction of mains cable
 * @param PP1,PP2,PP3 Peak peak value from each PAD
 * @retval none
 **/

void Get_Direction(uint16_t PP1, uint16_t PP2, uint16_t PP3)
{
	/*
	int16_t delta12 = PP1-PP2;
	int16_t delta23 = PP2-PP3;
	int16_t delta13 = PP1-PP3;*/

	uint16_t left=0, middle=0, right=0;
/*
	left = (uint16_t)(0.8*PP1+0.2*PP2);						//A
	right = (uint16_t)(0.8*PP3+0.2*PP2);					//B
	middle = (uint16_t)(0.75*PP2+0.125*PP1+0.125*PP3);		//C

	char text[10];
	snprintf(text, 10, "l  =%4d\n", (uint16_t)(left & 0xffff));
	HAL_UART_Transmit(&huart1, text, 9, 500);
	snprintf(text, 10, "r  =%4d\n", (uint16_t)(right & 0xffff));
	HAL_UART_Transmit(&huart1, text, 9, 500);
	snprintf(text, 10, "mid=%4d\n", (uint16_t)(middle & 0xffff));
	HAL_UART_Transmit(&huart1, text, 9, 500);

	if (left >= right && left >= middle) snprintf(text, 10, "left    \n", (uint16_t)(left & 0xffff));
	if (right >= left && right >= middle) snprintf(text, 10, "right   \n", (uint16_t)(right & 0xffff));
	if (middle >= left && middle >= right) snprintf(text, 10, "middle  \n", (uint16_t)(middle & 0xffff));
	HAL_UART_Transmit(&huart1, text, 9, 500);

*/
	/*
	 * difference smaller than 30% of middle pad->mitte
	 * PP1 grösser als 80% von PP3-> ganz links
	 */
/*
	if((PP2>PP1)&&(PP2>PP3)){
		HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(led5_GPIO_Port, led5_Pin, GPIO_PIN_SET);

	}
	else{
		HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(led5_GPIO_Port, led5_Pin, GPIO_PIN_SET);
	}*/

	if((abs(PP1-PP3))<(0.2*PP2)) {//middle

		HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(led5_GPIO_Port, led5_Pin, GPIO_PIN_SET);
	}
	else if(PP1>(0.8*PP3)){//left
		HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(led5_GPIO_Port, led5_Pin, GPIO_PIN_SET);

	}
	else if(PP3>(0.8*PP1)){//right
			HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(led5_GPIO_Port, led5_Pin, GPIO_PIN_RESET);

		}/*
	else if(PP1>(0.8*PP3)){//left-middle
			HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(led5_GPIO_Port, led5_Pin, GPIO_PIN_SET);

		}
		else if(PP3>(0.8*PP1)){//middle-right
				HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(led5_GPIO_Port, led5_Pin, GPIO_PIN_SET);

			}
	else {
				HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(led5_GPIO_Port, led5_Pin, GPIO_PIN_SET);
	}*/


}


