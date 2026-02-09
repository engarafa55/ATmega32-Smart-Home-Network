/******************************************************************************
 * Module: APP
 * File Name: menu.h
 * Description: Header file for Menu module
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
#ifndef APP_MENU_H_
#define APP_MENU_H_

/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "../HAL/Keypad/keypad_driver.h"
#include "../HAL/LCD/LCD.h"
#include "../LIB/STD_MESSAGES.h"
#include "../LIB/STD_Types.h"
#include "../MCAL/SPI/SPI.h"
#include "main_config.h"

/*******************************************************************************
 *                    Software Interfaces Declarations                  *
 *******************************************************************************/
/**
 * @brief  Compare two passwords
 * @param  pass1 Pointer to first password
 * @param  pass2 Pointer to second password
 * @param  size Size of password
 * @return TRUE if match, FALSE otherwise
 */
uint8 ui8ComparePass(const uint8 *pass1, const uint8 *pass2, const uint8 size);

/**
 * @brief  Handle Menu Options for Rooms
 * @param  u8SelectedRoom Selected Room ID
 * @param  u8LoginMode Login Mode (Admin/Guest)
 * @return Void
 */
void vMenuOption(const uint8 u8SelectedRoom, const uint8 u8LoginMode);

/**
 * @brief  Get Key Pressed with Timeout
 * @param  u8LoginMode Login Mode (Admin/Guest)
 * @return Key Pressed
 */
uint8 u8GetKeyPressed(const uint8 u8LoginMode);

#endif /* APP_MENU_H_ */