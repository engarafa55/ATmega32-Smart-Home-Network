/******************************************************************************
 * Module: DIO
 * File Name: DIO.h
 * Description: Header file for DIO module
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
#ifndef MCAL_DIO_DIO_H_
#define MCAL_DIO_DIO_H_

/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "../../LIB/STD_Types.h"
#include "../../LIB/std_macros.h"
#include "DIO_Slave_cfg.h"
#include <util/delay.h>

/*******************************************************************************
 *                    Software Interfaces Declarations                  *
 *******************************************************************************/
/**
 * @brief  Set the direction of a specific pin
 * @param  portname Port Name (A, B, C, D)
 * @param  pinnumber Pin Number (0-7)
 * @param  direction Direction (1 for Output, 0 for Input)
 * @return Void
 */
void DIO_vsetPINDir(uint8 portname, uint8 pinnumber, uint8 direction);

/**
 * @brief  Write a value to a specific pin
 * @param  portname Port Name (A, B, C, D)
 * @param  pinnumber Pin Number (0-7)
 * @param  outputvalue Value (1 for High, 0 for Low)
 * @return Void
 */
void DIO_write(uint8 portname, uint8 pinnumber, uint8 outputvalue);

/**
 * @brief  Read the value of a specific pin
 * @param  portname Port Name (A, B, C, D)
 * @param  pinnumber Pin Number (0-7)
 * @return Value of the pin (0 or 1)
 */
uint8 DIO_u8read(uint8 portname, uint8 pinnumber);

/**
 * @brief  Toggle the value of a specific pin
 * @param  portname Port Name (A, B, C, D)
 * @param  pinnumber Pin Number (0-7)
 * @return Void
 */
void DIO_toggle(uint8 portname, uint8 pinnumber);

/**
 * @brief  Write a value to the entire port
 * @param  portname Port Name (A, B, C, D)
 * @param  portvalue Value to write (8-bit)
 * @return Void
 */
void DIO_write_port(uint8 portname, uint8 portvalue);

/**
 * @brief  Clear the high nibble of a port
 * @param  portname Port Name (A, B, C, D)
 * @return Void
 */
void clear_high_nibble(uint8 portname);

/**
 * @brief  Write to the high nibble of a port
 * @param  portname Port Name (A, B, C, D)
 * @param  value Value to write (upper 4 bits used)
 * @return Void
 */
void dio_write_highnibble(uint8 portname, uint8 value);

/**
 * @brief  Enable or disable internal pull-up resistor
 * @param  portname Port Name (A, B, C, D)
 * @param  pinnumber Pin Number (0-7)
 * @param  connect_pullup 1 to enable, 0 to disable
 * @return Void
 */
void DIO_vconnectpullup(uint8 portname, uint8 pinnumber, uint8 connect_pullup);

#endif /* MCAL_DIO_DIO_H_ */