# Session 2: Vibe Coding with Copilot

GitHub Copilot Free is an AI coding helper. In this session, **vibe coding** means you describe the robot behavior you want in plain English, let Copilot draft C++ code, then you read it, test it, and fix it. You are still the driver. Copilot is your teammate, not your autopilot.

## Your code shape

Most prompts today should ask Copilot to write code inside this function:

```cpp
void runStudentProgram(IRobot& robot) {
    // Your robot idea goes here.
}
```

Use the same robot methods from Session 1, like `forward(ms)`, `turnLeft(ms)`, `scan()`, and `readLineSensors()`.

## How to write a good Copilot prompt

- **Be specific.** Say what the robot should do, how long it should move, and when it should stop.
- **Use the real method names.** Mention methods like `forward(1000)`, `turnRight(500)`, `scan()`, or `setMotorSpeed(left, right)`.
- **Say the goal and the rules.** Example: "Avoid obstacles, but do not use any sensors except `scan()`."
- **Ask for small steps.** One feature at a time is easier to test than one giant program.
- **Tell Copilot where the code goes.** Ask for C++ inside `runStudentProgram(IRobot& robot)`.
- **Test after each change.** If the robot surprises you, stop and read the code before trying again.

| Before | Better |
|---|---|
| "make it move" | "Write C++ inside `runStudentProgram(IRobot& robot)` that drives forward for 1 second, turns right about 90 degrees, and repeats 4 times to trace a square." |

## Golden rules for working with AI

- Copilot can be wrong. That is normal.
- Always read the code before you run it.
- You are responsible for what your robot does.
- Do not accept code you do not understand yet.
- If you are unsure, ask a mentor. Debugging together is part of engineering.

## IRobot method cheat sheet

| What you want | Methods and types you can use |
|---|---|
| Timed movement | `forward(ms)`, `reverse(ms)`, `turnLeft(ms)`, `turnRight(ms)`, `stop()` |
| Speed changes | `setMotorSpeed(left, right)`, `DEFAULT_SPEED` |
| Distance scanning | `lookLeft()`, `lookCenter()`, `lookRight()`, `scan()`, `OBSTACLE_DISTANCE_CM` |
| Remote control | `readRemote()`, `RemoteButton::Up`, `RemoteButton::Down`, `RemoteButton::Left`, `RemoteButton::Right`, `RemoteButton::Stop`, `quitRequested()` |
| Object sensors | `readObstacles()`, `.left`, `.right` |
| Line sensors | `readLineSensors()`, `s[0]`, `s[1]`, `s[2]`, `s[3]`, `s[4]` |

## 🟢 Warm-up challenges

| Challenge | Goal | IRobot methods likely used | Sample Copilot prompt |
|---|---|---|---|
| Square Driver | Drive in a square using a loop. | `forward(ms)`, `turnRight(ms)`, `stop()` | "Write C++ inside `runStudentProgram(IRobot& robot)` that uses a `for` loop to drive a square: forward for 1000 ms, turn right for about 500 ms, repeat 4 times, then stop." |
| Robot Dance | Make a short dance routine with different moves. | `setMotorSpeed(left, right)`, `forward(ms)`, `reverse(ms)`, `turnLeft(ms)`, `turnRight(ms)`, `stop()` | "Write a fun 20-second robot dance inside `runStudentProgram`. Use only `setMotorSpeed`, `forward`, `reverse`, `turnLeft`, `turnRight`, and `stop`. Include at least 3 speed changes." |
| Triangle Trek | Trace a triangle instead of a square. | `forward(ms)`, `turnRight(ms)`, `stop()` | "Write C++ that makes the robot drive a triangle. Use a loop that runs 3 times. Each side should drive forward for 1200 ms, then turn right long enough for a triangle corner." |
| Hallway Patrol | Patrol forward and back for a few rounds. | `forward(ms)`, `reverse(ms)`, `turnLeft(ms)`, `turnRight(ms)`, `stop()` | "Write C++ inside `runStudentProgram` that patrols 5 times: drive forward, turn around, drive back, turn around again, then stop at the end." |

## 🟡 Challenge challenges

