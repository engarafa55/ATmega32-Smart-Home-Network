/******************************************************************************
 * Module: Buzzer
 * File Name: buzzer.c
 * Description: Source file for Buzzer module
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "buzzer.h"

/*******************************************************************************
 *                        Functions Definitions                         *
 *******************************************************************************/
/**
 * @brief  Initialize Buzzer pin
 * @return Void
 */
void buzzer_init(void) {
  BUZZER_DDR |= (1 << BUZZER_PIN); // set PC3 as output
}

/**
 * @brief  Generate a short beep
 * @return Void
 */
void buzzer_click(void) {
  BUZZER_PORT |= (1 << BUZZER_PIN);
  _delay_ms(40);
  BUZZER_PORT &= ~(1 << BUZZER_PIN);
}

/**
 * @brief  Generate a double beep
 * @return Void
 */
void buzzer_double(void) {
  buzzer_click();
  _delay_ms(80);
  buzzer_click();
}

/**
 * @brief  Generate an alarm sequence
 * @return Void
 */
void buzzer_alarm(void) {
  for (unsigned char i = 0; i < 6; i++) {
    BUZZER_PORT |= (1 << BUZZER_PIN);
    _delay_ms(150);
    BUZZER_PORT &= ~(1 << BUZZER_PIN);
    _delay_ms(150);
  }
}