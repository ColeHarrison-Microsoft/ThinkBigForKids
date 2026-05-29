/*
 * Lesson 5: Obstacle Avoidance
 *
 * The car uses an ultrasonic sensor on a servo to "look around"
 * and avoid obstacles! It checks left, center, and right, then
 * decides which way to turn.
 *
 * YOUR TASKS:
 *   1. Fill in setMotorSpeed() — this time it takes TWO speeds (left and right)
 *   2. Fill in the forward() function
 *   3. Figure out obstacle detection conditions in loop()
 *   4. Add the "turn hard right" action
 */

#include <Servo.h>

/* === Pin Setup === */
#define speedPinR 9
#define RightMotorDirPin1 12
#define RightMotorDirPin2 11
#define speedPinL 6
#define LeftMotorDirPin1 7
#define LeftMotorDirPin2 8

#define SERVO_PIN 9   // Servo motor (turns the ultrasonic sensor)
#define ECHO_PIN  2   // Ultrasonic sensor echo pin
#define TRIG_PIN  10  // Ultrasonic sensor trigger pin

Servo head; // The servo that turns the sensor left and right
/* === End (Pin Setup) === */


/* === Speed Settings === */
#define FAST_SPEED  250
#define SPEED       120
#define TURN_SPEED  200

const int DISTANCE_LIMIT = 30;      // How close (in cm) before we consider it an obstacle
const int SIDE_DISTANCE_LIMIT = 30;
const float ECHO_TO_CM = 0.01657;    // Converts echo time to centimeters

const int TURN_TIME = 250; // How long to turn (milliseconds)
const int BACK_TIME = 300; // How long to back up (milliseconds)


/* === Motor Control Functions === */

// !EDIT #1: Set speed for left and right motors separately
// Unlike previous lessons, this takes TWO speeds so the car can curve!
void setMotorSpeed(int speedL, int speedR)
{

}

// !EDIT #2: Make the car drive forward (look at Lesson 1 for help!)
void forward()
{

}

void turnLeft()
{
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2, LOW);
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, HIGH);
  setMotorSpeed(200, 200);
}

void turnRight()
{
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, HIGH);
  digitalWrite(LeftMotorDirPin1, HIGH);
  digitalWrite(LeftMotorDirPin2, LOW);
  setMotorSpeed(200, 200);
}

void reverse()
{
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, HIGH);
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, HIGH);
  setMotorSpeed(200, 200);
}

void stop()
{
  setMotorSpeed(0, 0);
}
/* === End (Motor Control Functions) === */


/* === Ultrasonic Sensor === */

// Sends a sound pulse and measures how far away an object is (in cm)
int scan()
{
  long echo_distance, echo_time;

  // Send a quick pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(15);
  digitalWrite(TRIG_PIN, LOW);

  // Measure how long the echo takes to come back
  echo_time = pulseIn(ECHO_PIN, HIGH);

  // Convert to centimeters
  echo_distance = echo_time * ECHO_TO_CM;

  return round(echo_distance);
}


/*
 * Look in 5 directions and build a string showing where obstacles are.
 *
 * The string has 5 characters, one for each direction:
 *   Position:  Left | Left-Diagonal | Center | Right-Diagonal | Right
 *   Example:   "10010" means obstacles on the left AND right-diagonal
 *   "00000" = no obstacles anywhere, "11111" = surrounded!
 */
int leftScanVal, centerScanVal, rightScanVal, lDiagonalScanVal, rDiagonalScanVal;

