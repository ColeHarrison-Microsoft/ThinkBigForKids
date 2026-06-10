/*
 * Lesson 4 — Reference Solution (Line Following)
 *
 * Note: the finish check (#4) comes first, because at the finish bar all five
 * sensors are on the line — we want to stop, not turn.
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

        // !EDIT #4
        if (line.s[0] && line.s[1] && line.s[2] && line.s[3] && line.s[4]) {
            robot.stop();
            break;
        }

        if (line.s[0] || line.s[1]) {
            // !EDIT #1
            robot.turnLeft(TURN_MS);
        } else if (line.s[3] || line.s[4]) {
            // !EDIT #2
            robot.turnRight(TURN_MS);
        } else if (line.s[2]) {
            // !EDIT #3
            robot.forward(STEP_MS);
        } else {
            robot.stop();
        }
    }
}

}  // namespace robot
