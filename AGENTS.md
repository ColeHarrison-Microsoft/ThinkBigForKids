# AGENTS.md

Instructions for AI coding agents (GitHub Copilot, Cursor, Claude Code, etc.) working in this repository.

This file follows the [AGENTS.md](https://agents.md/) cross-tool convention. Read it in full before making changes.

## Repository purpose

This repo supports a volunteer-led Microsoft Garage field trip for ~80 students from Fairfax County Public Schools (FCPS). It contains:

1. **Event planning** documents.
2. **A C++ robotics interface library** with three backends (text simulator, mBot, DIY Arduino car) used by students during the event.

Start with [`README.md`](README.md), [`Planning.md`](Planning.md), and [`src/ToDo.md`](src/ToDo.md) for context.

## Operating modes

When a task arrives, identify which mode applies:

- **Event-planning mode** — updates to `Planning.md`, `docs/`, or `docs/handouts/`. Treat these as stakeholder-facing documents.
- **Code-authoring mode** — updates under `src/`. Follow the active task list in [`src/ToDo.md`](src/ToDo.md).

A single PR may touch both, but the rules below apply at all times.

## 🔒 Privacy rules (non-negotiable)

This repository is **public**. The following must **never** be committed:

- Names of any **student**, **teacher**, **school staff member**, **parent**, or **volunteer**.
- Email addresses, phone numbers, badge numbers, or any other PII for the people above.
- Photos, videos, or audio recordings of minors.
- Specific school names beyond the generic level **"Fairfax County Public Schools"** / **"FCPS"**. Do not name individual schools (e.g., do not write `<SCHOOL_A>` or `<SCHOOL_B>` in the repo — use the FCPS-level reference instead).
- Internal Microsoft information that is not already public (room numbers in private floor plans, internal alias lists, non-public contact info, internal-only URLs, etc.).
- Any content from the **source materials** under `src/reference/arduino-car-code-3/` and `docs/source-materials/` should be **paraphrased rather than re-pasted** when deriving new student-facing prose. PII has been reviewed and stripped, but treating the originals as reference (not copy-paste source) keeps the privacy posture durable.

If a task seems to require committing any of the above, **stop and ask the user**. Use placeholders such as `<TEACHER>`, `<STUDENT>`, `<SCHOOL>` instead.

Before opening a PR, do a `git diff` review specifically for the items above.

## Style and conventions

### Markdown / docs
- Use ATX-style headings (`#`, `##`, ...).
- Wrap lines at a sensible length but don't hard-wrap aggressively — reflow is fine.
- Prefer tables for structured comparisons (e.g., risk registers, fallback options).
- Cross-link with relative paths (`[Planning.md](Planning.md)`), not absolute URLs.

### C++ (under `src/`)
- **Standard:** C++17 where the toolchain allows; fall back to C++11 for Arduino/mBot targets if needed. Document the floor in `src/ToDo.md`.
- **Core abstraction:** all student-facing code targets the `IRobot` interface. Backends (`SimRobot`, `MBotRobot`, `ArduinoCarRobot`) implement it.
- **Naming:** `PascalCase` for types, `camelCase` for methods and variables, `UPPER_SNAKE_CASE` for compile-time constants. Header guards or `#pragma once`.
- **Header layout:** put public interfaces under `src/include/robot/`; backend implementations under `src/<backend>/`.
- **No globals** in student-facing headers — pass an `IRobot&` into student code.
- **Match Arduino timing semantics** in the simulator (e.g., `forward(2000)` blocks for ~2 seconds) so behavior translates to hardware.
- **Comments:** explain *why*, not *what*. Existing `.ino` files in the source materials are well-commented for kids — preserve that pedagogy when porting.

### Lesson scaffolds (`src/student/`)
- Mark blanks students fill in with `// !EDIT #N: ...` matching the pattern already used in `Lesson5.ino`.
- Provide a reference solution under `src/examples/` for volunteers (also public — students can peek; that's acceptable).

## Build / validation

Build tooling is not yet established (see [`src/ToDo.md`](src/ToDo.md) — PlatformIO with three envs is the planned approach). Until it lands:

- Don't add new linters, formatters, or CI without first updating `src/ToDo.md`.
- For Markdown-only PRs, no build step is required. Spot-check rendered output by viewing files in GitHub's preview.

## Pull-request etiquette

- Branch naming: `scaffold/...`, `docs/...`, `code/...`, `planning/...`.
- Include a short PR description with the purpose, the files touched, and any follow-ups (link them to `src/ToDo.md` entries when applicable).
- Always include the trailer:

  ```
  Co-authored-by: Copilot <223556219+Copilot@users.noreply.github.com>
  ```

  unless the user explicitly asks otherwise.
- **Do not auto-merge.** Leave PRs open for the user to review.

## Source materials (in this repo)

The original Arduino sketches and class handouts that this project ports from now live **in the repo**:

- `src/reference/arduino-car-code-3/Lesson{1..5}/Lesson{N}.ino` — original Arduino sketches the new `ArduinoCarRobot` backend ports from. PII-clean (verified before commit).
- `docs/source-materials/Robotics Class Instructions.docx` — original handout. **PII removed by the repo owner** before commit.
- `docs/source-materials/Robotics Class Slides.pptx` — original slide deck. **PII removed by the repo owner** before commit.
- [Makeblock-Libraries](https://github.com/Makeblock-official/Makeblock-Libraries) for the `MBotRobot` backend (external dependency, not vendored).

When deriving new student-facing material from these (e.g., porting a lesson into Markdown or into the new C++ structure), still **paraphrase prose rather than copying it verbatim**. This keeps the privacy guardrail durable in case a future revision of the source files reintroduces names or other identifying details.

If you ever update the `.docx` or `.pptx` from a newer copy, **re-run a PII review pass before committing** — the original archive contained material that needed scrubbing.

## When in doubt

Stop and ask the user via the available clarifying-question mechanism. The cost of a clarifying question is far lower than the cost of leaking a student's name into a public repo or shipping a broken Chromebook setup to FCPS IT.
