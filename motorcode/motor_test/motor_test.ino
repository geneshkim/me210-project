/*
This sketch aims to be equivalent to the one Dylan originally uploaded to test the motors.
Rather than instantiating lat/long motors, we can just instantiate a class of MotorControl by including MotorControl.h
*/


#include "MotorControl.h"


void setup() {
  MotorControl motors;
  //these motors can have different speeds
  motors.longMotors.setSpeed(255);
  motors.latMotors.setSpeed(255);
  motors.moveForward();
  delay(1000);
  motors.stopMotors();
  motors.strafeRight();
  delay(1000);
  motors.stopMotors();
}

void loop() {
}

