#ifndef MOTOR_HPP
#define MOTOR_HPP

// Class representing a motor driven with a LM298N chip.
class Motor {
    private:
        int PWM;
        int IN1;
        int IN2;

    public:
        // Initializes a Motor instance.
        // Sets PIN_PWM, PIN_IN1, and PIN_IN2 to OUTPUT mode.
        Motor(const int PIN_PWM, const int PIN_IN1, const int PIN_IN2);

        void off();
        void forward();
        void reverse();

        // Might overheat the motor if frequently used.
        void brake();
};

#endif