/******************************************************************************
 * Module: APP
 * File Name: APP_slave_Macros.h
 * Description: Configuration Macros for Slave APP
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
#ifndef APP_APP_SLAVE_MACROS_H_
#define APP_APP_SLAVE_MACROS_H_

/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "../LIB/STD_Types.h"

/*******************************************************************************
 *                             Definitions                              *
 *******************************************************************************/
#define AIR_CONDTIONING_ON (uint8)0x01
#define AIR_CONDTIONING_OFF (uint8)0x00

#define AIR_COND_PIN (uint8)2
#define TV_PIN (uint8)3
#define ROOM1_PIN (uint8)4
#define ROOM2_PIN (uint8)5
#define ROOM3_PIN (uint8)6
#define ROOM4_PIN (uint8)7

#define AIR_COND_PORT (uint8)'D'
#define TV_PORT (uint8)'D'
#define ROOM1_PORT (uint8)'D'
#define ROOM2_PORT (uint8)'D'
#define ROOM3_PORT (uint8)'D'
#define ROOM4_PORT (uint8)'D'

#endif /* APP_APP_SLAVE_MACROS_H_ */