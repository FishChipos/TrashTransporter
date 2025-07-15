#include "gripper.hpp"

#include <Arduino.h>
#include <ESP32Servo.h>

Gripper::Gripper(const int PIN_ARM, const int PIN_CLAW) {
    ARM = PIN_ARM;
    CLAW = PIN_CLAW;

    pinMode(ARM, OUTPUT);
    pinMode(CLAW, OUTPUT);
}