#pragma once

#include "robot/IRobot.h"
#include "Keyboard.h"
#include "SimWorld.h"

namespace robot {

// How the keyboard "virtual remote" is interpreted while a lesson runs.
enum class InputMode {
    Idle,    // keys only used to quit (Lessons 1, 4-follow, 5)
    Remote,  // keys set the current remote button (Lesson 2)
    Follow   // keys move the target object the robot follows (Lesson 3)
};

/*
 * SimRobot — implements IRobot against a SimWorld. It owns the Arduino-style
 * timing semantics: movement methods block for the requested milliseconds and
 * animate the world frame-by-frame while they run, so behavior matches what a
 * real car would do. It also owns the keyboard "virtual remote" and computes
 * the remote / object / line-sensor readings from the world.
 */
class SimRobot : public IRobot {
public:
    explicit SimRobot(SimWorld& world);

    // Choose how keyboard input is interpreted for the current lesson.
    void setInputMode(InputMode mode) { mode_ = mode; }

    // Interactive line-draw phase (Lesson 4): the user draws the track with
    // WASD / arrows and presses Enter to finish. Paints the line, then places
    // the robot at the line's start. Returns false if the user quit instead.
    bool runLineDrawPhase();

    // --- IRobot: movement ---
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

    // --- IRobot: controller / extra sensors ---
    RemoteButton readRemote() override;
    bool quitRequested() override;
    ObstacleReading readObstacles() override;
    LineReading readLineSensors() override;

private:
    // Drive along the heading: sign = +1 forward, -1 reverse. Blocks ~ms.
    void drive(int ms, int sign, const char* label);
    // Turn by the given quarter-turn direction (-1 left, +1 right). Blocks ~ms.
    void turn(int ms, int quarterSign, const char* label);
    int averageSpeed() const;
    void show(const std::string& status) const;

    // Poll the keyboard once and apply the effect for the current input mode.
    void pump();

    SimWorld& world_;
    Keyboard kb_;
    InputMode mode_ = InputMode::Idle;
    RemoteButton lastButton_ = RemoteButton::None;
    bool quit_ = false;

    int leftSpeed_;
    int rightSpeed_;
    LookDirection look_ = LookDirection::Center;
};

}  // namespace robot
