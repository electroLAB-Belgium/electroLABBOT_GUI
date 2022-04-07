/* Copyright 2022 electroLABBOT. All rights reserved. */
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

void send_sensors_values(uint32_t refresh_delay = 100) {
  static DynamicJsonDocument root(200);
  static uint32_t last_send_time = millis();

  if (millis() - last_send_time > refresh_delay) {
    root["distance"] = billy.measure_distance_in_cm();
    root["button_1"] = billy.read_button_1();
    root["button_2"] = billy.read_button_2();

    String json;
    serializeJson(root, json);
    ws.textAll(json);
    last_send_time = millis();
    Serial.println(json);
  }
}

void handle_websocket_message(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = reinterpret_cast<AwsFrameInfo *>(arg);
  if (info->final && info->index == 0 && info->len == len &&
      info->opcode == WS_TEXT) {
    data[len] = 0;
    Serial.print("Message Received: ");
    Serial.println(reinterpret_cast<char *>(data));

    DynamicJsonDocument json(1024);
    DeserializationError error =
        deserializeJson(json, reinterpret_cast<char *>(data));

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
      // digitalWrite(LED_BUILTIN, json["led_builtin"]);
      billy.led(LED_BUILTIN, json["led_builtin"]);
    }

    if (json.containsKey("motor_a")) {
      billy.left_motor_speed(json["motor_a"]);
    }

    if (json.containsKey("motor_b")) {
      billy.right_motor_speed(json["motor_b"]);
    }

    if (json.containsKey("distance_sensor_angle")) {
      billy.move_ultrasonic_sensor(json["distance_sensor_angle"]);
    }

    if (json.containsKey("active_buzzer")) {
      billy.active_buzzer(json["active_buzzer"]);
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
             AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(),
                  client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    handle_websocket_message(arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void init_websocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");
  billy.begin();

  // You can remove the password parameter if you want the AP to be open.
  // So I don't have to type in my password every time I connect to the AP.
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

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  init_websocket();
  server.begin();
  Serial.println("Server started");
}

void loop() {
  ws.cleanupClients();
  send_sensors_values();
}
