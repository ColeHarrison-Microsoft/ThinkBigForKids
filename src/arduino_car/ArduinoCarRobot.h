#pragma once

#include <Arduino.h>
#include <Servo.h>

#include "robot/IRobot.h"

namespace robot {

class ArduinoCarRobot : public IRobot {
public:
    ArduinoCarRobot();

    void begin();

    void forward(int ms) override;
    void reverse(int ms) override;
    void turnLeft(int ms) override;
    void turnRight(int ms) override;

    void setMotorSpeed(int leftSpeed, int rightSpeed) override;
    void stop() override;

    int scan() override;
    void lookLeft() override;
    void lookCenter() override;
    void lookRight() override;

    RemoteButton readRemote() override;
    bool quitRequested() override;
    ObstacleReading readObstacles() override;
    LineReading readLineSensors() override;

private:
    static constexpr int SPEED_PIN_L = 6;
    static constexpr int LEFT_MOTOR_DIR_PIN_1 = 7;
    static constexpr int LEFT_MOTOR_DIR_PIN_2 = 8;
    static constexpr int SPEED_PIN_R = 9;
    static constexpr int RIGHT_MOTOR_DIR_PIN_1 = 12;
    static constexpr int RIGHT_MOTOR_DIR_PIN_2 = 11;

    static constexpr int IR_PIN = 10;
    static constexpr int LEFT_OBSTACLE_SENSOR_PIN = 3;
    static constexpr int RIGHT_OBSTACLE_SENSOR_PIN = 2;

    static constexpr int LINE_SENSOR_0_PIN = A0;
    static constexpr int LINE_SENSOR_1_PIN = A1;
    static constexpr int LINE_SENSOR_2_PIN = A2;
    static constexpr int LINE_SENSOR_3_PIN = A3;
    static constexpr int LINE_SENSOR_4_PIN = A4;

    static constexpr int TRIG_PIN = 10;
    static constexpr int ECHO_PIN = 2;
    // MUST-VERIFY: Lesson5 lists SERVO_PIN as 9, but pin 9 is the right-motor
    // PWM pin. Pin 5 is free in the reference wiring and avoids that direct
    // conflict, but the real car harness must be checked before bring-up.
    static constexpr int SERVO_PIN = 5;

    static constexpr int HEAD_LEFT_DEGREES = 180;
    static constexpr int HEAD_CENTER_DEGREES = 90;
    static constexpr int HEAD_RIGHT_DEGREES = 0;
    static constexpr int SERVO_SETTLE_MS = 200;
    static constexpr unsigned long IR_BUTTON_HOLD_MS = 200;
    static constexpr unsigned long ULTRASONIC_TIMEOUT_US = 25000UL;
    static constexpr float ECHO_TO_CM = 0.01657f;

    int leftSpeed_;
    int rightSpeed_;
    RemoteButton lastButton_;
    unsigned long lastRemoteCommandMs_;

    bool irInitialized_;
    bool ultrasonicInitialized_;
    bool servoInitialized_;
    bool obstacleSensorsInitialized_;
    bool lineSensorsInitialized_;

    Servo head_;

    static int clampSpeed(int speed);

    void applyStoredSpeeds();
    void driveFor(int ms);
    void initializeRemote();
    void initializeUltrasonic();
    void initializeServo();
    void initializeObstacleSensors();
    void initializeLineSensors();
    void writeHead(int degrees);
};

}  // namespace robot
