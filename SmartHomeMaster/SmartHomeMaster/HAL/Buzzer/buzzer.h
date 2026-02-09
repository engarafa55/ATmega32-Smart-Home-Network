/******************************************************************************
 * Module: Buzzer
 * File Name: buzzer.h
 * Description: Header file for Buzzer module
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
#ifndef HAL_BUZZER_BUZZER_H_
#define HAL_BUZZER_BUZZER_H_

/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "../../LIB/std_macros.h"
#include <avr/io.h>
#include <util/delay.h>

/*******************************************************************************
 *                             Definitions                              *
 *******************************************************************************/
#define BUZZER_PORT PORTC
#define BUZZER_DDR DDRC
#define BUZZER_PIN PC3

/*******************************************************************************
 *                    Software Interfaces Declarations                  *
 *******************************************************************************/
/**
 * @brief  Initialize Buzzer pin
 * @return Void
 */
void buzzer_init(void);

/**
 * @brief  Generate a short beep
 * @return Void
 */
void buzzer_click(void);

/**
 * @brief  Generate a double beep
 * @return Void
 */
void buzzer_double(void);

/**
 * @brief  Generate an alarm sequence
 * @return Void
 */
void buzzer_alarm(void);

#endif /* HAL_BUZZER_BUZZER_H_ */