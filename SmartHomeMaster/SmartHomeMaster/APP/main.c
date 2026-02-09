/******************************************************************************
 * Module: APP
 * File Name: main.c
 * Description: Source file for Main Application
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "../HAL/Buzzer/buzzer.h"
#include "../HAL/Keypad/keypad_driver.h"
#include "../HAL/LCD/LCD.h"
#include "../HAL/LED/LED.h"
#include "../LIB/std_macros.h"
#include "../MCAL/SPI/SPI.h"
#include "../MCAL/Timer/timer_driver.h"
#include "main_config.h"
#include "menu.h"
#include <avr/io.h>
#include <util/delay.h>

/*******************************************************************************
 *                             Definitions                              *
 *******************************************************************************/
/* --- COMMAND MACROS --- */
#define BLOWER_TURN_ON 0x50
#define BLOWER_TURN_OFF 0x51
#define GET_LDR_STATUS 0x52

/* --- MENU STATE CONSTANTS --- */
#ifndef BLOWER_MENU
#define BLOWER_MENU 50
#endif

/* Global Variables */
volatile uint16 session_counter = 0;
uint8 timeout_flag = FALSE;

uint8 LOGIN_BLOCKED = FALSE;
uint8 temperature = 0;
uint8 temp_ones = NOT_SELECTED;
uint8 temp_tens = NOT_SELECTED;
uint8 login_mode = NO_MODE;
uint8 block_mode_flag = FALSE;
uint8 key_pressed = NOT_PRESSED;
uint8 pass_counter = 0;
uint8 pass[PASS_SIZE];

/* --- GLOBAL SMART VARIABLES --- */
uint8 smart_mode_active = FALSE;
uint8 night_handled = FALSE;

/* Passwords */
uint8 Adminpass[PASS_SIZE] = ADMIN_PASS;
uint8 Gestpass[PASS_SIZE] = GEST_PASS;

/*******************************************************************************
 *                        Functions Definitions                         *
 *******************************************************************************/
void initializeSystem(void);
void printWelcomeScreen(void);
void setAdminPassword(void);
void setGestPassword(void);
void AdiminLogin(void);
void GistLogin(void);

/**
 * @brief  Main Function
 * @return Integer
 */
