/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/
 * Arduino Smart Car Tutorial Lesson 4
 * CopyRight www.osoyoo.com

 * This lesson will let robot car automatically track a black line in ground
 *
 *
 */


/*Declare L298N Dual H-Bridge Motor Controller directly since there is not a library to load.*/
//Define L298N Dual H-Bridge Motor Controller Pins
#define speedPinR 9   // RIGHT PWM pin connect MODEL-X ENA
#define RightMotorDirPin1  12    //  Right Motor direction pin 1 to MODEL-X IN1
#define RightMotorDirPin2  11    // Right Motor direction pin 2 to MODEL-X IN2
#define speedPinL 6        //  Left PWM pin connect MODEL-X ENB
#define LeftMotorDirPin1  7    // Left Motor direction pin 1 to MODEL-X IN3
#define LeftMotorDirPin2  8   ///Left Motor direction pin 1 to MODEL-X IN4

/*From left to right, connect to D3,A1-A3 ,D10*/
#define LFSensor_0 A0
#define LFSensor_1 A1
#define LFSensor_2 A2
#define LFSensor_3 A3
#define LFSensor_4 A4

#define SPEED   150 //motor in   speed
#define FAST_SPEED 180
#define MID_SPEED 130
#define SLOW_SPEED  120     //back speed

 char sensor[5];
 /*read sensor value string, 1 stands for black, 0 starnds for white, i.e 10000 means the first sensor(from left) detect black line, other 4 sensors detected white ground */
String read_sensor_values()
{
  int sensorvalue=32;
  sensor[0]=digitalRead(LFSensor_0);
  sensor[1]=digitalRead(LFSensor_1);
  sensor[2]=digitalRead(LFSensor_2);
  sensor[3]=digitalRead(LFSensor_3);
  sensor[4]=digitalRead(LFSensor_4);

  sensorvalue +=sensor[0]*16+sensor[1]*8+sensor[2]*4+sensor[3]*2+sensor[4];

  String senstr= String(sensorvalue,BIN);
  // EDIT: make the senstr variable only have the last 5 characters. The first character is always a 1, so we don't need it.
  Serial.println(senstr);
  return senstr;
}

/*motor control*/
void forward(int t=0)  //Forward
{
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2,LOW);
  digitalWrite(LeftMotorDirPin1,HIGH);
  digitalWrite(LeftMotorDirPin2,LOW);
  set_Motorspeed(SPEED, SPEED);
  delay(t);
}
void turnLeft(int t=0)  //Turn left
{
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2,LOW);
  digitalWrite(LeftMotorDirPin1,LOW);
  digitalWrite(LeftMotorDirPin2,HIGH);
  set_Motorspeed(SPEED, SPEED);
  delay(t);
}
void turnRight(int t=0)  //Turn right
{
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2,HIGH);
  digitalWrite(LeftMotorDirPin1,HIGH);
  digitalWrite(LeftMotorDirPin2,LOW);
  set_Motorspeed(SPEED, SPEED);
  delay(t);
}
void reverse(int t=0)  //Reverse
{
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2,HIGH);
  digitalWrite(LeftMotorDirPin1,LOW);
  digitalWrite(LeftMotorDirPin2,HIGH);
  set_Motorspeed(SPEED, SPEED);
  delay(t);
}
void stop()    //Stop
{
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2,LOW);
  digitalWrite(LeftMotorDirPin1,LOW);
  digitalWrite(LeftMotorDirPin2,LOW);
  set_Motorspeed(0, 0);
}

/*set motor speed */
void set_Motorspeed(int speed_L,int speed_R)
{
  /* EDIT: set the motor speed for both */
}

void setup()
{
  pinMode(speedPinL, OUTPUT); //left motor PWM pin
  pinMode(speedPinR, OUTPUT); //rignt motor PWM  pin
  pinMode(RightMotorDirPin1, OUTPUT); //left motor direction pin1
  pinMode(RightMotorDirPin2, OUTPUT); //left motor direction pin2
  pinMode(LeftMotorDirPin1, OUTPUT); //right motor direction Pin 1
  pinMode(LeftMotorDirPin2, OUTPUT); //right motor direction Pin 2

    /*line follow sensors */
  pinMode(LFSensor_0,INPUT);
  pinMode(LFSensor_1,INPUT);
  pinMode(LFSensor_2,INPUT);
  pinMode(LFSensor_3,INPUT);
  pinMode(LFSensor_4,INPUT);
  Serial.begin(9600);
}

void auto_tracking(){
 String sensorval= read_sensor_values();

  if (sensorval=="10000")
  {
    //The black line is to the left of the car, need a left turn
    turnLeft();  //Turn left
    set_Motorspeed(FAST_SPEED,FAST_SPEED);
  }
  if (sensorval==/*EDIT: Insert sensor values*/ || sensorval==/*EDIT: Insert sensor values*/ || sensorval==/*EDIT: Insert sensor values*/ || sensorval==/*EDIT: Insert sensor values*/  || sensorval==/*EDIT: Insert sensor values*/ || sensorval==/*EDIT: Insert sensor values*/)
  {
    forward();  //Turn slight left
    set_Motorspeed(0,FAST_SPEED);
  }
  if (sensorval=="00001"){
    //The black line is to the right of the car, need a right turn
    turnRight();  //Turn right
    set_Motorspeed(FAST_SPEED,FAST_SPEED);
  }
  if (sensorval=="00011" || sensorval=="00010"  || sensorval=="00101" || sensorval=="00110" || sensorval=="00111" || sensorval=="01101" || sensorval=="01111"   || sensorval=="01011"  || sensorval=="01001")
  {
    forward();  //Turn slight right
    set_Motorspeed( FAST_SPEED,0);
  }

  if (sensorval==/*EDIT: Insert the sensor reading at the finish line*/){
    stop();   //The car reached the finish line!
  }
}

void loop(){
  auto_tracking();
}