| Challenge | Goal | IRobot methods likely used | Sample Copilot prompt |
|---|---|---|---|
| Smooth Obstacle Avoider | Turn more gently when an obstacle is close. | `lookCenter()`, `lookLeft()`, `lookRight()`, `scan()`, `forward(ms)`, `turnLeft(ms)`, `turnRight(ms)`, `OBSTACLE_DISTANCE_CM` | "Improve my obstacle avoidance code. Inside `runStudentProgram`, scan center, left, and right. If center is clear, drive forward. If blocked, choose the side with more distance and use a shorter turn if the side is only a little better." |
| Better Line Follower | Go faster on straight parts and slower on turns. | `readLineSensors()`, `setMotorSpeed(left, right)`, `forward(ms)`, `turnLeft(ms)`, `turnRight(ms)`, `stop()` | "Write a line-following loop. If only `s[2]` sees the line, set both motors faster and move forward. If the line is on `s[0]` or `s[1]`, slow down and turn left. If it is on `s[3]` or `s[4]`, slow down and turn right. Stop when all five sensors see the line." |
| Target Tracker | Follow an object, then search if it disappears. | `readObstacles()`, `.left`, `.right`, `forward(ms)`, `turnLeft(ms)`, `turnRight(ms)`, `stop()`, `quitRequested()` | "Write object-following C++ that uses `readObstacles()`. If both sensors see the object, move forward. If only left sees it, turn left. If only right sees it, turn right. If neither sees it, slowly wiggle left and right to search." |
| Remote Macro Button | Make one remote button start a routine. | `readRemote()`, `RemoteButton::Up`, `RemoteButton::Down`, `RemoteButton::Left`, `RemoteButton::Right`, `RemoteButton::Stop`, `forward(ms)`, `turnLeft(ms)`, `turnRight(ms)`, `stop()`, `quitRequested()` | "Write remote-control code. Arrow buttons drive normally. When `RemoteButton::Stop` is pressed, run a short dance macro, then return to remote control. Keep looping until `quitRequested()` is true." |

## 🔴 Stretch challenges

| Challenge | Goal | IRobot methods likely used | Sample Copilot prompt |
|---|---|---|---|
| Smart Maze Explorer | Remember which way looked clearer last time. | `lookCenter()`, `lookLeft()`, `lookRight()`, `scan()`, `forward(ms)`, `turnLeft(ms)`, `turnRight(ms)`, `OBSTACLE_DISTANCE_CM`, `stop()` | "Write a maze explorer inside `runStudentProgram`. It should scan center, left, and right. If center is blocked, turn toward the clearer side. Store the last chosen direction in a variable and use it as a tie-breaker when left and right are equal." |
| Gap-Safe Line Follower | Keep going briefly if the line disappears. | `readLineSensors()`, `forward(ms)`, `turnLeft(ms)`, `turnRight(ms)`, `stop()`, `quitRequested()` | "Write a line follower that remembers the last side where it saw the line. If no sensor sees the line for a few steps, turn toward the last seen side to find it again. Stop at the finish bar when all five sensors are true." |
| Synchronized Duo | Two robots perform the same routine together by timing. | `setMotorSpeed(left, right)`, `forward(ms)`, `reverse(ms)`, `turnLeft(ms)`, `turnRight(ms)`, `stop()` | "Create a timed robot routine for two robots to run at the same time. There is no communication API, so use exact movement times and simple steps. Make it start with a 2-second forward move, then mirror turns, then stop." |
| Motion Mood | Show a mood using motion only. | `setMotorSpeed(left, right)`, `forward(ms)`, `reverse(ms)`, `turnLeft(ms)`, `turnRight(ms)`, `stop()` | "Write three short routines inside `runStudentProgram`: happy, nervous, and sleepy. Use motion only. Pick one routine with a variable named `mood`. Use clear movement patterns and stop at the end." |

## Keep your prompts improving

If Copilot gives code that does not work, try a follow-up prompt:

> "This code uses a method that does not exist. Please rewrite it using only `forward(ms)`, `reverse(ms)`, `turnLeft(ms)`, `turnRight(ms)`, `setMotorSpeed(left, right)`, `stop()`, `scan()`, `lookLeft()`, `lookCenter()`, `lookRight()`, `readRemote()`, `quitRequested()`, `readObstacles()`, and `readLineSensors()`."

## Reflect

Before we wrap up, think about this:

> What felt faster in Session 2 with Copilot than in Session 1 by hand? What still needed your own brain, testing, and debugging?