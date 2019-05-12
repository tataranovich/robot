#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

byte trigPin = A5;
byte echoPin = A4;

void leftSpeed(byte speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
}

void rightSpeed(byte speed) {
  motor3.setSpeed(speed);
  motor4.setSpeed(speed);
}

void leftMode(byte mode) {
  motor3.run(mode);
  motor4.run(mode);
}

void rightMode(byte mode) {
  motor1.run(mode);
  motor2.run(mode);
}

void runForward(byte speed) {
  leftSpeed(speed);
  rightSpeed(speed);
  leftMode(BACKWARD);
  rightMode(BACKWARD);
}

void runBackward(byte speed) {
  leftSpeed(speed);
  rightSpeed(speed);
  leftMode(FORWARD);
  rightMode(FORWARD);
}

void turnLeft(byte speed) {
  leftMode(FORWARD);
  rightMode(BACKWARD);
  leftSpeed(speed);
  rightSpeed(speed);
}

void turnRight(byte speed) {
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
  int duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return duration / 58;
}
