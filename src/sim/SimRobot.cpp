#include "SimRobot.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>
#include <thread>
#include <utility>
#include <vector>

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

// --- Controller / extra sensors ---

void SimRobot::pump() {
    Key k = kb_.poll();
    if (k == Key::Quit) {
        quit_ = true;
        lastButton_ = RemoteButton::Stop;
        return;
    }
    if (k == Key::Eof) {
        // End of piped input. For the interactive lessons that means "done";
        // for the autonomous lessons (Idle) we ignore it and keep running.
        if (mode_ == InputMode::Remote || mode_ == InputMode::Follow) {
            quit_ = true;
            lastButton_ = RemoteButton::Stop;
        }
        return;
    }
    if (k == Key::None) {
        // Nothing pressed this cycle: in remote mode that means "let go" -> stop.
        if (mode_ == InputMode::Remote) lastButton_ = RemoteButton::None;
        return;
    }
    if (mode_ == InputMode::Remote) {
        switch (k) {
            case Key::Up:    lastButton_ = RemoteButton::Up;    break;
            case Key::Down:  lastButton_ = RemoteButton::Down;  break;
            case Key::Left:  lastButton_ = RemoteButton::Left;  break;
            case Key::Right: lastButton_ = RemoteButton::Right; break;
            case Key::Space: lastButton_ = RemoteButton::Stop;  break;
            default:         lastButton_ = RemoteButton::None;  break;
        }
    } else if (mode_ == InputMode::Follow) {
        switch (k) {
            case Key::Up:    world_.moveTarget(0, -1); break;
            case Key::Down:  world_.moveTarget(0, 1);  break;
            case Key::Left:  world_.moveTarget(-1, 0); break;
            case Key::Right: world_.moveTarget(1, 0);  break;
            default: break;
        }
    }
}

RemoteButton SimRobot::readRemote() {
    pump();
    return lastButton_;
}

bool SimRobot::quitRequested() {
    return quit_;
}

ObstacleReading SimRobot::readObstacles() {
    pump();
    ObstacleReading r;
    world_.senseTarget(r.left, r.right);
    return r;
}

LineReading SimRobot::readLineSensors() {
    pump();
    LineReading r;
    world_.readLine(r.s);
    return r;
}

namespace {
// Heading helpers used only by the draw phase.
void headingVec(Heading h, int& dx, int& dy) {
    switch (h) {
        case Heading::North: dx = 0; dy = -1; break;
        case Heading::East:  dx = 1; dy = 0;  break;
        case Heading::South: dx = 0; dy = 1;  break;
        case Heading::West:  dx = -1; dy = 0; break;
    }
}
void rightVec(Heading h, int& dx, int& dy) {
    switch (h) {
        case Heading::North: dx = 1;  dy = 0;  break;
        case Heading::East:  dx = 0;  dy = 1;  break;
        case Heading::South: dx = -1; dy = 0;  break;
        case Heading::West:  dx = 0;  dy = -1; break;
    }
}
Heading headingFromDelta(int dx, int dy) {
    if (dx > 0) return Heading::East;
    if (dx < 0) return Heading::West;
    if (dy > 0) return Heading::South;
    return Heading::North;  // dy < 0 or no move
}
}  // namespace

bool SimRobot::runLineDrawPhase() {
    int px = world_.width() / 2;
    int py = world_.height() - 2;
    std::vector<std::pair<int, int>> path;

    world_.setLine(px, py);
    path.push_back({px, py});
    world_.setPen(px, py, true);

    const char* status =
        "DRAW THE TRACK:  W/A/S/D (or arrows) to draw,  Enter to finish";

    for (;;) {
        show(status);
        Key k = kb_.poll();
        if (k == Key::Enter || k == Key::Quit || k == Key::Eof) break;

        int dx = 0, dy = 0;
        switch (k) {
            case Key::Up:    dy = -1; break;
            case Key::Down:  dy = 1;  break;
            case Key::Left:  dx = -1; break;
            case Key::Right: dx = 1;  break;
            case Key::None:  sleepMs(20); continue;  // interactive idle
            default: continue;
        }
        int nx = px + dx;
        int ny = py + dy;
        if (world_.inBounds(nx, ny)) {
            px = nx;
            py = ny;
            world_.setLine(px, py);
            world_.setPen(px, py, true);
            path.push_back({px, py});
        }
    }

    world_.setPen(0, 0, false);
    if (path.size() < 2) return false;

    // Robot starts at the first drawn cell, heading toward the second.
    int hx = path[1].first - path[0].first;
    int hy = path[1].second - path[0].second;
    Heading startHeading = headingFromDelta(hx, hy);

    // Paint a wide bar at the end so all five sensors read the line ("11111"),
    // giving the student a clear finish pattern to detect.
    int ex = path.back().first;
    int ey = path.back().second;
    int pex = path[path.size() - 2].first;
    int pey = path[path.size() - 2].second;
    Heading endHeading = headingFromDelta(ex - pex, ey - pey);
    int rx = 0, ry = 0;
    rightVec(endHeading, rx, ry);
    for (int kk = -2; kk <= 2; ++kk) {
        world_.setLine(ex + rx * kk, ey + ry * kk);
    }
    // Also extend one cell past the end so the front sensors reach the bar.
    int fx = 0, fy = 0;
    headingVec(endHeading, fx, fy);
    for (int kk = -2; kk <= 2; ++kk) {
        world_.setLine(ex + fx + rx * kk, ey + fy + ry * kk);
    }

    world_.setRobot(path[0].first, path[0].second, startHeading);
    return true;
}

}  // namespace robot
