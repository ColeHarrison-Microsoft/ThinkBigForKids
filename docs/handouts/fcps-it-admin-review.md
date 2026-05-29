# FCPS IT Admin Review Packet

> **For:** Fairfax County Public Schools IT administrators reviewing and approving (or denying) the Chromebook configuration changes required for student participation in the **Microsoft Garage Field Trip**, November 2026.
> **Submitted by:** Microsoft volunteer organizers (contact info provided separately, not in this public document).
> **Reading time:** ~10 minutes.

This document defaults to **deny** — please grant only the minimum capabilities needed, and refer to the alternatives in §7 if any item cannot be approved. The post-event posture is **harden in place**, not teardown — see §6.

## 1. Purpose and scope

A group of approximately 80 FCPS students will visit the Microsoft Garage for a one-day robotics-themed coding event. To participate, students need to write, build, and run C++ code on their school-issued Chromebooks against:

1. a text-based simulator (no hardware), and
2. a USB-connected hobbyist robot kit (mBot or Arduino-based DIY car) provided by the volunteers on the day.

The most realistic path on a Chromebook for both is the **ChromeOS Linux development environment ("Crostini")** with **Visual Studio Code** installed inside it. This document describes exactly what that requires, the risks involved, mitigations, and post-event cleanup.

The change is intended to **remain in place after the event** so students can continue using VS Code, the Linux container, and their GitHub sign-in for ongoing learning. §6 ("Hardening in place") defines the ongoing security posture; full teardown is reserved for the specific cases noted at the end of §6.

## 2. Exact ChromeOS settings being changed

| Setting | Where | Change | When |
|---|---|---|---|
| Linux development environment | Settings → Advanced → Developers | **Turn on** (creates a Debian Crostini container) | Pre-arrival |
| `dialout` group membership for the Linux user | Inside the Linux container | Add the student's Linux user to `dialout` | Day-of, only if flashing locally |
| USB device sharing to Linux | Settings → About ChromeOS → Linux dev environment → USB preferences | Toggle on, **only for the specific robot's USB-serial device while plugged in** | Day-of, transient |

No ChromeOS Developer Mode is requested. No policy bypass is requested. No Crostini "advanced" features (port forwarding, SSH server, GUI sharing beyond the default) are needed.

## 3. Software being installed inside the Linux container

| Package | Source | Why |
|---|---|---|
| Default Crostini Debian base | Google-managed | Provided automatically when Linux is enabled. |
| `gnome-keyring` | Debian repos (`apt`) | VS Code uses it for secret storage on Linux. |
| Visual Studio Code (`code` `.deb`) | <https://code.visualstudio.com/download> (Microsoft, signed) | Editor for student work. |
| **Optional, day-of:** `arduino-cli` and/or PlatformIO Core | Official upstream installer scripts | Compile and flash code to the USB-connected robots. May be installed by volunteers from a checked-in script the day of the event rather than by students. |

No browser plug-ins, no kernel modules, no ChromeOS-side software changes are requested.

## 4. Network and data flow

What the configured Chromebook will reach over the network:

| Destination | Purpose | Frequency |
|---|---|---|
| `update.googleapis.com` and ChromeOS update servers | Normal ChromeOS updates | Routine |
| Debian package mirrors used by Crostini | `apt` package installs | One-time during setup |
| `code.visualstudio.com` | VS Code `.deb` download and update checks | Setup + occasional |
| `marketplace.visualstudio.com` and CDN (`*.vscode-cdn.net`) | VS Code extensions | If extensions are installed |
| `github.com`, `api.github.com`, `*.githubusercontent.com` | Repo cloning, sign-in, Copilot | Throughout the event |
| `copilot.github.com`, `api.githubcopilot.com` | GitHub Copilot Free traffic for Session 2 | Session 2 only |

No outbound traffic to non-Microsoft / non-GitHub / non-Debian destinations is required by this configuration.

## 5. Risk register

Severity legend: **L** = low, **M** = medium, **H** = high.

