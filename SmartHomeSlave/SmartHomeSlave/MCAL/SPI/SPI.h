/******************************************************************************
 * Module: SPI
 * File Name: SPI.h
 * Description: Header file for Slave SPI
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
#ifndef MCAL_SPI_SPI_H_
#define MCAL_SPI_SPI_H_

/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "../../LIB/STD_MESSAGES.h"
#include "../../LIB/STD_Types.h"
#include "../../LIB/std_macros.h"

/*******************************************************************************
 *                             Definitions                              *
 *******************************************************************************/
#define SPI_SCK 7
#define SPI_MISO 6
#define SPI_MOSI 5
#define SPI_SS 4

/*******************************************************************************
 *                    Software Interfaces Declarations                  *
 *******************************************************************************/
/**
 * @brief  Initialize SPI as Master
 * @return Void
 */
void SPI_vInitMaster(void);

/**
 * @brief  Initialize SPI as Slave
 * @return Void
 */
void SPI_vInitSlave(void);

/**
 * @brief  Transmit and receive a byte via SPI
 * @param  data Data to transmit
 * @return Received data
 */
uint8 SPI_ui8TransmitRecive(uint8 data);

#endif /* MCAL_SPI_SPI_H_ */