#include "electroLABBOT.h"

bool led_state = false;
unsigned long previous_millis = 0;
unsigned long current_millis;

Led::Led() {
    FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    pinMode(LED_BUILTIN, OUTPUT);
}

Led::Led(int16_t pin_led) { pinMode(pin_led, OUTPUT); }

void Led::led_off(int16_t pin_led) { digitalWrite(pin_led, LOW); }

void Led::led_on(int16_t pin_led) { digitalWrite(pin_led, HIGH); }

void Led::led(int16_t pin_led, bool state) { digitalWrite(pin_led, state); }

void Led::rgb_blink(unsigned long blink_time) {
    current_millis = millis();

    if ((current_millis - previous_millis) >= blink_time) {
        // Serial.print("Toggle led");
        previous_millis = millis();

        for (uint8_t i = 0; i < 4; i++) {
            leds[i] = led_state ? 0x000000 : 0x00FF0000;
        }
        FastLED.show();
        led_state = !led_state;
    }
}

void Led::rgb_set_color(uint8_t led, uint8_t R, uint8_t G, uint8_t B) {
    uint32_t rgb = 0;
    uint32_t temp = 0;
    // Conversion % (int) -> 255 (uint8_t)
    // R = convert_percent_to_uint8_value(R);
    // Serial.print("uint8_t R :");
    // Serial.println(R, BIN);
    rgb = R << 16;
    /*
    Serial.print("uint32 rgb :");
    Serial.println(rgb, BIN);
    Serial.println();
    */
    // G = convert_percent_to_uint8_value(G);
    /*
    Serial.print("uint8_t G : ");
    Serial.println(G,BIN);
    */
    temp = G << 8;
    /*
    Serial.print("uint8_t temp : ");
    Serial.println(temp, BIN);
    */
    rgb = rgb | temp;
    /*
    Serial.print("uint8_t rgb : ");
    Serial.println(rgb, BIN);
    Serial.println();
    */
    // B = convert_percent_to_uint8_value(B);
    /*
    Serial.print("uint8_t B :");
    Serial.println(B, BIN);
    */
    temp = B;
    rgb = rgb | temp;
    /*
    Serial.print("uint 32 rgb : ");
    Serial.println(rgb,BIN);
    */
    if (led == LED_all) {
        for (int i = 0; i < 4; i++) {
            leds[i] = rgb;
        }

    } else
        leds[led] = rgb;

    FastLED.show();
}

uint8_t convert_percent_to_uint8_value(uint8_t value) {
    return value = 255 * value / 100;
}