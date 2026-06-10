#pragma once

#include <string>
#include <vector>

#include "robot/RobotTypes.h"

namespace robot {

/*
 * SimWorld — the state of the text-simulator world: a 2D grid with obstacles,
 * a robot pose (position + heading), and a visited "trail". It also knows how
 * to raycast for the ultrasonic sensor and how to render itself to the
 * terminal using ANSI escape codes.
 *
 * Grid coordinates: x grows to the right, y grows downward (row 0 is the top).
 */
class SimWorld {
public:
    // How many centimeters one grid cell represents. Used to convert raycast
    // cell counts into the centimeter distances scan() reports.
    static constexpr int CM_PER_CELL = 10;

    SimWorld(int width, int height);

    // --- World setup ---
    void addObstacle(int x, int y);
    void setRobot(int x, int y, Heading heading);

    // Optional target object the robot can follow (object-following lesson).
    void addTarget(int x, int y);
    bool hasTarget() const { return hasTarget_; }
    int targetX() const { return targetX_; }
    int targetY() const { return targetY_; }
    // Move the target by (dx,dy) if the destination is in bounds and clear.
    void moveTarget(int dx, int dy);

    // Line overlay (line-following lesson).
    void setLine(int x, int y);
    bool isLine(int x, int y) const;

    // Drawing pen cursor (used during the interactive line-draw phase).
    void setPen(int x, int y, bool visible);

    // --- Queries ---
    int width() const { return width_; }
    int height() const { return height_; }
    bool inBounds(int x, int y) const;
    bool isObstacle(int x, int y) const;

    int robotX() const { return robotX_; }
    int robotY() const { return robotY_; }
    Heading heading() const { return heading_; }

    // --- Robot actions (pure state changes; timing lives in SimRobot) ---

    // Try to move one cell along the current heading (forward=+1, so pass -1 to
    // reverse). Returns true if the robot moved, false if blocked by a wall or
    // obstacle (in which case the robot stays put).
    bool step(int sign);

    // Rotate the heading 90 degrees. Negative quarters turn left
    // (counter-clockwise), positive turn right (clockwise).
    void rotate(int quarterTurns);

    // Distance in centimeters to the nearest obstacle/wall when looking in the
    // given direction relative to the current heading. Capped at
    // MAX_SCAN_DISTANCE_CM.
    int raycast(LookDirection look) const;

    // Detect the target object on the robot's left/right (object-following).
    // Both true means it is roughly straight ahead.
    void senseTarget(bool& left, bool& right) const;

    // Sample the five line sensors across the robot's front, left (out[0]) to
    // right (out[4]); true = that sensor is over the line.
    void readLine(bool out[5]) const;

    // --- Rendering ---

    // Draw the world to stdout (ANSI). statusLine is printed beneath the grid.
    void render(const std::string& statusLine) const;

private:
    int index(int x, int y) const { return y * width_ + x; }
    void headingDelta(Heading h, int& dx, int& dy) const;
    void headingRight(Heading h, int& dx, int& dy) const;
    Heading lookHeading(LookDirection look) const;

    int width_;
    int height_;
    std::vector<bool> obstacles_;
    std::vector<bool> trail_;
    std::vector<bool> line_;

    int robotX_ = 0;
    int robotY_ = 0;
    Heading heading_ = Heading::North;
    bool robotVisible_ = false;

    bool hasTarget_ = false;
    int targetX_ = 0;
    int targetY_ = 0;

    bool penVisible_ = false;
    int penX_ = 0;
    int penY_ = 0;
};

// Format a distance as an ANSI-colored string (green = clear, yellow = close,
// red = within obstacle range).
std::string colorizeDistance(int cm);

}  // namespace robot
