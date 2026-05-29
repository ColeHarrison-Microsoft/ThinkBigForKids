/*
 * Lesson 3: Object Following
 *
 * The car uses two IR obstacle sensors on the back to follow your hand!
 * Move your hand near the sensors and the car will chase it.
 *
 * NOTE: The sensors are on the BACK of the car, so "reverse" actually
 * moves TOWARD the sensors (toward your hand).
 *
 * YOUR TASKS:
 *   1. Fill in setMotorSpeed()
 *   2. Add the stopping and turning behavior in loop()
 */

/* === Pin Setup === */
#define speedPinR 9
#define RightMotorDirPin1 12
#define RightMotorDirPin2 11
#define speedPinL 6
#define LeftMotorDirPin1 7
#define LeftMotorDirPin2 8

// The two IR obstacle sensors (they detect objects nearby)
#define RightObstacleSensor 2
#define LeftObstacleSensor 3
/* === End (Pin Setup) === */


/* === Motor Control Functions === */

// !EDIT #1: Set the speed for both motors
void setMotorSpeed(int speed)
{

}

// These are flipped from Lesson 1 because the sensors face backward
void forward()
{
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, HIGH);
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, HIGH);
  setMotorSpeed(200);
}

void turnLeft()
{
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, HIGH);
  digitalWrite(LeftMotorDirPin1, HIGH);
  digitalWrite(LeftMotorDirPin2, LOW);
  setMotorSpeed(200);
}

void turnRight()
{
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2, LOW);
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, HIGH);
  setMotorSpeed(200);
}

void reverse()
{
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2, LOW);
  digitalWrite(LeftMotorDirPin1, HIGH);
  digitalWrite(LeftMotorDirPin2, LOW);
  setMotorSpeed(200);
}

void stop()
{
  setMotorSpeed(0);
}
/* === End (Motor Control Functions) === */


void setup()
{
  pinMode(RightMotorDirPin1, OUTPUT);
  pinMode(RightMotorDirPin2, OUTPUT);
  pinMode(speedPinR, OUTPUT);
  pinMode(LeftMotorDirPin1, OUTPUT);
  pinMode(LeftMotorDirPin2, OUTPUT);
  pinMode(speedPinL, OUTPUT);
  stop();

  // Sensors are inputs (reading data IN)
  pinMode(RightObstacleSensor, INPUT);
  pinMode(LeftObstacleSensor, INPUT);
}

void loop()
{
  // Read sensors — LOW means an object is detected
  int objectOnLeft = digitalRead(LeftObstacleSensor) == LOW;
  int objectOnRight = digitalRead(RightObstacleSensor) == LOW;

  if (objectOnLeft && objectOnRight)
  {
    // Both sensors see something — drive straight toward it
    reverse();
  }
  // !EDIT #2: What should happen when...
  //   - Only the left sensor sees something? (turn left)
  //   - Only the right sensor sees something? (turn right)
  //   - Neither sensor sees anything? (stop)

}
