#include "MBotRobot.h"

#include <Arduino.h>
#include <MeMCore.h>

namespace robot {

namespace {
// --- Bring-up constants (safe to tune on the physical robot) ---

// mBot motors are mounted mirror-image, so "forward" needs opposite signs on
// the two motors. These are the usual classic-mBot values; if the robot drives
// backward or spins instead of going straight, flip one or both during bring-up
// (see the mBot test plan). MUST-VERIFY on hardware.
constexpr int LEFT_FORWARD_SIGN = -1;
constexpr int RIGHT_FORWARD_SIGN = +1;

// How long to pivot the body when "looking" left/right for scan(). A partial
// turn is enough to sample a different direction; tune for your floor.
constexpr int LOOK_PIVOT_MS = 200;

// Object-following: treat something within this distance (cm) as "the object to
// follow is right in front of me".
constexpr int FOLLOW_DISTANCE_CM = 25;

// Line sensors: calibrated on the actual taped course — a sensor's bit reads 0
// over the dark line and 1 over the light floor (the diagnostic showed P2=0 when
// centered on the line and P2=3 fully off it). So "on the line" = bit 0.
constexpr bool LINE_ON_WHEN_BIT_SET = false;  // "over the line" when the bit == 0
constexpr bool SWAP_LINE_SENSORS = false;     // set true if left/right are mirrored

// mBot IR remote command bytes (from the Makeblock MeInfraredReceiver header).
// getCode() returns the NEC command byte, so these compare directly.
constexpr unsigned char IR_UP = 0x40;
constexpr unsigned char IR_DOWN = 0x19;
constexpr unsigned char IR_LEFT = 0x07;
constexpr unsigned char IR_RIGHT = 0x09;
constexpr unsigned char IR_SETTING = 0x15;  // round "Setting" button -> Stop

// The mBot's onboard button is wired to analog pin A7: analogRead reads near 0
// when pressed and ~1023 when released (matches the Makeblock factory firmware,
// which treats < ~10 as pressed). We use a generous threshold for reliability.
constexpr int ONBOARD_BUTTON_PIN = A7;
constexpr int BUTTON_PRESSED_BELOW = 100;
}  // namespace

// All Makeblock hardware objects live here so <MeMCore.h> is included in exactly
// one translation unit (it defines the global mePort[] table). Ports reflect the
// classic mBot factory wiring: ultrasonic on Port 3, line follower on Port 2,
// motors on M1 (left) / M2 (right). MUST-VERIFY if your kit differs.
struct MBotRobot::Impl {
    MeDCMotor leftMotor;
    MeDCMotor rightMotor;
    MeUltrasonicSensor ultrasonic;
    MeLineFollower lineFinder;
    MeIR ir;

