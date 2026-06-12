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

// std::string is only available on the native/simulator build. The Arduino
// (AVR) toolchain that the mBot and Arduino-car backends use has no <string>,
// so it is guarded out for hardware. The only thing that needs it is the
// sim-only pattern() helper below, which no lesson depends on.
#if !defined(ARDUINO)
#include <string>
#endif

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

// A button press coming from the robot's controller. On hardware this is an IR
// remote; in the simulator it is the keyboard "virtual remote".
enum class RemoteButton {
    None,   // nothing pressed right now
    Up,     // drive forward
    Down,   // drive backward
    Left,   // turn left
    Right,  // turn right
    Stop    // stop
};

// Reading from the two object-detection sensors (one per side). true means an
// object is detected on that side. Used by the object-following lesson.
struct ObstacleReading {
    bool left = false;
    bool right = false;
};

// Reading from the five line-tracking sensors, left (s[0]) to right (s[4]).
// true means that sensor is over the line. Used by the line-following lesson.
struct LineReading {
    bool s[5] = {false, false, false, false, false};

#if !defined(ARDUINO)
    // Render the reading as a 5-character pattern like "00100" (1 = on the
    // line). Handy for comparing against known patterns, mirroring the original
    // Arduino lesson. Native/simulator build only (no <string> on AVR).
    std::string pattern() const {
        std::string out;
        for (bool on : s) out += (on ? '1' : '0');
        return out;
    }
#endif
};

// Result of looking in all three directions at once. Handy for obstacle
// avoidance logic without juggling three separate variables.
struct ScanResult {
    int left = MAX_SCAN_DISTANCE_CM;
    int center = MAX_SCAN_DISTANCE_CM;
    int right = MAX_SCAN_DISTANCE_CM;
};

}  // namespace robot