int main(void) {
  initializeSystem();
  printWelcomeScreen();

  if (SITPASS != TRUE) {
    LCD_vSend_string("Login for");
    LCD_movecursor(2, 1);
    LCD_vSend_string("first time");
    _delay_ms(1000);
    setAdminPassword();
    setGestPassword();
  }
  block_mode_flag = LOGIN_BLOCKED;

  while (1) {
    key_pressed = NOT_PRESSED;

    /* --- LOGIN LOOP --- */
    while (login_mode == NO_MODE) {
      if (block_mode_flag == TRUE) {
        LCD_clearscreen();
        LCD_vSend_string("Login blocked");
        LCD_movecursor(2, 1);
        LCD_vSend_string("wait 20s...");

        uint8 i;
        for (i = 0; i < 20; i++) {
          LED_vTurnOn(BLOCK_LED_PORT, BLOCK_LED_PIN);
          buzzer_click(); // Using HAL driver
          _delay_ms(500);

          LED_vTurnOff(BLOCK_LED_PORT, BLOCK_LED_PIN);
          // Buzzer off is handled inside buzzer_click but here it was toggling
          // manually Logic: PORT |= PIN; delay; PORT &= ~PIN; delay;
          // buzzer_click does: PORT|=PIN; delay(40); PORT &= ~PIN;
          // The original code: ON; delay(500); OFF; delay(500).
          // buzzer_click is short (40ms).
          // I should replicate the blocking alarm sound properly or just use
          // buzzer_alarm? Original: 20 times { ON; 500ms; OFF; 500ms; } = 20
          // seconds. I'll stick to original logic using HAL macros for manual
          // control? But I should use driver functions. I'll use buzzer_alarm()
          // maybe? No, alarm is 6 beeps. I'll manually set pin using DIO? No,
          // BUZZER HAL hides DIO usage? buzzer.h exposes BUZZER_PORT/DDR/PIN.
          // I'll use DIO commands since I am in APP? Or respect abstraction.
          // Abstraction: Add a function to HAL? No, forbidden to add new
          // functions unless necessary. I'll use DIO_write since I know the
          // pin? I'll use the original logic but adapted. Since I removed local
          // buzzer defs, I can't use BUZZER_PORT_REG. I must use definitions
          // from buzzer.h: BUZZER_PORT, BUZZER_PIN.
          BUZZER_PORT |= (1 << BUZZER_PIN);
          _delay_ms(500);
          BUZZER_PORT &= ~(1 << BUZZER_PIN);
          _delay_ms(500);
        }
        block_mode_flag = FALSE;
        LOGIN_BLOCKED = FALSE;
      }

      LCD_clearscreen();
      LCD_vSend_string("Select mode:");
      LCD_movecursor(2, 1);
      LCD_vSend_string("0:Admin 1:Guest");

      while (key_pressed == NOT_PRESSED) {
        key_pressed = keypad_u8check_press();
      }
      buzzer_click();
      _delay_ms(300);

      if (key_pressed != CHECK_ADMIN_MODE && key_pressed != CHECK_GUEST_MODE) {
        LCD_clearscreen();
        LCD_vSend_string("Wrong input");
        key_pressed = NOT_PRESSED;
        _delay_ms(1000);
        continue;
      }

      switch (key_pressed) {
      case CHECK_ADMIN_MODE:
        AdiminLogin();
        break;
      case CHECK_GUEST_MODE:
        GistLogin();
        break;
      }

      if (LOGIN_BLOCKED == TRUE)
        block_mode_flag = TRUE;
    }

    uint8 show_menu = MAIN_MENU;

    /* --- MAIN APPLICATION LOOP --- */
    while (login_mode != NO_MODE) {
      key_pressed = NOT_PRESSED;
      switch (show_menu) {
      case MAIN_MENU: {
        /* --- PHASE 1: SHOW MENU OPTIONS (500ms) --- */
        LCD_clearscreen();
        if (login_mode == ADMIN) {
          LCD_vSend_string("1:Lgh 2:Pas 3:AC");
          LCD_movecursor(2, 1);
          LCD_vSend_string("4:TV 5:Blo 0:Out");
        } else {
          LCD_vSend_string("1:Lght 0:Out");
        }

        /* Wait 500ms scanning for keys */
        uint8 k;
        for (k = 0; k < 50; k++) {
          key_pressed = keypad_u8check_press();
          if (key_pressed != NOT_PRESSED)
            break;
          _delay_ms(10);
        }

        if (key_pressed != NOT_PRESSED) {
          buzzer_click();
          _delay_ms(300);

          if (key_pressed == SELECT_LIGHT_CONTROL)
            show_menu = LIGHT_CONTROL_MENU;
          else if (key_pressed == SELECT_PASSWORD && login_mode == ADMIN)
            show_menu = PASSWORD_MENU;
          else if (key_pressed == SELECT_AIR_CONDITIONING &&
                   login_mode == ADMIN)
            show_menu = AIRCONDITIONING_MENU;
          else if (key_pressed == SELECT_TV && login_mode == ADMIN)
            show_menu = TV_MENU;
          else if (key_pressed == '5' && login_mode == ADMIN)
            show_menu = BLOWER_MENU;

          /* --- LOGOUT LOGIC --- */
          else if (key_pressed == '0') {
            LCD_clearscreen();
            LCD_vSend_string("Shutting Down...");

            SPI_ui8TransmitRecive(ROOM1_TURN_OFF);
            _delay_ms(10);
            SPI_ui8TransmitRecive(ROOM2_TURN_OFF);
            _delay_ms(10);
            SPI_ui8TransmitRecive(ROOM3_TURN_OFF);
            _delay_ms(10);
            SPI_ui8TransmitRecive(ROOM4_TURN_OFF);
            _delay_ms(10);
            SPI_ui8TransmitRecive(TV_TURN_OFF);
            _delay_ms(10);
            SPI_ui8TransmitRecive(AIR_COND_TURN_OFF);
            _delay_ms(10);
            SPI_ui8TransmitRecive(BLOWER_TURN_OFF);
            _delay_ms(10);

            LED_vTurnOff(GUEST_LED_PORT, GUEST_LED_PIN);
            LED_vTurnOff(ADMIN_LED_PORT, ADMIN_LED_PIN);

            smart_mode_active = FALSE;
            login_mode = NO_MODE;
            _delay_ms(1000);
          } else {
            LCD_clearscreen();
            LCD_vSend_string("Wrong input");
            _delay_ms(500);
          }
        } else if (smart_mode_active == TRUE) {
          /* --- PHASE 2: SMART MODE STATUS BLINK (500ms) --- */
          SPI_ui8TransmitRecive(GET_LDR_STATUS);
          _delay_ms(20);
          uint8 ldr_status = SPI_ui8TransmitRecive(DEFAULT_ACK);

          if (ldr_status == 1) {
            /* --- MORNING --- */
            night_handled = FALSE; /* Reset flag */

            LCD_clearscreen();
            LCD_vSend_string("Status: Morning");
            LCD_movecursor(2, 1);
            LCD_vSend_string("Lights OFF...");

            /* Auto OFF */
            SPI_ui8TransmitRecive(ROOM1_TURN_OFF);
            _delay_ms(5);
            SPI_ui8TransmitRecive(ROOM2_TURN_OFF);
            _delay_ms(5);
            SPI_ui8TransmitRecive(ROOM3_TURN_OFF);
            _delay_ms(5);
            SPI_ui8TransmitRecive(ROOM4_TURN_OFF);
            _delay_ms(5);

            /* Wait 500ms */
            for (k = 0; k < 50; k++) {
              key_pressed = keypad_u8check_press();
              if (key_pressed != NOT_PRESSED)
                break;
              _delay_ms(10);
            }
          } else {
            /* --- NIGHT --- */
            if (night_handled == FALSE) {
              /* RESTORED PREVIOUS LOGIC: Ask Yes/No */
              LCD_clearscreen();
              LCD_vSend_string("Night, Light ON?");
              LCD_movecursor(2, 1);
              LCD_vSend_string("1:Yes 2:No");

              key_pressed = NOT_PRESSED;
              /* Loop inside here so we don't flash back to main menu instantly
               */
              uint8 wait_loop;
              for (wait_loop = 0; wait_loop < 50; wait_loop++) {
                key_pressed = u8GetKeyPressed(login_mode);
                if (key_pressed != NOT_PRESSED)
                  break;
                _delay_ms(10);
              }

              if (key_pressed == '1') // YES
              {
                buzzer_click();
                _delay_ms(300);
                LCD_clearscreen();
                LCD_vSend_string("1:All 2:Select");

                key_pressed = NOT_PRESSED;
                while (key_pressed != '1' && key_pressed != '2') {
                  key_pressed = u8GetKeyPressed(login_mode);
                }
                buzzer_click();
                _delay_ms(300);

                if (key_pressed == '1') {
                  SPI_ui8TransmitRecive(ROOM1_TURN_ON);
                  _delay_ms(10);
                  SPI_ui8TransmitRecive(ROOM2_TURN_ON);
                  _delay_ms(10);
                  SPI_ui8TransmitRecive(ROOM3_TURN_ON);
                  _delay_ms(10);
                  SPI_ui8TransmitRecive(ROOM4_TURN_ON);
                  _delay_ms(10);
                  LCD_clearscreen();
                  LCD_vSend_string("All ROOMS' LIGHT");
                  LCD_movecursor(2, 1);
                  LCD_vSend_string("     ARE ON     ");
                  _delay_ms(500);
                } else {
                  show_menu = LIGHT_CONTROL_MENU; /* Go to manual select */
                }
                night_handled = TRUE;
              } else if (key_pressed == '2') // NO
              {
                buzzer_click();
                _delay_ms(300);
                night_handled = TRUE;
              }
            } else {
              /* ALREADY ANSWERED -> JUST SHOW STATUS */
              LCD_clearscreen();
              LCD_vSend_string("Status: Night");
              LCD_movecursor(2, 1);
              LCD_vSend_string("Lights ON...");

              for (k = 0; k < 50; k++) {
                key_pressed = keypad_u8check_press();
                if (key_pressed != NOT_PRESSED)
                  break;
                _delay_ms(10);
              }
            }
          }

          /* Handle Key Pressed during Phase 2 Wait (Navigation) */
          if (key_pressed != NOT_PRESSED && night_handled == TRUE) {
            buzzer_click();
            _delay_ms(300);

            if (key_pressed == SELECT_LIGHT_CONTROL)
              show_menu = LIGHT_CONTROL_MENU;
            else if (key_pressed == SELECT_PASSWORD && login_mode == ADMIN)
              show_menu = PASSWORD_MENU;
            else if (key_pressed == SELECT_AIR_CONDITIONING &&
                     login_mode == ADMIN)
              show_menu = AIRCONDITIONING_MENU;
            else if (key_pressed == SELECT_TV && login_mode == ADMIN)
              show_menu = TV_MENU;
            else if (key_pressed == '5' && login_mode == ADMIN)
              show_menu = BLOWER_MENU;
            else if (key_pressed == '0') {
              LCD_clearscreen();
              LCD_vSend_string("Shutting Down...");
              SPI_ui8TransmitRecive(ROOM1_TURN_OFF);
              _delay_ms(10);
              SPI_ui8TransmitRecive(ROOM2_TURN_OFF);
              _delay_ms(10);
              SPI_ui8TransmitRecive(ROOM3_TURN_OFF);
              _delay_ms(10);
              SPI_ui8TransmitRecive(ROOM4_TURN_OFF);
              _delay_ms(10);
              SPI_ui8TransmitRecive(TV_TURN_OFF);
              _delay_ms(10);
              SPI_ui8TransmitRecive(AIR_COND_TURN_OFF);
              _delay_ms(10);
              SPI_ui8TransmitRecive(BLOWER_TURN_OFF);
              _delay_ms(10);
              LED_vTurnOff(GUEST_LED_PORT, GUEST_LED_PIN);
              LED_vTurnOff(ADMIN_LED_PORT, ADMIN_LED_PIN);
              smart_mode_active = FALSE;
              login_mode = NO_MODE;
              _delay_ms(1000);
            }
          }
        }
        break;
      }

      case BLOWER_MENU:
        do {
          LCD_clearscreen();
          LCD_vSend_string("Blower Control");
          LCD_movecursor(2, 1);
          LCD_vSend_string("1:ON 2:OFF 0:Ret");

          key_pressed = u8GetKeyPressed(login_mode);
          if (key_pressed != NOT_PRESSED) {
            buzzer_click();
            _delay_ms(300);
          } else {
            _delay_ms(100);
          }

          if (key_pressed == '1') {
            SPI_ui8TransmitRecive(BLOWER_TURN_ON);
            LCD_clearscreen();
            LCD_vSend_string("Blower ON");
            _delay_ms(500);
            show_menu = MAIN_MENU;
          } else if (key_pressed == '2') {
            SPI_ui8TransmitRecive(BLOWER_TURN_OFF);
            LCD_clearscreen();
            LCD_vSend_string("Blower OFF");
            _delay_ms(500);
            show_menu = MAIN_MENU;
          } else if (key_pressed == '0')
            show_menu = MAIN_MENU;
        } while ((key_pressed < '0') || (key_pressed > '2'));
        break;

      case LIGHT_CONTROL_MENU:
        do {
          LCD_clearscreen();
          LCD_vSend_string("1:R1 2:R2 3:R3");
          LCD_movecursor(2, 1);
          LCD_vSend_string("4:R4 5:Smt 0:Ret");

          key_pressed = u8GetKeyPressed(login_mode);
          if (key_pressed != NOT_PRESSED) {
            buzzer_click();
            _delay_ms(300);
          } else {
            _delay_ms(100);
          }

          if (key_pressed == SELECT_ROOM1)
            show_menu = ROOM1_MENU;
          else if (key_pressed == SELECT_ROOM2)
            show_menu = ROOM2_MENU;
          else if (key_pressed == SELECT_ROOM3)
            show_menu = ROOM3_MENU;
          else if (key_pressed == SELECT_ROOM4)
            show_menu = ROOM4_MENU;
          else if (key_pressed == '0')
            show_menu = MAIN_MENU;

          else if (key_pressed == '5') {
            /* --- SMART SETUP MENU (Standardized) --- */
            do {
              LCD_clearscreen();
              /* Status on Row 1 */
              if (smart_mode_active)
                LCD_vSend_string("Smart Mode: ON");
              else
                LCD_vSend_string("Smart Mode: OFF");

              /* Options on Row 2 */
              LCD_movecursor(2, 1);
              LCD_vSend_string("1:On 2:Off 0:Ret");

              key_pressed = u8GetKeyPressed(login_mode);
              if (key_pressed != NOT_PRESSED) {
                buzzer_click();
                _delay_ms(300);
              } else {
                _delay_ms(100);
              }

              if (key_pressed == '1') {
                smart_mode_active = TRUE;
                LCD_clearscreen();
                LCD_vSend_string("Smart Enabled");
                _delay_ms(500);
                show_menu = MAIN_MENU; /* Go to main to run logic */
                break;
              } else if (key_pressed == '2') {
                smart_mode_active = FALSE;
                LCD_clearscreen();
                LCD_vSend_string("Smart Disabled");
                _delay_ms(500);
                /* Loop to refresh status */
              } else if (key_pressed == '0') {
                break; /* Return */
              }
            } while (1);
          }

        } while (((key_pressed < '0') || (key_pressed > '5')));
        break;

      case PASSWORD_MENU:
        do {
          LCD_clearscreen();
          LCD_vSend_string("1:Admin 2:Guest");
          LCD_movecursor(2, 1);
          LCD_vSend_string("0:Ret");

          key_pressed = u8GetKeyPressed(login_mode);
          if (key_pressed != NOT_PRESSED) {
            buzzer_click();
            _delay_ms(300);
          } else {
            _delay_ms(100);
          }

          if (key_pressed == SELECT_CHNG_ADMIN_PASS)
            show_menu = CHNG_ADMIN_PASS_MENU;
          else if (key_pressed == SELECT_CHNG_GUEST_PASS)
            show_menu = CHNG_GUEST_PASS_MENU;
          else if (key_pressed == '0')
            show_menu = MAIN_MENU;
        } while (((key_pressed < '0') || (key_pressed > '2')));
        break;

      case CHNG_ADMIN_PASS_MENU:
        LCD_clearscreen();
        setAdminPassword();
        show_menu = MAIN_MENU;
        break;
      case CHNG_GUEST_PASS_MENU:
        LCD_clearscreen();
        setGestPassword();
        show_menu = MAIN_MENU;
        break;

      case AIRCONDITIONING_MENU:
        do {
          LCD_clearscreen();
          LCD_vSend_string("1:Set Temp");
          LCD_movecursor(2, 1);
          LCD_vSend_string("2:Ctrl 0:Ret");
          key_pressed = u8GetKeyPressed(login_mode);
          if (key_pressed != NOT_PRESSED) {
            buzzer_click();
            _delay_ms(300);
          } else {
            _delay_ms(100);
          }

          if (key_pressed == SELECT_SET_TEMPERATURE)
            show_menu = TEMPERATURE_MENU;
          else if (key_pressed == SELECT_AIR_COND_CTRL)
            show_menu = AIRCOND_CTRL_MENU;
          else if (key_pressed == '0')
            show_menu = MAIN_MENU;
        } while (((key_pressed < '0') || (key_pressed > '2')));
        break;

      case ROOM1_MENU:
        vMenuOption(ROOM1_MENU, login_mode);
        show_menu = LIGHT_CONTROL_MENU;
        break;
      case ROOM2_MENU:
        vMenuOption(ROOM2_MENU, login_mode);
        show_menu = LIGHT_CONTROL_MENU;
        break;
      case ROOM3_MENU:
        vMenuOption(ROOM3_MENU, login_mode);
        show_menu = LIGHT_CONTROL_MENU;
        break;
      case ROOM4_MENU:
        vMenuOption(ROOM4_MENU, login_mode);
        show_menu = LIGHT_CONTROL_MENU;
        break;
      case TV_MENU:
        vMenuOption(TV_MENU, login_mode);
        show_menu = MAIN_MENU;
        break;

      case AIRCOND_CTRL_MENU:
        do {
          LCD_clearscreen();
          LCD_vSend_string("AC Control");
          LCD_movecursor(2, 1);
          LCD_vSend_string("1:ON 2:OFF 0:Ret");
          key_pressed = u8GetKeyPressed(login_mode);
          if (key_pressed != NOT_PRESSED) {
            buzzer_click();
            _delay_ms(300);
          } else {
            _delay_ms(100);
          }

          if (key_pressed == '1') {
            SPI_ui8TransmitRecive(AIR_COND_TURN_ON);
            LCD_clearscreen();
            LCD_vSend_string("AC Enabled");
            _delay_ms(500);
            show_menu = AIRCONDITIONING_MENU;
          } else if (key_pressed == '2') {
            SPI_ui8TransmitRecive(AIR_COND_TURN_OFF);
            LCD_clearscreen();
            LCD_vSend_string("AC Disabled");
            _delay_ms(500);
            show_menu = AIRCONDITIONING_MENU;
          } else if (key_pressed == '0')
            show_menu = AIRCONDITIONING_MENU;
        } while (key_pressed < '0' || key_pressed > '2');
        break;

      case TEMPERATURE_MENU:
        temperature = 0;
        while (temperature == 0) {
          key_pressed = NOT_PRESSED;
          LCD_clearscreen();
          LCD_vSend_string("Set temp.:__ ");
          LCD_vSend_char(DEGREES_SYMBOL);
          LCD_vSend_char('C');
          LCD_movecursor(1, 11);
          _delay_ms(200);
          key_pressed = u8GetKeyPressed(login_mode);
          if (key_pressed != NOT_PRESSED) {
            buzzer_click();
            _delay_ms(300);
          } else {
            _delay_ms(250);
          }
          if (key_pressed < '0' || key_pressed > '9') {
            LCD_clearscreen();
            LCD_vSend_string("Wrong input");
            _delay_ms(500);
            continue;
          } else {
            LCD_vSend_char(key_pressed);
            temp_tens = key_pressed - ASCII_ZERO;
            key_pressed = NOT_PRESSED;
          }

          key_pressed = u8GetKeyPressed(login_mode);
          if (key_pressed != NOT_PRESSED) {
            buzzer_click();
            _delay_ms(300);
          } else {
            _delay_ms(250);
          }
          if ((key_pressed < '0' || key_pressed > '9')) {
            LCD_clearscreen();
            LCD_vSend_string("Wrong input");
            _delay_ms(500);
            continue;
          } else {
            LCD_vSend_char(key_pressed);
            temp_ones = key_pressed - ASCII_ZERO;
            key_pressed = NOT_PRESSED;
          }

          temperature = temp_tens * 10 + temp_ones;
          SPI_ui8TransmitRecive(SET_TEMPERATURE);
          _delay_ms(200);
          SPI_ui8TransmitRecive(temperature);
          LCD_clearscreen();
          LCD_vSend_string("Temperature Sent");
          _delay_ms(500);
        }
        show_menu = AIRCONDITIONING_MENU;
        break;
      }
    }
  }
}

