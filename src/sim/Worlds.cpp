#include "Worlds.h"

namespace robot {

SimWorld buildDefaultWorld() {
    SimWorld world(28, 14);

    world.addObstacle(13, 5);
    world.addObstacle(14, 5);
    world.addObstacle(8, 8);
    world.addObstacle(20, 9);
    world.addObstacle(21, 9);
    world.addObstacle(6, 3);
    world.addObstacle(18, 2);

    // Robot starts near the bottom-center, pointing up (North).
    world.setRobot(14, 12, Heading::North);
    return world;
}

SimWorld buildMovementWorld() {
    SimWorld world(28, 14);

    // A few walls to drive around with the remote.
    for (int x = 6; x <= 10; ++x) world.addObstacle(x, 4);
    for (int x = 17; x <= 21; ++x) world.addObstacle(x, 9);
    world.addObstacle(14, 6);
    world.addObstacle(14, 7);

    world.setRobot(14, 11, Heading::North);
    return world;
}

SimWorld buildFollowWorld() {
    SimWorld world(28, 16);

    // Robot at the bottom, target a few cells ahead to chase.
    world.setRobot(14, 12, Heading::North);
    world.addTarget(14, 7);
    return world;
}

SimWorld buildLineWorld() {
    // Empty canvas; the student draws the track and the robot start is set
    // during the interactive draw phase.
    return SimWorld(30, 16);
}

}  // namespace robot
