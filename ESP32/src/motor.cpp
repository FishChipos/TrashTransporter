#include "motor.hpp"

#include <Arduino.h>

Motor::Motor(const int pwmPin, const int in1Pin, const int in2Pin) {
    PIN_PWM = pwmPin;
    PIN_IN1 = in1Pin;
    PIN_IN2 = in2Pin;

    pinMode(PIN_PWM, OUTPUT);
    pinMode(PIN_IN1, OUTPUT);
    pinMode(PIN_IN2, OUTPUT);
}

void Motor::off() {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
}

void Motor::forward() {
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
}

void Motor::reverse() {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
}

void Motor::brake() {
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, HIGH);
}