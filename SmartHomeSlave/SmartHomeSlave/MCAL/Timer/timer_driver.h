/******************************************************************************
 * Module: Timer
 * File Name: timer_driver.h
 * Description: Header file for Timer module
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
#ifndef MCAL_TIMER_TIMER_DRIVER_H_
#define MCAL_TIMER_TIMER_DRIVER_H_

/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "../../LIB/STD_Types.h"
#include "../../LIB/std_macros.h"
#include <avr/interrupt.h>
#include <avr/io.h>

/*******************************************************************************
 *                    Software Interfaces Declarations                  *
 *******************************************************************************/
/**
 * @brief  Initialize Timer0 in CTC Mode
 * @return Void
 */
void timer0_initializeCTC(void);

/**
 * @brief  Stop Timer0
 * @return Void
 */
void timer0_stop(void);

/**
 * @brief  Initialize Timer0 in Fast PWM Mode
 * @return Void
 */
void timer_initializefastpwm(void);

/**
 * @brief  Change Duty Cycle of PWM
 * @param  duty Duty cycle percentage (0-100)
 * @return Void
 */
void change_dutycycle(float64 duty);

#endif /* MCAL_TIMER_TIMER_DRIVER_H_ */