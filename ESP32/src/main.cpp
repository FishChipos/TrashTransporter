#include <Arduino.h>

#include <WiFi.h>
#include <ESPmDNS.h> // mDNS to broadcast API.
#include <ESP32PWM.h> // Handle motors and servos.
#include <SerialTransfer.h> // Handle transfer from ESP32-CAM.
#include <SoftwareSerial.h> // Handle serial from GPS.

#include "motor.hpp"
#include "gripper.hpp"

const int PIN_CAM_RX = 16;
const int PIN_CAM_TX = 17;

// Change as necessary.
Motor leftMotor(22, 5, 18);
Motor rightMotor(23, 19, 21);

Gripper frontGripper(13, 14);
Gripper backGripper(15, 25);

SerialTransfer camSerialTransfer;
// Using QVGA, the size should never exceed 320 x 240 bytes with JPEG compression.
uint8_t camRxBuffer[320 * 240];

unsigned long millisPrev;
unsigned long millisNow;

void setup() {
    // Helps with timer stability when controlling servos.
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    Serial.begin(115200);

    Serial1.begin(9600, SERIAL_8N1, PIN_CAM_RX, PIN_CAM_TX);
    camSerialTransfer.begin(Serial1, true, Serial, 500);

    

    millisPrev = millis();
}

void loop() {
    millisNow = millis();

    if (camSerialTransfer.available()) {
        // First 2 bytes are the packet number;
        uint16_t packet = (uint16_t)camSerialTransfer.packet.rxBuff[0] + (uint16_t)camSerialTransfer.packet.rxBuff[1] << (uint16_t)8;

        for (uint8_t byte = 2; byte < camSerialTransfer.bytesRead; byte++) {
            camRxBuffer[packet * (MAX_PACKET_SIZE - 2) + byte - 2] = camSerialTransfer.packet.rxBuff[byte];
        }
    }

    

    // terrainMap.update();
}