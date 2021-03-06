/** ***************************************************************************
 * @file
 * @brief See calculations.c
 *
 *****************************************************************************/

#ifndef CALCULATIONS_H_
#define CALCULATIONS_H_


typedef enum {									/// Measure pads
				PADS=0,							/// Measure coils
				COILS,							/// Measure both in order to get offset/zero levels (init)
				INIT}type_of_measurement;


typedef struct {						/// Peak Peak value Pad 1
	uint16_t PP_Pad1;					/// Peak Peak value Pad 2
	uint16_t PP_Pad2;					/// Peak Peak value Pad 3
	uint16_t PP_Pad3;					/// Peak Peak value Coil 1
	uint16_t PP_Coil1;					/// Peak Peak value Coil 2
	uint16_t PP_Coil2;					/// Array of ADC Values for PAD1/COIL1 1 period
	uint16_t array_pad1[50];			/// Array of ADC Values for PAD2/COIL2 1 period
	uint16_t array_pad2[50];			/// Array of ADC Values for PAD3 1 period
	uint16_t array_pad3[50];
} Struct_ADC_Values;

							/// Zeroing offset Pad 1
uint16_t 	Offset_PAD1, 	/// Zeroing offset Pad 2
			Offset_PAD2, 	/// Zeroing offset Pad 3
			Offset_PAD3;
							/// Zeroing offset Coil 1
uint16_t 	Offset_Coil1,	/// Zeroing offset Coil 2
			Offset_Coil2;


Struct_ADC_Values Get_Measurement_Data(type_of_measurement type);
void Single_Measurement(type_of_measurement type);
void Continuous_Measurement(type_of_measurement type);


void UART_Transmit_Pad(uint16_t *pointer1,uint16_t *pointer2,uint16_t *pointer3);
uint16_t Find_Peakpeak(uint16_t *array);
void Get_Direction(uint16_t PP1, uint16_t PP2, uint16_t PP3);



#endif
