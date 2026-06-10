/*
 * Lesson 4: Line Following
 *
 * First you DRAW a track: use the arrow keys (or W / A / S / D) to move the pen
 * (+) and draw a line, then press Enter. After that, the robot must follow the
 * line you drew, using its five line sensors, all the way to the end.
 *
 * The five sensors read left (s[0]) to right (s[4]); each is true when it is
 * over the line. A straight line under the middle reads "00100"; if the line
 * drifts left you'll see a 1 on the left, and so on. The wide bar at the end
 * lights up all five sensors ("11111").
 *
 * YOUR TASKS — fill in each // !EDIT block:
 *   #1: line is on the LEFT  (s[0] or s[1])  -> turn left
 *   #2: line is on the RIGHT (s[3] or s[4])  -> turn right
 *   #3: line is in the MIDDLE (s[2])         -> drive forward
 *   #4: all five sensors on the line         -> stop (reached the end!)
 */

#include "robot/IRobot.h"
#include "robot/RobotTypes.h"

using namespace robot;

namespace robot {

void runStudentProgram(IRobot& robot) {
    const int STEP_MS = 300;
    const int TURN_MS = 300;
    const int MAX_STEPS = 300;

    for (int i = 0; i < MAX_STEPS && !robot.quitRequested(); ++i) {
        LineReading line = robot.readLineSensors();

        // !EDIT #4: if all five sensors are on the line, stop and break (we
        // reached the finish bar).

        if (line.s[0] || line.s[1]) {
            // !EDIT #1: turn left for TURN_MS

        } else if (line.s[3] || line.s[4]) {
            // !EDIT #2: turn right for TURN_MS

        } else if (line.s[2]) {
            // !EDIT #3: drive forward for STEP_MS

        } else {
            // Lost the line — stop.
            robot.stop();
        }
    }
}

}  // namespace robot
