/** ***************************************************************************
 * @file
 * @brief The menu
 *
 * Initializes and displays the menu.
 * @n Provides the function MENU_check_transition() for polling user actions.
 * The variable MENU_transition is set to the touched menu item.
 * If no touch has occurred the variable MENU_transition is set to MENU_NONE
 * @n If the interrupt handler is enabled by calling BSP_TS_ITConfig();
 * the variable MENU_transition is set to the touched menu entry as above.
 * @n Either call once BSP_TS_ITConfig() to enable the interrupt
 * or MENU_check_transition() in the main while loop for polling.
 * @n The function MENU_get_transition() returns the new menu item.
 *
 * @author  Hanspeter Hochreutener, hhrt@zhaw.ch
 * @date	30.04.2020
 *****************************************************************************/


/******************************************************************************
 * Includes
 *****************************************************************************/
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"

#include "menu.h"
#include "gpio.h"

/******************************************************************************
 * Defines
 *****************************************************************************/
#define MENU_FONT				&Font12	///< Possible font sizes: 8 12 16 20 24
#define MENU_HEIGHT				40		///< Height of menu bar
#define MENU_MARGIN				2		///< Margin around a menu entry
/** Position of menu bar: 0 = top, (BSP_LCD_GetYSize()-MENU_HEIGHT) = bottom */
#define MENU_Y					(BSP_LCD_GetYSize()-MENU_HEIGHT)
#define ADC_DAC_RES				12


/******************************************************************************
 * Variables
 *****************************************************************************/
static MENU_item_t MENU_transition = MENU_NONE;	///< Transition to this menu
static MENU_entry_t MENU_entry[MENU_ENTRY_COUNT] = {
		{"Single",	"",			LCD_COLOR_BLACK,	LCD_COLOR_LIGHTBLUE},
		{"Conti-",	"nuous",	LCD_COLOR_BLACK,	LCD_COLOR_LIGHTGREEN},
		{"LED",	    "",			LCD_COLOR_BLACK,	LCD_COLOR_LIGHTRED},
		{"PWR",	    "",			LCD_COLOR_BLACK,	LCD_COLOR_LIGHTCYAN},
		{"DMA",	    "scan",		LCD_COLOR_BLACK,	LCD_COLOR_LIGHTMAGENTA},
		{"DAC",	    "on",		LCD_COLOR_BLACK,	LCD_COLOR_LIGHTYELLOW}
};										///< All the menu entries


/******************************************************************************
 * Functions
 *****************************************************************************/


/** ***************************************************************************
 * @brief Draw the menu onto the display.
 *
 * Each menu entry has two lines.
 * Text and background colors are applied.
 * @n These attributes are defined in the variable MENU_draw[].
 *****************************************************************************/
void MENU_draw(void)
{
	BSP_LCD_SetFont(MENU_FONT);
	uint32_t x, y, m, w, h;
	y = MENU_Y;
	m = MENU_MARGIN;
	w = BSP_LCD_GetXSize()/MENU_ENTRY_COUNT;
	h = MENU_HEIGHT;
	for (uint32_t i = 0; i < MENU_ENTRY_COUNT; i++) {
		x = i*w;
		BSP_LCD_SetTextColor(MENU_entry[i].back_color);
		BSP_LCD_FillRect(x+m, y+m, w-2*m, h-2*m);
		BSP_LCD_SetBackColor(MENU_entry[i].back_color);
		BSP_LCD_SetTextColor(MENU_entry[i].text_color);
		BSP_LCD_DisplayStringAt(x+3*m, y+m,
				(uint8_t *)MENU_entry[i].line1, LEFT_MODE);
		BSP_LCD_DisplayStringAt(x+3*m, y+h/2,
				(uint8_t *)MENU_entry[i].line2, LEFT_MODE);
	}
}


/** ***************************************************************************
 * @brief lets direction led blink.
 * @param none
 * @retval none
 *
 *****************************************************************************/
void blink_direction(void)
{
	HAL_GPIO_TogglePin(led1_GPIO_Port, led1_Pin);
	HAL_GPIO_TogglePin(led2_GPIO_Port, led2_Pin);
	HAL_GPIO_TogglePin(led3_GPIO_Port, led3_Pin);
	HAL_GPIO_TogglePin(led4_GPIO_Port, led4_Pin);
	HAL_GPIO_TogglePin(led5_GPIO_Port, led5_Pin);

}


