#include <Arduino.h>

#include <WiFi.h>
#include <ESPmDNS.h> // mDNS to broadcast API.
#include <ESP32PWM.h> // Handle motors and servos.
#include <SerialTransfer.h> // Handle transfer from ESP32-CAM.
#include <SoftwareSerial.h> // Handle serial from GPS.
#include <TinyGPSPlus.h> // Handle GPS data.

#include "motor.hpp"
#include "gripper.hpp"
#include "serverapi.hpp"
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

// Change as necessary.
Motor leftMotor(22, 5, 18);
Motor rightMotor(23, 19, 21);

Gripper frontGripper(13, 14);
Gripper backGripper(15, 25);

// Web server on port 80.
ServerAPI server(80);

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
    // Serial1.begin(115200, SERIAL_8N1, PIN_CAM_RX, PIN_CAM_TX);
    // camSerialTransfer.begin(Serial1);
    pinMode(PIN_GPS_VCC, OUTPUT);
    pinMode(PIN_GPS_GND, OUTPUT);
    digitalWrite(PIN_GPS_VCC, HIGH);
    digitalWrite(PIN_GPS_GND, LOW);

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

    Serial.println("Further logs can also be accessed from the webserver.");

    server.log("Log test.");

    millisPrev = millis();

    terrainMap.begin();
}

void loop() {
    millisNow = millis();

    server.handleClient();
    terrainMap.update();

    if (millisNow - millisPrev >= 2000) {
        // server.log(std::string("Latitude: ") + std::to_string(terrainMap.getCurrentLatitude()));
        // server.log(std::string("Longitude: ") + std::to_string(terrainMap.getCurrentLongitude()));
        millisPrev = millisNow;
    }
}