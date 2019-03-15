#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

#define DIR_NONE 0
#define DIR_FORWARD 1
#define DIR_BACKWARD 2
#define DIR_LEFT 4
#define DIR_RIGHT 8

int trigPin = A5; 
int echoPin = A4;

byte course;

void leftSpeed(int i) {
  motor1.setSpeed(i);
  motor2.setSpeed(i);
}

void rightSpeed(int i) {
  motor3.setSpeed(i);
  motor4.setSpeed(i);
}

void leftMode(int mode) {
  motor3.run(mode);
  motor4.run(mode);
}

void rightMode(int mode) {
  motor1.run(mode);
  motor2.run(mode);
}

void runForward(int speed) {
  leftSpeed(speed);
  rightSpeed(speed);
  leftMode(FORWARD);
  rightMode(FORWARD);
}

void runBackward(int speed) {
  leftSpeed(speed);
  rightSpeed(speed);
  leftMode(BACKWARD);
  rightMode(BACKWARD);
}

void turnLeft(int speed) {
  leftMode(RELEASE);
  rightMode(FORWARD);
  rightSpeed(speed);
}

void turnRight(int speed) {
  rightMode(RELEASE);
  leftMode(FORWARD);
  leftSpeed(speed);
}

void stop() {
  leftMode(RELEASE);
  rightMode(RELEASE);
}

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  leftMode(RELEASE);
  rightMode(RELEASE);
}

void loop() {
  int duration, distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58;
  if (distance < 20) {
    stop();
    runBackward(150);
    delay(500);
    stop();
    turnLeft(150);
    delay(500);
  } else {
    runForward(150);
  }
  delay(100);
}
