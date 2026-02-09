/******************************************************************************
 * Module: APP
 * File Name: main.c
 * Description: Source file for Slave Application (Priority Control + Auto
 * Logic) Author: Abdelrahman Arafa Email: engarafa55@gmail.com
 ******************************************************************************/
/*******************************************************************************
 *                             Includes                                 *
 *******************************************************************************/
#include "../HAL/LED/LED.h"
#include "../LIB/STD_MESSAGES.h"
#include "../LIB/std_macros.h"
#include "../MCAL/ADC/ADC_driver.h"
#include "../MCAL/DIO/DIO.h"
#include "../MCAL/SPI/SPI.h"
#include "../MCAL/Timer/timer_driver.h"
#include "APP_slave_Macros.h"
#include <avr/interrupt.h>
#include <avr/io.h>

/*******************************************************************************
 *                        Function Prototypes                           *
 *******************************************************************************/
uint16 ADC_u16ReadChannel_Custom(uint8 channel);
void vFanSetPositive(void);
void vFanSetNegative(void);
void vFanStop(void);
void vSystemInit(void);

/*******************************************************************************
 *                             Definitions                              *
 *******************************************************************************/
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* Command Macros */
#define BLOWER_TURN_ON 0x50
#define BLOWER_TURN_OFF 0x51
#define GET_LDR_STATUS 0x52

/* Hardware Definitions */
#define FAN_PORT PORTB
#define FAN_DDR DDRB
#define FAN_EN_PIN 0
#define FAN_IN1_PIN 1
#define FAN_IN2_PIN 2

#define HEATER_PORT PORTD
#define HEATER_DDR DDRD
#define HEATER_PIN 1

#define LDR_CHANNEL 1
#define TEMP_CHANNEL 0

/* Logic Constants */
#define LDR_THRESHOLD 512
#define MAX_TEMP 40
#define MIN_FAN_TEMP 30

volatile uint16 required_temperature = 24;
volatile uint16 temp_sensor_reading = 0;
volatile uint16 ldr_reading = 0;

/* Control Flags */
volatile uint8 fan_duty_cycle = 0;
volatile uint8 blower_mode = FALSE;

/* MASTER GATE for Temperature Automation (AC, Heater, AutoFan).
   TRUE: Sensors control devices.
   FALSE: Devices are forced OFF (Manual Shutdown).
*/
volatile uint8 auto_climate_active = TRUE;

/*******************************************************************************
 *                        Functions Definitions                         *
 *******************************************************************************/
/**
 * @brief  Custom ADC Read with Channel Selection
 * @param  channel ADC Channel
 * @return ADC Value
 */
uint16 ADC_u16ReadChannel_Custom(uint8 channel) {
  ADMUX &= 0xE0;
  ADMUX |= (channel & 0x1F);
  ADCSRA |= (1 << ADSC);
  while ((ADCSRA & (1 << ADIF)) == 0)
    ;
  ADCSRA |= (1 << ADIF);
  return ADC;
}

/**
 * @brief  Set Fan Direction Positive
 * @return Void
 */
void vFanSetPositive(void) {
  FAN_PORT |= (1 << FAN_IN1_PIN);
  FAN_PORT &= ~(1 << FAN_IN2_PIN);
}

/**
 * @brief  Set Fan Direction Negative
 * @return Void
 */
void vFanSetNegative(void) {
  FAN_PORT &= ~(1 << FAN_IN1_PIN);
  FAN_PORT |= (1 << FAN_IN2_PIN);
}

/**
 * @brief  Stop Fan
 * @return Void
 */
void vFanStop(void) {
  FAN_PORT &= ~(1 << FAN_IN1_PIN);
  FAN_PORT &= ~(1 << FAN_IN2_PIN);
  fan_duty_cycle = 0;
}

/**
 * @brief  Initialize Slave System
 * @return Void
 */
void vSystemInit(void) {
  ADC_vinit();
  SPI_vInitSlave();

  LED_vInit(AIR_COND_PORT, AIR_COND_PIN);
  LED_vInit(TV_PORT, TV_PIN);
  LED_vInit(ROOM1_PORT, ROOM1_PIN);
  LED_vInit(ROOM2_PORT, ROOM2_PIN);
  LED_vInit(ROOM3_PORT, ROOM3_PIN);
  LED_vInit(ROOM4_PORT, ROOM4_PIN);

  FAN_DDR |= (1 << FAN_EN_PIN) | (1 << FAN_IN1_PIN) | (1 << FAN_IN2_PIN);
  HEATER_DDR |= (1 << HEATER_PIN);

  TCCR0 = (1 << CS01) | (1 << CS00);
  TIMSK |= (1 << TOIE0);
  sei();
}

/**
 * @brief  Main Function
 * @return Integer
 */
