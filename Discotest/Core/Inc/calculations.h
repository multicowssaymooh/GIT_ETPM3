/** ***************************************************************************
 * @file
 * @brief See calculations.c
 *
 * Prefix MENU
 *
 *****************************************************************************/

#ifndef CALCULATIONS_H_
#define CALCULATIONS_H_



typedef struct {
	uint16_t PP_Pad1;					/// Peak Peak value Pad 1
	uint16_t PP_Pad2;					/// Peak Peak value Pad 2
	uint16_t PP_Pad3;					/// Peak Peak value Pad 3
	uint16_t PP_Coil1;					/// Peak Peak value Coil 1
	uint16_t PP_Coil2;					/// Peak Peak value Coil 2
} Struct_ADC_Values;

Struct_ADC_Values Single_Measurement_Pads(void);

void UART_Transmit_Pad(uint16_t *pointer1,uint16_t *pointer2,uint16_t *pointer3);
uint16_t Find_Peakpeak(uint16_t *array);
void Get_Direction(uint16_t PP1, uint16_t PP2, uint16_t PP3);


#endif
