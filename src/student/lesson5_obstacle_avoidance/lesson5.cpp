/*
 * Lesson 5: Obstacle Avoidance (capstone)
 *
 * The robot "looks" left, center, and right using its ultrasonic sensor, then
 * decides where to drive so it doesn't crash into obstacles (#).
 *
 * The same code runs on a real robot. (On an mBot, whose sensor doesn't swivel,
 * the robot briefly turns its body to "look" left and right.)
 *
 * YOUR TASKS — fill in each // !EDIT block below:
 *   #1: Decide when the path ahead is blocked.
 *   #2: Drive forward when the path is clear.
 *   #3: Turn left when there is more room on the left.
 *   #4: Turn right when there is more room on the right.
 *
 * Helpful facts:
 *   - robot.scan() returns the distance in centimeters to the nearest object.
 *   - OBSTACLE_DISTANCE_CM is the distance we treat as "too close".
 *   - robot.forward(ms), robot.turnLeft(ms), robot.turnRight(ms) take a number
 *     of milliseconds. A turn of about 500 ms rotates roughly 90 degrees.
 */

#include "robot/IRobot.h"
#include "robot/RobotTypes.h"

using namespace robot;

namespace robot {

void runStudentProgram(IRobot& robot) {
    const int STEPS = 40;      // How many look-and-move cycles to run
    const int DRIVE_MS = 600;  // How long to drive forward each clear step
    const int TURN_MS = 500;   // How long each turn takes (~90 degrees)

    for (int i = 0; i < STEPS; ++i) {
        // Look around: center, then left, then right.
        robot.lookCenter();
        int center = robot.scan();

        robot.lookLeft();
        int left = robot.scan();

        robot.lookRight();
        int right = robot.scan();

        robot.lookCenter();

        // !EDIT #1: When is the path ahead "blocked"? Replace `false` with a
        // comparison using `center` and OBSTACLE_DISTANCE_CM.
        bool blocked = false;

        if (!blocked) {
            // !EDIT #2: The path is clear — drive forward for DRIVE_MS.

        } else if (left >= right) {
            // More room on the left.
            // !EDIT #3: Turn left for TURN_MS.

        } else {
            // More room on the right.
            // !EDIT #4: Turn right for TURN_MS.

        }
    }

    robot.stop();
}

}  // namespace robot
