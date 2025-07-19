#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <ESP32PWM.h>

#include "motor.hpp"
#include "gripper.hpp"

// Blynk configuration.
// Should switch to dynamic wifi provisioning so the auth token and wifi details aren't exposed.
#define BLYNK_TEMPLATE_ID "TMPL6JE1H5lwL"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_FIRMWARE_VERSION "0.1.0"

// For debugging purposes.
#define APP_DEBUG
#define BLYNK_PRINT Serial

// So Edgent knows what board to use. (might change later)
#define USE_ESP32_DEV_MODULE

#include "BlynkEdgent.h"

// Change as necessary.
Motor leftMotor(22, 5, 18);
Motor rightMotor(23, 19, 21);

Gripper frontGripper(13, 14);
Gripper backGripper(15, 25);

void setup() {
    Serial.begin(115200);

    // Helps with timer stability when controlling servos.
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    BlynkEdgent.begin();
}

void loop() {
    BlynkEdgent.run();
}