/**
 * @brief  Initialize System Modules
 * @return Void
 */
void initializeSystem(void) {
  LED_vInit(ADMIN_LED_PORT, ADMIN_LED_PIN);
  LED_vInit(GUEST_LED_PORT, GUEST_LED_PIN);
  LED_vInit(BLOCK_LED_PORT, BLOCK_LED_PIN);
  LCD_vInit();
  keypad_vInit();
  SPI_vInitMaster();
  buzzer_init();
}

/**
 * @brief  Print Welcome Screen
 * @return Void
 */
void printWelcomeScreen(void) {
  LCD_vSend_string("Welcome to smart");
  LCD_movecursor(2, 1);
  LCD_vSend_string("home system");
  _delay_ms(1000);
  LCD_clearscreen();
}

/**
 * @brief  Set Admin Password
 * @return Void
 */
void setAdminPassword(void) {
  LCD_clearscreen();
  LCD_vSend_string("Set Admin pass");
  LCD_movecursor(2, 1);
  LCD_vSend_string("Admin pass:");
  key_pressed = NOT_PRESSED;
  pass_counter = 0;
  for (pass_counter = 0; pass_counter < PASS_SIZE; pass_counter++) {
    Adminpass[pass_counter] = NOT_STORED;
  }
  pass_counter = 0;
  while (pass_counter < PASS_SIZE) {
    key_pressed = NOT_PRESSED;
    while (key_pressed == NOT_PRESSED) {
      key_pressed = keypad_u8check_press();
    }
    buzzer_click();
    _delay_ms(300);
    Adminpass[pass_counter] = key_pressed - ASCII_ZERO;
    LCD_vSend_char(key_pressed);
    _delay_ms(CHARACTER_PREVIEW_TIME);
    LCD_movecursor(2, 12 + pass_counter);
    LCD_vSend_char(PASSWORD_SYMBOL);
    _delay_ms(100);
    pass_counter++;
  }
  LCD_clearscreen();
  LCD_vSend_string("Pass Saved");
  _delay_ms(500);
  LCD_clearscreen();
  LOGIN_BLOCKED = FALSE;
}

