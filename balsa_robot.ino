#include <Servo.h>

enum Speeds {
  fullForward =  90,
  fullBackwards = -90,
  turnSpeed = 10
} speeds;

enum Speeds forward = fullForward;

Servo rightServo;  // create servo object to control a servo
Servo leftServo;

int rightPos = 0;  // variable to store the servo position
int leftLeft = 0;
const int BASE = 90; //starting rotation for servos
boolean movementStarted = false;

int forwardTrigPin = A1;    //Trig - green Jumper
int forwardEchoPin = A0;    //Echo - yellow Jumper

int rightEchoPin = A2;
int rightTrigPin = A3;

int leftEchoPin = A5;
int leftTrigPin = A4;

void setup() {
  pinMode(forwardTrigPin, OUTPUT);
  pinMode(forwardEchoPin, INPUT);

  pinMode(rightTrigPin, OUTPUT);
  pinMode(rightEchoPin, INPUT);

  pinMode(leftTrigPin, OUTPUT);
  pinMode(leftEchoPin, INPUT);

  rightServo.attach(2);
  leftServo.attach(3);// attaches the servo on pins
  Serial.begin(9600);
}

void logStr(char* msg) {
  Serial.println(msg);
}

void log(int value) {
  Serial.println(value);
}

// Servos use rotational degrees between 0 and 180 degrees:
void runLeftServo(int rotation) {
  // left servo is installed 'upside down', so increment to base
  int correctRotation = BASE + rotation;
  leftServo.write(correctRotation);
}

void runRightServo(int rotation) {
  // decrement from base
  int correctRotation = BASE - rotation;
  rightServo.write(correctRotation);
}

void goForward() {
  enum Speeds s = fullForward;
  runRightServo((int) s);
  runLeftServo((int) s);
}

void turnRight() {
  enum Speeds turn = turnSpeed;
  logStr("KAANTYY OIKEALLE");
  log( (int) turn);
  runLeftServo(turn);

  enum Speeds forward = fullForward;
  //  Serial.print("right speed: ");
  //log( (int) forward);
  runRightServo(-120);
}

void turnLeft() {
  runRightServo(110);
  runLeftServo(-20);
}


unsigned long previousMillis = 0;
const long interval = 500;

void loop() {


  int leftDist = getDistance(leftEchoPin, leftTrigPin);
  //logStr("left: ");
  //log(leftDist);
  int rightDist = getDistance(rightEchoPin, rightTrigPin);
  //logStr("Right: ");
  //log(rightDist);

  int forwardDist = getDistance(forwardEchoPin, forwardTrigPin);
  //logStr("forward" );
  //log(forwardDist);

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    logStr("Left distance");
    log((int)leftDist);
    logStr("Right distance");
    log((int)rightDist);
    if (forwardDist <= 25) {
      logStr("Turning");
      if ((leftDist < rightDist) && (leftDist > 4) && (rightDist > 4)) {
        logStr("Turning right");
        turnRight();
      }
      if (leftDist < 10 ) {
        log((int)leftDist);
        logStr("Turning right");
        turnRight();
      }
      if (rightDist < 5 ) {
        log((int)rightDist);
        logStr("Turning left");
        turnLeft();
      }

    } else if(leftDist <5) {
        log((int)leftDist);
        logStr("Turning right");
        turnRight();
    } else if(rightDist <5) {
        log((int)rightDist);
        logStr("Turning left");
        turnLeft();        
    } else {
      logStr("Forward");
      goForward();
    }
  }
}


int getDistance(int echoPin, int triggerPin) {

  //logStr("echo");
  //log(echoPin);
  //logStr("trigger");
  //log(triggerPin);
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  long duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  int cm = (duration / 2) / 29.1;
  return cm;
}
