#include <cstdio>

#include "robot/IRobot.h"
#include "SimRobot.h"
#include "SimWorld.h"
#include "Worlds.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

/*
 * Which world/scenario to build is chosen at compile time by the PlatformIO
 * env via a -D flag:
 *   (none)        -> default course (Lessons 1 & 5)
 *   WORLD_REMOTE  -> IR-remote room       (Lesson 2)
 *   WORLD_FOLLOW  -> object-following area (Lesson 3)
 *   WORLD_LINE    -> interactive line draw (Lesson 4)
 */

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

}  // namespace

int main() {
    enableAnsi();

#if defined(WORLD_REMOTE)
    robot::SimWorld world = robot::buildMovementWorld();
#elif defined(WORLD_FOLLOW)
    robot::SimWorld world = robot::buildFollowWorld();
#elif defined(WORLD_LINE)
    robot::SimWorld world = robot::buildLineWorld();
#else
    robot::SimWorld world = robot::buildDefaultWorld();
#endif

    robot::SimRobot robot(world);

#if defined(WORLD_REMOTE)
    robot.setInputMode(robot::InputMode::Remote);
    robot::runStudentProgram(robot);
#elif defined(WORLD_FOLLOW)
    robot.setInputMode(robot::InputMode::Follow);
    robot::runStudentProgram(robot);
#elif defined(WORLD_LINE)
    robot.setInputMode(robot::InputMode::Idle);
    if (robot.runLineDrawPhase()) {
        robot::runStudentProgram(robot);
    }
#else
    robot::runStudentProgram(robot);
#endif

    std::printf("\nProgram finished. Press Enter to exit.\n");
    std::getchar();
    return 0;
}