/**
 * @brief  Set Guest Password
 * @return Void
 */
void setGestPassword(void) {
  LCD_clearscreen();
  LCD_vSend_string("Set Guest Pass");
  LCD_movecursor(2, 1);
  LCD_vSend_string("Guest Pass:");
  key_pressed = NOT_PRESSED;
  pass_counter = 0;
  for (pass_counter = 0; pass_counter < PASS_SIZE; pass_counter++) {
    Gestpass[pass_counter] = NOT_STORED;
  }
  pass_counter = 0;
  while (pass_counter < PASS_SIZE) {
    key_pressed = NOT_PRESSED;
    while (key_pressed == NOT_PRESSED) {
      key_pressed = keypad_u8check_press();
    }
    buzzer_click();
    _delay_ms(300);
    Gestpass[pass_counter] = key_pressed - ASCII_ZERO;
    LCD_vSend_char(key_pressed);
    _delay_ms(CHARACTER_PREVIEW_TIME);
    LCD_movecursor(2, 12 + pass_counter);
    LCD_vSend_char(PASSWORD_SYMBOL);
    _delay_ms(100);
    pass_counter++;
  }
  LCD_clearscreen();
  LCD_vSend_string("Pass Saved");
  _delay_ms(500);
  LCD_clearscreen();
  LOGIN_BLOCKED = FALSE;
}