    Impl()
        : leftMotor(M1),
          rightMotor(M2),
          ultrasonic(PORT_3),
          lineFinder(PORT_2) {}
};

MBotRobot::MBotRobot()
    : impl_(nullptr),
      leftSpeed_(DEFAULT_SPEED),
      rightSpeed_(DEFAULT_SPEED),
      look_(LookDirection::Center) {}

MBotRobot::~MBotRobot() { delete impl_; }

void MBotRobot::begin() {
    // Allocate the hardware objects here (not at global-construction time) so
    // the Arduino runtime is fully initialized first.
    if (impl_ == nullptr) impl_ = new Impl();
    Serial.begin(9600);  // handy for printing distances / IR codes during bring-up
    impl_->ir.begin();   // start listening for IR remote codes
    stop();
}

void MBotRobot::waitForStartButton() {
    pinMode(ONBOARD_BUTTON_PIN, INPUT);
    stop();  // make sure the motors are off while we wait
    Serial.println(F("Ready. Press the onboard button to start."));

    // Wait for a press...
    while (analogRead(ONBOARD_BUTTON_PIN) > BUTTON_PRESSED_BELOW) {
        delay(10);
    }
    // ...then for the release, so the press doesn't bleed into the program.
    while (analogRead(ONBOARD_BUTTON_PIN) <= BUTTON_PRESSED_BELOW) {
        delay(10);
    }
    delay(250);  // brief settle / debounce before the program drives
}

// --- Movement -------------------------------------------------------------

void MBotRobot::applyWheels(int leftDir, int rightDir) {
    impl_->leftMotor.run(LEFT_FORWARD_SIGN * leftDir * leftSpeed_);
    impl_->rightMotor.run(RIGHT_FORWARD_SIGN * rightDir * rightSpeed_);
}

void MBotRobot::pivot(int leftDir, int rightDir, int ms) {
    applyWheels(leftDir, rightDir);
    if (ms > 0) delay(ms);
    stop();
}

void MBotRobot::forward(int ms) {
    applyWheels(+1, +1);
    if (ms > 0) delay(ms);
    stop();
}

void MBotRobot::reverse(int ms) {
    applyWheels(-1, -1);
    if (ms > 0) delay(ms);
    stop();
}

void MBotRobot::turnLeft(int ms) {
    applyWheels(-1, +1);  // left wheel back, right wheel forward -> rotate left
    if (ms > 0) delay(ms);
    stop();
}

void MBotRobot::turnRight(int ms) {
    applyWheels(+1, -1);  // left wheel forward, right wheel back -> rotate right
    if (ms > 0) delay(ms);
    stop();
}

void MBotRobot::setMotorSpeed(int leftSpeed, int rightSpeed) {
    if (leftSpeed < MIN_SPEED) leftSpeed = MIN_SPEED;
    if (leftSpeed > MAX_SPEED) leftSpeed = MAX_SPEED;
    if (rightSpeed < MIN_SPEED) rightSpeed = MIN_SPEED;
    if (rightSpeed > MAX_SPEED) rightSpeed = MAX_SPEED;
    leftSpeed_ = leftSpeed;
    rightSpeed_ = rightSpeed;
}

void MBotRobot::stop() {
    impl_->leftMotor.stop();
    impl_->rightMotor.stop();
}

// --- Ultrasonic + "look" --------------------------------------------------

int MBotRobot::readUltrasonicCm() {
    double cm = impl_->ultrasonic.distanceCm();
    if (cm <= 0.0 || cm > MAX_SCAN_DISTANCE_CM) return MAX_SCAN_DISTANCE_CM;
    return static_cast<int>(cm + 0.5);
}

// Pivot the body to aim the fixed ultrasonic so look-left/right are meaningful.
// Build with -DMBOT_PIVOT_SCAN=0 to read straight ahead only instead.
#ifndef MBOT_PIVOT_SCAN
#define MBOT_PIVOT_SCAN 1
#endif

int MBotRobot::scan() {
#if MBOT_PIVOT_SCAN
    if (look_ == LookDirection::Left) {
        pivot(-1, +1, LOOK_PIVOT_MS);   // aim left
        int cm = readUltrasonicCm();
        pivot(+1, -1, LOOK_PIVOT_MS);   // restore heading
        return cm;
    }
    if (look_ == LookDirection::Right) {
        pivot(+1, -1, LOOK_PIVOT_MS);   // aim right
        int cm = readUltrasonicCm();
        pivot(-1, +1, LOOK_PIVOT_MS);   // restore heading
        return cm;
    }
#endif
    return readUltrasonicCm();
}

void MBotRobot::lookLeft() { look_ = LookDirection::Left; }
void MBotRobot::lookCenter() { look_ = LookDirection::Center; }
void MBotRobot::lookRight() { look_ = LookDirection::Right; }

// --- Controller / extra sensors -------------------------------------------

RemoteButton MBotRobot::readRemote() {
    // getCode() decodes the onboard IR receiver and returns the NEC command
    // byte (0 when nothing is pressed / after the button is released).
    unsigned char code = impl_->ir.getCode();
    switch (code) {
        case IR_UP:      return RemoteButton::Up;
        case IR_DOWN:    return RemoteButton::Down;
        case IR_LEFT:    return RemoteButton::Left;
        case IR_RIGHT:   return RemoteButton::Right;
        case IR_SETTING: return RemoteButton::Stop;
        default:
            // Help bring-up: surface any unmapped remote code so it can be
            // added above (a different remote may use different codes).
            if (code != 0) {
                Serial.print(F("Unmapped IR code: 0x"));
                Serial.println(code, HEX);
            }
            return RemoteButton::None;
    }
}

bool MBotRobot::quitRequested() {
    // Hardware runs until powered off; there is no "quit" key.
    return false;
}

ObstacleReading MBotRobot::readObstacles() {
    // The mBot has no side object sensors, so synthesize a "straight ahead"
    // reading from the single forward ultrasonic: an object within range looks
    // centered (both sides), so the student's follow code drives toward it. It
    // cannot tell left from right with one sensor.
    ObstacleReading r;
    int cm = readUltrasonicCm();
    if (cm <= FOLLOW_DISTANCE_CM) {
        r.left = true;
        r.right = true;
    }
    return r;
}

void MBotRobot::decodeLine(uint8_t raw, bool& leftOnLine, bool& rightOnLine) const {
    // MeLineFollower::readSensors() returns a 2-bit value (S1<<1 | S2).
    bool bitS1 = (raw & 0x02) != 0;
    bool bitS2 = (raw & 0x01) != 0;
    bool s1On = (bitS1 == LINE_ON_WHEN_BIT_SET);
    bool s2On = (bitS2 == LINE_ON_WHEN_BIT_SET);
    if (SWAP_LINE_SENSORS) {
        bool tmp = s1On;
        s1On = s2On;
        s2On = tmp;
    }
    leftOnLine = s1On;
    rightOnLine = s2On;
}

LineReading MBotRobot::readLineSensors() {
    bool leftOnLine = false, rightOnLine = false;
    decodeLine(static_cast<uint8_t>(impl_->lineFinder.readSensors()), leftOnLine, rightOnLine);

    // Map the two physical sensors into the five-sensor reading the lessons use.
    // The five-on-the-line "finish bar" pattern cannot occur with two sensors.
    LineReading r;
    if (leftOnLine && rightOnLine) {
        r.s[2] = true;  // centered on the line -> drive forward
    } else if (leftOnLine) {
        r.s[0] = true;  // line drifted to the left -> turn left
        r.s[1] = true;
    } else if (rightOnLine) {
        r.s[3] = true;  // line drifted to the right -> turn right
        r.s[4] = true;
    }
    // both off -> all false -> student code treats it as "lost the line".
    return r;
}

}  // namespace robot
