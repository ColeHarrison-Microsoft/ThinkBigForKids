# Code-side ToDo

This is the active work list for the **coding project** portion of the repo. Event-planning todos live in [`../Planning.md`](../Planning.md). Rules every contributor (human or AI) must follow are in [`../AGENTS.md`](../AGENTS.md).

## Goals (recap)

Build a portable C++ robotics interface library that students use during the field trip. One API, three backends:

1. **Text simulator** — ANSI-colored terminal UI on Linux. No hardware required.
2. **mBot** — uses the Makeblock-Libraries.
3. **DIY Arduino car** — ports the existing `Lesson1`–`Lesson5` `.ino` sketches.

Students should be able to switch between robots and progress through the same two sessions in the same order. Students who finish early should have material to extend with.

## Architecture sketch

```
src/
├── include/robot/
│   ├── IRobot.h          # Pure-virtual interface
│   └── RobotTypes.h      # Speed, duration, ScanResult, etc.
├── sim/
│   ├── SimRobot.{h,cpp}      # Implements IRobot against SimWorld
│   ├── SimWorld.{h,cpp}      # Grid, obstacles, ASCII renderer
│   └── main_sim.cpp          # Local entry point, sets up world + runs student program
├── mbot/
│   └── MBotRobot.{h,cpp}     # Implements IRobot using Makeblock-Libraries
├── arduino_car/
│   └── ArduinoCarRobot.{h,cpp}  # Implements IRobot using direct pin control (Lesson*.ino logic)
├── student/
│   ├── lesson1_basic_movement/
│   ├── lesson2_.../
│   └── ...
└── examples/             # Reference solutions (volunteer-facing, public OK)
```

Build system: **PlatformIO** with three envs (`sim`, `mbot`, `arduino_car`). The `sim` env builds a native Linux binary; the other two cross-compile to AVR.

## Active todos

> **Status — Lessons 1–5 simulator complete.** The `IRobot` interface (movement
> + ultrasonic + remote/object/line sensors), the text **simulator** backend
> (with a keyboard "virtual remote", a movable follow target, and interactive
> line-drawing), **all five lesson scaffolds + reference solutions**, and the
> **PlatformIO native envs** are implemented and verified (builds + runs on
> Windows via MinGW-w64; static-linked binaries). See [`BUILD.md`](BUILD.md).
> Deferred: mBot + Arduino-car AVR backends.

### IRobot interface
- [x] Define `IRobot` (pure virtual) in `include/robot/IRobot.h`. Methods modeled on the existing `.ino` lessons:
  - `void forward(int ms)`
  - `void reverse(int ms)`
  - `void turnLeft(int ms)`
  - `void turnRight(int ms)`
  - `void setMotorSpeed(int leftSpeed, int rightSpeed)`
  - `void stop()`
  - `int scan()` — returns distance in cm
  - `void lookLeft()`, `void lookCenter()`, `void lookRight()` — control servo head; no-op on robots without one
  - `RemoteButton readRemote()` — controller button (IR remote / keyboard virtual remote)
  - `bool quitRequested()` — user asked to quit (sim Q); always false on hardware
  - `ObstacleReading readObstacles()` — two side object sensors (object following)
  - `LineReading readLineSensors()` — five line-tracking sensors (line following)
- [x] Add `RobotTypes.h` with shared enums/constants (`Direction`, `DEFAULT_SPEED`, etc.).
- [x] Decide and document the C++ floor (target C++17 for sim, fall back to C++11 for AVR if needed).

### Simulator backend
- [x] Implement `SimWorld`: 2D grid, obstacle placement, robot pose tracking.
- [x] Implement `SimRobot`: implements `IRobot`; `forward(ms)` blocks for `ms` and updates pose; `scan()` raycasts in `SimWorld`.
- [x] ANSI renderer: redraw 10 Hz, robot glyphs (`^ > v <`), obstacles `#`, trail `·`, distance readout in color.
- [x] `main_sim.cpp`: load a default world, instantiate `SimRobot`, hand it to the student function `runStudentProgram(IRobot&)`.
- [x] Match Arduino timing semantics so behavior translates (e.g., `forward(2000)` blocks ~2s).

