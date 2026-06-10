/*
 * Lesson 2: Drive with the Virtual Remote
 *
 * Use the keyboard as a remote control! Press the arrow keys (or W / A / S / D)
 * to drive the robot around the room. Press Q to quit.
 *
 * The simulator turns your key presses into remote-button readings. Your job is
 * to make the robot move the right way for each button.
 *
 * YOUR TASKS — fill in each // !EDIT block:
 *   #1: Up    -> drive forward
 *   #2: Down  -> drive backward
 *   #3: Left  -> turn left
 *   #4: Right -> turn right
 *
 * Helpful facts:
 *   - robot.readRemote() returns a RemoteButton (Up, Down, Left, Right, Stop,
 *     or None when nothing is pressed).
 *   - robot.forward(ms) / reverse(ms) / turnLeft(ms) / turnRight(ms) take a
 *     number of milliseconds.
 */

#include "robot/IRobot.h"
#include "robot/RobotTypes.h"

using namespace robot;

namespace robot {

void runStudentProgram(IRobot& robot) {
    const int DRIVE_MS = 300;  // how far one press drives
    const int TURN_MS = 300;   // how far one press turns

    while (!robot.quitRequested()) {
        RemoteButton button = robot.readRemote();

        if (button == RemoteButton::Up) {
            // !EDIT #1: drive forward for DRIVE_MS

        } else if (button == RemoteButton::Down) {
            // !EDIT #2: drive backward for DRIVE_MS

        } else if (button == RemoteButton::Left) {
            // !EDIT #3: turn left for TURN_MS

        } else if (button == RemoteButton::Right) {
            // !EDIT #4: turn right for TURN_MS

        } else {
            robot.stop();
        }
    }
}

}  // namespace robot