/**
 * @brief  Admin Login Logic
 * @return Void
 */
void AdiminLogin(void) {
  uint8 local_pass_tries = 0;
  while (login_mode != ADMIN) {
    key_pressed = NOT_PRESSED;
    LCD_clearscreen();
    LCD_vSend_string("Admin mode");
    LCD_movecursor(2, 1);
    LCD_vSend_string("Enter Pass:");
    _delay_ms(200);
    pass_counter = 0;
    while (pass_counter < PASS_SIZE) {
      while (key_pressed == NOT_PRESSED) {
        key_pressed = keypad_u8check_press();
      }
      buzzer_click();
      _delay_ms(300);
      pass[pass_counter] = key_pressed - ASCII_ZERO;
      LCD_vSend_char(key_pressed);
      _delay_ms(CHARACTER_PREVIEW_TIME);
      LCD_movecursor(2, 12 + pass_counter);
      LCD_vSend_char(PASSWORD_SYMBOL);
      _delay_ms(100);
      pass_counter++;
      key_pressed = NOT_PRESSED;
    }
    if ((ui8ComparePass(pass, Adminpass, PASS_SIZE)) == TRUE) {
      login_mode = ADMIN;
      LCD_clearscreen();
      LCD_vSend_string("Right pass");
      LCD_movecursor(2, 1);
      LCD_vSend_string("Admin mode");
      buzzer_double();
      _delay_ms(500);
      LED_vTurnOn(ADMIN_LED_PORT, ADMIN_LED_PIN);
      LCD_clearscreen();
    } else {
      local_pass_tries++;
      LCD_clearscreen();
      LCD_vSend_string("Wrong Pass");
      LCD_movecursor(2, 1);
      LCD_vSend_string("Tries left:");
      LCD_vSend_char(TRIES_ALLOWED - local_pass_tries + ASCII_ZERO);
      _delay_ms(1000);
      if (local_pass_tries >= TRIES_ALLOWED) {
        LOGIN_BLOCKED = TRUE;
        return;
      }
    }
  }
}

