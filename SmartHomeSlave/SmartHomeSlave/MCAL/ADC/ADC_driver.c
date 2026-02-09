/******************************************************************************
 * Module: ADC
 * File Name: ADC_driver.c
 * Description: Source file for ADC module
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "../../LIB/STD_Types.h"
#include "../../LIB/std_macros.h"
#include <avr/io.h>

/*******************************************************************************
 *                        Functions Definitions                         *
 *******************************************************************************/
/**
 * @brief  Initialize ADC
 * @return Void
 */
void ADC_vinit(void) {
  SET_BIT(ADMUX, REFS0);
  SET_BIT(ADMUX, REFS1);

  SET_BIT(ADCSRA, ADEN); // Enable ADC

  SET_BIT(ADCSRA, ADPS2);
  SET_BIT(ADCSRA, ADPS1);
}

/**
 * @brief  Read value from ADC
 * @return 16-bit ADC value
 */
uint16 ADC_u16Read(void) {
  uint16 read_val;
  SET_BIT(ADCSRA, ADSC);
  while (IS_BIT_CLR(ADCSRA, ADIF))
    ;
  SET_BIT(ADCSRA, ADIF);
  read_val = (ADCL);
  read_val |= (ADCH << 8);
  return read_val;
}