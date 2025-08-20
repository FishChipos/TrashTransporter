#include "gripper.hpp"

#include <Arduino.h>
#include <ESP32Servo.h>

Gripper::Gripper(const int armPin, const int clawPin) {
    PIN_ARM = armPin;
    PIN_CLAW = clawPin;

    pinMode(PIN_ARM, OUTPUT);
    pinMode(PIN_CLAW, OUTPUT);
}