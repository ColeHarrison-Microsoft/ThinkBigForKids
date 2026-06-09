#pragma once

/*
 * RobotTypes.h — Shared constants and small value types used across every
 * robot backend (simulator, mBot, Arduino car) and by student code.
 *
 * C++ standard floor:
 *   - The simulator / native build targets C++17.
 *   - The AVR backends (mBot, Arduino car) may fall back to C++11 if a board's
 *     toolchain requires it. Keep everything in this header C++11-compatible so
 *     it can be shared by all three backends without changes.
 */

namespace robot {

// Motor speed range. Mirrors the Arduino PWM range used in the original
// lessons (analogWrite takes 0..255).
constexpr int MIN_SPEED = 0;
constexpr int MAX_SPEED = 255;

// Default driving speed. Matches setMotorSpeed(200) from the reference
// Lesson1.ino movement primitives.
constexpr int DEFAULT_SPEED = 200;

// Distance (in centimeters) at or below which scan() considers something an
// obstacle. Matches DISTANCE_LIMIT from the reference Lesson5.ino.
constexpr int OBSTACLE_DISTANCE_CM = 30;

// The largest distance scan() will report when nothing is in range.
constexpr int MAX_SCAN_DISTANCE_CM = 200;

// Compass-style heading the robot is facing. Used by the simulator to track
// pose; hardware backends ignore it.
enum class Heading {
    North,
    East,
    South,
    West
};

// Which way the ultrasonic "head" (servo) is pointed when scanning.
enum class LookDirection {
    Left,
    Center,
    Right
};

// Result of looking in all three directions at once. Handy for obstacle
// avoidance logic without juggling three separate variables.
struct ScanResult {
    int left = MAX_SCAN_DISTANCE_CM;
    int center = MAX_SCAN_DISTANCE_CM;
    int right = MAX_SCAN_DISTANCE_CM;
};

}  // namespace robot
