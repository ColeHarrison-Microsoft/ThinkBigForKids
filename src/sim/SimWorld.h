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

    // --- Rendering ---

    // Draw the world to stdout (ANSI). statusLine is printed beneath the grid.
    void render(const std::string& statusLine) const;

private:
    int index(int x, int y) const { return y * width_ + x; }
    void headingDelta(Heading h, int& dx, int& dy) const;
    Heading lookHeading(LookDirection look) const;

    int width_;
    int height_;
    std::vector<bool> obstacles_;
    std::vector<bool> trail_;

    int robotX_ = 0;
    int robotY_ = 0;
    Heading heading_ = Heading::North;
};

// Format a distance as an ANSI-colored string (green = clear, yellow = close,
// red = within obstacle range).
std::string colorizeDistance(int cm);

}  // namespace robot
