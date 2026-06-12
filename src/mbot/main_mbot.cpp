#include <Arduino.h>

#include "robot/IRobot.h"
#include "MBotRobot.h"

/*
 * Arduino entry point for the classic mBot. Each mBot PlatformIO env compiles
 * this file + MBotRobot + exactly one lesson (or example) that defines
 * runStudentProgram(). We build the robot, hand it to the student's program in
 * setup(), and leave loop() empty — the interactive lessons run their own loop
 * via `while (!robot.quitRequested())`, which never exits on hardware.
 */

namespace {
robot::MBotRobot mbotRobot;
}

void setup() {
    mbotRobot.begin();
    mbotRobot.waitForStartButton();  // wait on the desk until the button is pressed
    robot::runStudentProgram(mbotRobot);
}

void loop() {}
