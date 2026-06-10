/*
 * Lesson 2 — Reference Solution (Virtual Remote)
 */

#include "robot/IRobot.h"
#include "robot/RobotTypes.h"

using namespace robot;

namespace robot {

void runStudentProgram(IRobot& robot) {
    const int DRIVE_MS = 300;
    const int TURN_MS = 300;

    while (!robot.quitRequested()) {
        RemoteButton button = robot.readRemote();

        if (button == RemoteButton::Up) {
            robot.forward(DRIVE_MS);
        } else if (button == RemoteButton::Down) {
            robot.reverse(DRIVE_MS);
        } else if (button == RemoteButton::Left) {
            robot.turnLeft(TURN_MS);
        } else if (button == RemoteButton::Right) {
            robot.turnRight(TURN_MS);
        } else {
            robot.stop();
        }
    }
}

}  // namespace robot
