# Event Planning — Microsoft Garage Field Trip

> **Status:** Draft. This document tracks logistics, decisions, and open questions for the November 2026 field trip. A previous attempt at this event was cancelled; the structure here is carried forward from that planning round.

## Overview

| Field | Value |
|---|---|
| Event | Microsoft Garage Field Trip |
| Target date | November 2026 (specific day TBD) |
| Audience | ~80 students from Fairfax County Public Schools (FCPS) |
| Locations | Microsoft Garage and adjacent MPRs |
| Activity theme | Robotics + intro to software engineering |

## Day-of schedule

| Time | Activity |
|---|---|
| 9:00 AM | Volunteer setup and prep |
| 9:30 AM | Students arrive; split across the two rooms (Garage and MPRs) |
| 9:45 AM | Session 1 begins |
| 11:15 AM | Session 1 wraps up; Garage group moves to MPRs |
| 11:30 AM | Lunch + panel discussion in MPRs |
| 12:00 PM | Session 2 begins |
| 1:30 PM | Session 2 wraps up; students return to buses |

## Hardware inventory

| Kit | Quantity | Notes |
|---|---|---|
| mBot kits | 21 | Pre-assembled by students in prior programs |
| DIY Arduino-based robot car kits | 11 | Larger; based on Arduino board with motor/servo/ultrasonic |
| Student Chromebooks | ~80 (1 per student) | School-managed; Linux dev environment requirement is the key open question — see Risks |

## Session goals

### Session 1 — "Comfort with the dev loop"

- Get students writing, compiling, and running C++ in a familiar editor (VS Code on Chromebook Linux, or GitHub.dev as a fallback).
- Have students invoke functions from the shared robotics interface library against the **text simulator** and against a **physical robot** (mBot or DIY car). Students rotate through who is at the wheel.
- Faster students extend their command sequences and explore additional library functions.

### Session 2 — "Vibe coding with Copilot"

- Introduce GitHub Copilot Free.
- Same interface library; students prompt Copilot to generate more advanced behaviors (choreographed routines, obstacle-avoidance variants, line-following patterns).
- Reflection at the end on the speed delta vs. Session 1.

## Coding project at a glance

The repo provides a single C++ interface (`IRobot`) with three backends:

1. **Simulator** — runs in the Chromebook terminal. No hardware required.
2. **mBot** — uses the Makeblock-Libraries.
3. **DIY Arduino car** — ports the existing `Lesson1`–`Lesson5` `.ino` sketches.

Students use the same API across all three. Within a session, they swap between the simulator and a hardware backend so each student gets steering time. See [`src/ToDo.md`](src/ToDo.md) for the active code task list.

## Pre-arrival setup

Students must complete the steps in [`docs/chromebook-setup.md`](docs/chromebook-setup.md) before arriving. The teacher-facing version is [`docs/handouts/teacher-pre-arrival-checklist.md`](docs/handouts/teacher-pre-arrival-checklist.md). Some students will inevitably arrive with steps incomplete — volunteers should plan for an on-site rescue path (likely tethered flashing from a volunteer laptop and/or the simulator-only path).

## Volunteer roles (placeholder)

To be filled in as we recruit. Roles likely needed:

- **Floor leads** (one per room) — keep the schedule, manage transitions.
- **Coding mentors** — circulate, answer questions, debug.
- **Hardware techs** — flash robots from a tethered volunteer laptop when student Chromebooks can't.
- **Logistics** — check-in, lunch, panel coordination.

> **Privacy note:** when this section is populated, list roles only — do not put volunteer names in this file. Track the volunteer roster in a separate non-public document.

## Stakeholder handouts

- **Teacher pre-arrival checklist** — [`docs/handouts/teacher-pre-arrival-checklist.md`](docs/handouts/teacher-pre-arrival-checklist.md). Hand to the teacher coordinating each visiting class.
- **FCPS IT admin review packet** — [`docs/handouts/fcps-it-admin-review.md`](docs/handouts/fcps-it-admin-review.md). Provide to FCPS IT for sign-off on the Chromebook Linux + USB sharing requirements.

## Open questions

