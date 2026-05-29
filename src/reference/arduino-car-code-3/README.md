# `arduino-car-code-3/` — Reference Arduino Sketches

These five `.ino` files are the **original Arduino sketches** from a prior robotics program that used the same DIY Arduino car kits we'll use during the field trip. They are kept here, unmodified, so anyone porting them into the new C++ interface library (`src/arduino_car/`, see [`src/ToDo.md`](../../ToDo.md)) can compare against the original line by line.

## What's here

| File | What it teaches |
|---|---|
| [`Lesson1/Lesson1.ino`](Lesson1/Lesson1.ino) | Basic movement primitives (`forward`, `reverse`, `turnLeft`, `turnRight`, `setMotorSpeed`, `stop`) — fully provided. |
| [`Lesson2/Lesson2.ino`](Lesson2/Lesson2.ino) | Builds on Lesson 1; introduces sequences. |
| [`Lesson3/Lesson3.ino`](Lesson3/Lesson3.ino) | Continues progression. |
| [`Lesson4/Lesson4.ino`](Lesson4/Lesson4.ino) | Continues progression. |
| [`Lesson5/Lesson5.ino`](Lesson5/Lesson5.ino) | Capstone — obstacle avoidance with ultrasonic + servo. Uses `// !EDIT #N: ...` markers for student fill-ins; that convention is being carried forward into the new lesson scaffolds. |

## Privacy

These files were reviewed before being committed; they contain no student, teacher, or school identifiers. See [`AGENTS.md`](../../../AGENTS.md) for the repo-wide privacy rules.

## How they're used

The original sketches are **reference inputs**. They are not directly compiled by the new build system. The new `ArduinoCarRobot` backend (under `src/arduino_car/`) ports the same pin assignments and behaviors into the `IRobot` interface, and the new student lesson scaffolds (under `src/student/`) re-derive the per-lesson exercises from these files.

Do not edit these `.ino` files. If you find a bug in the original, fix it in the ported code and add a note in [`src/ToDo.md`](../../ToDo.md).