| # | Risk | Severity | Mitigation |
|---|---|:---:|---|
| **R1** | **Crostini sandbox isolation is not zero-trust.** A vulnerability in Crostini, the kernel, or `vmlinux` could in theory let code in the container affect ChromeOS. | M | Keep ChromeOS fully updated before enabling Linux. Crostini's design provides VM-level isolation; treat it as defense-in-depth, not airtight. Disable Linux (§6) immediately after the event to remove the attack surface. |
| **R2** | **Sideloaded `.deb` from outside Debian repos** (the VS Code `.deb`) is a supply-chain risk. | M | Restrict the source URL to `code.visualstudio.com`. Microsoft-signed package; teachers can verify the publisher in the install prompt. Optionally pin the version installed and document its SHA256 in advance. |
| **R3** | **VS Code Marketplace extensions** can execute arbitrary code with the user's permissions inside the container. | M | Provide a small allowlist of pre-approved extensions for the event (likely just the C/C++ extension and PlatformIO IDE). Disable VS Code's "auto-install recommended extensions" prompt. Consider a managed `argv.json` or extension-deny policy via VS Code settings sync. |
| **R4** | **USB device sharing** could be left enabled after the event, exposing future USB peripherals to the Linux container. | L | The toggle is per-device and per-session by default. The re-lockdown step (§6) explicitly revokes any leftover entries. |
| **R5** | **Minors creating GitHub accounts** raises COPPA / parental-consent concerns. | M | Teachers handle parental-consent collection before the event. Students who can't create an account use the simulator-only path. Account names are chosen to avoid full real names (this guidance is in the teacher handout). |
| **R6** | **GitHub Copilot for minors** — terms-of-service compliance for the Free tier on student accounts. | M | Volunteers will verify Copilot Free terms eligibility before the event and switch to a volunteer-driven Copilot demo for Session 2 if individual student access is blocked. |
| **R7** | **Privilege escalation via `dialout` and `sudo` in the container.** Any student-level escalation is confined inside the Crostini VM and does not grant ChromeOS-host privilege. | L | Document that `sudo` rights are local to the VM. The container is destroyed at the end of the event. |
| **R8** | **Data exfiltration channel.** Linux container has full outbound network access by default through the same Wi-Fi the Chromebook uses. | M | Existing FCPS network egress controls (DNS filtering, proxy, firewall) still apply to Crostini traffic. We will publish the destination list (§4) so it can be allowlisted explicitly if FCPS prefers. |
| **R9** | **Persistence of installed software / data after the event.** | L | Deliberate — students keep VS Code, the Linux container, and their GitHub sign-in to continue learning. Ongoing posture is defined in §6 (Hardening in place): apt update cadence, marketplace extension allowlist, USB-share revocation, and existing FCPS network controls. |
| **R10** | **Local credential storage** (GitHub OAuth tokens) inside `gnome-keyring` could be left behind. | L | Tokens stay only as long as the student remains signed in. Students can sign out from VS Code at any time; admins can also revoke active OAuth grants from the GitHub side. If the Linux environment is ever fully removed (see §6 "When to actually remove the environment"), `gnome-keyring` and its store go with it. |
| **R11** | **Hardware (USB) introducing malware.** A robot's USB-serial chip is presented to the Linux container as a `/dev/ttyUSBN` device only. | L | Robots are provided by event volunteers, not student-supplied. We can document the USB VID/PID list of the kits in advance. |
| **R12** | **Network saturation / mass downloads.** ~80 Chromebooks pulling VS Code marketplace traffic concurrently could hit caps. | L–M | Pre-arrival setup is intentionally **at home / at school in advance**, not on event day. On-site, no large downloads are scheduled. |

## 6. Hardening in place (post-event)

The Linux development environment, VS Code, and the students' GitHub sign-in are intended to **stay in place after the event** so students can keep learning, contributing, and exploring on their school Chromebooks. There is no teardown step.

What we ask FCPS IT (and participating teachers) to maintain afterward, in priority order:

1. **Keep ChromeOS up to date.** Standard FCPS update policy; Crostini security fixes ride along with it. No event-specific change required.
2. **Keep the Linux container patched.** Periodic (suggested monthly) `sudo apt update && sudo apt upgrade -y` inside the Linux terminal. Teachers can run this with the class as a 5-minute exercise.
3. **Pin VS Code to the official update channel.** VS Code self-updates from `code.visualstudio.com`. No marketplace-side configuration is needed; just leave the auto-update prompt on.
4. **Lock down VS Code Marketplace extensions to an allowlist.** The event uses a small set (e.g., the C/C++ extension, PlatformIO IDE, GitHub Copilot). Ongoing FCPS policy can:
   - Provide an explicit allowlist via VS Code's extension allow/deny settings (`extensions.allowed` / `extensions.blocked` in user settings, or via a managed configuration), or
   - Disable the "auto-install recommended extensions" prompt and educate teachers to refer extension requests back to IT for approval.
