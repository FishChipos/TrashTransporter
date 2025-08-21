#include <Arduino.h>

#include <WiFi.h>
#include <ESPmDNS.h> // mDNS to broadcast API.
#include <ESP32PWM.h> // Handle motors and servos.
#include <SerialTransfer.h> // Handle transfer from ESP32-CAM.
#include <SoftwareSerial.h> // Handle serial from GPS.
#include <TinyGPSPlus.h> // Handle GPS data.
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "settings.hpp"
#include "motor.hpp"
#include "gripper.hpp"
#include "api.hpp"
#include "terrainmap.hpp"

// Wifi details.
#define WIFI_SSID "HASDI2"
#define WIFI_PASS "CRV198573"

const int PIN_CAM_TX = 0;
const int PIN_CAM_RX = 0;

const int PIN_GPS_VCC = 26;
const int PIN_GPS_GND = 27;
const int PIN_GPS_RX = 16;
const int PIN_GPS_TX = 17;

Settings settings;

// Change as necessary.
Motor leftMotor(22, 5, 18);
Motor rightMotor(23, 19, 21);

Gripper frontGripper(13, 14);
Gripper backGripper(15, 25);

// Web server on port 80.
APIServer server(80, &settings);

TerrainMap terrainMap(PIN_GPS_RX, PIN_GPS_TX, 9600);

// SerialTransfer camSerialTransfer;
// // Using QVGA, the size should never exceed 320 x 240 bytes with JPEG compression.
// uint8_t rxBuffer[320 * 240];
// int receiveSize = 0;

unsigned long millisPrev;
unsigned long millisNow;

void setup() {
    // Helps with timer stability when controlling servos.
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    Serial.begin(115200);

    // Camera serial transfer code so I don't forget.
    // Serial1.begin(115200, SERIAL_8N1, PIN_CAM_RX, PIN_CAM_TX);
    // camSerialTransfer.begin(Serial1);

    delay(100);

    // Connect to wifi.
    Serial.print(F("Connecting to "));
    Serial.print(F(WIFI_SSID));

    WiFi.begin(F(WIFI_SSID), F(WIFI_PASS));

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(F("."));
    }

    Serial.println(F(""));
    Serial.println(F("Wifi connected."));

    // Set up mDNS for http://esp32.local
    if (!MDNS.begin(F("esp32"))) {
        Serial.println(F("Error while setting up mDNS responder!"));
        while (1) {
            delay(1000);
        }
    }

    Serial.println(F("mDNS responder started for host: http://esp32.local"));

    // Set up the HTTP web server.
    server.enableLogging(true);
    server.begin();
    Serial.println(F("HTTP server started on port 80."));

    Serial.println(F("Further logs can also be accessed from the webserver."));

    server.log(F("Log test."));

    millisPrev = millis();

    // terrainMap.begin();
}

void loop() {
    millisNow = millis();

    // terrainMap.update();
}