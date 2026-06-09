#include "SimRobot.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>
#include <thread>

namespace robot {

namespace {
// At full speed (255) the robot covers one grid cell every this many ms.
constexpr int MS_PER_CELL_AT_FULL = 400;
// A turn of at least this many ms rotates the robot 90 degrees.
constexpr int MS_PER_QUARTER_TURN = 500;

void sleepMs(int ms) {
    if (ms > 0) std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

const char* lookName(LookDirection look) {
    switch (look) {
        case LookDirection::Left:   return "LEFT";
        case LookDirection::Right:  return "RIGHT";
        case LookDirection::Center:
        default:                    return "CENTER";
    }
}
}  // namespace

SimRobot::SimRobot(SimWorld& world)
    : world_(world), leftSpeed_(DEFAULT_SPEED), rightSpeed_(DEFAULT_SPEED) {}

int SimRobot::averageSpeed() const {
    return (leftSpeed_ + rightSpeed_) / 2;
}

void SimRobot::show(const std::string& status) const {
    world_.render(status);
}

void SimRobot::drive(int ms, int sign, const char* label) {
    int avg = std::max(0, std::min(MAX_SPEED, averageSpeed()));
    double factor = static_cast<double>(avg) / MAX_SPEED;
    int cells = static_cast<int>(std::lround(factor * ms / MS_PER_CELL_AT_FULL));

    std::string status = std::string(label) + "  speed L/R=" +
                         std::to_string(leftSpeed_) + "/" + std::to_string(rightSpeed_);

    if (cells <= 0) {
        // Not enough speed/time to cover a whole cell — still block for ms.
        show(status);
        sleepMs(ms);
        return;
    }

    int sleepPer = std::max(1, ms / cells);
    for (int i = 0; i < cells; ++i) {
        if (!world_.step(sign)) {
            show(std::string(label) + "  BLOCKED!");
            sleepMs(sleepPer);
            break;
        }
        show(status);
        sleepMs(sleepPer);
    }
}

void SimRobot::turn(int ms, int quarterSign, const char* label) {
    int quarters = std::max(1, static_cast<int>(std::lround(
                                  static_cast<double>(ms) / MS_PER_QUARTER_TURN)));
    int sleepPer = std::max(1, ms / quarters);
    for (int i = 0; i < quarters; ++i) {
        world_.rotate(quarterSign);
        show(label);
        sleepMs(sleepPer);
    }
}

void SimRobot::forward(int ms) { drive(ms, +1, "FORWARD"); }
void SimRobot::reverse(int ms) { drive(ms, -1, "REVERSE"); }
void SimRobot::turnLeft(int ms) { turn(ms, -1, "TURN LEFT"); }
void SimRobot::turnRight(int ms) { turn(ms, +1, "TURN RIGHT"); }

void SimRobot::setMotorSpeed(int leftSpeed, int rightSpeed) {
    leftSpeed_ = std::max(MIN_SPEED, std::min(MAX_SPEED, leftSpeed));
    rightSpeed_ = std::max(MIN_SPEED, std::min(MAX_SPEED, rightSpeed));
}

void SimRobot::stop() {
    show("STOP");
    sleepMs(150);
}

int SimRobot::scan() {
    int cm = world_.raycast(look_);
    show(std::string("SCAN ") + lookName(look_) + " -> " + colorizeDistance(cm));
    sleepMs(150);
    return cm;
}

void SimRobot::lookLeft() {
    look_ = LookDirection::Left;
    show("LOOK LEFT");
    sleepMs(150);
}

void SimRobot::lookCenter() {
    look_ = LookDirection::Center;
    show("LOOK CENTER");
    sleepMs(150);
}

void SimRobot::lookRight() {
    look_ = LookDirection::Right;
    show("LOOK RIGHT");
    sleepMs(150);
}

}  // namespace robot
