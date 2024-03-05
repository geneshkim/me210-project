/*
Implementation of tape sensing using three IR sensors
Assumes turning left, turning right, and driving forward functions are already implemented
*/

#include <MotorControl.h>

//define constants and analog pins to read IR sensor values/enable pin to turn on/off IR sensing
#define LIGHT_THRESHOLD        350  //empirically determined: if IR reading is higher than threshold, that sensor is on the tape.
#define LEFT_IR_PIN         A0   
#define MID_IR_PIN         A1   
#define RIGHT_IR_PIN         A2   
//#define ENABLE_PIN         5   


//State definitions
typedef enum {
  STATE_STOPPED, STATE_MOVING_FORWARD, STATE_TURNING_LEFT, STATE_TURNING_RIGHT
} States_t;


//variables and flags
States_t state;
bool leftIsOnTape;
bool rightIsOnTape;
bool midIsOnTape;
bool allAreOnTape;

//Function Prototypes
void checkGlobalEvents(void);
void handleMoveForward(void);
void handleTurnLeft();
void handleTurnRight();
void handleStop();
bool testForLeftIsOnTape();
bool testForRightIsOnTape();
bool testForMidIsOnTape();
bool testForAllAreOnTape();
void respToLeftIsOnTape();
void respToRightIsOnTape();
void respToMidIsOnTape();
void respToAllAreOnTape();


void setup() {
  MotorControl motors;
  //initialize variables
  state = STATE_MOVING_FORWARD;
  bool leftIsOnTape = false;
  bool rightIsOnTape = false;
  bool midIsOnTape = false;
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);
  pinMode(MID_IR_PIN, INPUT);
}


/*
Overview of finite state machine:
start at the stop state.
  if mid sensor on tape, move forward
  otherwise stay stopped
if in state moving forward
  turn left if left sensor on tape
  turn right if right sensor on tape
  otherwise keep moving forward
if state turning left
  if left sensor still on tape, stay in state turning left
  if left sensor off tape, move forward
if state turning right
  if right sensor still on tape, stay in state turning right
  if right sensor off tape, move forward 
*/

void loop() {
  // delay(1500);
  checkGlobalEvents();
  switch (state) {
    case STATE_STOPPED:
      if (!midIsOnTape && !rightIsOnTape && !leftIsOnTape) {
        respToMidIsOnTape();
          state = STATE_MOVING_FORWARD;
      } else {
        state = STATE_STOPPED;
      }
      break;
    case STATE_MOVING_FORWARD:
      if (allAreOnTape) {
        respToAllAreOnTape();
        state = STATE_STOPPED;
      } else if (leftIsOnTape) {
        respToLeftIsOnTape();
        state = STATE_TURNING_LEFT;
      } else if (rightIsOnTape) {
        respToRightIsOnTape();
        state = STATE_TURNING_RIGHT;
      //} else if (!midIsOnTape) {
        //state = STATE_STOPPED;
      }else {
        state = STATE_MOVING_FORWARD;
      }
      break;
    case STATE_TURNING_LEFT:
      if (allAreOnTape) {
        respToAllAreOnTape();
        state = STATE_STOPPED;
      } else if (leftIsOnTape) {
        state = STATE_TURNING_LEFT;
     } else {
        handleMoveForward();
        state = STATE_MOVING_FORWARD;
      }
      break;
    case STATE_TURNING_RIGHT:
      if (allAreOnTape) {
        respToAllAreOnTape();
        state = STATE_STOPPED;
      } else if (rightIsOnTape) {
        state = STATE_TURNING_RIGHT;
      } else {
        handleMoveForward();
        state = STATE_MOVING_FORWARD;
      }
      break;
    default:    // Should never get into an unhandled state
      //Serial.println("What is this I do not even...");
      break;
  }

  // Serial.println(state);
}


//helper functions
void checkGlobalEvents(void) {
  // set event flags
  leftIsOnTape = testForLeftIsOnTape();
  rightIsOnTape = testForRightIsOnTape();
  midIsOnTape = testForMidIsOnTape();
  allAreOnTape = testForAllAreOnTape();
}

void handleMoveForward() {
  motors.moveForward();
}

void handleTurnLeft() {
  motors.rotateLeft();
}

void handleTurnRight() {
  motors.rotateRight();
}

void handleStop() {
  motors.stopMotors();
}

bool testForLeftIsOnTape() {
  //Serial.print("left sensor has reading ");
  //Serial.println(analogRead(LEFT_IR_PIN));
  return (uint16_t)analogRead(LEFT_IR_PIN) > LIGHT_THRESHOLD;
}

bool testForRightIsOnTape() {
  //Serial.print("right sensor has reading ");
  //Serial.println(analogRead(RIGHT_IR_PIN));
  return (uint16_t)analogRead(RIGHT_IR_PIN) > LIGHT_THRESHOLD;
}

bool testForMidIsOnTape() {
  //Serial.print("mid sensor has reading ");
  //Serial.println(analogRead(MID_IR_PIN));
  return (uint16_t)analogRead(MID_IR_PIN) > LIGHT_THRESHOLD;
}

bool testForAllAreOnTape() {
  return testForLeftIsOnTape() && testForMidIsOnTape() && testForRightIsOnTape();
}

void respToLeftIsOnTape() {
  handleTurnLeft();
}

void respToRightIsOnTape() {
  handleTurnRight();
}

void respToMidIsOnTape() {
  handleMoveForward();
}

void respToAllAreOnTape() {
  handleStop();
}