/**
 * @brief  Guest Login Logic
 * @return Void
 */
void GistLogin(void) {
  uint8 local_pass_tries = 0;
  while (login_mode != GUEST) {
    key_pressed = NOT_PRESSED;
    LCD_clearscreen();
    LCD_vSend_string("Guest mode");
    LCD_movecursor(2, 1);
    LCD_vSend_string("Enter pass:");
    _delay_ms(200);
    pass_counter = 0;
    while (pass_counter < PASS_SIZE) {
      while (key_pressed == NOT_PRESSED) {
        key_pressed = keypad_u8check_press();
      }
      buzzer_click();
      _delay_ms(300);
      pass[pass_counter] = key_pressed - ASCII_ZERO;
      LCD_vSend_char(key_pressed);
      _delay_ms(CHARACTER_PREVIEW_TIME);
      LCD_movecursor(2, 12 + pass_counter);
      LCD_vSend_char(PASSWORD_SYMBOL);
      _delay_ms(100);
      pass_counter++;
      key_pressed = NOT_PRESSED;
    }
    if (ui8ComparePass(pass, Gestpass, PASS_SIZE) == TRUE) {
      login_mode = GUEST;
      LCD_clearscreen();
      LCD_vSend_string("Right pass");
      LCD_movecursor(2, 1);
      LCD_vSend_string("Guest mode");
      buzzer_double();
      _delay_ms(500);
      LED_vTurnOn(GUEST_LED_PORT, GUEST_LED_PIN);
      LCD_clearscreen();
    } else {
      local_pass_tries++;
      LCD_clearscreen();
      LCD_vSend_string("Wrong pass");
      LCD_movecursor(2, 1);
      LCD_vSend_string("Tries left:");
      LCD_vSend_char(TRIES_ALLOWED - local_pass_tries + ASCII_ZERO);
      _delay_ms(1000);
      if (local_pass_tries >= TRIES_ALLOWED) {
        LOGIN_BLOCKED = TRUE;
        return;
      }
    }
  }
}