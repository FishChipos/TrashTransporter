#include <Arduino.h>

#include <WiFi.h>
#include <ESPmDNS.h> // mDNS to broadcast API.
#include <SerialTransfer.h>
#include <SoftwareSerial.h> // Handle serial from GPS.
#include <esp32cam.h>

#include "api.hpp"

// Wifi details.
#define WIFI_SSID "thinNet"
#define WIFI_PASS "156200AF"

const int PIN_RX = 13;
const int PIN_TX = 14;

Settings settings;

// Web server on port 80.
APIServer server(80, &settings);

const esp32cam::Resolution resolution = esp32cam::Resolution::find(320, 240);

void setup() {
    // Serial for logging to PC.
    Serial.begin(115200);


    esp32cam::setLogger(Serial);
    esp32cam::Config cameraConfig;
    cameraConfig.setBufferCount(3);
    cameraConfig.setJpeg(80);
    cameraConfig.setPins(esp32cam::pins::AiThinker);
    cameraConfig.setResolution(resolution);

    esp32cam::Camera.begin(cameraConfig);

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
        delay(2000);
        ESP.restart();
    }

    Serial.println(F("mDNS responder started for host: http://esp32.local"));

    // Set up the HTTP web server.
    server.enableLogging(true);
    server.begin();
    Serial.println(F("HTTP server started on port 80."));

    Serial.println(F("Further logs can also be accessed from the webserver."));

    server.log(F("Log test."));
}

void loop() {

    delay(100);
}