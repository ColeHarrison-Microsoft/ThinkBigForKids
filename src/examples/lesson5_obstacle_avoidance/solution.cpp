/*
 * Lesson 5 — Reference Solution (Obstacle Avoidance)
 *
 * Volunteer-facing reference with the // !EDIT blocks filled in. Students may
 * peek; that's acceptable per the repo conventions.
 */

#include "robot/IRobot.h"
#include "robot/RobotTypes.h"

using namespace robot;

namespace robot {

void runStudentProgram(IRobot& robot) {
    const int STEPS = 40;
    const int DRIVE_MS = 600;
    const int TURN_MS = 500;

    for (int i = 0; i < STEPS; ++i) {
        robot.lookCenter();
        int center = robot.scan();

        robot.lookLeft();
        int left = robot.scan();

        robot.lookRight();
        int right = robot.scan();

        robot.lookCenter();

        // !EDIT #1
        bool blocked = (center <= OBSTACLE_DISTANCE_CM);

        if (!blocked) {
            // !EDIT #2
            robot.forward(DRIVE_MS);
        } else if (left >= right) {
            // !EDIT #3
            robot.turnLeft(TURN_MS);
        } else {
            // !EDIT #4
            robot.turnRight(TURN_MS);
        }
    }

    robot.stop();
}

}  // namespace robot
