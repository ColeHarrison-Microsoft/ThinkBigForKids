# mBot bring-up & test plan

Step-by-step procedure for flashing and verifying the **classic mBot** (blue mCore
board, ATmega328P) against the `MBotRobot` backend. Run these in order — each step
builds confidence before the next. Commands are run from the `src\` directory on
the Windows machine the mBot is plugged into.

> The backend is written to be **tuned, not rewritten**, during bring-up. Every
> value you might need to flip is a named constant near the top of
> [`MBotRobot.cpp`](MBotRobot.cpp); the relevant one is called out in each step.

## Bring-up status (last live session)

Verified on a physical classic mBot over USB (CH340 serial):

| Lesson | Status | Notes |
|---|---|---|
| 1 — movement | ✅ verified | Forward / reverse / turn directions all correct; the motor-sign constants are right. |
| 2 — IR remote | ✅ verified | Onboard receiver + the documented codes (▲ 0x40, ▼ 0x19, ◀ 0x07, ▶ 0x09, Setting 0x15) map correctly. |
| 5 — obstacle avoidance | ✅ verified | Ultrasonic detects on Port 3; the body-pivot "look" runs and the robot reacts when blocked. (Confirmed wheels-up; a floor run is still nice to do.) |
| 4 — line following | ✅ verified & tuned | Line sensor on Port 2 (dark = 0). On hardware it uses **smooth differential steering** (drive forward with unequal wheel speeds), not pivots, which jittered on narrow tape. Tuned values (in `examples/lesson4_line_following/solution.cpp` and the student scaffold's provided hardware path): `STEP_MS=30, FAST=230, SLOW=100` (gap 130), `SEEK=30`, `STRAIGHT=205`. Follows sharp 90° corners *and* gradual tight curves, and seeks back when it loses the line. |
| 3 — object following | ⏳ untested | Uses the already-verified ultrasonic, so it should work; tune `FOLLOW_DISTANCE_CM`. |

**Start gate:** every mBot program now waits for the **onboard button** (pin A7)
before running — see `MBotRobot::waitForStartButton()`, called from
`main_mbot.cpp`. So a freshly flashed or just-powered-on robot sits idle until you
press the button, instead of driving off the table or while you place it. (This
also makes uploads more reliable, since the idle robot isn't busy driving when the
bootloader needs to catch the next flash.)

**Tip — if an upload hangs** with `stk500_getsync()`/`not in sync`, power-cycle the
mBot (off/on) and flash again immediately.

**To finish bring-up (robot on the floor):** run `mbot_example3` and confirm it
drives toward a nearby object; tune `FOLLOW_DISTANCE_CM` if needed.

## 0. Prerequisites

- Classic mBot, charged batteries, power switch ON.
- USB cable from the mBot to this machine.
- Toolchain already verified: `pio`, the `atmelavr` platform, and the Makeblock
  library all build (see [`BUILD.md`](BUILD.md)).
- The mBot's USB serial uses a **CH340** chip. If the port doesn't appear,
  install the CH340 driver, then replug.

Detect the board and note its COM port:

```pwsh
pio device list
```

Look for the CH340 / USB-SERIAL entry (e.g. `COM5`). The upload commands below
auto-detect the port; if detection ever picks the wrong one, append
`--upload-port COMx`.

> **Safety first:** for the first movement test, stand the mBot on a book or box
> so the wheels spin freely off the ground. Confirm directions before letting it
> drive on the floor.

## 1. Lesson 1 — basic movement (validates motors + wiring)

```pwsh
pio run -e mbot_example1 -t upload
```

Expected: forward ~2 s, reverse ~2 s, rotate left ~2 s, rotate right ~2 s, stop.

| Symptom | Fix (in `MBotRobot.cpp`) |
|---|---|
| "Forward" drives backward (both wheels) | Flip BOTH `LEFT_FORWARD_SIGN` and `RIGHT_FORWARD_SIGN` |
| Robot spins instead of going straight | Flip ONE of `LEFT_FORWARD_SIGN` / `RIGHT_FORWARD_SIGN` |
| Turns are reversed (left ↔ right) | Both signs are likely swapped relative to wiring — flip both, re-test forward first |
| One wheel doesn't move | Hardware: check that motor's cable into M1/M2; confirm batteries are charged |

Re-flash after any change and confirm forward/turn look correct before moving on.

## 2. Lesson 2 — IR remote (validates onboard IR receiver)

```pwsh
pio run -e mbot_example2 -t upload
```

Point the mBot remote at the robot:

- ▲ / ▼ → forward / reverse, ◀ / ▶ → turn left / right, round **Setting** button → stop.

If a button does nothing or the codes differ from this remote, open the serial
monitor and press the button — unmapped codes are printed as `Unmapped IR code: 0xNN`:

```pwsh
pio device monitor -b 9600
```

Map any new code by editing the `IR_UP / IR_DOWN / IR_LEFT / IR_RIGHT / IR_SETTING`
constants in `MBotRobot.cpp`, then re-flash. (Close the monitor before re-uploading
— it holds the COM port.)

## 3. Lesson 5 — obstacle avoidance (validates ultrasonic + pivot-look)

```pwsh
pio run -e mbot_example5 -t upload
```

Expected: the robot drives forward; periodically it briefly **pivots** left/right
to "look" with the fixed ultrasonic, then steers toward the more open side and
away from obstacles (`#`).