/** ***************************************************************************
 * @brief Displays peak peak value from PAD1-PAD3 for debbuging purposes
 * @param PP1,PP2,PP3 Peak peak values to be displayed
 * @retval none
 * @bug First line for display must be outputed twice in order to work corectly
 *
 *****************************************************************************/
void Display_peak_peak(uint16_t PP1, uint16_t PP2, uint16_t PP3)
{

	const uint32_t Y_OFFSET = 260;
	const uint32_t X_SIZE = 240;
	const uint32_t f = (1 << ADC_DAC_RES) / Y_OFFSET + 1;	// Scaling factor
	//char text[14]={""};
	uint32_t data;
	uint32_t data_last;
	// Clear the display
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillRect(0, 0, X_SIZE, Y_OFFSET+1);

	//BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetFont(&Font20);
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

	char text[16];
	snprintf(text, 15, "              ", (int)(PP1 & 0xffff));
	BSP_LCD_DisplayStringAt(0, 50, (uint8_t *)text, LEFT_MODE);
	snprintf(text, 15, "PP Pad1 = %4d", (int)(PP1 & 0xffff));
	BSP_LCD_DisplayStringAt(0, 50, (uint8_t *)text, LEFT_MODE);
	snprintf(text, 15, "PP Pad2 = %4d", (int)(PP2 & 0xffff));
	BSP_LCD_DisplayStringAt(0, 80, (uint8_t *)text, LEFT_MODE);
	snprintf(text, 15, "PP Pad3 = %4d", (int)(PP3 & 0xffff));
	BSP_LCD_DisplayStringAt(0, 110, (uint8_t *)text, LEFT_MODE);


	/*
	snprintf(text, 14, "1. PAD1= %4d", (int)(PP1 & 0xffff));
	Delay_us(1);
	BSP_LCD_DisplayStringAt(0, 80, (uint8_t *)text, LEFT_MODE);
	Delay_us(1);
	snprintf(text, 14, "2. PAD1= %4d", (int)(PP2 & 0xffff));
	BSP_LCD_DisplayStringAt(0, 110, (uint8_t *)text, LEFT_MODE);*/

	/*
	// Draw the lower values of the buffer content as a curve
	data = (ADC_samples[0] & 0xffff) / f;
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	for (uint32_t i = 1; i < ADC_NUMS; i++){
		data_last = data;
		data = (ADC_samples[i] & 0xffff) / f;
		if (data > Y_OFFSET) { data = Y_OFFSET; }// Limit value, prevent crash
		BSP_LCD_DrawLine(4*(i-1), Y_OFFSET-data_last, 4*i, Y_OFFSET-data);
		BSP_LCD_DrawPixel(4*(i-1), Y_OFFSET-data_last, LCD_COLOR_RED);
	}
	BSP_LCD_DrawPixel(4*(ADC_NUMS-1), 260-data, LCD_COLOR_RED);
	// Draw the upper values of the buffer content as a curve
	data = (ADC_samples[0] >> 16) / f;
	if (data > Y_OFFSET) { data = Y_OFFSET; }	// Limit value, prevent crash
	BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
	for (uint32_t i = 1; i < ADC_NUMS; i++){
		data_last = data;
		data = (ADC_samples[i] >> 16) / f;
		if (data > Y_OFFSET) { data = Y_OFFSET; }// Limit value, prevent crash
		BSP_LCD_DrawLine(4*(i-1), Y_OFFSET-data_last, 4*i, Y_OFFSET-data);
		BSP_LCD_DrawPixel(4*(i-1), Y_OFFSET-data_last, LCD_COLOR_BLUE);
	}
	BSP_LCD_DrawPixel(4*(ADC_NUMS-1), 260-data, LCD_COLOR_BLUE);
	// Clear buffer and flag
	for (uint32_t i = 0; i < ADC_NUMS; i++){
		ADC_samples[i] = 0;
		ADC_samples[i] = 0;
	}
	ADC_sample_count = 0;
	MEAS_data_ready = false;

*/
}




/** ***************************************************************************
 * @brief Shows a hint at startup.
 *
 *****************************************************************************/
