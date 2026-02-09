/******************************************************************************
 * Module: Timer
 * File Name: timer_driver.c
 * Description: Source file for Timer module
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "timer_driver.h"

/*******************************************************************************
 *                        Functions Definitions                         *
 *******************************************************************************/
/**
 * @brief  Initialize Timer0 in CTC Mode
 * @return Void
 */
void timer0_initializeCTC(void) {
  OCR0 = 78;

  SET_BIT(TCCR0, WGM01);
  CLR_BIT(TCCR0, WGM00);

  SET_BIT(TCCR0, CS00);
  CLR_BIT(TCCR0, CS01);
  SET_BIT(TCCR0, CS02);

  sei();

  SET_BIT(TIMSK, OCIE0);
}

/**
 * @brief  Stop Timer0
 * @return Void
 */
void timer0_stop(void) {
  CLR_BIT(TCCR0, CS00);
  CLR_BIT(TCCR0, CS01);
  CLR_BIT(TCCR0, CS02);
}

/**
 * @brief  Initialize Timer0 in Fast PWM Mode
 * @return Void
 */
void timer_initializefastpwm(void) {
  OCR0 = 128;

  SET_BIT(TCCR0, WGM00);
  SET_BIT(TCCR0, WGM01);

  sei();

  SET_BIT(TCCR0, COM01);
  SET_BIT(TIMSK, TOIE0);

  SET_BIT(TCCR0, CS00);
  CLR_BIT(TCCR0, CS01);
  SET_BIT(TCCR0, CS02);
}

/**
 * @brief  Change Duty Cycle of PWM
 * @param  duty Duty cycle percentage (0-100)
 * @return Void
 */
void change_dutycycle(float64 duty) { OCR0 = (duty / 100) * 256; }