5. **Revoke event-day-only USB sharing entries.** Settings → About ChromeOS → Linux dev environment → USB preferences. Any robot USB-serial device that was toggled on for the event should be toggled off the day after the event. The `dialout` group membership inside the container is harmless to leave in place; it only matters when a USB device is actively shared.
6. **Apply existing FCPS network egress controls to Crostini traffic.** Crostini uses the same Wi-Fi as the host, so the school's existing DNS filtering, proxy, and firewall rules apply. The destination list in §4 is what students will reach — no other destinations are required for ongoing use.
7. **Optional — rotate the Linux container password periodically.** Setting it during enable is a one-time student step; rotating it on a school cadence is purely defense-in-depth.
8. **Optional — keep the Linux user as a non-root day-to-day account.** Crostini's default user is in `sudo`, which is fine for this use. If FCPS prefers, IT can create a non-sudo user and reserve `sudo` for periodic apt updates only. Most schools find this unnecessary because the container is isolated from the host regardless.

### Linux password vs. ChromeOS host password

Some teachers prefer to set the **Linux container password the same as the student's Chromebook host login** so students don't have a second password to remember. This is a usability/security trade-off:

| Approach | Pros | Cons |
|---|---|---|
| Same password as host | Easy for students; less help-desk overhead | Reuses a credential across boundaries; if either credential is compromised, the other is at risk |
| Distinct, written-on-handout password | Clear separation; matches typical multi-credential hygiene | Students will lose the password and need a teacher reset |
| Distinct, student-chosen password | Reasonable middle ground | Some students will pick something weak |

We do not recommend a specific choice — it's an FCPS policy call. If you have no preference, "distinct, student-chosen password" is the most common middle ground.

### When to actually remove the environment

The only times we'd suggest fully disabling the Linux environment (Settings → Developers → Linux development environment → Turn off) are:

- The student is leaving the school district / turning in their Chromebook.
- A specific incident has compromised the container.
- FCPS policy changes and Linux is no longer permitted on student devices.

In any of those cases, turning off the Linux environment deletes the container, VS Code, all extensions, and stored credentials in a single step.

## 7. Alternatives if any item cannot be approved

| Option | Fidelity to original plan | What it requires from FCPS IT | What it requires from organizers |
|---|---|---|---|
| **A. As proposed above** (Crostini + VS Code on student Chromebooks) | Highest | Approve Linux dev environment + USB sharing on day-of | Provide setup docs, day-of support |
| **B. Microsoft loaner laptops for the day** | High | Allow students to use non-FCPS laptops for the event only | Source ~80 pre-imaged laptops; logistics for distribution and collection |
| **C. Web-only path: GitHub.dev / Codespaces Free + tethered flashing** | Medium-High | Allow github.com / codespaces from Chromebooks (likely already permitted). No Linux change needed. | Provide a tethered flashing station per group of students, run by volunteers |
| **D. Simulator-only event with a volunteer-run robot demo** | Medium-Low | Nothing beyond standard Chromebook web access | Adapt session 1 to be entirely simulator-based; replace the hardware portion with a guided demo |

We're asking for **Option A** because it gives the best learning experience, but Options C and D both produce a successful event with effectively zero new IT exposure if needed.

---

## 8. Sign-off

Please indicate the items below that are approved.

- [ ] **Linux development environment** may be enabled on participating student Chromebooks for the period leading up to and including the event.
- [ ] **`gnome-keyring`** may be installed inside the Crostini container.
- [ ] **Visual Studio Code** (the official `.deb` from `code.visualstudio.com`) may be installed inside the Crostini container.
- [ ] **VS Code Marketplace** access from inside the container is permitted (with an allowlist of extensions provided by the organizers, if requested).
- [ ] **Day-of USB device sharing** to the Linux container for event-provided robot USB-serial devices is permitted.
- [ ] **GitHub account use** by participating students is permitted (parental consent handled separately).
- [ ] **GitHub Copilot Free** use is permitted on those accounts during the event.
- [ ] The **hardening-in-place guidance (§6)** will be adopted as the ongoing post-event posture (rather than tearing the environment down).

Approver name (FCPS IT): _________________________________________

Title: _________________________________________

Signature: _________________________________________   Date: ___________

If denying any item above, please indicate which alternative (B, C, D from §7) FCPS would prefer the organizers plan around.

---

*Questions or follow-up requests: please contact the volunteer organizer named in the cover email accompanying this packet. We are happy to adjust scope based on FCPS feedback.*
