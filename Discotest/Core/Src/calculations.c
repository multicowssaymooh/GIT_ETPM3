/** ***************************************************************************
 * @file
 * @brief Calculations for the cable monitor
 *
 * @n Provides the function Get_Measurement_Data() to gather all values from Pads/Coils which returns a struct (see ::Struct_ADC_Values) with all peak-peak values and array with samples
 * @n This function is used in Single_Measurement() and Continuous_Measurement(), where it is called either once or 5 times to build the mean values.
 * @n Find_Peakpeak() calculates the peak-peak value from the given array. Get_Direction() calculates the direction of the mains cable and displays the distance
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


/// This array contains the lookup table for the measurement of the distance
uint16_t LUT[201]={12889,12084,11291,10516,9763,9038,8346,7693,7083,6522,6016,5543,5092,4680,4325,4043,3825,3641,3481,3333,3186,3038,2897,2762,2635,2518,2407,2300,2200,2111,2036,1975,1922,1875,1828,1779,1724,1666,1609,1556,1513,1478,1448,1421,1394,1367,1338,1309,1280,1253,1228,1205,1182,1160,1139,1119,1100,1081,1063,1046,1030,1015,1000,987,974,961,949,936,924,912,900,888,875,863,851,839,827,815,804,792,781,770,759,748,737,727,717,707,697,687,678,669,661,653,646,639,631,623,616,607,598,588,577,564,552,539,526,514,502,492,483,475,468,461,454,448,442,436,430,425,420,415,410,406,401,397,393,389,385,381,377,373,369,365,361,357,353,349,345,341,337,333,328,324,319,315,310,305,301,296,291,286,281,275,270,264,259,254,249,244,240,236,232,229,225,222,219,216,213,210,207,204,202,199,197,195,192,190,187,184,181,177,172,166,160,154,148,142,137,132,129,126,123,121,118,116,114,112,111,110,109};



/**
 * @brief Reads one period of samples from PAD1..3 and Coil1..2, calculates peak peak values and returns them as struct
 * @param type Specify which measurements are done; see ::type_of_measurement
 * @retval Str_ADC_Values Struct with PP values from pad1..3 and coil1..2; see ::Struct_ADC_Values
 * @todo update names in coil measurements, PAD1..2 hold values from coils1..2
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
		  // TIM2_ellapses set every 1/2000Hz to get 40 samples per period, see timer irq
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
 * @todo Subtract offset/zerolevel from coil1..2
 **/
void Single_Measurement(type_of_measurement type)
{
	static Struct_ADC_Values Res;
	uint32_t PP1=0, PP2=0, PP3=0, PP4=0, PP5=0;

	if(type == PADS)
	{

		 Res = Get_Measurement_Data(PADS);

		 PP1 = PP1 + Res.PP_Pad1;
		 PP2 = PP2 + Res.PP_Pad2;
		 PP3 = PP3 + Res.PP_Pad3;

		 PP1 = abs(PP1-Offset_PAD1);
		 PP2 = abs(PP2-Offset_PAD2);
		 PP3 = abs(PP3-Offset_PAD3);

		 Display_Signal_Pads(Res.array_pad1,Res.array_pad2,Res.array_pad3);
		 Display_peak_peak(PP1,PP2,PP3);
		 Get_Direction(PP1,PP2,PP3);
	}
	if(type == COILS)
	{

		 Res = Get_Measurement_Data(COILS);

		 Display_Signal_Pads(Res.array_pad1,Res.array_pad2,0);
		 Display_peak_peak(Res.PP_Coil1,Res.PP_Coil2,0);
	}
	Display_Type_of_Measurement(type);
}

/**
 * @brief Measures 5 periods of Data and averages them. Displays samples, peak-peak values and direction+distance of cable
 * @param type Specify which measurements are done (Pads or Coils); see ::type_of_measurement
 * @retval none
 * @todo pass number of periods as function parameter
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
		//calculate mean
		PP1 = (uint16_t)(PP1/i);
		PP2 = (uint16_t)(PP2/i);
		PP3 = (uint16_t)(PP3/i);

		//subtract offset
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

		//subtract offset
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
 * @todo calculate size of array
 **/
uint16_t Find_Peakpeak(uint16_t *array)
{
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
 * @todo Display distance at position which is cleared completely every iteration. (lower pixels stay on)
 **/
void Get_Direction(uint16_t PP1, uint16_t PP2, uint16_t PP3)
{

	uint16_t sum=0,mean=0;
	uint16_t distance=0;
	uint16_t i=0;

	PP1 = (uint16_t) (1*PP1);
	PP2 = (uint16_t) (1.6*PP2);
	PP3 = (uint16_t) (0.8*PP3);

	sum = PP1+PP2+PP3;
	mean = (uint16_t)(sum/3);

	// find index (distance9 from measured value
	for(i=200;i>=0;i--)
	{
		distance = LUT[i];
		if(LUT[i]>sum)
		{
			distance = i;
			break;
		}
	}

	char text[15];
	snprintf(text, 15, "Distance%4dmm", (uint16_t)(distance & 0xffff));
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetFont(&Font20);
	BSP_LCD_DisplayStringAt(15, 250, (uint8_t *)text, LEFT_MODE);

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


