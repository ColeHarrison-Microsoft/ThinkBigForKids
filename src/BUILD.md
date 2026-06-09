# Building & Running the Simulator

This is the verified build/run guide for the **text simulator** backend — the
no-hardware path that runs in a terminal. The mBot and Arduino-car backends are
not implemented yet (see [`ToDo.md`](ToDo.md)).

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
| `lesson5` | Lesson 5 student scaffold (obstacle avoidance) |
| `example_lesson1` | Lesson 1 reference solution |
| `example_lesson5` | Lesson 5 reference solution |

`default_envs` is `lesson1`, so a bare `pio run` builds that.

## What you'll see

A top-down ANSI view of the world: the robot (`^ > v <` showing its heading),
obstacles (`#`), the trail it has driven (`.`), a border, and a colored status
line (current action and, while scanning, the measured distance). Movement
blocks for the requested milliseconds — `forward(2000)` drives for ~2 seconds —
so behavior matches the real cars.

## Notes

- **C++ standard:** the native/sim build targets **C++17** (`-std=c++17`).
- Lessons 2–4 and the AVR backends are deferred; see [`ToDo.md`](ToDo.md).
