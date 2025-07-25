#include <Arduino.h>

#include <WiFi.h>
#include <ESPmDNS.h>
#include <ESP32PWM.h>
#include <SerialTransfer.h>

#include "motor.hpp"
#include "gripper.hpp"
#include "serverapi.hpp"

// Wifi details.
#define WIFI_SSID "CHANGE ME"
#define WIFI_PASS "CHANGE ME"

const int PIN_TX = 0;
const int PIN_RX = 0;

// Change as necessary.
Motor leftMotor(22, 5, 18);
Motor rightMotor(23, 19, 21);

Gripper frontGripper(13, 14);
Gripper backGripper(15, 25);

// Web server on port 80.
ServerAPI server(80);

SerialTransfer serial1Transfer;
// Using QVGA, the size should never exceed 320 x 240 bytes with JPEG compression.
uint8_t rxBuffer[320 * 240];
int receiveSize = 0;

void setup() {
    // Helps with timer stability when controlling servos.
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, PIN_RX, PIN_TX);
    serial1Transfer.begin(Serial1);

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

    // There is no way this works.
    if (serial1Transfer.available()) {
        if (serial1Transfer.currentPacketID() == 1) {
            receiveSize = serial1Transfer.rxObj(rxBuffer, receiveSize, 320 * 240);
        }
    }
}