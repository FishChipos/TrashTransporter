#ifndef MOTOR_HPP
#define MOTOR_HPP

// Class representing a motor driven with a LM298N chip.
class Motor {
    private:
        int PIN_PWM;
        int PIN_IN1;
        int PIN_IN2;

    public:
        // Initializes a Motor instance.
        // Sets pwmPin, in1Pin, and in2Pin to OUTPUT mode.
        Motor(const int pwmPin, const int in1Pin, const int in2Pin);

        void off();
        void forward();
        void reverse();

        // Might overheat the motor if frequently used.
        void brake();
};

#endif