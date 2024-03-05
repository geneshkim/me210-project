#include "HC_SR04.h"
#include "ezButton.h"
#include "Servo.h"
#include <L298NX2.h>
#include "config.h"

// Initialize electronics
HC_SR04<ECHO_PIN> ultra(TRIG_PIN);

ezButton STOP_butt(STOP_PIN);
ezButton ORI_butt(ORIENT_PIN);
ezButton L_butt(BUMP_PIN_L);
ezButton C_butt(BUMP_PIN_C);
ezButton R_butt(BUMP_PIN_R);

L298NX2 myMotors1(EN_A1, IN1_A1, IN2_A1, EN_B1, IN1_B1, IN2_B1);
L298NX2 myMotors2(EN_A2, IN1_A2, IN2_A2, EN_B2, IN1_B2, IN2_B2);

Servo gateServo;
Servo celebServo;

// Motor Control Functions
void handleMoveForward();
void handleTurnLeft();
void handleTurnRight();
void handleStop();
void handleStrafeLeft();
void handleStrafeRight();

enum startzone {
  WAVING, // Power on initiation
  WAIT_FOR_BUTT, // Waiting for orient button
  ROTATE_IN_PLACE, // Rotate while orient button pressed
  CROSS_BOX, // crossing line of start zone, all 3 IR sensors will trigger
  TO_T_ZONE // line following until T-zone
};

enum contact {
  TO_DIAGONAL, // drive to slanted line
  TO_CONTACT   // follow line to contact
};

enum shooting {
  TO_SHOOT, // driving along wall to shooting zone
  FORWARD_ADJUST,
  SHOOT, // open gate
  CELEB, // final celebration
  FINISH // end code
};

// tracking which group of states we are in. will definitely grow
enum stateGroup {
  startGroup,   // startzone states
  contactGroup, // contact zone states
  shootGroup    // shooting states
};

enum line_following {
  STATE_STOPPED,
  STATE_MOVING_FORWARD,
  STATE_TURNING_LEFT,
  STATE_TURNING_RIGHT
};

// initializing state trackers
startzone startStates = WAVING;
shooting shootStates = TO_SHOOT;
contact contactStates = TO_DIAGONAL;
line_following lineStates = STATE_MOVING_FORWARD;

// tracking states
stateGroup curStateGroup = startGroup;

// other variables
bool leftOrRight; // true means strafe right to shooting, false means strafe left
unsigned long startTime;
unsigned int dist_cm;


void setup() {  
  Serial.begin(9600); 
  while(!Serial) continue;
 
  STOP_butt.setDebounceTime(100);
  ORI_butt.setDebounceTime(50);
  L_butt.setDebounceTime(50);
  C_butt.setDebounceTime(50);
  R_butt.setDebounceTime(50);

  ultra.begin();

  gateServo.attach(GATE_SERVO_PIN);
  celebServo.attach(CELEB_SERVO_PIN);

  gateServo.write(90); //  set to blocking position

  // first two motors
  pinMode(IN1_A1, OUTPUT);
  pinMode(IN2_A1, OUTPUT);
  pinMode(EN_A1, OUTPUT);
  pinMode(IN1_B1, OUTPUT);
  pinMode(IN2_B1, OUTPUT);
  pinMode(EN_B1, OUTPUT);

  digitalWrite(IN1_A1, HIGH); // in1 and in2 control motor direction (okay for these to be synched)
  digitalWrite(IN2_A1, LOW);

  // second two motors
  pinMode(IN1_A2, OUTPUT);
  pinMode(IN2_A2, OUTPUT);
  pinMode(EN_A2, OUTPUT);
  pinMode(IN1_B2, OUTPUT);
  pinMode(IN2_B2, OUTPUT);
  pinMode(EN_B2, OUTPUT);

  // these two motors can have different speeds
  myMotors1.setSpeedA(70); // random initial speed
  myMotors1.setSpeedB(70); // random initial speed

  // other two motors must have same speed because same enable signal
  myMotors2.setSpeed(0);

  // IR pins. Rest of IR initializiation in Line_Following.ino
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);
  pinMode(MID_IR_PIN, INPUT);
}

void loop() {
  checkGlobalEvents();

  switch (curStateGroup) {
    case startGroup:
      switch (startStates) {
        case WAVING:
          // run blocking code waving at people
          for (int i = 0; i < 3; i++) {
            celebServo.write(180);
            delay(500);
            celebServo.write(0);
            delay(200);
          }
          startStates = WAIT_FOR_BUTT;
          break;
        case WAIT_FOR_BUTT:
          // begins rotating in place when button is pressed
          if(ORI_butt.isPressed()) {
            startStates = ROTATE_IN_PLACE;
            startTime = millis();
          }
          break;
        case ROTATE_IN_PLACE:
          handleRotateLeftInPlace(); // choice between turning left / right is arbitrary
          if(ORI_butt.isReleased()) {
            handleStop();
            startStates = CROSS_BOX;
          }
          break;
        case CROSS_BOX:
          if(toEdgeStartZone()) {       // keeps pushing forward until toEdgeStartZone returns true
            startStates = TO_T_ZONE;
          }
          break;
        case TO_T_ZONE:
          lineFollow();
          if (lineStates == STATE_STOPPED) {
            curStateGroup = contactGroup;
          }
          break;
      }

    case contactGroup:
      switch (contactStates) {
        case TO_DIAGONAL:
          if(toDiagonal()) {
            contactStates = TO_CONTACT;
          }
          break;
        case TO_CONTACT:
          lineFollow();
          if (C_butt.isPressed()) {
            curStateGroup = shootGroup;
          }
          break;
      }

    case shootGroup:
      switch (shootStates) {
        case TO_SHOOT:
          if (leftOrRight) {
            handleStrafeRight();
            if (R_butt.isPressed()) {
              handleStop();
              shootStates = FORWARD_ADJUST;
            }
          } else {
            handleStrafeLeft();
            if (L_butt.isPressed()) {
              handleStop();
              shootStates = FORWARD_ADJUST;
            }
          }
          // need to implement ultrasonic sensor
          break;
        case FORWARD_ADJUST:
          handleForward();
          if (C_butt.isPressed()) {
            handleStop();
            shootStates = SHOOT;
          }
          break;
        case SHOOT:
          gateServo.write(180); // open position
          delay(3000);
          gateServo.write(90); // back to blocking
          delay(100);
          shootStates = CELEB;
          break;
        case CELEB:
          // blocking code while celebrating
          for (int i = 0; i < 3; i++) {
            celebServo.write(180);
            delay(200);
            celebServo.write(0);
            delay(200);
          }
          shootStates = FINISH;
          break;
        case FINISH:
          while(true) {
            // ends code
          }
      }
  }
}

void checkGlobalEvents() {
  STOP_butt.loop();
  ORI_butt.loop();
  L_butt.loop();
  C_butt.loop();
  R_butt.loop();

  ultra.startMeasure();
  dist_cm = ultra.getDist_cm();

  // ends after 2:10, or 130000 ms
  unsigned long curTime = millis();
  if((curTime - startTime) > 130000) {
    while(true) {
      // ends code
    }
  }

  // emergency stop code
  if(STOP_butt.isReleased()) {
    while(true) {
      // ends code
    }
  }
}

void handleForward() {
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

// HC_SR04 code, will go in loop eventually
// void SensorCode() {
//   if(sensor.isFinished()){
//     // Do something with the range...
//     Serial.print(sensor.getRange());
//     Serial.println("cm");
//     delay(1000);
//     sensor.start();
//   }
//   // Do other things...
// }