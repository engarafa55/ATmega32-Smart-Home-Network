/******************************************************************************
 * Module: LCD
 * File Name: LCD.c
 * Description: Source file for LCD module
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "LCD.h"

static void send_falling_edge(void);

/*******************************************************************************
 *                        Functions Definitions                         *
 *******************************************************************************/
/**
 * @brief  Initialize LCD
 * @return Void
 */
void LCD_vInit(void) {
#if defined eight_bits_mode
  DIO_vsetPINDir(LCD_PORT, LCD_FIRST_PIN, 1);
  DIO_vsetPINDir(LCD_PORT, LCD_SECOND_PIN, 1);
  DIO_vsetPINDir(LCD_PORT, LCD_THIRD_PIN, 1);
  DIO_vsetPINDir(LCD_PORT, LCD_FOURTH_PIN, 1);
  DIO_vsetPINDir(LCD_PORT, LCD_FIFTH_PIN, 1);
  DIO_vsetPINDir(LCD_PORT, LCD_SIXTH_PIN, 1);
  DIO_vsetPINDir(LCD_PORT, LCD_SEVENTH_PIN, 1);
  DIO_vsetPINDir(LCD_PORT, LCD_EIGHTH_PIN, 1);

  DIO_vsetPINDir(LCD_CONTROL_PORT, LCD_EN_PIN, 1);
  DIO_vsetPINDir(LCD_CONTROL_PORT, LCD_RW_PIN, 1);
  DIO_vsetPINDir(LCD_CONTROL_PORT, LCD_RS_PIN, 1);

  DIO_write(LCD_CONTROL_PORT, LCD_RW_PIN, 0);
  LCD_vSend_cmd(EIGHT_BIT_MODE);
  LCD_vSend_cmd(CLR_SCREEN);
  LCD_vSend_cmd(DISPLAY_ON_CURSOR_ON);
  _delay_ms(10);

#elif defined four_bits_mode
  DIO_vsetPINDir(LCD_PORT, LCD_FIFTH_PIN, 1);
  DIO_vsetPINDir(LCD_PORT, LCD_SIXTH_PIN, 1);
  DIO_vsetPINDir(LCD_PORT, LCD_SEVENTH_PIN, 1);
  DIO_vsetPINDir(LCD_PORT, LCD_EIGHTH_PIN, 1);

  DIO_vsetPINDir(LCD_CONTROL_PORT, LCD_EN_PIN, 1);
  DIO_vsetPINDir(LCD_CONTROL_PORT, LCD_RW_PIN, 1);
  DIO_vsetPINDir(LCD_CONTROL_PORT, LCD_RS_PIN, 1);

  DIO_write(LCD_CONTROL_PORT, LCD_RW_PIN, 0);
  LCD_vSend_cmd(FOUR_BIT_MODE_I);
  LCD_vSend_cmd(FOUR_BIT_MODE_II);
  LCD_vSend_cmd(CLR_SCREEN);
  LCD_vSend_cmd(DISPLAY_ON_CURSOR_ON);
  _delay_ms(10);
#endif
}

/**
 * @brief  Generate falling edge on EN pin
 * @return Void
 */
static void send_falling_edge(void) {
  DIO_write(LCD_CONTROL_PORT, LCD_EN_PIN, 1);
  _delay_ms(2);
  DIO_write(LCD_CONTROL_PORT, LCD_EN_PIN, 0);
  _delay_ms(2);
}

/**
 * @brief  Send command to LCD
 * @param  cmd Command code
 * @return Void
 */
void LCD_vSend_cmd(uint8 cmd) {
#if defined eight_bits_mode
  DIO_write_port(LCD_PORT, cmd);
  DIO_write(LCD_CONTROL_PORT, LCD_RS_PIN, 0);
  send_falling_edge();

#elif defined four_bits_mode
  clear_high_nibble(LCD_PORT, cmd);
  dio_write_highnibble(LCD_PORT, cmd & 0xf0);
  DIO_write(LCD_CONTROL_PORT, LCD_RS_PIN, 0);
  send_falling_edge();
  clear_high_nibble(LCD_PORT, cmd);
  dio_write_highnibble(LCD_PORT, cmd << 4);
  DIO_write(LCD_CONTROL_PORT, LCD_RS_PIN, 0);
  send_falling_edge();
#endif
}

/**
 * @brief  Send character to LCD
 * @param  data Character to display
 * @return Void
 */
void LCD_vSend_char(uint8 data) {
#if defined eight_bits_mode
  DIO_write_port(LCD_PORT, data);
  DIO_write(LCD_CONTROL_PORT, LCD_RS_PIN, 1);
  send_falling_edge();

#elif defined four_bits_mode
  clear_high_nibble(LCD_PORT, data);
  dio_write_highnibble(LCD_PORT, data & 0xf0);
  DIO_write(LCD_CONTROL_PORT, LCD_RS_PIN, 1);
  send_falling_edge();
  clear_high_nibble(LCD_PORT, data);
  dio_write_highnibble(LCD_PORT, data << 4);
  DIO_write(LCD_CONTROL_PORT, LCD_RS_PIN, 1);
  send_falling_edge();
#endif
}

/**
 * @brief  Send string to LCD
 * @param  data String pointer
 * @return Void
 */
void LCD_vSend_string(char *data) {
  while ((*data) != 0) {
    LCD_vSend_char(*data);
    data++;
  }
}

/**
 * @brief  Clear LCD screen
 * @return Void
 */
void LCD_clearscreen(void) { LCD_vSend_cmd(CLR_SCREEN); }

/**
 * @brief  Move cursor to specific position
 * @param  row Row number (1 or 2)
 * @param  coloumn Column number (1-16)
 * @return Void
 */
void LCD_movecursor(uint8 row, uint8 coloumn) {
  uint8 data;
  if (row > 2 || row < 1 || coloumn > 16 || coloumn < 1) {
    data = 0x80;
  }

  if (row == 1) {
    data = 0x80 + coloumn - 1;
  } else if (row == 2) {
    data = 0xc0 + coloumn - 1;
  }

  LCD_vSend_cmd(data); // set the position  of the cursor
}