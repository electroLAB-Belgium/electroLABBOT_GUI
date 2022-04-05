/* Copyright 2022 electroLABBOT. All rights reserved. */
#include "electroLABBOT.h"

Motors::Motors() {
  // Setup PWM and digital pins for motors
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);

  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);

  ledcSetup(PWM_MOTOR1_CHANNEL, PWM_MOTORS_FREQ, PWM_MOTORS_RES);
  ledcSetup(PWM_MOTOR2_CHANNEL, PWM_MOTORS_FREQ, PWM_MOTORS_RES);

  ledcAttachPin(LEFT_MOTOR_EN, PWM_MOTOR1_CHANNEL);
  ledcAttachPin(RIGHT_MOTOR_EN, PWM_MOTOR2_CHANNEL);
}

void Motors::move_left(int16_t left_motor_speed) {
  if (previous_left_motor_speed != left_motor_speed) {
    if (left_motor_speed != 0) {
      digitalWrite(LEFT_MOTOR_BACKWARD, left_motor_speed < 0);
      digitalWrite(LEFT_MOTOR_FORWARD, left_motor_speed > 0);

      left_motor_speed = constrain(left_motor_speed, -4096, 4096);
      ledcWrite(PWM_MOTOR1_CHANNEL, abs(left_motor_speed));
    } else {
      digitalWrite(LEFT_MOTOR_FORWARD, LOW);
      digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
    }

    previous_left_motor_speed = left_motor_speed;
  }
}

void Motors::move_right(int16_t right_motor_speed) {
  if (previous_right_motor_speed != right_motor_speed) {
    if (right_motor_speed != 0) {
      digitalWrite(RIGHT_MOTOR_BACKWARD, right_motor_speed < 0);
      digitalWrite(RIGHT_MOTOR_FORWARD, right_motor_speed > 0);

      right_motor_speed = constrain(right_motor_speed, -4096, 4096);
      ledcWrite(PWM_MOTOR2_CHANNEL, abs(right_motor_speed));
    } else {
      digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
      digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
    }
    previous_right_motor_speed = right_motor_speed;
  }
}

void Motors::move(int16_t left_motor_speed, int16_t right_motor_speed) {
  this->move_left(left_motor_speed);
  this->move_right(right_motor_speed);
}
