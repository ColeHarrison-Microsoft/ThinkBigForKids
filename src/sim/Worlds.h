#pragma once

#include "SimWorld.h"

namespace robot {

// Course used by Lessons 1 & 5 (movement + obstacle avoidance).
SimWorld buildDefaultWorld();

// Room with obstacles for the IR-remote lesson (Lesson 2).
SimWorld buildMovementWorld();

// Open area with a target object to follow (Lesson 3).
SimWorld buildFollowWorld();

// Empty world; the line is drawn interactively before the follow phase
// (Lesson 4).
SimWorld buildLineWorld();

}  // namespace robot
