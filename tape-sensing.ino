/*
Implementation of tape sensing using three IR sensors
Assumes turning left, turning right, and driving forward functions are already implemented
/*


//define constants and analog pins to read IR sensor values/enable pin to turn on/off IR sensing
#define LIGHT_THRESHOLD         400 //empirically determined: if IR reading is lower than threshold, that sensor is on the tape.
#define LEFT_IR_PIN         A0   
#define RIGHT_IR_PIN         A1   
#define MID_IR_PIN         A2   
#define ENABLE_PIN         5   


//State definitions
typedef enum {
  STATE_STOPPED, STATE_MOVING_FORWARD, STATE_TURNING_LEFT, STATE_TURNING_RIGHT
} States_t;


//variables and flags
States_t state;
bool leftIsLow;
bool rightIsLow;
bool midIsLow;

//Function Prototypes
void checkGlobalEvents(void);
void handleMoveForward(void);
void handleTurnLeft();
void handleTurnRight();
void handleStop();
bool testForLeftIsLow();
bool testForRightIsLow();
bool testForMidIsLow();
bool testForAllAreLow();
void respToLeftIsLow();
void respToRightIsLow();
void respToMidIsLow();
void respToAllAreLow();


void setup() {
  Serial.begin(9600);
  //initialize variables
  state = STATE_STOPPED;
  bool leftIsLow = FALSE;
  bool rightIsLow = false;
  bool midIsLow = false;
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
/*
void loop() {
  checkGlobalEvents();
  switch (state) {
    case STATE_STOPPED:
      if (midIsLow) {
        respToMidIsLow();
          state = STATE_MOVING_FORWARD;
      } else {
        state = STATE_STOPPED;
      }
      break;
    case STATE_MOVING_FORWARD:
      if (allAreLow) {
        respToAllAreLow();
        state = STATE_STOPPED;
      } else if (leftIsLow && midIsLow()) {
        respToLeftIsLow();
        state = STATE_TURNING_LEFT;
      } else if (rightIsLow && midIsLow) {
        respToRightIsLow();
        state = STATE_TURNING_RIGHT;
      } else {
        state = STATE_MOVING_FORWARD;
      }
      break;
    case STATE_TURNING_LEFT:
      if (leftIsLow) {
        state = STATE_TURNING_LEFT;
      } else {
        handleMoveForward();
        state = STATE_MOVING_FORWARD;
      }
      break;
    case STATE_TURNING_RIGHT:
      if (rightIsLow) {
        state = STATE_TURNING_RIGHT;
      } else {
        handleMoveForward();
        state = STATE_MOVING_FORWARD;
      }
      break;
    default:    // Should never get into an unhandled state
      Serial.println("What is this I do not even...");
  }

  Serial.println(state);
}


//helper functions
void checkGlobalEvents(void) {
  // set event flags
  leftIsLow = testForLeftIsLow();
  rightIsLow = testForRightIsLow();
  midIsLow = testForMidIsLow();
  allAreLow = testForAllAreLow();
}

void handleMoveForward() {
  //assumes this is implemented in driving/turning.
  Serial.println("We're moving forward. Wahoo!");
}

void handleTurnLeft() {
  //assumes this is implemented in driving/turning.
  Serial.println("We're moving left. Wahoo!");
}

void handleTurnRight() {
  //assumes this is implemented in driving/turning.
  Serial.println("We're moving right. Wahoo!");
}

void handleStop() {
  //assumes this is implemented in driving/turning.
  Serial.println("We stopped. Wahoo!");
}

bool testForLeftIsLow() {
  return (uint_16)analogRead(LEFT_IR_PIN) > LIGHT_THRESHOLD;
}

bool testForRightIsLow() {
  return (uint_16)analogRead(RIGHT_IR_PIN) > LIGHT_THRESHOLD;
}

bool testForMidIsLow() {
  return (uint_16)analogRead(MID_IR_PIN) > LIGHT_THRESHOLD;
}

bool testForAllAreLow() {
  return testForLeftIsLow() && testForMidIsLow() && testForRightIsLow();
}

void respToLeftIsLow() {
  handleTurningLeft();
}

void respToRightIsLow() {
  handleTurningRight();
}

void respToMidIsLow() {
  handleMoveForward();
}

void testForAllAreLow() {
  handleStop();
}

