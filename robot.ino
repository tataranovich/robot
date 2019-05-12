#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

int trigPin = A5;
int echoPin = A4;

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
  leftMode(BACKWARD);
  rightMode(BACKWARD);
}

void runBackward(int speed) {
  leftSpeed(speed);
  rightSpeed(speed);
  leftMode(FORWARD);
  rightMode(FORWARD);
}

void turnLeft(int speed) {
  leftMode(FORWARD);
  rightMode(BACKWARD);
  leftSpeed(speed);
  rightSpeed(speed);
}

void turnRight(int speed) {
  leftMode(BACKWARD);
  rightMode(FORWARD);
  leftSpeed(speed);
  leftSpeed(speed);
}

void stop() {
  leftMode(RELEASE);
  rightMode(RELEASE);
}

void setup() {
  stop();
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  if (getDistance() < 20) {
    stop();
    runBackward(200);
    delay(500);
    turnLeft(200);
    delay(500);
    stop();
  } else {
    runForward(200);
  }
  delay(100);
}

int getDistance() {
  int duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return duration / 58;
}
