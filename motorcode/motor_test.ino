/*
This sketch aims to be equivalent to the one Dylan originally uploaded to test the motors.
MotorControl has instance variables latMotors and longMotors, in addition to the move forward/back, rotate left/right, shafe.

*/


#include <MotorControl.h>


void setup() {

  Serial.begin(9600);
  MotorControl motorcontrol;
  // these two motors can have different speeds
  motorcontrol.longMotors.setSpeed(255);
  motorcontrol.latMotors.setSpeed(255);
  motorcontrol.longMotors.forwardA();
  motorcontrol.longMotors.backwardB();
}

void loop() {
  /*
  motorcontrol.moveForward();
  delay(1000);
  motorcontrol.moveBackward();
  delay(1000);
  motorcontrol.rotateLeft();
  delay(1000);
  motorcontrol.rotateRight();
  delay(1000);
  */
  // if detect line
    // turn left/right to stay on line
  // if all 3 IR sensors == 1 then at T-zone
    // drive forward until detect line
}