String scanSurroundings()
{
  int obstacleStatus = B100000; // Start with bit 6 set (ensures we always get 5 digits)

  // Check center
  centerScanVal = scan();
  if (centerScanVal < DISTANCE_LIMIT) {
    stop();
    obstacleStatus |= B100;
  }

  // Turn head left-diagonal and check
  head.write(120);
  delay(100);
  lDiagonalScanVal = scan();
  if (lDiagonalScanVal < DISTANCE_LIMIT) {
    stop();
    obstacleStatus |= B1000;
  }

  // Turn head fully left and check
  head.write(180);
  delay(300);
  leftScanVal = scan();
  if (leftScanVal < SIDE_DISTANCE_LIMIT) {
    stop();
    obstacleStatus |= B10000;
  }

  // Re-check center
  head.write(90);
  delay(100);
  centerScanVal = scan();
  if (centerScanVal < DISTANCE_LIMIT) {
    stop();
    obstacleStatus |= B100;
  }

  // Turn head right-diagonal and check
  head.write(40);
  delay(100);
  rDiagonalScanVal = scan();
  if (rDiagonalScanVal < DISTANCE_LIMIT) {
    stop();
    obstacleStatus |= B10;
  }

  // Turn head fully right and check
  head.write(0);
  delay(100);
  rightScanVal = scan();
  if (rightScanVal < SIDE_DISTANCE_LIMIT) {
    stop();
    obstacleStatus |= B1;
  }

  // Return head to center
  head.write(90);
  delay(300);

  // Convert to a 5-character string like "01010"
  String obstacleStr = String(obstacleStatus, BIN);
  return obstacleStr.substring(1, 6);
}
/* === End (Ultrasonic Sensor) === */


int numCycles = 0;
int objectAhead = 0;
bool go = true;


void setup()
{
  pinMode(RightMotorDirPin1, OUTPUT);
  pinMode(RightMotorDirPin2, OUTPUT);
  pinMode(speedPinR, OUTPUT);
  pinMode(LeftMotorDirPin1, OUTPUT);
  pinMode(LeftMotorDirPin2, OUTPUT);
  pinMode(speedPinL, OUTPUT);
  stop();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  head.attach(SERVO_PIN);
  head.write(90); // Point sensor straight ahead
  delay(2000);    // Wait for servo to get in position
}

void loop()
{
  if (!go) return; // Car has given up — stay stopped

  numCycles += 1;

  // Every 20 loops, stop and look around
  if (numCycles >= 20)
  {
    stop();
    String obstacleString = scanSurroundings();

    if (obstacleString == "10000") {
      // Something only on the far left — steer slightly right
      setMotorSpeed(FAST_SPEED, SPEED);
      forward();
      delay(TURN_TIME);
      stop();

    } else if (obstacleString == "00001") {
      // Something only on the far right — steer slightly left
      setMotorSpeed(SPEED, FAST_SPEED);
      forward();
      delay(TURN_TIME);
      stop();

    } else if (/* !EDIT #3: Obstacle on the left side — when should the car turn right?
                  Think about which patterns have a "1" on the left or center.
                  There should be 7 patterns here, separated by || (which means "or").
                  Hint: "01000", "00100", "01100", ... */) {
      turnRight();
      setMotorSpeed(TURN_SPEED, TURN_SPEED);
      delay(TURN_TIME);
      stop();

    } else if (/* !EDIT #4: Obstacle on the right side — when should the car turn left?
                  There should be 6 patterns here.
                  Hint: "00010", "00011", ... */) {
      turnLeft();
      setMotorSpeed(TURN_SPEED, TURN_SPEED);
      delay(TURN_TIME);
      stop();

    } else if (obstacleString == "01111" || obstacleString == "10111" || obstacleString == "11111") {
      // Almost surrounded — hard left turn
      turnLeft();
      setMotorSpeed(FAST_SPEED, SPEED);
      delay(BACK_TIME);
      stop();

    } else if (obstacleString == "11011" || obstacleString == "11101" || obstacleString == "11110" || obstacleString == "01110") {
      // !EDIT #5: Almost surrounded from the other side — do a hard right turn
      // (Look at the hard left turn above for help!)

    }

    numCycles = 0;
  }
  else
  {
    // No obstacles detected recently — keep driving forward
    setMotorSpeed(SPEED, SPEED);
    forward();
    delay(BACK_TIME);
    stop();
  }

  // Quick forward check while driving
  int distance = scan();

  if (distance < DISTANCE_LIMIT) {
    // Something ahead — try turning away
    turnRight();
    setMotorSpeed(SPEED, FAST_SPEED);
    delay(BACK_TIME * 3 / 2);
    objectAhead += 1;

    // If we keep hitting something 25 times in a row, give up and stop
    if (objectAhead > 25) {
      stop();
      go = false;
    }
  }
  else {
    objectAhead = 0; // Reset the counter
  }
}
