#include "SimWorld.h"

#include <cstdio>

namespace robot {

namespace {
// ANSI color helpers (kept local to the renderer).
constexpr const char* RESET = "\x1b[0m";
constexpr const char* GREEN = "\x1b[32m";
constexpr const char* YELLOW = "\x1b[33m";
constexpr const char* RED = "\x1b[31m";
constexpr const char* CYAN = "\x1b[36m";
constexpr const char* MAGENTA = "\x1b[35m";
}  // namespace

SimWorld::SimWorld(int width, int height)
    : width_(width),
      height_(height),
      obstacles_(static_cast<size_t>(width) * height, false),
      trail_(static_cast<size_t>(width) * height, false),
      line_(static_cast<size_t>(width) * height, false) {}

bool SimWorld::inBounds(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

bool SimWorld::isObstacle(int x, int y) const {
    if (!inBounds(x, y)) return false;
    return obstacles_[index(x, y)];
}

void SimWorld::addObstacle(int x, int y) {
    if (inBounds(x, y)) obstacles_[index(x, y)] = true;
}

void SimWorld::setRobot(int x, int y, Heading heading) {
    robotX_ = x;
    robotY_ = y;
    heading_ = heading;
    robotVisible_ = true;
    if (inBounds(x, y)) trail_[index(x, y)] = true;
}

void SimWorld::addTarget(int x, int y) {
    hasTarget_ = true;
    targetX_ = x;
    targetY_ = y;
}

void SimWorld::moveTarget(int dx, int dy) {
    if (!hasTarget_) return;
    int nx = targetX_ + dx;
    int ny = targetY_ + dy;
    if (inBounds(nx, ny) && !isObstacle(nx, ny)) {
        targetX_ = nx;
        targetY_ = ny;
    }
}

void SimWorld::setLine(int x, int y) {
    if (inBounds(x, y)) line_[index(x, y)] = true;
}

bool SimWorld::isLine(int x, int y) const {
    if (!inBounds(x, y)) return false;
    return line_[index(x, y)];
}

void SimWorld::setPen(int x, int y, bool visible) {
    penX_ = x;
    penY_ = y;
    penVisible_ = visible;
}

void SimWorld::headingRight(Heading h, int& dx, int& dy) const {
    // Right-hand perpendicular of the heading direction.
    switch (h) {
        case Heading::North: dx = 1;  dy = 0;  break;
        case Heading::East:  dx = 0;  dy = 1;  break;
        case Heading::South: dx = -1; dy = 0;  break;
        case Heading::West:  dx = 0;  dy = -1; break;
    }
}

void SimWorld::senseTarget(bool& left, bool& right) const {
    left = false;
    right = false;
    if (!hasTarget_) return;

    int fx = 0, fy = 0, rx = 0, ry = 0;
    headingDelta(heading_, fx, fy);
    headingRight(heading_, rx, ry);

    int dx = targetX_ - robotX_;
    int dy = targetY_ - robotY_;
    int forward = dx * fx + dy * fy;       // how far ahead
    int lateral = dx * rx + dy * ry;       // + = right of robot
    int dist2 = dx * dx + dy * dy;

    const int RANGE = 6;
    if (forward < 0 || dist2 > RANGE * RANGE) return;  // behind or too far

    if (lateral >= -1 && lateral <= 1) {
        left = true;
        right = true;       // roughly straight ahead
    } else if (lateral > 1) {
        right = true;
    } else {
        left = true;
    }
}

void SimWorld::readLine(bool out[5]) const {
    int rx = 0, ry = 0;
    headingRight(heading_, rx, ry);

    // Sample a row of 5 cells across the robot, perpendicular to the heading:
    // k = -2 (far left) .. +2 (far right), centered on the robot's cell so that
    // 90-degree corners in the track are detected.
    for (int k = -2; k <= 2; ++k) {
        int cx = robotX_ + rx * k;
        int cy = robotY_ + ry * k;
        out[k + 2] = isLine(cx, cy);
    }
}

void SimWorld::headingDelta(Heading h, int& dx, int& dy) const {
    switch (h) {
        case Heading::North: dx = 0; dy = -1; break;
        case Heading::East:  dx = 1; dy = 0;  break;
        case Heading::South: dx = 0; dy = 1;  break;
        case Heading::West:  dx = -1; dy = 0; break;
    }
}

bool SimWorld::step(int sign) {
    int dx = 0, dy = 0;
    headingDelta(heading_, dx, dy);
    int nx = robotX_ + dx * sign;
    int ny = robotY_ + dy * sign;
    if (!inBounds(nx, ny) || isObstacle(nx, ny)) {
        return false;  // Blocked — robot stays put.
    }
    robotX_ = nx;
    robotY_ = ny;
    trail_[index(nx, ny)] = true;
    return true;
}

void SimWorld::rotate(int quarterTurns) {
    // Heading order clockwise: North, East, South, West.
    int h = static_cast<int>(heading_);
    h = ((h + quarterTurns) % 4 + 4) % 4;
    heading_ = static_cast<Heading>(h);
}

Heading SimWorld::lookHeading(LookDirection look) const {
    switch (look) {
        case LookDirection::Left:
            return static_cast<Heading>(((static_cast<int>(heading_) - 1) % 4 + 4) % 4);
        case LookDirection::Right:
            return static_cast<Heading>((static_cast<int>(heading_) + 1) % 4);
        case LookDirection::Center:
        default:
            return heading_;
    }
}

int SimWorld::raycast(LookDirection look) const {
    int dx = 0, dy = 0;
    headingDelta(lookHeading(look), dx, dy);
    const int maxCells = MAX_SCAN_DISTANCE_CM / CM_PER_CELL;
    for (int k = 1; k <= maxCells; ++k) {
        int cx = robotX_ + dx * k;
        int cy = robotY_ + dy * k;
        if (!inBounds(cx, cy) || obstacles_[index(cx, cy)]) {
            return k * CM_PER_CELL;  // Wall and obstacle both count as objects.
        }
    }
    return MAX_SCAN_DISTANCE_CM;
}

void SimWorld::render(const std::string& statusLine) const {
    std::string frame;
    frame.reserve(static_cast<size_t>((width_ + 3) * (height_ + 4)));

    // Clear screen and move cursor home.
    frame += "\x1b[2J\x1b[H";

    // Top border.
    frame += '+';
    for (int x = 0; x < width_; ++x) frame += '-';
    frame += "+\n";

    char robotGlyph = '^';
    switch (heading_) {
        case Heading::North: robotGlyph = '^'; break;
        case Heading::East:  robotGlyph = '>'; break;
        case Heading::South: robotGlyph = 'v'; break;
        case Heading::West:  robotGlyph = '<'; break;
    }

    for (int y = 0; y < height_; ++y) {
        frame += '|';
        for (int x = 0; x < width_; ++x) {
            if (robotVisible_ && x == robotX_ && y == robotY_) {
                frame += CYAN;
                frame += robotGlyph;
                frame += RESET;
            } else if (penVisible_ && x == penX_ && y == penY_) {
                frame += MAGENTA;
                frame += '+';
                frame += RESET;
            } else if (hasTarget_ && x == targetX_ && y == targetY_) {
                frame += GREEN;
                frame += 'O';
                frame += RESET;
            } else if (obstacles_[index(x, y)]) {
                frame += '#';
            } else if (line_[index(x, y)]) {
                frame += YELLOW;
                frame += '=';
                frame += RESET;
            } else if (trail_[index(x, y)]) {
                frame += '.';
            } else {
                frame += ' ';
            }
        }
        frame += "|\n";
    }

    // Bottom border.
    frame += '+';
    for (int x = 0; x < width_; ++x) frame += '-';
    frame += "+\n";

    frame += statusLine;
    frame += '\n';

    std::fputs(frame.c_str(), stdout);
    std::fflush(stdout);
}

// Returns an ANSI-colored distance string: green when clear, yellow when
// getting close, red when within obstacle range. Free helper used by SimRobot
// when building status lines.
std::string colorizeDistance(int cm) {
    const char* color = GREEN;
    if (cm <= OBSTACLE_DISTANCE_CM) {
        color = RED;
    } else if (cm <= OBSTACLE_DISTANCE_CM * 2) {
        color = YELLOW;
    }
    return std::string(color) + std::to_string(cm) + "cm" + RESET;
}

}  // namespace robot
