#include "HC_SR04.h"
#include "ezButton.h"
#include "Servo.h"
#include <L298NX2.h>
#include "config.h"
#include "MotorControl.h"

// Initialize electronics
// HC_SR04 ultra(TRIG_PIN, ECHO_PIN, ECHO_PIN);

ezButton ORI_butt(ORIENT_PIN);
// ezButton L_butt(BUMP_PIN_L);
// ezButton C_butt(BUMP_PIN_C);
// ezButton R_butt(BUMP_PIN_R);

Servo gateServo;
Servo celebServo;

enum states {
  TO_DIAGONAL_THEN_CONTACT, // to diagonal, then strafe to contact
  TO_SHOOT, // driving along wall to shooting zone
  LEFT_ADJUST,
  SHOOT_CELEB // open gate
};

enum line_following {
  STATE_STOPPED,
  STATE_MOVING_FORWARD,
  STATE_TURNING_LEFT,
  STATE_TURNING_RIGHT
};

// initializing state trackers
states state = TO_DIAGONAL_THEN_CONTACT;
line_following lineStates = STATE_MOVING_FORWARD;

// other variables
bool leftOrRight; // true means strafe right to shooting, false means strafe left
unsigned long startTime;
unsigned int dist_cm;
MotorControl motors;

void setup() {  
  Serial.begin(9600); 
  while(!Serial) continue;

  ORI_butt.setDebounceTime(50);
  // L_butt.setDebounceTime(50);
  // C_butt.setDebounceTime(50);
  // R_butt.setDebounceTime(50);

  // ultra.begin();

  gateServo.attach(GATE_SERVO_PIN);
  celebServo.attach(CELEB_SERVO_PIN);

  gateServo.write(90); //  set to blocking position


  // IR pins. Rest of IR initializiation in Line_Following.ino
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);
  pinMode(MID_IR_PIN, INPUT);

  // celebration
  // for (int i = 0; i < 3; i++) {
  //   celebServo.write(180);
  //   delay(500);
  //   celebServo.write(0);
  //   delay(200);
  // }
  delay(1000);
  toTZone();
}

void loop() {
  checkGlobalEvents();
  switch (state) {
    case TO_DIAGONAL_THEN_CONTACT:
      Serial.println("to any edge");
      while (!toAnyEdgeDetected()) {
        // empty
      }
      delay(500);
      // Serial.println("strafe right 1.5 seconds");
      motors.strafeLeft();
      delay(1800);
      // Serial.println("STOP");
      motors.stopMotors();
      delay(500);
      state = TO_SHOOT;
      break;

    case TO_SHOOT:
      // new code moving forward along wall
      // Serial.print("FORWARD ALONG WALL");
      motors.strafeRight();
      delay(250);
      motors.stopMotors();
      delay(300);
      motors.moveForward();
      delay(2500);
      motors.stopMotors();
      delay(300);
      state = LEFT_ADJUST;
        // if (L_butt.isPressed()) {
        //   motors.stopMotors();
        //   state = FORWARD_ADJUST;
        // }
      break;

    case LEFT_ADJUST:
      motors.rotateLeft();
      delay(700);
      motors.stopMotors();
      delay(200);
      motors.moveForward();
      delay(300);
      motors.stopMotors();
      state = SHOOT_CELEB;
      break;

    case SHOOT_CELEB:
      for (int i = 0; i < 3; i++) {
        gateServo.write(180);
        delay(400);
        gateServo.write(0);
        delay(400);
      }

      Serial.println("celebrating");
      for (int i = 0; i < 3; i++) {
        celebServo.write(180);
        delay(200);
        celebServo.write(0);
        delay(200);
      }
      break;
  }
}

void checkGlobalEvents() {
  ORI_butt.loop();
  // L_butt.loop();
  // C_butt.loop();
  // R_butt.loop();

  // ends after 2:10, or 130000 ms
  unsigned long curTime = millis();
  if((curTime - startTime) > 130000) {
    while(true) {
      // ends code
    }
  }
}

void toTZone() {
  // wave hello code
  celebServo.write(180);
  delay(200);
  celebServo.write(0);
  delay(200);

  uint8_t temp = 0;
  while (temp == 0) {
    checkGlobalEvents();
    // Serial.println("waiting for button");
    if (ORI_butt.isPressed()) {
      motors.rotateLeft();
      temp += 1;
    }
  }

  while (temp == 1) {
    checkGlobalEvents();
    //Serial.println("rotating while button is pressed");
    if (ORI_butt.isReleased()) {
      motors.stopMotors();
      //Serial.println("motors stopped");
      delay(1000);
      temp += 1;
    }
  }

  Serial.println("toEdgeCode");
  while(!toEdgeStartZone()) {
  }

  delay(300);
  motors.moveForward();
  delay(500);
  motors.stopMotors();

  // Serial.println("beginLineFollow");
  while(lineStates != STATE_STOPPED) {
    lineFollow();
  }

  delay(300);
  motors.moveForward();
  delay(700);
  motors.stopMotors();
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