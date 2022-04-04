#ifndef ELECTROLABBOT_CONFIG_H
#define ELECTROLABBOT_CONFIG_H

/* Distance sensor parameters */
#define pin_servo 13
#define pin_ultrason 27

const uint8_t LEFT_FREE = 1;
const uint8_t RIGHT_FREE = 2;
const uint8_t NO_FREE = 3;
const uint8_t KEEP_GOING = 0;
const uint8_t STOP = 4;
const uint8_t PROBLEM = 5;

/* LED related constants */
#define COLOR_ORDER GRB
#define LED_BUILTIN 2
const uint8_t NUM_LEDS = 4;
const uint8_t DATA_PIN = 32;
const uint8_t LED_D11 = 0;
const uint8_t LED_D12 = 1;
const uint8_t LED_D13 = 2;
const uint8_t LED_D14 = 3;
const uint8_t LED_all = 4;

/* Self driving related constants */
#define IR_SENSOR_1 26
#define IR_SENSOR_2 14
const uint16_t IR_threshold = 1000;

/* Motors related constants */
// LEFT = moteur 1
#define LEFT_MOTOR_EN 5
#define LEFT_MOTOR_FORWARD 18
#define LEFT_MOTOR_BACKWARD 19

// RIGHT = moteur 2
#define RIGHT_MOTOR_EN 4
#define RIGHT_MOTOR_FORWARD 17
#define RIGHT_MOTOR_BACKWARD 16

#define ALL_MOTORS_FORWARD 1
#define ALL_MOTORS_BACKWARD 2
#define ALL_MOTORS_STOP 3
#define ALL_MOTORS_DETACH 6


/* Buzzer related constants */
const uint8_t BUZZER_PIN = 15;

/* Button related constants */
const uint8_t BUTTON_1 = 33;
const uint8_t BUTTON_2 = 25;
#endif
