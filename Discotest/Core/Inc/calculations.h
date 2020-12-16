/** ***************************************************************************
 * @file
 * @brief See calculations.c
 *
 * Prefix MENU
 *
 *****************************************************************************/

#ifndef CALCULATIONS_H_
#define CALCULATIONS_H_

typedef enum {PADS=0,COILS}type_of_measurement;

typedef struct {
	uint16_t PP_Pad1;					/// Peak Peak value Pad 1
	uint16_t PP_Pad2;					/// Peak Peak value Pad 2
	uint16_t PP_Pad3;					/// Peak Peak value Pad 3
	uint16_t PP_Coil1;					/// Peak Peak value Coil 1
	uint16_t PP_Coil2;					/// Peak Peak value Coil 2
	uint16_t array_pad1[50];			/// Array of ADC Values for PAD1 1 period
	uint16_t array_pad2[50];			/// Array of ADC Values for PAD2 1 period
	uint16_t array_pad3[50];			/// Array of ADC Values for PAD3 1 period
} Struct_ADC_Values;

Struct_ADC_Values Get_Measurement_Data(void);
void Single_Measurement(type_of_measurement type);
void Continuous_Measurement(void);


void UART_Transmit_Pad(uint16_t *pointer1,uint16_t *pointer2,uint16_t *pointer3);
uint16_t Find_Peakpeak(uint16_t *array);
void Get_Direction(uint16_t PP1, uint16_t PP2, uint16_t PP3);



#endif
