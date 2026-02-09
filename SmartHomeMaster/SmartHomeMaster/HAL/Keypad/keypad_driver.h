/******************************************************************************
 * Module: Keypad
 * File Name: keypad_driver.h
 * Description: Header file for Keypad module
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
#ifndef HAL_KEYPAD_KEYPAD_DRIVER_H_
#define HAL_KEYPAD_KEYPAD_DRIVER_H_

/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "../../MCAL/DIO/DIO.h"

/*******************************************************************************
 *                             Definitions                              *
 *******************************************************************************/
#define NOT_PRESSED 0xff

/*******************************************************************************
 *                    Software Interfaces Declarations                  *
 *******************************************************************************/
/**
 * @brief  Initialize Keypad pins
 * @return Void
 */
void keypad_vInit(void);

/**
 * @brief  Check if any key is pressed and return it
 * @return Pressed key or NOT_PRESSED
 */
uint8 keypad_u8check_press(void);

#endif /* HAL_KEYPAD_KEYPAD_DRIVER_H_ */