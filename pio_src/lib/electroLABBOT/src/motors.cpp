/* Copyright 2022 electroLABBOT. All rights reserved. */
#include "electroLABBOT.h"

// Queues used to handle motor speed and acceleration
// QueueHandle_t xQueue_left_motor_speed;
// QueueHandle_t xQueue_right_motor_speed;
QueueHandle_t xQueue_left_motor_acceleration;
QueueHandle_t xQueue_right_motor_acceleration;
QueueHandle_t xQueue_right_motor_target_speed;
QueueHandle_t xQueue_left_motor_target_speed;

void motors_task(void *parameter) {
  uint16_t left_motor_acceleration = 0;
  uint16_t right_motor_acceleration = 0;
  int16_t left_motor_speed = 0;
  int16_t right_motor_speed = 0;
  int16_t left_motor_target_speed = 0;
  int16_t right_motor_target_speed = 0;

  for (;;) {
    ESP_LOGI(TAG, "Read motor speed and acceleration from queues");
    // Read motor speed and acceleration from queues
    xQueuePeek(xQueue_left_motor_acceleration, &left_motor_acceleration,
               portMAX_DELAY);
    xQueuePeek(xQueue_right_motor_acceleration, &right_motor_acceleration,
               portMAX_DELAY);
    // xQueuePeek(xQueue_left_motor_speed, &left_motor_speed, portMAX_DELAY);
    // xQueuePeek(xQueue_right_motor_speed, &right_motor_speed, portMAX_DELAY);
    xQueuePeek(xQueue_left_motor_target_speed, &left_motor_target_speed,
               portMAX_DELAY);
    xQueuePeek(xQueue_right_motor_target_speed, &right_motor_target_speed,
               portMAX_DELAY);

    ESP_LOGI(TAG, "Set motor speed and acceleration");
    // Set motor speed and acceleration
    left_motor_acceleration =
        ((double)left_motor_acceleration / 1000.0) * (double)MOTORS_REFRESH_MS;
    left_motor_acceleration =
        min((uint16_t)abs(left_motor_target_speed - left_motor_speed),
            left_motor_acceleration);
    if (left_motor_speed > left_motor_target_speed) {
      left_motor_speed -= left_motor_acceleration;
    } else if (left_motor_speed < left_motor_target_speed) {
      left_motor_speed += left_motor_acceleration;
    }
    ESP_LOGI(TAG, "left_motor_speed: %d", left_motor_speed);
    ESP_LOGI(TAG, "left_motor_target_speed: %d", left_motor_target_speed);
    if (left_motor_speed != 0) {
      digitalWrite(LEFT_MOTOR_BACKWARD, left_motor_speed < 0);
      digitalWrite(LEFT_MOTOR_FORWARD, left_motor_speed > 0);

      left_motor_speed = constrain(left_motor_speed, -4096, 4096);
      ledcWrite(PWM_MOTOR1_CHANNEL, abs(left_motor_speed));
    } else {
      digitalWrite(LEFT_MOTOR_FORWARD, LOW);
      digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
    }
    // xQueueOverwrite(xQueue_left_motor_speed, &left_motor_speed);

    ESP_LOGI(TAG, "right_motor_speed: %d", right_motor_speed);
    ESP_LOGI(TAG, "right_motor_target_speed: %d", right_motor_target_speed);
    right_motor_acceleration =
        ((double)right_motor_acceleration / 1000.0) * (double)MOTORS_REFRESH_MS;
    right_motor_acceleration =
        min((uint16_t)abs(right_motor_target_speed - right_motor_speed),
            right_motor_acceleration);
    if (right_motor_speed > right_motor_target_speed) {
      right_motor_speed -= right_motor_acceleration;
    } else if (right_motor_speed < right_motor_target_speed) {
      right_motor_speed += right_motor_acceleration;
    }
    if (right_motor_speed != 0) {
      digitalWrite(RIGHT_MOTOR_BACKWARD, right_motor_speed < 0);
      digitalWrite(RIGHT_MOTOR_FORWARD, right_motor_speed > 0);

      right_motor_speed = constrain(right_motor_speed, -4096, 4096);
      ledcWrite(PWM_MOTOR2_CHANNEL, abs(right_motor_speed));
    } else {
      digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
      digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
    }
    // xQueueOverwrite(xQueue_right_motor_speed, &right_motor_speed);

    delay(MOTORS_REFRESH_MS);
  }
}

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

