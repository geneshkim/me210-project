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

#include <L298NX2.h>

// motor instance (longitudinal = forward/backwards, lateral = side to side)
L298NX2 latMotors(EN_FrontMotor, IN1_FrontMotor, IN2_FrontMotor, EN_BackMotor, IN1_BackMotor, IN2_BackMotor);
L298NX2 longMotors(EN_LeftMotor, IN1_LeftMotor, IN2_LeftMotor, EN_RightMotor, IN1_RightMotor, IN2_RightMotor);


void setup() {

  Serial.begin(9600);

  // these two motors can have different speeds
  longMotors.setSpeed(255);
  latMotors.setSpeed(255);
  longMotors.forwardA();
  longMotors.backwardB();
  longMotors.
}

void loop() {
  /*
  moveForward();
  delay(1000);
  moveBackward();
  delay(1000);
  rotateLeft();
  delay(1000);
  rotateRight();
  delay(1000);
  */
  // if detect line
    // turn left/right to stay on line
  // if all 3 IR sensors == 1 then at T-zone
    // drive forward until detect line
}

//motor functions
/* As it stands, we don't have individual forward/backward control for the longitudinal motors, so since we're not really changing speed,
I wonder if it's better to have that ability to turn in place by having the two sets of motors sharing a set of input pins w/ individual enable pwm.
*/
void moveForward() {
  longMotors.forward();
}

void moveBackward() {
  longMotors.backward();
}

void rotateLeft() {
  longMotors.forwardA();
}

void rotateRight() {
  longMotors.forwardB();
}

void strafeLeft() {
  latMotors.forward();
}

void strafeRight() {
  latMotors.backward();
}

void stopMotors() {
  longMotors.stop();
  latMotors.stop();
}
