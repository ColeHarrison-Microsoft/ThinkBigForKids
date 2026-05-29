# Teacher Handout — Student Pre-Arrival Checklist

> **For:** Teachers preparing students for the **Microsoft Garage Field Trip**, November 2026.
> **Time required (per student):** ~45–60 minutes, ideally split across two short sessions.
> **What you need:** Each student's school Chromebook, a stable Wi-Fi connection, and (where applicable) parental consent for the GitHub account step.
> **After the event:** the VS Code / Linux environment students set up here is intended to **stay** so they can keep using it for class projects and personal learning. There is no teardown step.

The full technical reference is at [`../chromebook-setup.md`](../chromebook-setup.md). This page is the one to print or share with students.

---

## Before students start

- [ ] Confirm with FCPS IT that the **Linux development environment** and **VS Code installation** are approved for these Chromebooks. (See the [IT admin review packet](fcps-it-admin-review.md).)
- [ ] Confirm whether students need an admin password to enable the Linux environment. If yes, IT must either pre-enable it or provide the password to the supervising teacher.
- [ ] Send the parental-consent note for the GitHub account step (see Step 7).

---

## Student steps

### Step 1 — Update the Chromebook

1. Open **Settings**.
2. Click **About ChromeOS**.
3. Click **Check for updates**.
4. If an update installs, click **Restart** when prompted.

✅ When done, the same screen says **Your Chromebook is up to date**.

### Step 2 — Turn on the Linux environment

1. Open **Settings**.
2. Click **Advanced** in the left menu, then **Developers**.
3. Next to **Linux development environment**, click **Turn on**.
4. Click **Next** and **Install** when prompted. (Default settings are fine.)
5. Wait for installation. A black terminal window will open when it's done.

### Step 3 — Update Linux

In the black terminal window, type each line and press **Enter**:

```bash
sudo apt update
sudo apt upgrade -y
```

If it asks for a password, that is the **Linux** password just set up — not the Chromebook login password.

### Step 4 — Install one helper program

Still in the terminal:

```bash
sudo apt-get update
sudo apt-get install -y gnome-keyring
```

### Step 5 — Find out which VS Code to download

In the terminal:

```bash
dpkg --print-architecture
```

Write down what it says:

- `amd64` → download the **64-bit** version of VS Code in the next step.
- `arm64` → download the **ARM64** version.

### Step 6 — Download and install VS Code

1. Open Chrome and go to <https://code.visualstudio.com/download>.
2. Click the `.deb` button that matches your architecture from Step 5.
3. Open the **Files** app on the Chromebook.
4. **Move** the downloaded `.deb` file from **Downloads** into **Linux files**.
5. Back in the terminal, type (replacing the file name with the one you actually downloaded):

   ```bash
   sudo apt install ./code_*.deb
   ```

6. If you see "unmet dependencies," run:

   ```bash
   sudo apt --fix-broken install
   ```

7. If a window asks "Allow connection to Microsoft repository?" choose **Yes**.

✅ After it finishes, open the **Launcher** → **Linux apps** folder. **Visual Studio Code** (penguin icon) should appear there. You can close the terminal.

### Step 7 — Create a GitHub account *(parental consent required)*

1. Go to <https://github.com/signup>.
2. Use a school email address (or another email parents/guardians have approved).
3. Pick a username that is **not** the student's full real name.
4. Confirm the email and finish the signup.

> **Teachers / parents:** GitHub's terms generally require account-holders to be at least 13 years old and may require additional consent for younger students. Please review their current rules before having students sign up. A student who can't make an account can still fully participate in Session 1.

### Step 8 — Sign in to VS Code with GitHub

1. Open **Visual Studio Code** from the **Linux apps** folder.
2. On the **Get Started** screen, click **Use AI Features**.
3. In the **bottom-right corner**, the status bar shows **Signed Out** — click it.
4. Follow the on-screen prompts to sign in with the GitHub account from Step 7.

✅ The status bar should now show the student's GitHub username instead of "Signed Out."

This step is also what enables **GitHub Copilot Free**, which we use in the second session of the event.

---

## What to bring on the day of the event

- The student's Chromebook (fully charged).
- The Chromebook charger.
- The slip of paper with the student's GitHub username (NOT the password — they should know that themselves).

Volunteers will handle robots, USB cables, and any flashing-from-Chromebook setup on the day. Students do not need to install anything robot-specific before arriving.

---

## If a step doesn't work

| Symptom | Try this |
|---|---|
| "Linux development environment" option is missing or grayed out | Likely blocked by FCPS IT policy. Mark this student for the on-site fallback path; they can still participate. |
| Asked for a Chromebook admin password | Stop. The student can't proceed without IT help. |
| `sudo apt install ./code_*.deb` says "unmet dependencies" | Run `sudo apt --fix-broken install`, then re-run the install command. |
| VS Code icon doesn't appear in **Linux apps** | Reboot the Chromebook and check again. If still missing, restart from Step 6. |
| GitHub sign-up rejects the student's age | Skip Step 7 and 8 — the student will use the simulator-only path. |
| Wi-Fi keeps dropping during the install | Don't worry — the steps are safe to re-run. |

If a student is stuck after trying the table above, **note which step failed** and ask them to bring the Chromebook to the event in that state. Volunteers will help on the day.

---

## How many students need to finish this?

We hope every student completes Steps 1–6 (everything except GitHub) before the event. If 70% of students arrive with VS Code installed and a working Linux environment, the event runs smoothly. Students who arrive without it can still participate via the simulator running on a volunteer's machine, but they'll have a less hands-on experience.

Thank you for helping with this — it makes a huge difference.
