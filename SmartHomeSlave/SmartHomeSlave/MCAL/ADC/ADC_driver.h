/******************************************************************************
 * Module: ADC
 * File Name: ADC_driver.h
 * Description: Header file for ADC module
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
#ifndef MCAL_ADC_ADC_DRIVER_H_
#define MCAL_ADC_ADC_DRIVER_H_

/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "../../LIB/STD_Types.h"

/*******************************************************************************
 *                    Software Interfaces Declarations                  *
 *******************************************************************************/
/**
 * @brief  Initialize ADC
 * @return Void
 */
void ADC_vinit(void);

/**
 * @brief  Read value from ADC
 * @return 16-bit ADC value
 */
uint16 ADC_u16Read(void);

#endif /* MCAL_ADC_ADC_DRIVER_H_ */