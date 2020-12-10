/** ***************************************************************************
 * @file
 * @brief See calculations.c
 *
 * Prefix MENU
 *
 *****************************************************************************/

#ifndef CALCULATIONS_H_
#define CALCULATIONS_H_

void Single_Measurement_Pads(void);

void UART_Transmit_Pad(uint16_t *pointer1,uint16_t *pointer2,uint16_t *pointer3);
uint16_t Find_Peakpeak(uint16_t *array);
uint8_t Get_Direction(uint16_t pad1, uint16_t pad2, uint16_t pad3);

#endif
