/******************************************************************************
 * Module: APP
 * File Name: menu.c
 * Description: Source file for Menu module
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "menu.h"

extern volatile uint16 session_counter;
extern uint8 timeout_flag;

/*******************************************************************************
 *                        Functions Definitions                         *
 *******************************************************************************/
/**
 * @brief  Compare two passwords
 * @param  pass1 Pointer to first password
 * @param  pass2 Pointer to second password
 * @param  size Size of password
 * @return TRUE if match, FALSE otherwise
 */
uint8 ui8ComparePass(const uint8 *pass1, const uint8 *pass2, const uint8 size) {
  uint8 pin_counter = 0;
  uint8 ret_value = TRUE;
  while (pin_counter < size) {
    if (*(pass1 + pin_counter) != *(pass2 + pin_counter)) {
      ret_value = FALSE;
      break;
    }
    pin_counter++;
  }
  return ret_value;
}

/**
 * @brief  Handle Menu Options for Rooms
 * @param  u8SelectedRoom Selected Room ID
 * @param  u8LoginMode Login Mode (Admin/Guest)
 * @return Void
 */
void vMenuOption(const uint8 u8SelectedRoom, const uint8 u8LoginMode) {
  uint8 u8StatusCode = 0;
  uint8 u8TurnOnCode = 0;
  uint8 u8TurnOffCode = 0;
  uint8 response = DEFAULT_ACK;
  uint8 key_pressed = NOT_PRESSED;

  do {
    LCD_clearscreen();
    switch (u8SelectedRoom) {
    case ROOM1_MENU:
      u8StatusCode = ROOM1_STATUS;
      u8TurnOnCode = ROOM1_TURN_ON;
      u8TurnOffCode = ROOM1_TURN_OFF;
      LCD_vSend_string("Room1 S:");
      break;
    case ROOM2_MENU:
      u8StatusCode = ROOM2_STATUS;
      u8TurnOnCode = ROOM2_TURN_ON;
      u8TurnOffCode = ROOM2_TURN_OFF;
      LCD_vSend_string("Room2 S:");
      break;
    case ROOM3_MENU:
      u8StatusCode = ROOM3_STATUS;
      u8TurnOnCode = ROOM3_TURN_ON;
      u8TurnOffCode = ROOM3_TURN_OFF;
      LCD_vSend_string("Room3 S:");
      break;
    case ROOM4_MENU:
      u8StatusCode = ROOM4_STATUS;
      u8TurnOnCode = ROOM4_TURN_ON;
      u8TurnOffCode = ROOM4_TURN_OFF;
      LCD_vSend_string("Room4 S:");
      break;
    case TV_MENU:
      u8StatusCode = TV_STATUS;
      u8TurnOnCode = TV_TURN_ON;
      u8TurnOffCode = TV_TURN_OFF;
      LCD_vSend_string("TV S:");
      break;
    case AIRCOND_CTRL_MENU:
      u8StatusCode = AIR_COND_STATUS;
      u8TurnOnCode = AIR_COND_TURN_ON;
      u8TurnOffCode = AIR_COND_TURN_OFF;
      LCD_vSend_string("Air Cond. S:");
      break;
    }

    SPI_ui8TransmitRecive(u8StatusCode);
    _delay_ms(100);
    response = SPI_ui8TransmitRecive(DEMAND_RESPONSE);
    if (response == ON_STATUS) {
      LCD_vSend_string("ON");
    } else {
      LCD_vSend_string("OFF");
    }

    LCD_movecursor(2, 1);
    LCD_vSend_string("1-On 2-Off 0-RET");

    key_pressed = u8GetKeyPressed(u8LoginMode);
    _delay_ms(100);

    if (key_pressed == '1') {
      SPI_ui8TransmitRecive(u8TurnOnCode);
    } else if (key_pressed == '2') {
      SPI_ui8TransmitRecive(u8TurnOffCode);
    } else if ((key_pressed != NOT_PRESSED) && (key_pressed != '0')) {
      LCD_clearscreen();
      LCD_vSend_string("Wrong input");
      _delay_ms(500);
    }
  } while (((key_pressed < '0') || (key_pressed > '2')) &&
           (timeout_flag == FALSE));
}

/**
 * @brief  Get Key Pressed with Timeout
 * @param  u8LoginMode Login Mode (Admin/Guest)
 * @return Key Pressed
 */
uint8 u8GetKeyPressed(const uint8 u8LoginMode) {
  uint8 key_pressed = NOT_PRESSED;
  while (key_pressed == NOT_PRESSED) {
    if ((session_counter >= ADMIN_TIMEOUT && u8LoginMode == ADMIN) ||
        (session_counter >= GUEST_TIMEOUT &&
         u8LoginMode == GUEST)) // check for timeout
    {
      timeout_flag = TRUE;
      break;
    }

    key_pressed = keypad_u8check_press();
  }
  return key_pressed;
}