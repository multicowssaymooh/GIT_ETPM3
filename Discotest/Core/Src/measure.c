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
