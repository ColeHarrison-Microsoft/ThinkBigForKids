/*
 * Lesson 1: Basic Movement
 *
 * The robot drives forward, backward, turns left, turns right, then stops.
 * Each movement lasts 2 seconds.
 *
 * Everything is provided here — just build, run, and watch it go! This is a
 * warm-up so you get comfortable building and running a program.
 *
 * Look at how each line calls a method on `robot`:
 *   robot.forward(2000);  means "drive forward for 2000 milliseconds (2s)".
 */

#include "robot/IRobot.h"

using namespace robot;

namespace robot {

void runStudentProgram(IRobot& robot) {
    robot.forward(2000);    // Drive forward for 2 seconds
    robot.reverse(2000);    // Drive backward for 2 seconds
    robot.turnLeft(2000);   // Turn left
    robot.turnRight(2000);  // Turn right
    robot.stop();           // Stop the motors
}

}  // namespace robot