| Symptom | Fix |
|---|---|
| Never detects obstacles / `scan()` always far | Confirm the ultrasonic is on **Port 3**; if it's on another port, change `PORT_3` in `MBotRobot.cpp`'s `Impl` |
| Pivoting to look is too jerky / drifts a lot | Lower `LOOK_PIVOT_MS`, or rebuild with the pivot disabled: `pio run -e mbot_example5 -t upload --build-flag "-DMBOT_PIVOT_SCAN=0"` (then it just reads straight ahead and turns left when blocked) |
| Stops too late / too early | Tune `OBSTACLE_DISTANCE_CM` in `RobotTypes.h` (shared) — note this also affects the simulator |

Check the live distance readings any time with the serial monitor (the backend
prints nothing by default during scans; distances can be added temporarily if needed).

## 4. Lesson 4 — line following (validates line sensor + 2→5 mapping)

Lay down a dark line (electrical tape) on a light floor. Place the robot on the line.

```pwsh
pio run -e mbot_example4 -t upload
```

Expected: the robot follows the line, turning back toward it when it drifts, and
stops when it fully loses the line.

| Symptom | Fix (in `MBotRobot.cpp`) |
|---|---|
| Steers AWAY from the line / immediately "loses" it on the line | Flip `LINE_ON_WHEN_BIT_SET` |
| Turns the wrong way to recover (left vs right) | Set `SWAP_LINE_SENSORS = true` |
| Line follower on a different port | Change `PORT_2` in the `Impl` constructor |

> Known limit: the mBot has only two line sensors, so the five-sensors-on-the-line
> "finish bar" never triggers — the robot just keeps following until the line is
> gone. This is expected and documented for volunteers.

## 5. Lesson 3 — object following (validates synthesized obstacle reading)

```pwsh
pio run -e mbot_example3 -t upload
```

Hold a hand or box in front of the ultrasonic within ~25 cm: the robot drives
toward it; remove it and the robot stops.

| Symptom | Fix |
|---|---|
| Follows from too far / too close | Tune `FOLLOW_DISTANCE_CM` in `MBotRobot.cpp` |
| Doesn't react | Same ultrasonic/port check as Lesson 5 |

> Known limit: with a single forward sensor the robot can only go "toward / not
> toward" — it can't steer left/right to chase. The student's left/right branches
> simply won't fire on the mBot. Expected and documented.

## 6. Flashing a student's own code

After a student fills in a lesson's `// !EDIT` blocks, flash their scaffold (not the
example):

```pwsh
pio run -e mbot_lesson2 -t upload   # or mbot_lesson1 .. mbot_lesson5
```

An unfilled scaffold compiles but mostly idles (the blanks are empty) — that's
expected until the student completes the edits.

## Tunable constants quick reference

| Constant | File | Purpose |
|---|---|---|
| `LEFT_FORWARD_SIGN`, `RIGHT_FORWARD_SIGN` | `MBotRobot.cpp` | Motor direction polarity |
| `LOOK_PIVOT_MS` | `MBotRobot.cpp` | How far the body pivots to "look" for `scan()` |
| `MBOT_PIVOT_SCAN` (build flag) | `MBotRobot.cpp` | `=0` disables pivot-look (read straight ahead only) |
| `FOLLOW_DISTANCE_CM` | `MBotRobot.cpp` | Object-following trigger distance |
| `LINE_ON_WHEN_BIT_SET`, `SWAP_LINE_SENSORS` | `MBotRobot.cpp` | Line-sensor polarity / left-right swap |
| `OBSTACLE_DISTANCE_CM` | `RobotTypes.h` | "Too close" threshold (shared with the simulator) |
| Ports `M1`/`M2`/`PORT_3`/`PORT_2` | `MBotRobot.cpp` (`Impl`) | Which mBot ports the motors/ultrasonic/line use |

## Upload troubleshooting

- **Port busy / access denied:** close any open `pio device monitor` or Arduino
  IDE serial monitor, then retry.
- **No port / upload times out:** check the USB cable, the mBot power switch, and
  the CH340 driver; try `pio device list` again.
- **Wrong board detected:** append `--upload-port COMx` from `pio device list`.
- **Nothing moves but upload succeeded:** batteries low, or the robot is still on
  its safety blocks — set it down (after directions are confirmed).
