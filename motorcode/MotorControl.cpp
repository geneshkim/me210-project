#include <MotorControl.h>
#include <Arduino.h>


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
#define IN1_BackMotor   A5
#define IN2_BackMotor   7
#define EN_BackMotor    11

// need two diff enable pins because want these motors to go diff speeds to turn
#define IN1_LeftMotor   A4 // in1 and in2 control motor direction (okay for these to be synched)
#define IN2_LeftMotor   8
#define EN_LeftMotor    5
#define IN1_RightMotor   A4
#define IN2_RightMotor   8
#define EN_RightMotor    6

//class constructor
MotorControl::MotorControl() {
    latMotors(EN_FrontMotor, IN1_FrontMotor, IN2_FrontMotor, EN_BackMotor, IN1_BackMotor, IN2_BackMotor);
    longMotors(EN_LeftMotor, IN1_LeftMotor, IN2_LeftMotor, EN_RightMotor, IN1_RightMotor, IN2_RightMotor);
}

//motor functions
/* As it stands, we don't have individual forward/backward control for the longitudinal motors, so since we're not really changing speed,
I wonder if it's better to have that ability to turn in place by having the two sets of motors sharing a set of input pins w/ individual enable pwm.
*/
void MotorControl::moveForward() {
  longMotors.forward();
}

void MotorControl::moveBackward() {
  longMotors.backward();
}

void MotorControl::rotateLeft() {
  longMotors.forwardA();
}

void MotorControl::rotateRight() {
  longMotors.forwardB();
}

void MotorControl::strafeLeft() {
  latMotors.forward();
}

void MotorControl::strafeRight() {
  latMotors.backward();
}

void MotorControl::stopMotors() {
  longMotors.stop();
  latMotors.stop();
}