- [ ] **FCPS IT sign-off** on enabling Crostini Linux + USB device sharing on student Chromebooks (see IT review packet).
- [ ] **Minor account creation on GitHub** — confirm parental-consent workflow and that Copilot Free is available to the resulting accounts.
- [ ] **Pre-arrival completion rate** — how many students will actually finish the setup? Need a per-classroom check-in plan.
- [ ] **Lunch logistics** — number of meals, dietary restrictions, allergies (handled at the school side, not in this repo).
- [ ] **Panel composition** — who speaks during lunch, what topics, how long.
- [ ] **Loaner laptop budget** — if FCPS IT denies Linux, can Microsoft loan ~80 laptops for the day? (See Risks.)
- [ ] **Robot-to-student ratio** — 21 + 11 = 32 robots for ~80 students; the rotation model assumes 2–3 students per robot at any moment. Confirm this works with two simultaneous rooms.
- [ ] **Network** — Garage/MPR Wi-Fi capacity for ~80 student Chromebooks pulling VS Code marketplace + GitHub.

## Risks and mitigations

| # | Risk | Severity | Mitigation |
|---|---|---|---|
| R1 | FCPS IT denies the Linux dev environment requirement | High | Submit IT review packet early. Maintain a "no-Linux" fallback path: GitHub.dev for editing, volunteer-tethered flashing, and simulator-only as the worst case. |
| R2 | School-managed ChromeOS policy disables Crostini regardless of admin sign-off | High | Same fallback as R1. Test with a sample Chromebook before committing to the path. |
| R3 | USB device sharing to Linux is blocked or unstable | Medium | Pre-flash robots before the event so on-day flashing is optional, not mandatory. |
| R4 | Students can't create GitHub accounts (age / parental consent) | Medium | Prepare a small pool of pre-created shared accounts for Session 2 demonstration if needed; document who can use them. |
| R5 | Copilot Free not available on minor accounts | Medium | Have a volunteer-driven Copilot demo as a backup for Session 2. |
| R6 | Pre-arrival setup low completion rate | Medium | Build a 15-minute setup catch-up block into the start of Session 1; have volunteers spot-fix common failures. |
| R7 | Hardware breakage on the day (robots, USB cables, Chromebook chargers) | Low–Medium | Bring spares; establish a "hand-up = hardware swap" rule so coding mentors don't get pulled into hardware debugging. |
| R8 | Wi-Fi saturation from ~80 simultaneous downloads | Medium | Pre-arrival setup is intentionally heavy on the at-home side. On-site, avoid mass extension installs. |
| R9 | Student Chromebooks have non-trivial software (VS Code + Linux) on them after the event | Low | Deliberate — students are encouraged to keep using the environment. Ongoing security posture documented in the IT admin review packet (§6, "Hardening in place"): `apt update` cadence, VS Code marketplace allowlist, USB-share revocation, existing FCPS network egress controls. |

## Decision log

| Date | Decision | Rationale |
|---|---|---|
| 2026-05-29 | Use C++ for the interface library, not C | Idiomatic for Arduino/mBot; supports a clean class-based interface for the three backends. |
| 2026-05-29 | Simulator is an ANSI terminal UI, not a web canvas | Runs everywhere with zero browser/WASM dependency; matches the C++ build path; minimal Linux setup. |
| 2026-05-29 | Hardware path is Crostini Linux + arduino-cli/PlatformIO over USB | Lets students use the same VS Code workflow for both editing and flashing, with school-IT approval. |
| 2026-05-29 | Repo is public; PII guardrails enforced via `AGENTS.md` | Lowers friction for volunteers and students to access the materials. |
| 2026-05-29 | Post-event posture is "harden in place," not teardown | Students benefit from keeping VS Code, the Linux container, and their GitHub sign-in for ongoing learning. Ongoing security guidance in `docs/handouts/fcps-it-admin-review.md` §6. |
| 2026-05-29 | Original `.ino` source files and class docs/slides committed to the repo | User confirmed PII removed from `.docx` and `.pptx`; `.ino` files were already PII-clean. Located at `src/reference/arduino-car-code-3/` and `docs/source-materials/`. |
