#pragma once

#include "robot/IRobot.h"
#include "SimWorld.h"

namespace robot {

/*
 * SimRobot — implements IRobot against a SimWorld. It owns the Arduino-style
 * timing semantics: movement methods block for the requested milliseconds and
 * animate the world frame-by-frame while they run, so behavior matches what a
 * real car would do.
 */
class SimRobot : public IRobot {
public:
    explicit SimRobot(SimWorld& world);

    void forward(int ms) override;
    void reverse(int ms) override;
    void turnLeft(int ms) override;
    void turnRight(int ms) override;
    void setMotorSpeed(int leftSpeed, int rightSpeed) override;
    void stop() override;
    int scan() override;
    void lookLeft() override;
    void lookCenter() override;
    void lookRight() override;

private:
    // Drive along the heading: sign = +1 forward, -1 reverse. Blocks ~ms.
    void drive(int ms, int sign, const char* label);
    // Turn by the given quarter-turn direction (-1 left, +1 right). Blocks ~ms.
    void turn(int ms, int quarterSign, const char* label);
    int averageSpeed() const;
    void show(const std::string& status) const;

    SimWorld& world_;
    int leftSpeed_;
    int rightSpeed_;
    LookDirection look_ = LookDirection::Center;
};

}  // namespace robot
