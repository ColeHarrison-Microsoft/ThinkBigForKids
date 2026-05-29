/*
 * Lesson 1: Basic Movement
 * 
 * The car will go forward, backward, left, and right — then stop.
 * Each movement lasts 2 seconds.
 * 
 * All code is provided — just upload and watch it go!
 */

/* === Pin Setup === */
// These numbers match the wires connecting the Arduino to the motors
#define speedPinR 9
#define RightMotorDirPin1 12
#define RightMotorDirPin2 11
#define speedPinL 6
#define LeftMotorDirPin1 7
#define LeftMotorDirPin2 8
/* === End (Pin Setup) === */


/* === Motor Control Functions === */

// Sets how fast both motors spin (0 = stopped, 255 = full speed)
void setMotorSpeed(int speed)
{
  analogWrite(speedPinL, speed);
  analogWrite(speedPinR, speed);
}

void stop()
{
  setMotorSpeed(0);
}

// Drive forward for t milliseconds (1000ms = 1 second)
void forward(int t)
{
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2, LOW);
  digitalWrite(LeftMotorDirPin1, HIGH);
  digitalWrite(LeftMotorDirPin2, LOW);
  setMotorSpeed(200);
  delay(t);
  stop();
}

// Drive backward for t milliseconds
void reverse(int t)
{
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, HIGH);
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, HIGH);
  setMotorSpeed(200);
  delay(t);
  stop();
}

// Turn left for t milliseconds
void turnLeft(int t)
{
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2, LOW);
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, HIGH);
  setMotorSpeed(200);
  delay(t);
  stop();
}

// Turn right for t milliseconds
void turnRight(int t)
{
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, HIGH);
  digitalWrite(LeftMotorDirPin1, HIGH);
  digitalWrite(LeftMotorDirPin2, LOW);
  setMotorSpeed(200);
  delay(t);
  stop();
}
/* === End (Motor Control Functions) === */


// setup() runs once when the Arduino turns on
void setup()
{
  pinMode(RightMotorDirPin1, OUTPUT);
  pinMode(RightMotorDirPin2, OUTPUT);
  pinMode(speedPinR, OUTPUT);
  pinMode(LeftMotorDirPin1, OUTPUT);
  pinMode(LeftMotorDirPin2, OUTPUT);
  pinMode(speedPinL, OUTPUT);

  forward(2000);   // Drive forward for 2 seconds
  reverse(2000);   // Drive backward for 2 seconds
  turnLeft(2000);  // Turn left for 2 seconds
  turnRight(2000); // Turn right for 2 seconds
}

// loop() runs over and over after setup
// we leave it empty here because we only want to run the movements once
void loop() {}
