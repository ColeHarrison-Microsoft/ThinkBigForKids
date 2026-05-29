# ThinkBigForKids

A volunteer-driven coding project supporting a **Microsoft Garage Field Trip** for ~80 students from Fairfax County Public Schools (FCPS), planned for **November 2026**.

The repository serves two purposes:

1. **Event planning** — schedule, logistics, pre-arrival setup, stakeholder handouts, and risk tracking. Start with [`Planning.md`](Planning.md).
2. **Coding project** — a portable C++ robotics interface library with three backends so students can write the same code once and run it against:
   - a **text-based simulator** (no hardware required, runs in a terminal),
   - **mBot** robots (using the Makeblock Arduino libraries),
   - a **DIY Arduino car** kit.

   Code lives under [`src/`](src/) — the active task list is in [`src/ToDo.md`](src/ToDo.md).

## Audience

- **Volunteers** organizing or running the field trip.
- **Teachers** preparing students before the event — see the [teacher pre-arrival checklist](docs/handouts/teacher-pre-arrival-checklist.md).
- **FCPS IT administrators** evaluating the Chromebook configuration changes — see the [IT admin review packet](docs/handouts/fcps-it-admin-review.md).
- **Students** (during and after the event) following along with the lesson scaffolds in `src/student/`.

## Quick links

| Topic | File |
|---|---|
| Event plan, schedule, open questions | [`Planning.md`](Planning.md) |
| Rules for AI agents working in this repo | [`AGENTS.md`](AGENTS.md) |
| Chromebook pre-arrival setup (full reference) | [`docs/chromebook-setup.md`](docs/chromebook-setup.md) |
| Teacher handout (student steps) | [`docs/handouts/teacher-pre-arrival-checklist.md`](docs/handouts/teacher-pre-arrival-checklist.md) |
| FCPS IT admin review packet | [`docs/handouts/fcps-it-admin-review.md`](docs/handouts/fcps-it-admin-review.md) |
| Original class docs / slide deck (reference) | [`docs/source-materials/`](docs/source-materials/) |
| Original Arduino sketches (reference) | [`src/reference/arduino-car-code-3/`](src/reference/arduino-car-code-3/) |
| Code-side TODO | [`src/ToDo.md`](src/ToDo.md) |

## Privacy

This repository is **public**. No names of students, teachers, school staff, or volunteers are committed here, and school references stay at the "FCPS" granularity. See [`AGENTS.md`](AGENTS.md) for the full guardrails any contributor (human or AI) must follow.

## License

MIT — see [`LICENSE`](LICENSE).
