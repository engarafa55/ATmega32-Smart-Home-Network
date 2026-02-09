/******************************************************************************
 * Module: DIO
 * File Name: DIO.c
 * Description: Source file for DIO module
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "DIO.h"
#include "../../LIB/std_macros.h"
#include <avr/io.h>

/*******************************************************************************
 *                        Functions Definitions                         *
 *******************************************************************************/
/**
 * @brief  Set the direction of a specific pin
 * @param  portname Port Name (A, B, C, D)
 * @param  pinnumber Pin Number (0-7)
 * @param  direction Direction (1 for Output, 0 for Input)
 * @return Void
 */
void DIO_vsetPINDir(uint8 portname, uint8 pinnumber, uint8 direction) {
  switch (portname) {
  case 'A':
    if (direction == 1) {
      SET_BIT(DDRA, pinnumber);
    } else {
      CLR_BIT(DDRA, pinnumber);
    }
    break;
  case 'B':
    if (direction == 1) {
      SET_BIT(DDRB, pinnumber);
    } else {
      CLR_BIT(DDRB, pinnumber);
    }
    break;
  case 'C':
    if (direction == 1) {
      SET_BIT(DDRC, pinnumber);
    } else {
      CLR_BIT(DDRC, pinnumber);
    }
    break;
  case 'D':
    if (direction == 1) {
      SET_BIT(DDRD, pinnumber);
    } else {
      CLR_BIT(DDRD, pinnumber);
    }
    break;
  default:
    break;
  }
}

/**
 * @brief  Write a value to a specific pin
 * @param  portname Port Name (A, B, C, D)
 * @param  pinnumber Pin Number (0-7)
 * @param  outputvalue Value (1 for High, 0 for Low)
 * @return Void
 */
void DIO_write(uint8 portname, uint8 pinnumber, uint8 outputvalue) {
  switch (portname) {
  case 'A':
    if (outputvalue == 1) {
      SET_BIT(PORTA, pinnumber);
    } else {
      CLR_BIT(PORTA, pinnumber);
    }
    break;
  case 'B':
    if (outputvalue == 1) {
      SET_BIT(PORTB, pinnumber);
    } else {
      CLR_BIT(PORTB, pinnumber);
    }
    break;
  case 'C':
    if (outputvalue == 1) {
      SET_BIT(PORTC, pinnumber);
    } else {
      CLR_BIT(PORTC, pinnumber);
    }
    break;
  case 'D':
    if (outputvalue == 1) {
      SET_BIT(PORTD, pinnumber);
    } else {
      CLR_BIT(PORTD, pinnumber);
    }
    break;
  default:
    break;
  }
}

/**
 * @brief  Read the value of a specific pin
 * @param  portname Port Name (A, B, C, D)
 * @param  pinnumber Pin Number (0-7)
 * @return Value of the pin (0 or 1)
 */
uint8 DIO_u8read(uint8 portname, uint8 pinnumber) {
  uint8 return_value = 0;
  switch (portname) {
  case 'A':
    return_value = Read_bIT(PINA, pinnumber);
    break;
  case 'B':
    return_value = Read_bIT(PINB, pinnumber);
    break;
  case 'C':
    return_value = Read_bIT(PINC, pinnumber);
    break;
  case 'D':
    return_value = Read_bIT(PIND, pinnumber);
    break;
  default:
    break;
  }
  return return_value;
}

/**
 * @brief  Toggle the value of a specific pin
 * @param  portname Port Name (A, B, C, D)
 * @param  pinnumber Pin Number (0-7)
 * @return Void
 */
void DIO_toggle(uint8 portname, uint8 pinnumber) {
  switch (portname) {
  case 'A':
    TOG_BIT(PORTA, pinnumber);
    break;
  case 'B':
    TOG_BIT(PORTB, pinnumber);
    break;
  case 'C':
    TOG_BIT(PORTC, pinnumber);
    break;
  case 'D':
    TOG_BIT(PORTD, pinnumber);
    break;
  default:
    break;
  }
}

/**
 * @brief  Write a value to the entire port
 * @param  portname Port Name (A, B, C, D)
 * @param  portvalue Value to write (8-bit)
 * @return Void
 */
void DIO_write_port(uint8 portname, uint8 portvalue) {
  switch (portname) {
  case 'A':
    PORTA = portvalue;
    break;
  case 'B':
    PORTB = portvalue;
    break;
  case 'C':
    PORTC = portvalue;
    break;
  case 'D':
    PORTD = portvalue;
    break;
  default:
    break;
  }
}

/**
 * @brief  Write to the high nibble of a port
 * @param  portname Port Name (A, B, C, D)
 * @param  value Value to write (upper 4 bits used)
 * @return Void
 */
void dio_write_highnibble(uint8 portname, uint8 value) {
  switch (portname) {
  case 'A':
    PORTA |= (value & 0xf0);
    break;
  case 'B':
    PORTB |= (value & 0xf0);
    break;
  case 'C':
    PORTC |= (value & 0xf0);
    break;
  case 'D':
    PORTD |= (value & 0xf0);
    break;
  default:
    break;
  }
}

/**
 * @brief  Clear the high nibble of a port
 * @param  portname Port Name (A, B, C, D)
 * @return Void
 */
void clear_high_nibble(uint8 portname) {
  switch (portname) {
  case 'A':
    PORTA &= 0x0f;
    break;
  case 'B':
    PORTB &= 0x0f;
    break;
  case 'C':
    PORTC &= 0x0f;
    break;
  case 'D':
    PORTD &= 0x0f;
    break;
  default:
    break;
  }
}

/**
 * @brief  Enable or disable internal pull-up resistor
 * @param  portname Port Name (A, B, C, D)
 * @param  pinnumber Pin Number (0-7)
 * @param  connect_pullup 1 to enable, 0 to disable
 * @return Void
 */
void DIO_vconnectpullup(uint8 portname, uint8 pinnumber, uint8 connect_pullup) {
  switch (portname) {
  case 'A':
    if (connect_pullup == 1) {
      CLR_BIT(SFIOR, PUD);
      CLR_BIT(DDRA, pinnumber);
      SET_BIT(PORTA, pinnumber);
    } else {
      CLR_BIT(PORTA, pinnumber);
    }
    break;
  case 'B':
    if (connect_pullup == 1) {
      CLR_BIT(SFIOR, PUD);
      CLR_BIT(DDRB, pinnumber);
      SET_BIT(PORTB, pinnumber);
    } else {
      CLR_BIT(PORTB, pinnumber);
    }
    break;
  case 'C':
    if (connect_pullup == 1) {
      CLR_BIT(SFIOR, PUD);
      CLR_BIT(DDRC, pinnumber);
      SET_BIT(PORTC, pinnumber);
    } else {
      CLR_BIT(PORTC, pinnumber);
    }
    break;
  case 'D':
    if (connect_pullup == 1) {
      CLR_BIT(SFIOR, PUD);
      CLR_BIT(DDRD, pinnumber);
      SET_BIT(PORTD, pinnumber);
    } else {
      CLR_BIT(PORTD, pinnumber);
    }
    break;
  }
}