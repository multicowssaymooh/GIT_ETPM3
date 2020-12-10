/** ***************************************************************************
 * @file
 * @brief See measure.c
 *
 * Prefix MENU
 *
 *****************************************************************************/

#ifndef MEASURE_H_
#define MEASURE_H_




void Delay_us (uint32_t delay);

uint16_t* 	Get_ADC_Values_Pads(void);
uint16_t	Get_ADC_Values_Coils(void);
uint16_t	Get_ADC_Pad3(void);



#endif