int main(void) {
  vSystemInit();
  uint8 request = DEFAULT_ACK;
  uint8 response = DEFAULT_ACK;

  while (1) {
    request = SPI_ui8TransmitRecive(DEFAULT_ACK);

    switch (request) {
    case ROOM1_STATUS:
      response =
          LED_u8ReadStatus(ROOM1_PORT, ROOM1_PIN) ? ON_STATUS : OFF_STATUS;
      SPI_ui8TransmitRecive(response);
      break;
    case ROOM2_STATUS:
      response =
          LED_u8ReadStatus(ROOM2_PORT, ROOM2_PIN) ? ON_STATUS : OFF_STATUS;
      SPI_ui8TransmitRecive(response);
      break;
    case ROOM3_STATUS:
      response =
          LED_u8ReadStatus(ROOM3_PORT, ROOM3_PIN) ? ON_STATUS : OFF_STATUS;
      SPI_ui8TransmitRecive(response);
      break;
    case ROOM4_STATUS:
      response =
          LED_u8ReadStatus(ROOM4_PORT, ROOM4_PIN) ? ON_STATUS : OFF_STATUS;
      SPI_ui8TransmitRecive(response);
      break;
    case AIR_COND_STATUS:
      response = LED_u8ReadStatus(AIR_COND_PORT, AIR_COND_PIN) ? ON_STATUS
                                                               : OFF_STATUS;
      SPI_ui8TransmitRecive(response);
      break;
    case TV_STATUS:
      response = LED_u8ReadStatus(TV_PORT, TV_PIN) ? ON_STATUS : OFF_STATUS;
      SPI_ui8TransmitRecive(response);
      break;

    case ROOM1_TURN_ON:
      LED_vTurnOn(ROOM1_PORT, ROOM1_PIN);
      break;
    case ROOM2_TURN_ON:
      LED_vTurnOn(ROOM1_PORT, ROOM2_PIN);
      break;
    case ROOM3_TURN_ON:
      LED_vTurnOn(ROOM3_PORT, ROOM3_PIN);
      break;
    case ROOM4_TURN_ON:
      LED_vTurnOn(ROOM4_PORT, ROOM4_PIN);
      break;
    case TV_TURN_ON:
      LED_vTurnOn(TV_PORT, TV_PIN);
      break;

    case ROOM1_TURN_OFF:
      LED_vTurnOff(ROOM1_PORT, ROOM1_PIN);
      break;
    case ROOM2_TURN_OFF:
      LED_vTurnOff(ROOM2_PORT, ROOM2_PIN);
      break;
    case ROOM3_TURN_OFF:
      LED_vTurnOff(ROOM3_PORT, ROOM3_PIN);
      break;
    case ROOM4_TURN_OFF:
      LED_vTurnOff(ROOM4_PORT, ROOM4_PIN);
      break;
    case TV_TURN_OFF:
      LED_vTurnOff(TV_PORT, TV_PIN);
      break;

    case AIR_COND_TURN_ON:
      auto_climate_active = TRUE; /* Enable Auto Logic (Heater/AC/AutoFan) */
      break;

    case AIR_COND_TURN_OFF:
      auto_climate_active = FALSE;               /* Kill Auto Logic */
      LED_vTurnOff(AIR_COND_PORT, AIR_COND_PIN); /* AC OFF */
      HEATER_PORT &= ~(1 << HEATER_PIN);         /* Heater OFF */
      /* AutoFan stops in ISR next cycle */
      break;

    case SET_TEMPERATURE:
      required_temperature = SPI_ui8TransmitRecive(DEFAULT_ACK);
      break;

    case BLOWER_TURN_ON:
      blower_mode = TRUE;
      vFanSetNegative();
      fan_duty_cycle = 100;
      break;

    case BLOWER_TURN_OFF:
      blower_mode = FALSE;
      vFanStop();
      break;

    case GET_LDR_STATUS:
      ldr_reading = ADC_u16ReadChannel_Custom(LDR_CHANNEL);
      response = (ldr_reading > LDR_THRESHOLD) ? 1 : 0;
      SPI_ui8TransmitRecive(response);
      break;
    }
  }
}

/**
 * @brief  Timer0 Overflow ISR for PWM and Sensor Logic
 * @return Void
 */
ISR(TIMER0_OVF_vect) {
  static uint8 pwm_counter = 0;
  static uint8 temp_check_tick = 0;

  /* 1. Soft PWM Generation */
  pwm_counter++;
  if (pwm_counter >= 100)
    pwm_counter = 0;
  if (pwm_counter < fan_duty_cycle)
    FAN_PORT |= (1 << FAN_EN_PIN);
  else
    FAN_PORT &= ~(1 << FAN_EN_PIN);

  /* 2. Sensor Logic (Every ~150 ticks) */
  temp_check_tick++;
  if (temp_check_tick >= 150) {
    temp_check_tick = 0;

    /* ONLY Run Logic if System is Enabled */
    if (auto_climate_active == TRUE) {
      temp_sensor_reading = (0.25 * ADC_u16ReadChannel_Custom(TEMP_CHANNEL));

      /* --- HEATER LOGIC (< 10 C) --- */
      if (temp_sensor_reading < 10) {
        HEATER_PORT |= (1 << HEATER_PIN);
      } else {
        HEATER_PORT &= ~(1 << HEATER_PIN);
      }

      /* --- AC LOGIC (> 25 C) --- */
      if (temp_sensor_reading > 25) {
        LED_vTurnOn(AIR_COND_PORT, AIR_COND_PIN);
      } else if (temp_sensor_reading < required_temperature) {
        LED_vTurnOff(AIR_COND_PORT, AIR_COND_PIN);
      }

      /* --- AUTO FAN LOGIC (> 30 C) --- */
      /* Only runs if Manual Blower is NOT active */
      if (blower_mode == FALSE) {
        if (temp_sensor_reading > 30) {
          vFanSetPositive();
          if (temp_sensor_reading >= MAX_TEMP)
            fan_duty_cycle = 100;
          else
            fan_duty_cycle = (temp_sensor_reading - MIN_FAN_TEMP) * 10;
        } else {
          vFanStop();
        }
      }
    } else {
      /* SYSTEM DISABLED: FORCE OFF */
      /* This ensures that if you logged out, they STAY off */
      LED_vTurnOff(AIR_COND_PORT, AIR_COND_PIN);
      HEATER_PORT &= ~(1 << HEATER_PIN);

      /* Fan is Off (Unless Manual Blower Mode is active) */
      /* Blower Mode is handled by BLOWER_TURN_OFF command from Master on Logout
       */
      if (blower_mode == FALSE) {
        vFanStop();
      }
    }
  }
}