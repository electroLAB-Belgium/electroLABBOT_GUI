#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <WiFiAP.h>

#include "electroLABBOT.h"

AsyncWebServer server(42);
AsyncWebSocket ws("/ws");
ElectroLABBOT billy;

// void notifyClients() { ws.textAll(String(ledState)); }

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len &&
        info->opcode == WS_TEXT) {
        data[len] = 0;
        Serial.print("Message Received: ");
        Serial.println((char *)data);

        DynamicJsonDocument json(1024);
        DeserializationError error = deserializeJson(json, (char *)data);

        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
            return;
        }

        for (uint8_t index(0); index < NUM_LEDS; index++) {
            if (json.containsKey("rgb_" + String(index))) {
                Serial.println("Setting LED " + String(index));
                uint8_t R = json["rgb_" + String(index)][0];
                uint8_t G = json["rgb_" + String(index)][1];
                uint8_t B = json["rgb_" + String(index)][2];
                billy.rgb_set_color(index, R, G, B);
            }
        }

        if (json.containsKey("led_builtin")) {
            digitalWrite(LED_BUILTIN, json["led_builtin"]);
            // billy.led(LED_BUILTIN, json["led_builtin"]);
        }

        if (json.containsKey("motor_a")) {
            billy.move_left(json["motor_a"]);
        }

        if (json.containsKey("motor_b")) {
            billy.move_right(json["motor_b"]);
        }

        // billy.led(LED_BUILTIN, json["led_builtin"].as<bool>() );
        // JSONVar object = JSON.parse((char *)data);

        // if (JSON.typeof(object) == "undefined") {
        //     Serial.println("Parsing input failed!");
        //     return;|
        // }

        // Serial.print("JSON.typeof(object) = ");
        // Serial.println(JSON.typeof(object));

        // // Serial.print("object.keys() = ");
        // // Serial.println(object.keys());
        // // Serial.print("object.keys().length() = ");
        // // Serial.println(object.keys().length());
        // int number_of_objects = object.keys().length();
        // for (int index(0); index < number_of_objects; index++) {
        //     Serial.print("object.keys().length() = ");
        //     Serial.println(object.keys().length());
        //     JSONVar key = object.keys()[index];
        //     // Serial.print(key);
        //     // Serial.print(": ");
        //     // Serial.println(object[key]);

        //     if (key == JSONVar("led_builtin")) {
        //         Serial.print("int(object[key]): ");
        //         Serial.println(int(object[key]));
        //         int(object[key]) ? billy.led_on(LED_BUILTIN)
        //                           : billy.led_off(LED_BUILTIN);
        //     }

        //     else if (key == JSONVar("rgb_0")) {
        //         billy.rgb_set_color(0, int(object[key][0]),
        //         int(object[key][1]),
        //                             int(object[key][2]));
        //     }

        //     else if (key == JSONVar("rgb_1")) {
        //         billy.rgb_set_color(1, int(object[key][0]),
        //         int(object[key][1]),
        //                             int(object[key][2]));
        //     }

        //     else if (key == JSONVar("rgb_2")) {
        //         billy.rgb_set_color(2, int(object[key][0]),
        //         int(object[key][1]),
        //                             int(object[key][2]));
        //     }

        //     else if (key == JSONVar("rgb_3")) {
        //         billy.rgb_set_color(3, int(object[key][0]),
        //         int(object[key][1]),
        //                             int(object[key][2]));
        //     }

        //     else if (key == JSONVar("motor_a")) {
        //         billy.move_left(-int(object[key]));
        //     }

        //     else if (key == JSONVar("motor_b")) {
        //         billy.move_right(-int(object[key]));
        //     }
        // }

        // // if (strcmp((char *)data,
        // // "toggle") == 0) {
        // //     ledState = !ledState;
        // //     notifyClients();
        // // }
    }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
             AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n",
                          client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void initWebSocket() {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

void setup() {
    // pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
    Serial.println();
    Serial.println("Configuring access point...");
    // billy.rgb_set_color(LED_all, 0, 30, 0);

    // You can remove the password parameter if you want the AP to be open.
    String ssid = "electroLABBOT";
    String wifi_mac_address = WiFi.macAddress();
    wifi_mac_address.replace(":", "");
    wifi_mac_address = "_" + wifi_mac_address.substring(6, 12);
    ssid += wifi_mac_address;
    Serial.println(ssid);
    WiFi.softAP(ssid.c_str());

    IPAddress access_point_ip(192, 168, 100, 1);
    IPAddress netmask_ip(255, 255, 255, 0);
    IPAddress gateway_ip(192, 168, 100, 1);
    WiFi.softAPConfig(access_point_ip, gateway_ip, netmask_ip);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    initWebSocket();
    server.begin();
    Serial.println("Server started");
}

void loop() { ws.cleanupClients(); }
