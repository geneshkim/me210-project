/*
Implementation of motor control class as specified by MotorControl.h
Intended to be used in other .ino sketches for modularity.
*/


#include "MotorControl.h"
#include <Arduino.h> //to allow arduino macros in C++ class implementation
#include <L298NX2.h>



/*

         , - ~ ~ ~ - ,
     , '       F       ' ,
   ,                       ,
  ,        IR  IR  IR       ,
 ,                           ,
 , L                        R,
 ,                           ,
  ,                         ,
   ,                       ,
     ,         B        , '
       ' - , _ _ _ ,  '

*/
// these two motors will be used when we want to move sideways to shoot
// (so can be controlled by same pwm because no need to change direction)
#define IN1_FrontMotor   A5 // in1 and in2 control motor direction (okay for these to be synched)
#define IN2_FrontMotor   7
#define EN_FrontMotor    11
#define IN1_BackMotor   A4
#define IN2_BackMotor   8
#define EN_BackMotor    11

// need two diff enable pins because want these motors to go diff speeds to turn
#define IN1_LeftMotor   A5 // in1 and in2 control motor direction (okay for these to be synched)
#define IN2_LeftMotor   7
#define EN_LeftMotor    6
#define IN1_RightMotor   A4
#define IN2_RightMotor   8
#define EN_RightMotor    6

#define longSpeed        200
#define latSpeed         200

/*
Constructor for motor control
instantiates lat and long motors from L298NX2 library
(longitudinal = forward/backwards, lateral = side to side)
*/
MotorControl::MotorControl():
  latMotors(EN_FrontMotor, IN1_FrontMotor, IN2_FrontMotor, EN_BackMotor, IN1_BackMotor, IN2_BackMotor),
  longMotors(EN_LeftMotor, IN1_LeftMotor, IN2_LeftMotor, EN_RightMotor, IN1_RightMotor, IN2_RightMotor)
{
}

void MotorControl::moveForward() {
  longMotors.setSpeed(longSpeed);
  latMotors.setSpeed(0);
  longMotors.forward();
  // Serial.println("State: Moving Forward");
}

void MotorControl::moveBackward() {
  longMotors.setSpeed(longSpeed);
  latMotors.setSpeed(0);
  longMotors.backward();
  // Serial.println("State: Moving Backward");
}

void MotorControl::rotateLeft() {
  longMotors.setSpeed(longSpeed);
  latMotors.setSpeed(0);
  longMotors.forwardA();
  longMotors.backwardB();
  // Serial.println("State: Turning Left");
}

void MotorControl::rotateRight() {
  longMotors.setSpeed(longSpeed);
  latMotors.setSpeed(0);
  longMotors.forwardB();
  longMotors.backwardA();
  // Serial.println("State: Turning Right");
}

void MotorControl::strafeLeft() {
  longMotors.setSpeed(0);
  latMotors.setSpeed(latSpeed);
  latMotors.forward();
  // Serial.println("State: Strafing Left");
}

void MotorControl::strafeRight() {
  longMotors.setSpeed(0);
  latMotors.setSpeed(latSpeed);
  latMotors.backward();
  // Serial.println("State: Strafing Right");
}

void MotorControl::stopMotors() {
  longMotors.stop();
  latMotors.stop();
  // Serial.println("State: Stopping");
}
