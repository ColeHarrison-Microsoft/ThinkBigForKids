#include "Keyboard.h"

#include <cctype>
#include <cstdio>

#if defined(_WIN32)
#include <conio.h>
#include <io.h>
#define ISATTY_STDIN() (_isatty(_fileno(stdin)) != 0)
#else
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#define ISATTY_STDIN() (isatty(STDIN_FILENO) != 0)
#endif

namespace robot {

namespace {

// Map a plain ASCII byte (WASD / space / enter / q) to a Key.
Key mapAscii(int c) {
    switch (c) {
        case 'w': case 'W': return Key::Up;
        case 's': case 'S': return Key::Down;
        case 'a': case 'A': return Key::Left;
        case 'd': case 'D': return Key::Right;
        case ' ':           return Key::Space;
        case '\r': case '\n': return Key::Enter;
        case 'q': case 'Q': return Key::Quit;
        default:            return Key::Other;
    }
}

}  // namespace

Keyboard::Keyboard() {
    interactive_ = ISATTY_STDIN();
#if !defined(_WIN32)
    if (interactive_) {
        termios t{};
        if (tcgetattr(STDIN_FILENO, &t) == 0) {
            termios raw = t;
            raw.c_lflag &= ~(ICANON | ECHO);
            raw.c_cc[VMIN] = 0;   // non-blocking
            raw.c_cc[VTIME] = 0;
            if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == 0) {
                rawApplied_ = true;
            }
        }
    }
#endif
}

Keyboard::~Keyboard() {
#if !defined(_WIN32)
    if (rawApplied_) {
        termios t{};
        if (tcgetattr(STDIN_FILENO, &t) == 0) {
            t.c_lflag |= (ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &t);
        }
    }
#endif
}

#if defined(_WIN32)

Key Keyboard::poll() {
    if (interactive_) {
        if (!_kbhit()) return Key::None;
        int c = _getch();
        if (c == 0 || c == 0xE0) {  // extended key (arrows): a second byte follows
            int c2 = _getch();
            switch (c2) {
                case 72: return Key::Up;
                case 80: return Key::Down;
                case 75: return Key::Left;
                case 77: return Key::Right;
                default: return Key::Other;
            }
        }
        return mapAscii(c);
    }
    // Piped stdin: consume one byte; EOF -> Eof.
    int c = std::fgetc(stdin);
    if (c == EOF) return Key::Eof;
    return mapAscii(c);
}

#else

Key Keyboard::poll() {
    unsigned char c = 0;
    ssize_t n = read(STDIN_FILENO, &c, 1);
    if (n <= 0) {
        // No data right now (interactive) or end-of-input (piped).
        return interactive_ ? Key::None : Key::Eof;
    }
    if (c == 27) {  // ESC: possible arrow-key sequence "ESC [ A/B/C/D"
        unsigned char seq[2] = {0, 0};
        if (read(STDIN_FILENO, &seq[0], 1) == 1 &&
            read(STDIN_FILENO, &seq[1], 1) == 1 && seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return Key::Up;
                case 'B': return Key::Down;
                case 'C': return Key::Right;
                case 'D': return Key::Left;
                default:  return Key::Other;
            }
        }
        return Key::Other;
    }
    return mapAscii(c);
}

#endif

}  // namespace robot
