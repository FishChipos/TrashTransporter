#include "gripper.hpp"

#include <Arduino.h>

Gripper::Gripper(const int PIN_ARM, const int PIN_CLAW) {
    ARM = PIN_ARM;
    CLAW = PIN_CLAW;

    pinMode(ARM, OUTPUT);
    pinMode(CLAW, OUTPUT);
}