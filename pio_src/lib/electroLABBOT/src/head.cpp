#include "electroLABBOT.h"

Servo servo;
bool attach = false;

Head::Head() {
    // Serial.println("attach servo");
    // digitalWrite(LED_BUILTIN, HIGH);
    servo.attach(pin_servo);
    // le attach ne se fait pas pourtant le constructeur est réalisé ...
}

void Head::move_head(int16_t degrees) {
    if (attach == false) {
        servo.attach(pin_servo);
        attach = true;
    }
    servo.write(degrees);
}

uint8_t Head::look_around() {
    if (attach == false) {
        servo.attach(pin_servo);
        attach = true;
    }
    double distance_left_side = 0;
    double distance_right_side = 0;

    // Serial.println("\r\n move_head(180)");
    move_head(180);
    while (servo.read() < 179)
        ;
    delay(1000);
    distance_left_side = look();
    /*
    Serial.print("distance_left_side \t ");
    Serial.println(distance_left_side);
    Serial.println("\r\n move_head(10)");
    */
    move_head(5);
    while (servo.read() > 11)
        ;
    delay(1000);
    distance_right_side = look();
    /*
    Serial.print("distance_right_side \t ");
    Serial.println(distance_right_side);
    */

    if (distance_left_side < 8 && distance_right_side < 8)
        return NO_FREE;

    else if (distance_left_side > distance_right_side)
        return LEFT_FREE;

    else
        return RIGHT_FREE;
}

double Head::look() {
    uint32_t start_time = 0, stop_time = 0;
    double distance = 0;
    bool ancien_etat = 0;

    pinMode(pin_ultrason, OUTPUT);
    digitalWrite(pin_ultrason, HIGH);
    delayMicroseconds(2);

    vTaskSuspendAll();

    digitalWrite(pin_ultrason, HIGH);
    delayMicroseconds(5);
    digitalWrite(pin_ultrason, LOW);

    pinMode(pin_ultrason, INPUT);

    start_time = micros();
    ancien_etat = digitalRead(pin_ultrason);
    while (ancien_etat == digitalRead(pin_ultrason) &&
           ((micros() - start_time) < 40000))
        ;

    start_time = micros();

    ancien_etat = digitalRead(pin_ultrason);
    while (ancien_etat == digitalRead(pin_ultrason) &&
           ((micros() - start_time) < 40000))
        ;

    stop_time = micros();

    xTaskResumeAll();

    distance = fabs(((stop_time - 40 - start_time) / 58.31));

    return distance < 500 ? distance : 501;
}

uint8_t Head::look_where_you_step(uint8_t stop_distance) {
    // if (look() > stop_distance)
    //     return KEEP_GOING;
    // else
    //     return STOP;

    // return PROBLEM;
    return look() > stop_distance ? KEEP_GOING : STOP;
}
