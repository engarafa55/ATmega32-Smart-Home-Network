/******************************************************************************
 * Module: LED
 * File Name: LED.h
 * Description: Header file for Slave LED
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
#ifndef HAL_LED_LED_H_
#define HAL_LED_LED_H_

/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "../../LIB/STD_Types.h"
#include "../../MCAL/DIO/DIO.h"

/*******************************************************************************
 *                    Software Interfaces Declarations                  *
 *******************************************************************************/
/**
 * @brief  Initialize LED pin as output
 * @param  portname Port Name
 * @param  pinnumber Pin Number
 * @return Void
 */
void LED_vInit(uint8 portname, uint8 pinnumber);

/**
 * @brief  Turn LED On
 * @param  portname Port Name
 * @param  pinnumber Pin Number
 * @return Void
 */
void LED_vTurnOn(uint8 portname, uint8 pinnumber);

/**
 * @brief  Turn LED Off
 * @param  portname Port Name
 * @param  pinnumber Pin Number
 * @return Void
 */
void LED_vTurnOff(uint8 portname, uint8 pinnumber);

/**
 * @brief  Toggle LED State
 * @param  portname Port Name
 * @param  pinnumber Pin Number
 * @return Void
 */
void LED_vToggle(uint8 portname, uint8 pinnumber);

/**
 * @brief  Read LED Status
 * @param  portname Port Name
 * @param  pinnumber Pin Number
 * @return Status (0 or 1)
 */
uint8 LED_u8ReadStatus(uint8 portname, uint8 pinnumber);

#endif /* HAL_LED_LED_H_ */