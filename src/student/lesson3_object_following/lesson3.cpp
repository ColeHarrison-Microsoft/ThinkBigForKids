/*
 * Lesson 3: Object Following
 *
 * The robot has an object sensor on each side. It should chase a target (shown
 * as O). YOU move the target around with the arrow keys (or W / A / S / D), and
 * the robot's job — your code — is to steer toward it. Press Q to quit.
 *
 * YOUR TASKS — fill in each // !EDIT block:
 *   #1: object seen on BOTH sides (straight ahead) -> drive forward
 *   #2: object seen only on the LEFT  -> turn left
 *   #3: object seen only on the RIGHT -> turn right
 *   (if neither sensor sees it, the robot waits)
 *
 * Helpful facts:
 *   - robot.readObstacles() returns an ObstacleReading with .left and .right
 *     (each true when that sensor detects the object).
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
            // !EDIT #1: drive forward for DRIVE_MS

        } else if (object.left) {
            // !EDIT #2: turn left for TURN_MS

        } else if (object.right) {
            // !EDIT #3: turn right for TURN_MS

        } else {
            robot.stop();
        }
    }
}

}  // namespace robot
