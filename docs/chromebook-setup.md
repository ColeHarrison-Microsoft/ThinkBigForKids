# Chromebook pre-arrival setup

> Full reference for getting a school-managed Chromebook ready to run the field-trip coding activities. The teacher-facing one-pager that distils these steps for students is in [`handouts/teacher-pre-arrival-checklist.md`](handouts/teacher-pre-arrival-checklist.md).

## What we're installing and why

| Component | Why we need it |
|---|---|
| ChromeOS Linux (Crostini) dev environment | Provides a Debian-based container in which we can install desktop Linux apps. |
| `gnome-keyring` (Linux package) | Required by VS Code for credential storage on Linux. |
| Visual Studio Code (`.deb` from Microsoft) | The editor students will use to write C++ for the robots. |
| GitHub account (one per student) | Sign-in target for VS Code; required for GitHub Copilot Free in Session 2. |
| **Day-of only:** USB device sharing to Linux | Lets students flash code from VS Code to a USB-connected mBot or Arduino car. Not needed for pre-arrival. |

> **Important for FCPS IT:** the security review for these changes lives in [`handouts/fcps-it-admin-review.md`](handouts/fcps-it-admin-review.md). Do not enable any of the below on a managed Chromebook until the IT admin has signed off.

## Step 0 — Read about Linux on Chromebooks (optional context)

<https://chromeos.dev/en/linux>

## Step 0.5 — Make sure ChromeOS is fully up to date

<https://support.google.com/chromebook/answer/177889>

Open **Settings → About ChromeOS → Check for updates**, install any pending updates, and reboot.

## Step 1 — Enable the Linux development environment

Reference: <https://chromeos.dev/en/linux/setup>

1. Open **Settings**.
2. Expand the **Advanced** menu, choose **Developers**.
3. Next to **Linux development environment**, click **Turn on** and follow the prompts.

When it finishes, a Linux terminal opens.

## Step 2 — Update the Linux container

In the Linux terminal:

```bash
sudo apt update
sudo apt upgrade -y
```

## Step 3 — Install VS Code prerequisites

Reference: <https://code.visualstudio.com/blogs/2020/12/03/chromebook-get-started#_install-vs-code>

```bash
sudo apt-get update
sudo apt-get install -y gnome-keyring
```

## Step 4 — Determine the architecture

```bash
dpkg --print-architecture
```

- If the output is `amd64`, download the **64-bit** VS Code `.deb`.
- If the output is `arm64`, download the **ARM64** VS Code `.deb`.

Download from <https://code.visualstudio.com/download>.

## Step 5 — Install VS Code

1. From the ChromeOS Files app, **move** the downloaded `.deb` into the **Linux files** folder (this makes it visible to the Linux container).
2. In the Linux terminal:

   ```bash
   sudo apt install ./<downloaded-file-name>.deb
   ```

3. If `apt` reports unmet dependencies, run the suggested fix:

   ```bash
   sudo apt --fix-broken install
   ```

4. ChromeOS may prompt you to allow a connection to the Microsoft VS Code repository / marketplace — accept it.

## Step 6 — Confirm VS Code is installed

Open the home page → **Linux apps** folder → **Visual Studio Code** (the icon is a penguin). The Linux terminal can now be closed; you won't need it again before the event.

## Step 7 — Create a GitHub account (one per student)

<https://docs.github.com/en/get-started/start-your-journey/creating-an-account-on-github>

> Account creation by minors typically requires parental consent. Teachers and parents should review GitHub's terms before having students sign up. If a student can't create an account, that student can still participate in Session 1 against the simulator without signing in.

## Step 8 — Sign VS Code into GitHub

1. Open VS Code.
2. On the **Get Started** page, click **Use AI Features**.
3. In the bottom-right corner of VS Code, the status bar shows **Signed Out** — click it.
4. Follow the prompts to sign in with the student's GitHub account.

This same flow enables **GitHub Copilot Free** for Session 2.

---

## Day-of: USB device sharing (volunteers will guide students)

Students do **not** need to do this before the event. On the day:

1. Plug the robot's USB cable into the Chromebook.
2. ChromeOS pops up a **Connect to Linux** prompt — choose the listed USB device.
   (If the prompt doesn't appear: **Settings → About ChromeOS → Linux development environment → USB preferences → toggle the device on**.)
3. The first time only, in the Linux terminal:

   ```bash
   sudo usermod -aG dialout $USER
   ```

   Log out and back in (or reboot) for the group change to take effect.

If `arduino-cli` or PlatformIO is needed for flashing, volunteers will install that on the day from a known-good script — students do not need to install it themselves.

## After the event — re-lockdown

To revert the Chromebook to its pre-event state, see the **Re-lockdown checklist** in [`handouts/fcps-it-admin-review.md`](handouts/fcps-it-admin-review.md). The short version: turning off the Linux development environment in **Settings → Developers** deletes the container and everything we installed inside it in a single step.
