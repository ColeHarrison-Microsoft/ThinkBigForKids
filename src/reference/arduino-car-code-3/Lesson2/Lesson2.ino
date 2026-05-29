/*
 * Lesson 2: IR Remote Control
 *
 * Use the infrared remote to drive the car!
 * Press the arrow buttons to move, and OK to stop.
 *
 * YOUR TASK: Fill in the setMotorSpeed() function.
 * Hint: Look at how it worked in Lesson 1!
 */

/* === IR Receiver Setup === */
#include <IRremote.hpp>

#define DECODE_NEC
#define IR_PIN 10  // IR receiver is connected to pin 10

IRrecv IR(IR_PIN);
decode_results IRresults;

// These codes match the buttons on the remote
#define IR_ADVANCE 0x18 // "▲" button
#define IR_BACK 0x52    // "▼" button
#define IR_RIGHT 0x5A   // ">" button
#define IR_LEFT 0x8     // "<" button
#define IR_STOP 0x1C    // "OK" button
/* === End (IR Receiver Setup) === */


/* === Pin Setup === */
#define speedPinR 9
#define RightMotorDirPin1 12
#define RightMotorDirPin2 11
#define speedPinL 6
#define LeftMotorDirPin1 7
#define LeftMotorDirPin2 8
/* === End (Pin Setup) === */


/* === Motor Control Functions === */

// !EDIT: Set the speed for both motors (look at Lesson 1 for help!)
void setMotorSpeed(int speed)
{

}

void forward()
{
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2, LOW);
  digitalWrite(LeftMotorDirPin1, HIGH);
  digitalWrite(LeftMotorDirPin2, LOW);
  setMotorSpeed(200);
}

void turnLeft()
{
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2, LOW);
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, HIGH);
  setMotorSpeed(200);
}

void turnRight()
{
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, HIGH);
  digitalWrite(LeftMotorDirPin1, HIGH);
  digitalWrite(LeftMotorDirPin2, LOW);
  setMotorSpeed(200);
}

void reverse()
{
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, HIGH);
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, HIGH);
  setMotorSpeed(200);
}

void stop()
{
  setMotorSpeed(0);
}
/* === End (Motor Control Functions) === */


String direction = "stop";
unsigned long lastCommandTime = 0;


void setup()
{
  pinMode(RightMotorDirPin1, OUTPUT);
  pinMode(RightMotorDirPin2, OUTPUT);
  pinMode(speedPinR, OUTPUT);
  pinMode(LeftMotorDirPin1, OUTPUT);
  pinMode(LeftMotorDirPin2, OUTPUT);
  pinMode(speedPinL, OUTPUT);
  stop();

  pinMode(IR_PIN, INPUT);
  digitalWrite(IR_PIN, HIGH);
  IR.enableIRIn(); // Start listening for IR signals
}

void loop()
{
  // Check if the remote sent a signal
  if (IR.decode())
  {
    lastCommandTime = millis(); // Remember when we got this command

    if (IR.decodedIRData.command == IR_ADVANCE)
    {
      direction = "forward";
    }
    else if (IR.decodedIRData.command == IR_RIGHT)
    {
      direction = "right";
    }
    else if (IR.decodedIRData.command == IR_LEFT)
    {
      direction = "left";
    }
    else if (IR.decodedIRData.command == IR_BACK)
    {
      direction = "back";
    }
    else if (IR.decodedIRData.command == IR_STOP)
    {
      direction = "stop";
    }

    IR.resume(); // Ready to receive the next signal
  }

  // If no button has been pressed for 0.2s, stop the car
  if (millis() - lastCommandTime > 200)
  {
    direction = "stop";
  }

  // Move the car in the chosen direction
  if (direction == "forward")
  {
    forward();
  }
  else if (direction == "left")
  {
    turnLeft();
  }
  else if (direction == "right")
  {
    turnRight();
  }
  else if (direction == "back")
  {
    reverse();
  }
  else
  {
    stop();
  }
}