void MENU_hint(void)
{
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_DisplayStringAt(5,10, (uint8_t *)"DOMIS-CODE", LEFT_MODE);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(5, 60, (uint8_t *)"Touch a menu item", LEFT_MODE);
	BSP_LCD_DisplayStringAt(5, 80, (uint8_t *)"to start a demo", LEFT_MODE);

	//BSP_LCD_DisplayStringAt(5, 120, (uint8_t *)"Blamiere bim programmiere", LEFT_MODE);
	//BSP_LCD_DisplayStringAt(5, 140, (uint8_t *)"programmiere", LEFT_MODE);
}


/** ***************************************************************************
 * @brief Set a menu entry.
 * @param [in] item number of menu bar
 * @param [in] entry attributes for that item
 *
 * @note Call MENU_draw() to update the display.
 *****************************************************************************/
void MENU_set_entry(const MENU_item_t item, const MENU_entry_t entry)
{
	if ((0 <= item) && (MENU_ENTRY_COUNT > item)) {
		MENU_entry[item] = entry;
	}
}


/** ***************************************************************************
 * @brief Get a menu entry.
 * @param [in] item number of menu bar
 * @return Menu_entry[item] or Menu_entry[0] if item not in range
 *****************************************************************************/
MENU_entry_t MENU_get_entry(const MENU_item_t item)
{
	MENU_entry_t entry = MENU_entry[0];
	if ((0 <= item) && (MENU_ENTRY_COUNT > item)) {
		entry = MENU_entry[item];
	}
	return entry;
}


/** ***************************************************************************
 * @brief Get menu selection/transition
 *
 * @return the selected MENU_item or MENU_NONE if no MENU_item was selected
 *
 * MENU_transition is used as a flag.
 * When the value is read by calling MENU_get_transition()
 * this flag is cleared, respectively set to MENU_NONE.
 *****************************************************************************/
MENU_item_t MENU_get_transition(void)
{
	MENU_item_t item = MENU_transition;
	MENU_transition = MENU_NONE;
	return item;
}


/** ***************************************************************************
 * @brief Check for selection/transition
 *
 * If the last transition has been consumed (MENU_NONE == MENU_transition)
 * and the touchscreen has been touched for a defined period
 * the variable MENU_transition is set to the touched item.
 *****************************************************************************/
void MENU_check_transition(void)
{
	static MENU_item_t item_old = MENU_NONE;
	static MENU_item_t item_new = MENU_NONE;
	static TS_StateTypeDef  TS_State;	// State of the touch controller
	BSP_TS_GetState(&TS_State);			// Get the state
	if (TS_State.TouchDetected) {		// If a touch was detected
		/* Do only if last transition not pending anymore */
		if (MENU_NONE == MENU_transition) {
			item_old = item_new;		// Store old item
			/* If touched within the menu bar? */
			if ((MENU_Y < TS_State.Y) && (MENU_Y+MENU_HEIGHT > TS_State.Y)) {
				item_new = TS_State.X	// Calculate new item
						/ (BSP_LCD_GetXSize()/MENU_ENTRY_COUNT);
				if ((0 > item_new) || (MENU_ENTRY_COUNT <= item_new)) {
					item_new = MENU_NONE;	// Out of bounds
				}
				if (item_new == item_old) {	// 2 times the same menu item
					item_new = MENU_NONE;
					MENU_transition = item_old;
				}
			}
		}
	}
}



/** ***************************************************************************
 * @brief Interrupt handler for the touchscreen
 *
 * @note BSP_TS_ITConfig(); must be called in the main function
 * to enable touchscreen interrupt.
 * @note There are timing issues when interrupt is enabled.
 * It seems that polling is the better choice with this evaluation board.
 * @n Call MENU_check_transition() from the while loop in main for polling.
 *
 * The touchscreen interrupt is connected to PA15.
 * @n The interrupt handler for external line 15 to 10 is called.
 *****************************************************************************/
void EXTI15_10_IRQHandler(void)
{
	if (EXTI->PR & EXTI_PR_PR15) {		// Check if interrupt on touchscreen
		EXTI->PR |= EXTI_PR_PR15;		// Clear pending interrupt on line 15
		if (BSP_TS_ITGetStatus()) {		// Get interrupt status
			BSP_TS_ITClear();				// Clear touchscreen controller int.
			MENU_check_transition();
		}
		EXTI->PR |= EXTI_PR_PR15;		// Clear pending interrupt on line 15
	}
}

