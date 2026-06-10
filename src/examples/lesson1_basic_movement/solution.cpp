/*
 * Lesson 1 — Reference Solution (Basic Movement)
 *
 * Volunteer-facing reference. Lesson 1 is fully provided to students, so this
 * matches the student file exactly.
 */

#include "robot/IRobot.h"

using namespace robot;

namespace robot {

void runStudentProgram(IRobot& robot) {
    robot.forward(2000);
    robot.reverse(2000);
    robot.turnLeft(2000);
    robot.turnRight(2000);
    robot.stop();
}

}  // namespace robot
