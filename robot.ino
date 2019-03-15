#include <AFMotor.h>
#include <SymaLib.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

#define DIR_NONE 0
#define DIR_FORWARD 1
#define DIR_BACKWARD 2
#define DIR_LEFT 4
#define DIR_RIGHT 8

#define IRpin_PIN PIND
#define IRpin 2
 
#define MAXPULSE 2000
#define NUMPULSES 50
 
#define RESOLUTION 20
#define FUZZINESS 30

uint16_t pulses[NUMPULSES][2];
uint8_t currentpulse = 0;
 
SymaLib protocolparser;

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
  course = DIR_NONE;
  leftMode(RELEASE);
  rightMode(RELEASE);
}

void loop() {
  int numberpulses;
  String ctrldata = "";

  numberpulses = listenForIR();

  boolean result = protocolparser.setPulseListParse(pulses,numberpulses,RESOLUTION,FUZZINESS);

  if (result) {
    processSymaState(protocolparser.getYawDec(), protocolparser.getPitchDec(), protocolparser.getThrottleDec());
  }
}

int listenForIR(void) {
  currentpulse = 0;
 
  while (1) {
    uint16_t highpulse, lowpulse;
    highpulse = lowpulse = 0;
 
    while (IRpin_PIN & _BV(IRpin)) {
 
       highpulse++;
       delayMicroseconds(RESOLUTION);
 
       if (((highpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
         return currentpulse;
       }
    }
 
    pulses[currentpulse][0] = highpulse;
 
    while (! (IRpin_PIN & _BV(IRpin))) {
 
       lowpulse++;
       delayMicroseconds(RESOLUTION);
 
        if (((lowpulse >= MAXPULSE)  && (currentpulse != 0))|| currentpulse == NUMPULSES) {
         return currentpulse;
       }
    }
    pulses[currentpulse][1] = lowpulse;
 
    currentpulse++;
  }
}

void processSymaState(int yaw, int pitch, int throttle) {
  int speed;
  int left;
  int right;
  
  byte course = DIR_NONE;  
  speed = throttle * 2;

  if (yaw > 68) {
    left = 0;
    right = speed;
    course = DIR_LEFT;
  } else if (yaw < 58) {
    right = 0;
    left = speed;
    course = DIR_RIGHT;
  } else {
    left = speed;
    right = speed;
  }

  if (speed > 10) {
    if (pitch > 68) {
      course = course + DIR_BACKWARD;
    } else if (pitch < 68) {
      course = course + DIR_FORWARD;
    }
  } else {
    course = DIR_NONE;
  }

  if (course == DIR_NONE) {
    Serial.println("Stop");
    stop();
  }
  if (course == DIR_FORWARD) {
    Serial.println("Forward");
    runForward(speed);
  }
  if (course == DIR_BACKWARD) {
    Serial.println("Backward");
    runBackward(speed);
  }
  if (course == (DIR_FORWARD | DIR_LEFT)) {
    Serial.println("Forward and left");
    turnLeft(speed);
  }
  if (course == (DIR_FORWARD | DIR_RIGHT)) {
    Serial.println("Forward and right");
    turnRight(speed);
  }
  if (course == (DIR_BACKWARD | DIR_LEFT)) {
    Serial.println("Backward and left");
  }
  if (course == (DIR_BACKWARD | DIR_RIGHT)) {
    Serial.println("Backward and right");
  }
}
