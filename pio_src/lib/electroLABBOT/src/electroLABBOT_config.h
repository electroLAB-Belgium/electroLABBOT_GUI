/* Copyright 2022 electroLABBOT. All rights reserved. */
#ifndef PIO_SRC_LIB_ELECTROLABBOT_SRC_ELECTROLABBOT_CONFIG_H_
#define PIO_SRC_LIB_ELECTROLABBOT_SRC_ELECTROLABBOT_CONFIG_H_

/* Distance sensor parameters */
const uint8_t SERVOMOTOR_PIN = 13;
const uint8_t ULTRASONIC_SENSOR_PIN = 27;

const uint8_t LEFT_FREE = 1;
const uint8_t RIGHT_FREE = 2;
const uint8_t NO_FREE = 3;
const uint8_t KEEP_GOING = 0;
const uint8_t STOP = 4;
const uint8_t PROBLEM = 5;

/* LED related constants */
const EOrder COLOR_ORDER = GRB;
const uint8_t LED_BUILTIN = 2;
const uint8_t NUM_LEDS = 4;
const uint8_t DATA_PIN = 32;
const uint8_t LED_D11 = 0;
const uint8_t LED_D12 = 1;
const uint8_t LED_D13 = 2;
const uint8_t LED_D14 = 3;
const uint8_t LED_ALL = 4;

/* Self driving related constants */
const uint8_t IR_SENSOR_1 = 26;
const uint8_t IR_SENSOR_2 = 14;
const uint8_t LEFT_IR_SENSOR_PIN = IR_SENSOR_2;
const uint8_t RIGHT_IR_SENSOR_PIN = IR_SENSOR_1;
const uint16_t IR_THRESHOLD = 1000;

/* Motors related constants */
// LEFT = moteur 1
const uint8_t LEFT_MOTOR_EN = 5;
const uint8_t LEFT_MOTOR_FORWARD = 18;
const uint8_t LEFT_MOTOR_BACKWARD = 19;

// RIGHT = moteur 2
const uint8_t RIGHT_MOTOR_EN = 4;
const uint8_t RIGHT_MOTOR_FORWARD = 17;
const uint8_t RIGHT_MOTOR_BACKWARD = 16;

const uint8_t ALL_MOTORS_FORWARD = 1;
const uint8_t ALL_MOTORS_BACKWARD = 2;
const uint8_t ALL_MOTORS_STOP = 3;
const uint8_t ALL_MOTORS_DETACH = 6;

const int16_t PWM_MOTOR1_CHANNEL = 2;
const int16_t PWM_MOTOR2_CHANNEL = 3;
const int16_t PWM_MOTORS_FREQ = 3000;
const int16_t PWM_MOTORS_RES = 12;

const uint32_t MOTORS_REFRESH_MS = 20;
const uint16_t MOTORS_ACCELERATION_PER_SECOND = 4096 * 20;

/* Buzzer related constants */
const uint8_t BUZZER_PIN = 15;

/* Button related constants */
const uint8_t BUTTON_1_PIN = 33;
const uint8_t BUTTON_2_PIN = 25;
#endif // PIO_SRC_LIB_ELECTROLABBOT_SRC_ELECTROLABBOT_CONFIG_H_
