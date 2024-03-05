
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

#include <L298NX2.h>

// motor instance (longitudinal = forward/backwards, lateral = side to side)
L298NX2 latMotors(EN_FrontMotor, IN1_FrontMotor, IN2_FrontMotor, EN_BackMotor, IN1_BackMotor, IN2_BackMotor);
L298NX2 longMotors(EN_LeftMotor, IN1_LeftMotor, IN2_LeftMotor, EN_RightMotor, IN1_RightMotor, IN2_RightMotor);


void setup() {

  Serial.begin(9600);

  // these two motors can have different speeds
  longMotors.setSpeed(255);
  latMotors.setSpeed(255);
  pinMode(A5, OUTPUT);
  pinMode(A4, OUTPUT);
  moveForward();
  delay(1000);
  stopMotors();
  strafeRight();
  delay(1000);
  stopMotors();
}

void loop() {
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
  longMotors.setSpeed(255);
  latMotors.setSpeed(0);
  longMotors.forward();
  Serial.println("State: Moving Forward");
}

void moveBackward() {
  longMotors.setSpeed(255);
  latMotors.setSpeed(0);
  longMotors.backward();
  Serial.println("State: Moving Backward");
}

void rotateLeft() {
  longMotors.setSpeed(255);
  latMotors.setSpeed(0);
  longMotors.forwardA();
  longMotors.backwardB();
  Serial.println("State: Turning Left");
}

void rotateRight() {
  longMotors.setSpeed(255);
  latMotors.setSpeed(0);
  longMotors.forwardB();
  longMotors.backwardA();
  Serial.println("State: Turning Right");
}

void strafeLeft() {
  longMotors.setSpeed(0);
  latMotors.setSpeed(255);
  latMotors.forward();
  Serial.println("State: Strafing Left");
}

void strafeRight() {
  longMotors.setSpeed(0);
  latMotors.setSpeed(255);
  latMotors.backward();
  Serial.println("State: Strafing Right");
}

void stopMotors() {
  longMotors.stop();
  latMotors.stop();
  Serial.println("State: Stopping");
}