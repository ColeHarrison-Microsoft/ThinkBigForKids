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
}  // namespace

SimWorld::SimWorld(int width, int height)
    : width_(width),
      height_(height),
      obstacles_(static_cast<size_t>(width) * height, false),
      trail_(static_cast<size_t>(width) * height, false) {}

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
    if (inBounds(x, y)) trail_[index(x, y)] = true;
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
            if (x == robotX_ && y == robotY_) {
                frame += CYAN;
                frame += robotGlyph;
                frame += RESET;
            } else if (obstacles_[index(x, y)]) {
                frame += '#';
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