void Motors::begin() {
  int16_t value = 0;
  // xQueue_left_motor_speed = xQueueCreate(1, sizeof(int16_t));

  // if (xQueue_left_motor_speed == NULL) {
  //   ESP_LOGE("Error creating the queue \"xQueue_left_motor_speed\"");
  // }

  // if (!xQueueSend(xQueue_left_motor_speed, &value, portMAX_DELAY)) {
  //   ESP_LOGE("Error setting the queue \"xQueue_left_motor_speed\"");
  // }

  // xQueue_right_motor_speed = xQueueCreate(1, sizeof(int16_t));

  // if (xQueue_right_motor_speed == NULL) {
  //   ESP_LOGE("Error creating the queue \"xQueue_right_motor_speed\"");
  // }

  // if (!xQueueSend(xQueue_right_motor_speed, &value, portMAX_DELAY)) {
  //   ESP_LOGE("Error setting the queue \"xQueue_right_motor_speed\"");
  // }

  xQueue_left_motor_acceleration = xQueueCreate(1, sizeof(uint16_t));

  if (xQueue_left_motor_acceleration == NULL) {
    ESP_LOGE("Error creating the queue \"xQueue_left_motor_acceleration\"");
  }

  if (!xQueueSend(xQueue_left_motor_acceleration, &value, portMAX_DELAY)) {
    ESP_LOGE("Error setting the queue \"xQueue_left_motor_acceleration\"");
  }

  xQueue_right_motor_acceleration = xQueueCreate(1, sizeof(uint16_t));

  if (xQueue_right_motor_acceleration == NULL) {
    ESP_LOGE("Error creating the queue \"xQueue_right_motor_acceleration\"");
  }

  if (!xQueueSend(xQueue_right_motor_acceleration, &value, portMAX_DELAY)) {
    ESP_LOGE("Error setting the queue \"xQueue_right_motor_acceleration\"");
  }

  xQueue_right_motor_target_speed = xQueueCreate(1, sizeof(int16_t));

  if (xQueue_right_motor_target_speed == NULL) {
    ESP_LOGE("Error creating the queue \"xQueue_right_motor_target_speed\"");
  }

  if (!xQueueSend(xQueue_right_motor_target_speed, &value, portMAX_DELAY)) {
    ESP_LOGE("Error setting the queue \"xQueue_right_motor_target_speed\"");
  }

  xQueue_left_motor_target_speed = xQueueCreate(1, sizeof(int16_t));

  if (xQueue_left_motor_target_speed == NULL) {
    ESP_LOGE("Error creating the queue \"xQueue_left_motor_target_speed\"");
  }

  if (!xQueueSend(xQueue_left_motor_target_speed, &value, portMAX_DELAY)) {
    ESP_LOGE("Error setting the queue \"xQueue_left_motor_target_speed\"");
  }

  xTaskCreatePinnedToCore(motors_task, "motors_task", 10000, NULL, 1, NULL, 1);
}

void Motors::left_motor_speed(int16_t left_motor_speed, uint16_t acceleration) {
  uint16_t _acceleration = acceleration;
  int16_t _left_motor_speed = left_motor_speed;
  xQueueOverwrite(xQueue_left_motor_acceleration, &_acceleration);
  xQueueOverwrite(xQueue_left_motor_target_speed, &_left_motor_speed);
}

void Motors::right_motor_speed(int16_t right_motor_speed,
                               uint16_t acceleration) {
  uint16_t _acceleration = acceleration;
  int16_t _right_motor_speed = right_motor_speed;
  xQueueOverwrite(xQueue_right_motor_acceleration, &_acceleration);
  xQueueOverwrite(xQueue_right_motor_target_speed, &_right_motor_speed);
}

void Motors::motors_speed(int16_t left_motor_speed, int16_t right_motor_speed) {
  this->left_motor_speed(left_motor_speed);
  this->right_motor_speed(right_motor_speed);
}
