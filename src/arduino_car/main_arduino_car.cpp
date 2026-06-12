#include <Arduino.h>

#include "ArduinoCarRobot.h"
#include "robot/IRobot.h"

robot::ArduinoCarRobot carRobot;

void setup() {
    carRobot.begin();
    robot::runStudentProgram(carRobot);
}

void loop() {}
