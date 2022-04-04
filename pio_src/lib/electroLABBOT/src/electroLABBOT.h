#ifndef ELECTROLABBOT_H
#define ELECTROLABBOT_H
#include <Arduino.h>
#include <ESP32Servo.h>
#include <FastLED.h>

#include "electroLABBOT_config.h"

class Led {
   private:
    void rgb_init();
    CRGB leds[NUM_LEDS];

   public:
    Led();
    Led(int16_t pin_led);

    void led_off(int16_t pin_led);
    void led_on(int16_t pin_led);
    void led(int16_t pin_led, bool state);

    void rgb_blink(unsigned long blink_time);
    void rgb_set_color(uint8_t led, uint8_t R, uint8_t G, uint8_t B);
    /*
        led = LED_D11, LED_D12, LED_D13, LED_D14 ou LED_all (pour affecter la
       couleur aux 4 leds) R, G, B correspodnent au pourcentage de rouge, vert
       ou bleu dans la led.
    */
};

uint8_t convert_percent_to_uint8_value(uint8_t value);

class Head {
   private:
   public:
    Head();
    void move_head(int16_t);
    double look();
    uint8_t look_around();
    // regarde où tu mets les pieds !
    uint8_t look_where_you_step(uint8_t);
};

class Motors {
   private:
    const int16_t PWM_MOTOR1_CHANNEL = 2;
    const int16_t PWM_MOTOR2_CHANNEL = 3;
    const int16_t PWM_MOTORS_FREQ = 3000;
    const int16_t PWM_MOTORS_RES = 8;

    const int16_t PWM_MOTOR_MIN_VALUE_DUTYCYCLE = 60;
    int16_t previous_left_motor_speed = 0;
    int16_t previous_right_motor_speed = 0;
    const int16_t max_val_motors_speed = 200;
    const int16_t start_val_right_motor_speed = 0;
    const int16_t start_val_left_motor_speed = 0;

   public:
    Motors();
    void move_left(int16_t roue_gauche);
    void move_right(int16_t roue_droite);
    void move(int16_t roue_droite, int16_t roue_gauche);
};

void attach_detach_PWM(uint8_t moteur, uint8_t vitesse, uint8_t attach);

class Self_driving : public Motors, public Head {
   private:
    uint16_t ir_sens_2_value;
    uint16_t ir_sens_1_value;

   public:
    Self_driving();
    void self_driving_ultrason(uint8_t, uint8_t);
    void self_driving_IR(uint8_t);
};

void average_IR(int16_t[]);

class ElectroLABBOT : public Led, public Self_driving {
   private:
   public:
    ElectroLABBOT();
};

#endif