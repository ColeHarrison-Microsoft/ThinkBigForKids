/*
 * Lesson 3 — Reference Solution (Object Following)
 */

#include "robot/IRobot.h"
#include "robot/RobotTypes.h"

using namespace robot;

namespace robot {

void runStudentProgram(IRobot& robot) {
    const int DRIVE_MS = 300;
    const int TURN_MS = 300;

    while (!robot.quitRequested()) {
        ObstacleReading object = robot.readObstacles();

        if (object.left && object.right) {
            robot.forward(DRIVE_MS);
        } else if (object.left) {
            robot.turnLeft(TURN_MS);
        } else if (object.right) {
            robot.turnRight(TURN_MS);
        } else {
            robot.stop();
        }
    }
}

}  // namespace robot
