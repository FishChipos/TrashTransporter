#include "motor.hpp"

#include <Arduino.h>

Motor::Motor(const int PIN_PWM, const int PIN_IN1, const int PIN_IN2) {
    PWM = PIN_PWM;
    IN1 = PIN_IN1;
    IN2 = PIN_IN2;

    pinMode(PWM, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
}

void Motor::off() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
}

void Motor::forward() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
}

void Motor::reverse() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
}

void Motor::brake() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
}