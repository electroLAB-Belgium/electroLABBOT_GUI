#include "electroLABBOT.h"

double last_look;
// int vect_ir[10];

Self_driving::Self_driving() {
    pinMode(IR_SENSOR_1, INPUT);
    pinMode(IR_SENSOR_2, INPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void Self_driving::self_driving_ultrason(uint8_t vitesse_pourcent,
                                         uint8_t stop_distance) {
    Serial.println("Self_driving");

    if (look_where_you_step(stop_distance) == KEEP_GOING) {
        // Serial.println("\t KEEP GOING");
        move_head(90);
        move(vitesse_pourcent, vitesse_pourcent);
    }

    else {
        // Serial.println("\t STOP");
        move(0, 0);
        delay(1000);

        switch (look_around()) {
            case LEFT_FREE:
                // Serial.println("\t \t LEFT_FREE");
                move(vitesse_pourcent, vitesse_pourcent);
                delay(1000);
                move(0, 0);
                delay(1000);
                break;

            case RIGHT_FREE:
                // Serial.println("\t \t RIGHT_FREE");
                move(vitesse_pourcent, vitesse_pourcent);
                move(vitesse_pourcent, vitesse_pourcent);
                delay(1000);
                move(0, 0);
                delay(1000);
                break;

            case NO_FREE:
                Serial.println("\t \t NO FREE");
                move(vitesse_pourcent, vitesse_pourcent);
                delay(2000);
                break;
            default:
                Serial.println("default case switch self_driving");
                break;
        }
    }
}

void Self_driving::self_driving_IR(uint8_t vitesse_pourcent) {
    // sens 1 doit être dans la bande noire
    ir_sens_1_value = analogRead(IR_SENSOR_1);
    // sens 2 doit être dans la bande claire
    ir_sens_2_value = analogRead(IR_SENSOR_2);
    /*
    Serial.print("ir_sens_1 \t \t ir_sens_2 \n");
    Serial.print(ir_sens_1_value);
    Serial.print("\t \t");
    Serial.print(ir_sens_2_value);
    Serial.println();
    */
    // update faire une moyenne sur les dernières valeurs

    if (ir_sens_1_value < IR_threshold && ir_sens_2_value < IR_threshold) {
        // Aller vers la droite
        move(vitesse_pourcent, 0);
    }

    else if (ir_sens_2_value > IR_threshold && ir_sens_1_value > IR_threshold) {
        // Aller vers la gauche
        move(0, vitesse_pourcent);
        // Serial.println("Left wheels stop");
    }

    else {
        move(vitesse_pourcent, vitesse_pourcent);
        // Serial.println("All wheels forward");
    }
    // couleur claire : petite valeur
    // couleur foncé  : grande valeur
}