### mBot backend
- [ ] Pull in Makeblock-Libraries (https://github.com/Makeblock-official/Makeblock-Libraries) as a PlatformIO lib dep.
- [ ] Implement `MBotRobot` mapping `IRobot` methods onto `MeAuriga` / `MeUltrasonicSensor` / `MeRGBLed` calls.

### Arduino DIY car backend
- [ ] Port `Lesson1.ino` movement primitives into `ArduinoCarRobot::forward / reverse / turnLeft / turnRight / setMotorSpeed / stop` using the same pin assignments (pins 6/7/8 for L motor, 9/11/12 for R motor; servo on the established pin; ultrasonic TRIG/ECHO).
- [ ] Port the `Lesson5.ino` ultrasonic `scan()` and head-servo control.
- [ ] Mirror the timing semantics of the simulator.

### Student lesson scaffolds
- [x] Port Lesson 1 (basic movement) — students fill in nothing, just compile/run/observe (warm-up).
- [x] Port Lesson 2–4 from the source `.ino` files into student-facing scaffolds with `// !EDIT #N: ...` blanks (matching the convention already used in `Lesson5.ino`). _Lesson 2 = keyboard "virtual remote"; Lesson 3 = object following (student drives the target `O` with keys, robot follows); Lesson 4 = student draws the track interactively, then the robot follows the line. All re-themed for the simulator's sensors rather than the original IR/line hardware._
- [x] Port Lesson 5 (obstacle avoidance) as the capstone.
- [x] Provide reference solutions in `examples/`. _(All five lessons.)_
- [ ] **Strip every name / class identifier from the source `.docx` / `.pptx`** before deriving any handout from them. PII review is mandatory before commit (see `AGENTS.md`).

### Build / tooling
- [x] Add `platformio.ini` with three envs (`sim`, `mbot`, `arduino_car`). _Done for the native sim (per-program envs: `lesson1`, `lesson5`, `example_lesson1`, `example_lesson5`); `mbot`/`arduino_car` left as commented stubs._
- [x] `sim` env should build a native Linux binary that runs in the Crostini terminal. _Verified native build + run on Windows (MinGW-w64, static-linked); Crostini/Linux build expected to work with system g++ but still to be confirmed on a Chromebook._
- [x] Document the build commands in a `src/BUILD.md` (or in `README.md`) once envs are working.
- [ ] Decide on whether to add a CI build for the `sim` env on PR (low priority).

### Session 2 prep
- [ ] Curate a list of "extension challenges" suitable for Copilot-assisted coding (line-following pattern, choreography, multi-robot synchronized routines, ...).
- [ ] Add a short student handout explaining what "vibe coding with Copilot" means and how to write effective prompts (kept simple).

## Notes / decisions

- **Why C++ and not C** — Arduino/mBot toolchains are C++; using C++ lets us model `IRobot` cleanly with virtual methods. Student code stays simple (call methods on a robot reference); no template gymnastics.
- **Why an ANSI terminal sim and not a web/WASM sim** — runs natively on Crostini Linux, no browser/WASM toolchain, same C++ build path as the hardware. Matches the existing `.ino` pedagogy. Decision can be revisited if a web demo is needed for the panel.
- **Why PlatformIO** — multi-env support handles native + AVR in one config; integrates cleanly with VS Code via the PlatformIO IDE extension; same workflow students use during the event.
- **No PII in code** — even in comments. Use `<STUDENT>`, `<TEACHER>`, `<SCHOOL>` placeholders if illustrative names are needed in lesson prose.

## Out of scope (for now)

- Bluetooth / wireless control of mBot.
- Multi-robot coordination protocols.
- Cloud build (GitHub Codespaces) — kept as a fallback in `Planning.md` Risk R1, not as primary.
- Replacing the existing `.ino` lessons in their original Arduino IDE form. Those still exist outside the repo; we are deriving a new structure here.
