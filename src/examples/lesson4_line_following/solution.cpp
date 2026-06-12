/*
 * Lesson 4 — Reference Solution (Line Following)
 *
 * Notes:
 *  - On a REAL robot we steer by driving forward with the two wheels at
 *    DIFFERENT speeds, so the robot curves toward the line while still moving
 *    ahead (pivoting in place just jitters across narrow tape and never
 *    progresses). The wider the speed gap, the sharper the curve.
 *  - In the SIMULATOR the world is a grid and the robot can only face N/E/S/W,
 *    so there we follow the drawn track with 90-degree pivots instead.
 *  - Either way, if the robot drifts off the line entirely it does NOT give up:
 *    it keeps seeking toward the side where it last saw the line.
 */

#include "robot/IRobot.h"
#include "robot/RobotTypes.h"

using namespace robot;

namespace robot {

void runStudentProgram(IRobot& robot) {
#if defined(MBOT) || defined(ARDUINO_CAR)
    // --- Real robot: smooth differential steering ---
    // Steer by driving forward with the wheels at different speeds. The bigger
    // the gap, the sharper the curve. The inner wheel nearly stops for the
    // "lost the line" recovery so the robot swings back hard to re-find it.
    // Steer by driving forward with the wheels at different speeds. The SPEED
    // GAP (FAST - SLOW) sets how hard it turns; the higher both speeds are, the
    // more it drives forward through the turn (less side-to-side "waddle").
    const int STEP_MS = 30;     // short bursts -> samples often, tight feedback loop
    const int FAST = 230;       // outer wheel
    const int SLOW = 100;       // inner wheel on a normal correction (gap 130)
    const int SEEK = 30;        // inner wheel when fully off the line (sharpest)
    const int STRAIGHT = 205;   // both wheels when centered on the line
    const int MAX_STEPS = 3000;

    int lastSeen = 1;  // which way the line last was: -1 = left, +1 = right

    for (int i = 0; i < MAX_STEPS && !robot.quitRequested(); ++i) {
        LineReading line = robot.readLineSensors();

        if (line.s[0] || line.s[1]) {
            lastSeen = -1;
            robot.setMotorSpeed(SLOW, FAST);   // curve left, toward the line
            robot.forward(STEP_MS);
        } else if (line.s[3] || line.s[4]) {
            lastSeen = +1;
            robot.setMotorSpeed(FAST, SLOW);   // curve right, toward the line
            robot.forward(STEP_MS);
        } else if (line.s[2]) {
            robot.setMotorSpeed(STRAIGHT, STRAIGHT);  // centered -> drive straight
            robot.forward(STEP_MS);
        } else {
            // Off the line: swing back hard toward the side we last saw it,
            // still creeping forward, so the robot seeks instead of stopping.
            if (lastSeen < 0) {
                robot.setMotorSpeed(SEEK, FAST);
            } else {
                robot.setMotorSpeed(FAST, SEEK);
            }
            robot.forward(STEP_MS);
        }
    }
    robot.stop();
#else
    // --- Simulator: grid world, follow with 90-degree pivots ---
    const int STEP_MS = 300;
    const int TURN_MS = 300;
    const int MAX_STEPS = 300;

    int lastSeen = 1;

    for (int i = 0; i < MAX_STEPS && !robot.quitRequested(); ++i) {
        LineReading line = robot.readLineSensors();

        // Finish bar: all five sensors on the line.
        if (line.s[0] && line.s[1] && line.s[2] && line.s[3] && line.s[4]) {
            robot.stop();
            break;
        }

        if (line.s[0] || line.s[1]) {
            lastSeen = -1;
            robot.turnLeft(TURN_MS);
        } else if (line.s[3] || line.s[4]) {
            lastSeen = +1;
            robot.turnRight(TURN_MS);
        } else if (line.s[2]) {
            robot.forward(STEP_MS);
        } else {
            // Lost the line — search back toward where we last saw it.
            if (lastSeen < 0) {
                robot.turnLeft(TURN_MS);
            } else {
                robot.turnRight(TURN_MS);
            }
        }
    }
#endif
}

}  // namespace robot
