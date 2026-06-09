#include <cstdio>

#include "robot/IRobot.h"
#include "SimRobot.h"
#include "SimWorld.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace {

// On Windows, turn on ANSI escape-code processing so the colored/animated
// terminal UI renders instead of showing raw escape sequences. No-op elsewhere.
void enableAnsi() {
#if defined(_WIN32)
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out == INVALID_HANDLE_VALUE) return;
    DWORD mode = 0;
    if (!GetConsoleMode(out, &mode)) return;
    SetConsoleMode(out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

// Build the default course the student program drives through. A clear lane up
// the middle with a scattering of obstacles to detect and avoid.
robot::SimWorld buildDefaultWorld() {
    robot::SimWorld world(28, 14);

    world.addObstacle(13, 5);
    world.addObstacle(14, 5);
    world.addObstacle(8, 8);
    world.addObstacle(20, 9);
    world.addObstacle(21, 9);
    world.addObstacle(6, 3);
    world.addObstacle(18, 2);

    // Robot starts near the bottom-center, pointing up (North).
    world.setRobot(14, 12, robot::Heading::North);
    return world;
}

}  // namespace

int main() {
    enableAnsi();

    robot::SimWorld world = buildDefaultWorld();
    robot::SimRobot robot(world);

    robot::runStudentProgram(robot);

    std::printf("\nProgram finished. Press Enter to exit.\n");
    std::getchar();
    return 0;
}
