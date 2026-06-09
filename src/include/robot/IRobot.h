#pragma once

#include "RobotTypes.h"

namespace robot {

/*
 * IRobot — the one interface every student program talks to.
 *
 * A student writes their code against an IRobot reference and it runs
 * unchanged against any backend:
 *   - SimRobot       (text simulator, no hardware)
 *   - MBotRobot      (Makeblock mBot)
 *   - ArduinoCarRobot (DIY Arduino car)
 *
 * Timing semantics (important): the movement methods BLOCK for the requested
 * number of milliseconds and then stop the motors, just like the original
 * Arduino lessons where forward(2000) drives for ~2 seconds. Every backend —
 * including the simulator — honors this so behavior translates to hardware.
 *
 * No globals: student code receives an IRobot& and never reaches for hardware
 * pins or simulator internals directly.
 */
class IRobot {
public:
    virtual ~IRobot() = default;

    // --- Movement (each blocks for ms milliseconds, then stops) ---

    // Drive straight forward for ms milliseconds.
    virtual void forward(int ms) = 0;

    // Drive straight backward for ms milliseconds.
    virtual void reverse(int ms) = 0;

    // Rotate left (counter-clockwise) for ms milliseconds.
    virtual void turnLeft(int ms) = 0;

    // Rotate right (clockwise) for ms milliseconds.
    virtual void turnRight(int ms) = 0;

    // --- Motor / speed control ---

    // Set the left and right motor speeds independently (0..255). Setting
    // different values lets the robot curve. Does not block — it just changes
    // how fast subsequent movement runs.
    virtual void setMotorSpeed(int leftSpeed, int rightSpeed) = 0;

    // Stop both motors immediately.
    virtual void stop() = 0;

    // --- Sensing ---

    // Measure the distance (in centimeters) to the nearest object in the
    // direction the head is currently pointed. Returns MAX_SCAN_DISTANCE_CM
    // when nothing is in range.
    virtual int scan() = 0;

    // Point the ultrasonic "head" (servo) left / straight ahead / right.
    // On robots without a servo head these are no-ops.
    virtual void lookLeft() = 0;
    virtual void lookCenter() = 0;
    virtual void lookRight() = 0;
};

/*
 * Entry point every student lesson implements. The backend's main() builds a
 * concrete robot and hands it to this function.
 */
void runStudentProgram(IRobot& robot);

}  // namespace robot
