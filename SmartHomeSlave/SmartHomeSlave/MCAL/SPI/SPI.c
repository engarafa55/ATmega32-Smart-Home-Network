/******************************************************************************
 * Module: SPI
 * File Name: SPI.c
 * Description: Source file for SPI module
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "SPI.h"
#include "../../LIB/STD_MESSAGES.h"
#include "../../LIB/std_macros.h"
#include <avr/io.h>

/*******************************************************************************
 *                        Functions Definitions                         *
 *******************************************************************************/
/**
 * @brief  Initialize SPI as Master
 * @return Void
 */
void SPI_vInitMaster(void) {
  DDRB |= (1 << SPI_MOSI) | (1 << SPI_SS) | (1 << SPI_SCK);
  SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

/**
 * @brief  Initialize SPI as Slave
 * @return Void
 */
void SPI_vInitSlave(void) {
  DDRB |= (1 << SPI_MISO);
  SPCR |= (1 << SPE);
}

/**
 * @brief  Transmit and receive a byte via SPI
 * @param  data Data to transmit
 * @return Received data
 */
uint8 SPI_ui8TransmitRecive(uint8 data) {
  SPDR = data;
  while (((SPSR & (1 << SPIF)) >> SPIF) == 0)
    ;
  return SPDR;
}