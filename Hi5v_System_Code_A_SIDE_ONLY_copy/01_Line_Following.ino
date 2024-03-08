/*
Implementation of tape sensing using three IR sensors
Assumes turning left, turning right, and driving forward functions are already implemented

Overview of finite lineStates machine:
start at the stop lineStates.
  if mid sensor on tape, move forward
  otherwise stay stopped
if in lineStates moving forward
  turn left if left sensor on tape
  turn right if right sensor on tape
  otherwise keep moving forward
if lineStates turning left
  if left sensor still on tape, stay in lineStates turning left
  if left sensor off tape, move forward
if lineStates turning right
  if right sensor still on tape, stay in lineStates turning right
  if right sensor off tape, move forward 
*/

static bool leftIsOnTape = false;
static bool rightIsOnTape = false;
static bool midIsOnTape = false;
static bool allAreOnTape;


// Local Function Prototypes
static void checkLineEvents();
static bool testForLeftIsOnTape();
static bool testForRightIsOnTape();
static bool testForMidIsOnTape();
static bool testForAllAreOnTape();
static void respToLeftIsOnTape();
static void respToRightIsOnTape();
static void respToMidIsOnTape();
static void respToAllAreOnTape();

bool toEdgeStartZone() {
  checkLineEvents();
  motors.moveForwardSlow();
  if (allAreOnTape) {
    respToAllAreOnTape();
    return true;
  }
  return false;
}

bool toAnyEdgeDetected() {
  checkLineEvents();
  motors.moveForwardSlow();
  if (leftIsOnTape || midIsOnTape || rightIsOnTape) {
    motors.stopMotors(); // Assuming stopping is the desired response
    return true;
  }
  return false;
}

bool backToAnyEdgeDetected() {
  checkLineEvents();
  motors.moveBackward();
  if (leftIsOnTape || midIsOnTape || rightIsOnTape) {
    motors.stopMotors(); // Assuming stopping is the desired response
    return true;
  }
  return false;
}


void lineFollow() {
  checkLineEvents();
  switch (lineStates) {
    case STATE_STOPPED:
      if (!midIsOnTape && !rightIsOnTape && !leftIsOnTape) {
        respToMidIsOnTape();
          lineStates = STATE_MOVING_FORWARD;
      } else {
        lineStates = STATE_STOPPED;
      }
      break;
    case STATE_MOVING_FORWARD:
      if (allAreOnTape) {
        respToAllAreOnTape();
        lineStates = STATE_STOPPED;
      } else if (leftIsOnTape) {
        respToLeftIsOnTape();
        lineStates = STATE_TURNING_LEFT;
      } else if (rightIsOnTape) {
        respToRightIsOnTape();
        lineStates = STATE_TURNING_RIGHT;
      } else if (midIsOnTape) {
        respToMidIsOnTape();
        lineStates = STATE_MOVING_FORWARD;
      } else {
        lineStates = STATE_MOVING_FORWARD;
        motors.rotateLeftSlow();
      }
      break;
    case STATE_TURNING_LEFT:
      if (allAreOnTape) {
        respToAllAreOnTape();
        lineStates = STATE_STOPPED;
      } else if (leftIsOnTape) {
        lineStates = STATE_TURNING_LEFT;
      } else {
        motors.moveForwardSlow();
        lineStates = STATE_MOVING_FORWARD;
      }
      break;
    case STATE_TURNING_RIGHT:
      if (allAreOnTape) {
        respToAllAreOnTape();
        lineStates = STATE_STOPPED;
      } else if (rightIsOnTape) {
        lineStates = STATE_TURNING_RIGHT;
      } else {
        motors.moveForwardSlow();
        lineStates = STATE_MOVING_FORWARD;
      }
      break;
    default:    // Should never get into an unhandled lineStates
      //Serial.println("What is this I do not even...");
      break;
  }

  // Serial.println(lineStates);
}


//helper functions
void checkLineEvents() {
  // set event flags
  leftIsOnTape = testForLeftIsOnTape();
  rightIsOnTape = testForRightIsOnTape();
  midIsOnTape = testForMidIsOnTape();
  allAreOnTape = testForAllAreOnTape();
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
  motors.rotateLeftSlow();
}

void respToRightIsOnTape() {
  motors.rotateRightSlow();
}

void respToMidIsOnTape() {
  motors.moveForwardSlow();
}

void respToAllAreOnTape() {
  motors.stopMotors();
}