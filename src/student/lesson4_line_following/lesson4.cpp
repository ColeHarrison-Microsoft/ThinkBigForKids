/*
 * Lesson 4: Line Following
 *
 * First you DRAW a track: use the arrow keys (or W / A / S / D) to move the pen
 * (+) and draw a line, then press Enter. After that, the robot must follow the
 * line you drew, using its five line sensors, all the way to the end.
 *
 * The same line-following code runs on a real robot, which follows a real line
 * taped on the floor.
 *
 * The five sensors read left (s[0]) to right (s[4]); each is true when it is
 * over the line. A straight line under the middle reads "00100"; if the line
 * drifts left you'll see a 1 on the left, and so on. The wide bar at the end
 * lights up all five sensors ("11111").
 *
 * YOUR TASKS (for the simulator) — fill in each // !EDIT block:
 *   #1: line is on the LEFT  (s[0] or s[1])  -> turn left
 *   #2: line is on the RIGHT (s[3] or s[4])  -> turn right
 *   #3: line is in the MIDDLE (s[2])         -> drive forward
 *   #4: all five sensors on the line         -> stop (reached the end!)
 *
 * If the robot loses the line, it does NOT give up — the provided code turns
 * back toward the side where it last saw the line.
 *
 * On a REAL robot, turning in place loses a narrow line, so a tuned
 * "steer while driving" follower is provided for you further down. Your job is
 * the simulator version.
 */

#include "robot/IRobot.h"
#include "robot/RobotTypes.h"

using namespace robot;

namespace robot {

void runStudentProgram(IRobot& robot) {
#if !defined(MBOT) && !defined(ARDUINO_CAR)
    // ===== Simulator: YOUR line-following code =====
    // The simulator world is a grid, so the robot follows the track by driving
    // forward and making 90-degree turns.
    const int STEP_MS = 300;
    const int TURN_MS = 300;
    const int MAX_STEPS = 300;

    int lastSeen = 1;  // which way the line last was: -1 = left, +1 = right

    for (int i = 0; i < MAX_STEPS && !robot.quitRequested(); ++i) {
        LineReading line = robot.readLineSensors();

        // !EDIT #4: if all five sensors are on the line, stop and break (we
        // reached the finish bar).

        if (line.s[0] || line.s[1]) {
            lastSeen = -1;  // remember the line was on our left
            // !EDIT #1: turn left for TURN_MS

        } else if (line.s[3] || line.s[4]) {
            lastSeen = +1;  // remember the line was on our right
            // !EDIT #2: turn right for TURN_MS

        } else if (line.s[2]) {
            // !EDIT #3: drive forward for STEP_MS

        } else {
            // Lost the line — search back toward where we last saw it.
            if (lastSeen < 0) {
                robot.turnLeft(TURN_MS);
            } else {
                robot.turnRight(TURN_MS);
            }
        }
    }
#else
    // ===== Real robot: PROVIDED smooth follower (you don't edit this) =====
    // A real robot can't pivot in place without losing a narrow line, so it
    // steers by driving forward with the wheels at different speeds, curving
    // toward the line. The speed GAP sets how hard it turns. These values were
    // tuned on a real mBot.
    const int STEP_MS = 30;
    const int FAST = 230;       // outer wheel
    const int SLOW = 100;       // inner wheel on a normal correction (gap 130)
    const int SEEK = 30;        // inner wheel when fully off the line (sharpest)
    const int STRAIGHT = 205;   // both wheels when centered on the line
    const int MAX_STEPS = 3000;

    int lastSeen = 1;

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
            // Off the line: swing back hard toward the side we last saw it.
            if (lastSeen < 0) {
                robot.setMotorSpeed(SEEK, FAST);
            } else {
                robot.setMotorSpeed(FAST, SEEK);
            }
            robot.forward(STEP_MS);
        }
    }
    robot.stop();
#endif
}

}  // namespace robot
