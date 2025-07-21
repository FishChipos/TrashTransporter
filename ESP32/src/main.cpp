#include <Arduino.h>

#include <WiFi.h>
#include <ESPmDNS.h>
#include <ESP32PWM.h>

#include "motor.hpp"
#include "gripper.hpp"
#include "serverapi.hpp"

// Wifi details.
#define WIFI_SSID "CHANGE ME"
#define WIFI_PASS "CHANGE ME"

// Change as necessary.
Motor leftMotor(22, 5, 18);
Motor rightMotor(23, 19, 21);

Gripper frontGripper(13, 14);
Gripper backGripper(15, 25);

// Web server on port 80.
ServerAPI server(80);

void setup() {
    // Helps with timer stability when controlling servos.
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    Serial.begin(115200);
    delay(100);

    Serial.print("Connecting to ");
    Serial.print(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("Wifi connected.");

    if (!MDNS.begin("esp32")) {
        Serial.println("Error while setting up mDNS responder!");
        while (1) {
            delay(1000);
        }
    }

    Serial.println("mDNS responder started.");

    server.begin();
    Serial.println("HTTP server started.");
}

void loop() {
    server.handleClient();
}