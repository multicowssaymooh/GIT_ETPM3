/** ***************************************************************************
 * @file
 * @brief Calculations for the cable monitor
 *
 * @n Provides the function Get_Measurement_Data() to gather all values from Pads/Coils which returns a struct (see ::Struct_ADC_Values) with all peak-peak values and array with samples
 * @n This function is used in Single_Measurement() and Continuous_Measurement(), where it is called either once or 5 times to build the mean values.
 * @n Find_Peakpeak() calculates the peak-peak value from the given array. Get_Direction() calculates the direction of the mains cable and displays the distance (TBD)
 *
 *
 * @author  bergedo1, landojon
 * @date	18.12.2020
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
#define ON 0
#define OFF 1
//#define DEBUG_UART

// global variables

/// If cable is present LEDs should blink --> toggle every iteration
uint8_t blink=0;


/**
 * @brief Reads one period of samples from PAD1..3 and Coil1..2, calculates peak peak values and returns them as struct
 * @param type Specify which measurements are done; see ::type_of_measurement
 * @retval Str_ADC_Values Struct with PP values from pad1..3 and coil1..2; see ::Struct_ADC_Values
 * @todo Result_PADxy beinhaltelt auch Werte von Spulen --> Unschön Name neu vergeben
 **/
Struct_ADC_Values Get_Measurement_Data(type_of_measurement type)
{
	  uint16_t *Results_ADC1;
	  uint16_t *Results_ADC3;

	  uint16_t Result_PAD1[NO_SAMPLES];
	  uint16_t Result_PAD2[NO_SAMPLES];
	  uint16_t Result_PAD3[NO_SAMPLES];

	  uint16_t Result_Coil1[NO_SAMPLES];
	  uint16_t Result_Coil2[NO_SAMPLES];

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

			  if(type==INIT)
			  {
				  Result_PAD1[i] = Results_ADC3[0];
				  Result_PAD2[i] = Results_ADC3[1];
				  Result_PAD3[i] = Results_ADC1[1];

				  Result_Coil1[i] = Results_ADC1[0];
				  Result_Coil2[i] = Results_ADC1[2];


				  pp_Pad1 = Find_Peakpeak(Result_PAD1);
				  pp_Pad2 = Find_Peakpeak(Result_PAD2);
				  pp_Pad3 = Find_Peakpeak(Result_PAD3);

				  pp_Coil1 = Find_Peakpeak(Result_Coil1);
				  pp_Coil2 = Find_Peakpeak(Result_Coil2);

				  //------------ Fill struct ------------- //
				  Str_ADC_Values.PP_Pad1 = pp_Pad1;
				  Str_ADC_Values.PP_Pad2 = pp_Pad2;
				  Str_ADC_Values.PP_Pad3 = pp_Pad3;
				  Str_ADC_Values.PP_Coil1 = pp_Coil1;
				  Str_ADC_Values.PP_Coil2 = pp_Coil2;
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
 * @brief Measures one period of Data, displays samples, peak-peak values and direction+distance of cable
 * @param type Specify which measurements are done (Pads or Coils); see ::type_of_measurement
 * @retval none
 * @todo Offsets auch hier von PP Werte abziehen (Nicht nur im continuous).
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
 * @brief Measures 5 periods of Data and averages them. Displays samples, peak-peak values and direction+distance of cable
 * @param type Specify which measurements are done (Pads or Coils); see ::type_of_measurement
 * @retval none
 * @todo Anzahl Messungen als Uebergabeparameter
 **/
void Continuous_Measurement(type_of_measurement type)
{
	static Struct_ADC_Values Res;
	uint16_t i=0;
	uint32_t PP1=0, PP2=0, PP3=0, PP4=0, PP5=0;

	if(type == PADS)
	{
		for(i=0;i<5;i++)
		{
			Res = Get_Measurement_Data(PADS);
			PP1 = PP1 + Res.PP_Pad1;
			PP2 = PP2 + Res.PP_Pad2;
			PP3 = PP3 + Res.PP_Pad3;
		}
		PP1 = (uint16_t)(PP1/i);
		PP2 = (uint16_t)(PP2/i);
		PP3 = (uint16_t)(PP3/i);

		PP1 = abs(PP1-Offset_PAD1);
		PP2 = abs(PP2-Offset_PAD2);
		PP3 = abs(PP3-Offset_PAD3);

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
		//calculate mean
		PP1 = (uint16_t)(PP1/i);
		PP2 = (uint16_t)(PP2/i);

		//subtract mean
		PP1 = abs(PP1-Offset_Coil1);
		PP2 = abs(PP2-Offset_Coil2);

		Display_Signal_Pads(Res.array_pad1,Res.array_pad2,0);
		Display_peak_peak(PP1,PP2,0);
	}

	if(type == INIT)
	{
		for(i=0;i<20;i++)
		{
			Res = Get_Measurement_Data(INIT);
			PP1 = PP1 + Res.PP_Pad1;
			PP2 = PP2 + Res.PP_Pad2;
			PP3 = PP3 + Res.PP_Pad3;
			PP4 = PP4 + Res.PP_Coil1;
			PP5 = PP5 + Res.PP_Coil2;
		}
		Offset_PAD1 = (uint16_t)(PP1/i);
		Offset_PAD2 = (uint16_t)(PP2/i);
		Offset_PAD3 = (uint16_t)(PP3/i);
		Offset_Coil1 = (uint16_t)(PP4/i);
		Offset_Coil2 = (uint16_t)(PP5/i);

		//Display_Signal_Pads(Res.array_pad1,Res.array_pad2,0);
		//Display_peak_peak(PP1,PP2,0);
	}
	Display_Type_of_Measurement(type);
}


/**
 * @brief Prints all measurements from PADs via UART for debugging
 * @param *pointer1,*pointer2,*pointer3 pointer to uint_16t array with samples
 * @retval none
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
 * @brief Find peakpeak value from given array
 * @param *array pointer to uint_16t array with samples
 * @retval result peakpeak value
 * @todo find size of array
 **/
uint16_t Find_Peakpeak(uint16_t *array)
{
	//static uint16_t result=0;
	uint8_t k=0;
	uint16_t max=0, min=4000;


	 for(k=0;k<NO_SAMPLES-1;k++)
	 {
		 if(abs(array[k]-array[k+1])<200)
		 {
		 	 if(max<array[k]) max=array[k];
		 	 if(min>array[k]) min=array[k];
		 }
	 }
#ifdef DEBUG_UART
	 char text[10];
	 snprintf(text, 10, "MAX=%4d\n", (uint16_t)(max & 0xffff));
	 HAL_UART_Transmit(&huart1, text, 9, 500);
	 snprintf(text, 10, "MIN=%4d\n", (uint16_t)(min & 0xffff));
	 HAL_UART_Transmit(&huart1, text, 9, 500);
#endif

	 return (max-min);

}


/**
 * @brief Finds direction of mains cable
 * @param PP1,PP2,PP3 Peak-peak value from each PAD
 * @retval none
 * @todo Distance
 **/
void Get_Direction(uint16_t PP1, uint16_t PP2, uint16_t PP3)
{

	uint16_t sum=0,mean=0;

	PP1 = (uint16_t) (1*PP1);
	PP2 = (uint16_t) (1.5*PP2);
	PP3 = (uint16_t) (0.8*PP3);

	sum = PP1+PP2+PP3;
	mean = (uint16_t)(sum/3);

	//if cable between 0cm to 10cm
	if(sum>950)
	{
		//left
		if(((PP1-mean>30) && (PP2-mean<0) && (PP3-mean<0)) || ((PP1>PP2) && (PP1>PP3)))
		{
			set_LEDs_direction(6, OFF);
			set_LEDs_direction(0, ON);
		}

		//right
		if(((PP3-mean>30)&&(PP2-mean<0)&&(PP1-mean<0)) || ((PP3>PP1) && (PP3>PP2)))
		{
			set_LEDs_direction(6, OFF);
			set_LEDs_direction(4, ON);
		}
		//middle
		if(((abs(PP3-mean)<30) && (abs(PP2-mean)<30) && (abs(PP1-mean)<30)) || ((PP2>PP1) && (PP2>PP3)))
		{
			set_LEDs_direction(6, OFF);
			set_LEDs_direction(2, ON);
		}
	}
	else
	{

		// if cable present(between 10cm and 20cm)
		if(sum>250)
		{
			blink = !blink;
			set_LEDs_direction(6, blink);
		}
		else
		{
			set_LEDs_direction(6, OFF);
		}
	}


#ifdef DEBUG_UART
	char text[10];
	snprintf(text, 10, "SUM=%4d\n", (uint16_t)(sum & 0xffff));
	HAL_UART_Transmit(&huart1, text, 9, 500);
	snprintf(text, 10, "mea=%4d\n", (uint16_t)(mean & 0xffff));
	HAL_UART_Transmit(&huart1, text, 9, 500);
	snprintf(text, 10, "PP1=%4d\n", (uint16_t)(PP1 & 0xffff));
	HAL_UART_Transmit(&huart1, text, 9, 500);
	snprintf(text, 10, "PP2=%4d\n", (uint16_t)(PP2 & 0xffff));
	HAL_UART_Transmit(&huart1, text, 9, 500);
	snprintf(text, 10, "PP3=%4d\n", (uint16_t)(PP3 & 0xffff));
	HAL_UART_Transmit(&huart1, text, 9, 500);
#endif
}


