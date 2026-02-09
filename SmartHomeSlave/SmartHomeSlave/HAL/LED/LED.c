/******************************************************************************
 * Module: LED
 * File Name: LED.c
 * Description: Source file for LED module
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "LED.h"

/*******************************************************************************
 *                        Functions Definitions                         *
 *******************************************************************************/
/**
 * @brief  Initialize LED pin as output
 * @param  portname Port Name
 * @param  pinnumber Pin Number
 * @return Void
 */
void LED_vInit(uint8 portname, uint8 pinnumber) {
  DIO_vsetPINDir(portname, pinnumber, 1);
}

/**
 * @brief  Turn LED On
 * @param  portname Port Name
 * @param  pinnumber Pin Number
 * @return Void
 */
void LED_vTurnOn(uint8 portname, uint8 pinnumber) {
  DIO_write(portname, pinnumber, 1);
}

/**
 * @brief  Turn LED Off
 * @param  portname Port Name
 * @param  pinnumber Pin Number
 * @return Void
 */
void LED_vTurnOff(uint8 portname, uint8 pinnumber) {
  DIO_write(portname, pinnumber, 0);
}

/**
 * @brief  Toggle LED State
 * @param  portname Port Name
 * @param  pinnumber Pin Number
 * @return Void
 */
void LED_vToggle(uint8 portname, uint8 pinnumber) {
  DIO_toggle(portname, pinnumber);
}

/**
 * @brief  Read LED Status
 * @param  portname Port Name
 * @param  pinnumber Pin Number
 * @return Status (0 or 1)
 */
uint8 LED_u8ReadStatus(uint8 portname, uint8 pinnumber) {
  return DIO_u8read(portname, pinnumber);
}