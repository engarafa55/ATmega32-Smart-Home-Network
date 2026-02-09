/******************************************************************************
 * Module: LCD
 * File Name: LCD.h
 * Description: Header file for LCD module
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
#ifndef HAL_LCD_LCD_H_
#define HAL_LCD_LCD_H_

/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "../../MCAL/DIO/DIO.h"
#include "LCD_config.h"

/*******************************************************************************
 *                             Definitions                              *
 *******************************************************************************/
#define CLR_SCREEN 0X01
#define DISPLAY_ON_CURSOR_ON 0X0E
#define EIGHT_BIT_MODE 0X38
#define FOUR_BIT_MODE_I 0X02
#define FOUR_BIT_MODE_II 0X28

/*******************************************************************************
 *                    Software Interfaces Declarations                  *
 *******************************************************************************/
/**
 * @brief  Initialize LCD
 * @return Void
 */
void LCD_vInit(void);

/**
 * @brief  Send command to LCD
 * @param  cmd Command code
 * @return Void
 */
void LCD_vSend_cmd(uint8 cmd);

/**
 * @brief  Send character to LCD
 * @param  data Character to display
 * @return Void
 */
void LCD_vSend_char(uint8 data);

/**
 * @brief  Send string to LCD
 * @param  data String pointer
 * @return Void
 */
void LCD_vSend_string(char *data);

/**
 * @brief  Clear LCD screen
 * @return Void
 */
void LCD_clearscreen(void);

/**
 * @brief  Move cursor to specific position
 * @param  row Row number (1 or 2)
 * @param  column Column number (1-16)
 * @return Void
 */
void LCD_movecursor(uint8 row, uint8 column);

#endif /* HAL_LCD_LCD_H_ */