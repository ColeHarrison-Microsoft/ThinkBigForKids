#pragma once

#include <stdint.h>

#include "robot/IRobot.h"
#include "robot/RobotTypes.h"

namespace robot {

/*
 * MBotRobot — IRobot backend for the classic mBot (blue mCore board,
 * ATmega328P) using the Makeblock library.
 *
 * The IRobot interface was modeled on the DIY Arduino car, which has a richer
 * sensor set than the mBot. To keep the event's "write once, run on the
 * simulator or any robot" promise, this backend maps the mBot's real hardware
 * onto the full interface as faithfully as possible and documents the
 * differences:
 *
 *   - Ultrasonic head: the mBot's ultrasonic sensor is FIXED (no servo). To make
 *     the obstacle-avoidance lesson's "look left / center / right" still mean
 *     something, scan() briefly PIVOTS the body to aim the fixed sensor, reads,
 *     then pivots back to restore the heading. (Build with -DMBOT_PIVOT_SCAN=0
 *     to fall back to a forward-only reading.)
 *   - Line following: the mBot has only TWO line sensors, not five. The two
 *     readings are mapped into the five-sensor LineReading so the student's
 *     line-following code runs unchanged. The five-on-the-line "finish bar"
 *     pattern cannot occur with two sensors.
 *   - Object following: the mBot has no side object sensors, so readObstacles()
 *     is synthesized from the single forward ultrasonic — it reports an object
 *     "straight ahead" (both sides) when something is within range. The robot
 *     drives toward an object in front but cannot steer left/right toward it.
 *
 * Implementation note: the Makeblock umbrella header (<MeMCore.h>) defines the
 * global mePort[] table, so it must be included in exactly one translation
 * unit. The hardware objects therefore live behind an opaque Impl (defined only
 * in MBotRobot.cpp); this header stays free of Makeblock includes so it can be
 * pulled into main_mbot.cpp without a duplicate-symbol link error.
 *
 * Anything to confirm on the physical robot during bring-up is marked
 * "MUST-VERIFY" and exposed as a named constant so it can be flipped without
 * restructuring the code.
 */
class MBotRobot : public IRobot {
public:
    MBotRobot();
    ~MBotRobot() override;

    // Initialize hardware (call once from Arduino setup() before running a
    // student program).
    void begin();

    // Block until the mBot's onboard button is pressed (and released), so a
    // freshly flashed or just-powered-on robot waits on the desk instead of
    // driving off. Call from the entry point before runStudentProgram().
    void waitForStartButton();

    // --- IRobot: movement ---
    void forward(int ms) override;
    void reverse(int ms) override;
    void turnLeft(int ms) override;
    void turnRight(int ms) override;
    void setMotorSpeed(int leftSpeed, int rightSpeed) override;
    void stop() override;

    // --- IRobot: sensing ---
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
    // Drive the two wheels in the given directions (+1 forward, -1 reverse) at
    // the current per-side speeds. Does not block.
    void applyWheels(int leftDir, int rightDir);
    // Run a brief in-place pivot (used to aim the fixed ultrasonic for scan()).
    void pivot(int leftDir, int rightDir, int ms);
    // Read the ultrasonic, clamped to [0, MAX_SCAN_DISTANCE_CM] (returns the max
    // when nothing is in range).
    int readUltrasonicCm();
    // Decode the 2-bit line reading into "left sensor / right sensor over line".
    void decodeLine(uint8_t raw, bool& leftOnLine, bool& rightOnLine) const;

    struct Impl;   // holds the Makeblock hardware objects (see MBotRobot.cpp)
    Impl* impl_;

    int leftSpeed_;
    int rightSpeed_;
    LookDirection look_;
};

}  // namespace robot
