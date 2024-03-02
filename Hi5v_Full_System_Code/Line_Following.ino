/*
Implementation of tape sensing using three IR sensors
Assumes turning left, turning right, and driving forward functions are already implemented

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


enum line_following {
  STATE_STOPPED,
  STATE_MOVING_FORWARD,
  STATE_TURNING_LEFT,
  STATE_TURNING_RIGHT
};

lineStates = STATE_MOVING_FORWARD;
static bool leftIsOnTape = false;
static bool rightIsOnTape = false;
static bool midIsOnTape = false;

//variables and flags
static bool leftIsOnTape;
static bool rightIsOnTape;
static bool midIsOnTape;
static bool allAreOnTape;

// Motor Control Functions
void handleMoveForward();
void handleTurnLeft();
void handleTurnRight();
void handleStop();
void handleStrafeLeft();
void handleStrafeRight();


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
  handleMoveForward();
  checkLineEvents();
  if (allAreOnTape) {
    respToAllAreOnTape();
    return true;
  }
  return false;
}

bool toDiagonal() {
  handleMoveForward();
  checkLineEvents();
  if (leftIsOnTape) {
    leftOrRight = false;
    handleStop();
    return true;
  }
  if (rightIsOnTape) {
    leftOrRight = true;
    handleStop();
    return true;
  }
  return false;
}

void lineFollow() {
  checkLineEvents();
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
      } else {
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
void checkLineEvents() {
  // set event flags
  leftIsOnTape = testForLeftIsOnTape();
  rightIsOnTape = testForRightIsOnTape();
  midIsOnTape = testForMidIsOnTape();
  allAreOnTape = testForAllAreOnTape();
}

void handleMoveForward() {
  myMotors1.forward();
}

void handleTurnLeft() {
  //assumes this is implemented in driving/turning.
  //Serial.println("We're moving left. Wahoo!");
  Serial.println(-1);
}

void handleTurnRight() {
  //assumes this is implemented in driving/turning.
  // Serial.println("We're moving right. Wahoo!");
  Serial.println(1);
}

void handleStop() {
  myMotors1.stop();
  myMotors2.stop();
}

void handleStrafeLeft() {
  // strafe left
}

void handleStrafeRight() {
  // strafe right
}

void handleRotateLeftInPlace() {
  // rotate left in place
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