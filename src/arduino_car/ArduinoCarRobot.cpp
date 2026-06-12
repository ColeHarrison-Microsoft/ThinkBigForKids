#include "ArduinoCarRobot.h"

#include <Arduino.h>
#include <Servo.h>

#define DECODE_NEC
#include <IRremote.hpp>

namespace robot {

ArduinoCarRobot::ArduinoCarRobot()
    : leftSpeed_(DEFAULT_SPEED),
      rightSpeed_(DEFAULT_SPEED),
      lastButton_(RemoteButton::None),
      lastRemoteCommandMs_(0),
      irInitialized_(false),
      ultrasonicInitialized_(false),
      servoInitialized_(false),
      obstacleSensorsInitialized_(false),
      lineSensorsInitialized_(false) {}

void ArduinoCarRobot::begin() {
    pinMode(SPEED_PIN_L, OUTPUT);
    pinMode(LEFT_MOTOR_DIR_PIN_1, OUTPUT);
    pinMode(LEFT_MOTOR_DIR_PIN_2, OUTPUT);
    pinMode(SPEED_PIN_R, OUTPUT);
    pinMode(RIGHT_MOTOR_DIR_PIN_1, OUTPUT);
    pinMode(RIGHT_MOTOR_DIR_PIN_2, OUTPUT);

    // The original lessons reuse pins 2 and 10 for different sensors. Each
    // subsystem is initialized lazily so a single-lesson firmware only claims
    // the pins that lesson physically wires.
    stop();
}

void ArduinoCarRobot::forward(int ms) {
    digitalWrite(RIGHT_MOTOR_DIR_PIN_1, HIGH);
    digitalWrite(RIGHT_MOTOR_DIR_PIN_2, LOW);
    digitalWrite(LEFT_MOTOR_DIR_PIN_1, HIGH);
    digitalWrite(LEFT_MOTOR_DIR_PIN_2, LOW);
    driveFor(ms);
}

void ArduinoCarRobot::reverse(int ms) {
    digitalWrite(RIGHT_MOTOR_DIR_PIN_1, LOW);
    digitalWrite(RIGHT_MOTOR_DIR_PIN_2, HIGH);
    digitalWrite(LEFT_MOTOR_DIR_PIN_1, LOW);
    digitalWrite(LEFT_MOTOR_DIR_PIN_2, HIGH);
    driveFor(ms);
}

void ArduinoCarRobot::turnLeft(int ms) {
    digitalWrite(RIGHT_MOTOR_DIR_PIN_1, HIGH);
    digitalWrite(RIGHT_MOTOR_DIR_PIN_2, LOW);
    digitalWrite(LEFT_MOTOR_DIR_PIN_1, LOW);
    digitalWrite(LEFT_MOTOR_DIR_PIN_2, HIGH);
    driveFor(ms);
}

void ArduinoCarRobot::turnRight(int ms) {
    digitalWrite(RIGHT_MOTOR_DIR_PIN_1, LOW);
    digitalWrite(RIGHT_MOTOR_DIR_PIN_2, HIGH);
    digitalWrite(LEFT_MOTOR_DIR_PIN_1, HIGH);
    digitalWrite(LEFT_MOTOR_DIR_PIN_2, LOW);
    driveFor(ms);
}

void ArduinoCarRobot::setMotorSpeed(int leftSpeed, int rightSpeed) {
    leftSpeed_ = clampSpeed(leftSpeed);
    rightSpeed_ = clampSpeed(rightSpeed);
    applyStoredSpeeds();
}

void ArduinoCarRobot::stop() {
    digitalWrite(RIGHT_MOTOR_DIR_PIN_1, LOW);
    digitalWrite(RIGHT_MOTOR_DIR_PIN_2, LOW);
    digitalWrite(LEFT_MOTOR_DIR_PIN_1, LOW);
    digitalWrite(LEFT_MOTOR_DIR_PIN_2, LOW);
    analogWrite(SPEED_PIN_L, 0);
    analogWrite(SPEED_PIN_R, 0);
}

int ArduinoCarRobot::scan() {
    initializeServo();
    initializeUltrasonic();

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(15);
    digitalWrite(TRIG_PIN, LOW);

    unsigned long echo = pulseIn(ECHO_PIN, HIGH, ULTRASONIC_TIMEOUT_US);
    if (echo == 0) {
        return MAX_SCAN_DISTANCE_CM;
    }

    float cm = echo * ECHO_TO_CM;
    int roundedCm = static_cast<int>(cm + 0.5f);
    if (roundedCm < 0) {
        return 0;
    }
    if (roundedCm > MAX_SCAN_DISTANCE_CM) {
        return MAX_SCAN_DISTANCE_CM;
    }
    return roundedCm;
}

void ArduinoCarRobot::lookLeft() {
    writeHead(HEAD_LEFT_DEGREES);
}

void ArduinoCarRobot::lookCenter() {
    writeHead(HEAD_CENTER_DEGREES);
}

void ArduinoCarRobot::lookRight() {
    writeHead(HEAD_RIGHT_DEGREES);
}

RemoteButton ArduinoCarRobot::readRemote() {
    initializeRemote();

    if (IrReceiver.decode()) {
        lastRemoteCommandMs_ = millis();
        switch (IrReceiver.decodedIRData.command) {
            case 0x18: lastButton_ = RemoteButton::Up; break;
            case 0x52: lastButton_ = RemoteButton::Down; break;
            case 0x08: lastButton_ = RemoteButton::Left; break;
            case 0x5A: lastButton_ = RemoteButton::Right; break;
            case 0x1C: lastButton_ = RemoteButton::Stop; break;
            default: break;
        }
        IrReceiver.resume();
    }

    if (millis() - lastRemoteCommandMs_ > IR_BUTTON_HOLD_MS) {
        lastButton_ = RemoteButton::None;
    }

    return lastButton_;
}

bool ArduinoCarRobot::quitRequested() {
    return false;
}

ObstacleReading ArduinoCarRobot::readObstacles() {
    initializeObstacleSensors();

    ObstacleReading reading;
    reading.left = (digitalRead(LEFT_OBSTACLE_SENSOR_PIN) == LOW);
    reading.right = (digitalRead(RIGHT_OBSTACLE_SENSOR_PIN) == LOW);
    return reading;
}

LineReading ArduinoCarRobot::readLineSensors() {
    initializeLineSensors();

    LineReading reading;
    // MUST-VERIFY: The reference treats HIGH as black line. The actual sensor
    // array polarity may need inverting if black/white readings are reversed.
    reading.s[0] = (digitalRead(LINE_SENSOR_0_PIN) == HIGH);
    reading.s[1] = (digitalRead(LINE_SENSOR_1_PIN) == HIGH);
    reading.s[2] = (digitalRead(LINE_SENSOR_2_PIN) == HIGH);
    reading.s[3] = (digitalRead(LINE_SENSOR_3_PIN) == HIGH);
    reading.s[4] = (digitalRead(LINE_SENSOR_4_PIN) == HIGH);
    return reading;
}

int ArduinoCarRobot::clampSpeed(int speed) {
    if (speed < MIN_SPEED) {
        return MIN_SPEED;
    }
    if (speed > MAX_SPEED) {
        return MAX_SPEED;
    }
    return speed;
}

void ArduinoCarRobot::applyStoredSpeeds() {
    analogWrite(SPEED_PIN_L, leftSpeed_);
    analogWrite(SPEED_PIN_R, rightSpeed_);
}

void ArduinoCarRobot::driveFor(int ms) {
    applyStoredSpeeds();
    if (ms > 0) {
        delay(static_cast<unsigned long>(ms));
    }
    stop();
}

void ArduinoCarRobot::initializeRemote() {
    if (irInitialized_) {
        return;
    }

    pinMode(IR_PIN, INPUT);
    digitalWrite(IR_PIN, HIGH);
    IrReceiver.begin(IR_PIN, false);
    irInitialized_ = true;
}

void ArduinoCarRobot::initializeUltrasonic() {
    if (ultrasonicInitialized_) {
        return;
    }

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    digitalWrite(TRIG_PIN, LOW);
    ultrasonicInitialized_ = true;
}

void ArduinoCarRobot::initializeServo() {
    if (servoInitialized_) {
        return;
    }

    // MUST-VERIFY: On Arduino Uno, Servo uses Timer1 and disables analogWrite()
    // PWM on pins 9 and 10 while a servo is attached. The right motor speed pin
    // is pin 9, so Lesson 5 speed control needs real-hardware verification.
    head_.attach(SERVO_PIN);
    head_.write(HEAD_CENTER_DEGREES);
    delay(SERVO_SETTLE_MS);
    servoInitialized_ = true;
}

void ArduinoCarRobot::initializeObstacleSensors() {
    if (obstacleSensorsInitialized_) {
        return;
    }

    pinMode(LEFT_OBSTACLE_SENSOR_PIN, INPUT);
    pinMode(RIGHT_OBSTACLE_SENSOR_PIN, INPUT);
    obstacleSensorsInitialized_ = true;
}

void ArduinoCarRobot::initializeLineSensors() {
    if (lineSensorsInitialized_) {
        return;
    }

    pinMode(LINE_SENSOR_0_PIN, INPUT);
    pinMode(LINE_SENSOR_1_PIN, INPUT);
    pinMode(LINE_SENSOR_2_PIN, INPUT);
    pinMode(LINE_SENSOR_3_PIN, INPUT);
    pinMode(LINE_SENSOR_4_PIN, INPUT);
    lineSensorsInitialized_ = true;
}

void ArduinoCarRobot::writeHead(int degrees) {
    initializeServo();
    head_.write(degrees);
    delay(SERVO_SETTLE_MS);
}

}  // namespace robot
