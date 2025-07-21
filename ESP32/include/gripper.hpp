#ifndef GRIPPER_HPP
#define GRIPPER_HPP

// Class representing a basic gripper with one joint and a claw.
class Gripper {
    private:
        int ARM;
        int CLAW;

    public:
        // Initializes a Gripper instance.
        // Sets PIN_ARM and PIN_CLAW to OUTPUT mode.
        Gripper(const int PIN_ARM, const int PIN_CLAW);
};

#endif