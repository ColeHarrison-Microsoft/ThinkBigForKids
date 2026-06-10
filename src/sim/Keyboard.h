#pragma once

namespace robot {

// A single logical key from the "virtual remote". Arrow keys and WASD both map
// to the directional values so either works.
enum class Key {
    None,
    Up,
    Down,
    Left,
    Right,
    Space,
    Enter,
    Quit,   // user pressed Q
    Eof,    // end of piped/redirected input (no more keys)
    Other
};

/*
 * Keyboard — non-blocking keyboard input for the simulator.
 *
 * Two input paths so the same code works for a person at a terminal *and* for
 * automated tests that pipe a sequence of keys into stdin:
 *   - Interactive TTY: poll() returns the pressed key, or Key::None when
 *     nothing is waiting (so the animation keeps running).
 *   - Piped / redirected stdin: poll() consumes one byte per call and returns
 *     Key::Quit at end-of-input.
 *
 * On POSIX the terminal is put into raw (no-echo, non-canonical) mode for the
 * lifetime of the object and restored in the destructor.
 */
class Keyboard {
public:
    Keyboard();
    ~Keyboard();

    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;

    // Return the next key, or Key::None if nothing is available right now.
    Key poll();

private:
    bool interactive_ = true;
#if !defined(_WIN32)
    bool rawApplied_ = false;
#endif
};

}  // namespace robot
