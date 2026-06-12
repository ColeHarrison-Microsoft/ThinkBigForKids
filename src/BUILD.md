# Building & Running the Simulator

This is the verified build/run guide. The **text simulator** backend is the
no-hardware path that runs in a terminal; the **mBot** and **Arduino-car**
backends cross-compile to the robots (see
[Hardware backends](#hardware-backends-mbot--arduino-car) below).

## What you need

The simulator builds with **PlatformIO** (native env) on top of a **GCC**
toolchain. On the project's Windows setup the verified toolchain is:

| Tool | Version used | How it was installed |
|---|---|---|
| Python | 3.12.x | `winget install -e --id Python.Python.3.12` |
| PlatformIO Core | 6.1.x | `pip install platformio` |
| GCC (MinGW-w64) | 16.x (UCRT) | `winget install -e --id BrechtSanders.WinLibs.POSIX.UCRT` |

After installing, make sure `python`, `pio`, and `g++` are on your `PATH`
(open a new terminal so PATH changes take effect). Verify:

```pwsh
python --version
pio --version
g++ --version
```

On Linux/macOS, install Python + PlatformIO and a system `g++`/`clang`; the
`native` env uses the system compiler.

## Project layout

All build files live under `src/`, so run `pio` from the `src/` directory:

```
src/
├── platformio.ini          # native "sim" envs (one per runnable program)
├── include/robot/          # IRobot.h, RobotTypes.h (the student-facing API)
├── sim/                    # simulator backend + main_sim.cpp (the entry point)
├── student/<lesson>/       # student scaffolds (fill in the // !EDIT blocks)
├── examples/<lesson>/      # reference solutions (volunteer-facing)
└── scripts/static_link.py  # links the runtimes statically (portable binary)
```

Each runnable program is its own PlatformIO env that compiles the simulator
core (`sim/`) plus exactly one program file that defines `runStudentProgram()`.

## Build & run

From `src/`:

```pwsh
# Build a student scaffold
pio run -e lesson1
pio run -e lesson5

# Build and run (the -t exec target launches the binary)
pio run -e lesson1 -t exec

# Build / run the reference solutions
pio run -e example_lesson1 -t exec
pio run -e example_lesson5 -t exec

# Build everything at once
pio run
```

The compiled binary is written to
`.pio/build/<env>/program.exe` (`program` on Linux/macOS) and can also be run
directly. Binaries are linked statically, so they run without the GCC runtime
DLLs on `PATH`.

## Available envs

| Env | Program |
|---|---|
| `lesson1` | Lesson 1 student scaffold (basic movement) |
| `lesson2` | Lesson 2 student scaffold (virtual remote) |
| `lesson3` | Lesson 3 student scaffold (object following) |
| `lesson4` | Lesson 4 student scaffold (line following) |
| `lesson5` | Lesson 5 student scaffold (obstacle avoidance) |
| `example_lesson1` … `example_lesson5` | matching reference solutions |

`default_envs` is `lesson1`, so a bare `pio run` builds that.

## What you'll see

A top-down ANSI view of the world: the robot (`^ > v <` showing its heading),
obstacles (`#`), the line/track (`=`), the follow target (`O`), the draw pen
(`+`), the trail it has driven (`.`), a border, and a colored status line
(current action and, while scanning, the measured distance). Movement blocks for
the requested milliseconds — `forward(2000)` drives for ~2 seconds — so behavior
matches the real cars.

## Keyboard controls (interactive lessons)

Lessons 2–4 read a keyboard "virtual remote". Use the **arrow keys** or
**W / A / S / D**:

| Lesson | What the keys do |
|---|---|
| 2 — Virtual Remote | Drive the robot directly; **Q** quits |
| 3 — Object Following | Move the target `O`; the robot follows it; **Q** quits |
| 4 — Line Following | Move the pen `+` to **draw the track**, **Enter** to finish, then the robot follows the line you drew |

For automated runs you can also pipe keys into the program, e.g.
`echo wwddq | ./program` (end of input quits the interactive lessons).

## Hardware backends: mBot & Arduino car

The same lesson code also runs on the two robots. These envs cross-compile to AVR
(ATmega328P) with the Arduino framework. The **first** hardware build auto-installs
the `atmelavr` platform and clones the per-robot libraries (Makeblock for the mBot;
IRremote + Servo for the Arduino car), so it needs network access and takes a little
longer; later builds are fast.

Build (compile-check) from `src/`:

```pwsh
pio run -e mbot_example1         # mBot, Lesson 1 reference solution
pio run -e arduino_car_example5  # Arduino car, Lesson 5 reference solution
```

Flash to a connected robot over USB with the `upload` target:

```pwsh
pio run -e mbot_example1 -t upload
pio run -e mbot_lesson5  -t upload   # a student's filled-in Lesson 5 scaffold
```

| Env prefix | Robot | Program |
|---|---|---|
| `mbot_lesson1` … `mbot_lesson5` | classic mBot (mCore) | student scaffolds |
| `mbot_example1` … `mbot_example5` | classic mBot (mCore) | reference solutions |
| `arduino_car_lesson1` … `arduino_car_lesson5` | DIY Arduino car | student scaffolds |
| `arduino_car_example1` … `arduino_car_example5` | DIY Arduino car | reference solutions |

- **mBot bring-up:** follow the step-by-step [`mbot/TEST_PLAN.md`](mbot/TEST_PLAN.md).
- **Per-robot differences** (the mBot's fixed ultrasonic, 2 line sensors, and lack
  of side object sensors; the Arduino car's servo-pin / sensor-polarity
  `MUST-VERIFY` notes) are documented at the top of `mbot/MBotRobot.cpp` and
  `arduino_car/ArduinoCarRobot.cpp`.
- Both backends honor the same `IRobot` timing contract as the simulator, so a
  lesson that works in the sim behaves the same way on a robot.

## Notes

- **C++ standard:** the native/sim build targets **C++17** (`-std=c++17`).
- **Hardware (AVR) backends** use the Arduino framework; the shared headers stay
  AVR-safe (no `<string>`/STL on AVR — guarded with `#if !defined(ARDUINO)`).
