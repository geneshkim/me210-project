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
  TO_DIAGONAL, // drive to slanted line
  TO_CONTACT,   // follow line to contact
  TO_SHOOT, // driving along wall to shooting zone
  FORWARD_ADJUST,
  SHOOT_CELEB // open gate
};

enum line_following {
  STATE_STOPPED,
  STATE_MOVING_FORWARD,
  STATE_TURNING_LEFT,
  STATE_TURNING_RIGHT
};

// initializing state trackers
states state = TO_DIAGONAL;
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


  for (int i = 0; i < 3; i++) {
    celebServo.write(180);
    delay(500);
    celebServo.write(0);
    delay(200);
  }
  uint8_t temp = 0;
  while (temp == 0) {
    checkGlobalEvents();
    Serial.println("waiting for button");
    if (ORI_butt.isPressed()) {
      motors.rotateLeft();
      temp += 1;
    }
  }

  while (temp == 1) {
    checkGlobalEvents();
    Serial.println("rotating while button is pressed");
    if (ORI_butt.isReleased()) {
      motors.stopMotors();
      Serial.println("motors stopped");
      delay(1000);
      temp += 1;
    }
  }

  Serial.println("toEdgeCode");
  while(!toEdgeStartZone()) {
  }

  delay(700);
  motors.moveForward();
  delay(500);
  motors.stopMotors();

  Serial.println("beginLineFollow");
  while(lineStates != STATE_STOPPED) {
    lineFollow();
  }

  // to know we are done, for debugging
  delay(1000);
  Serial.println("hey");
  motors.moveForward();
  delay(1200);
  Serial.println("ho");
  motors.stopMotors();
}

void loop() {
  checkGlobalEvents();
  switch (state) {
    delay(200);
    motors.moveBackward();
    delay(1000);
    motors.stopMotors();
    case TO_DIAGONAL:
      Serial.println("TO_DIAGONAL");
      while(!toEdgeStartZone()) {
      }
      Serial.println("BACK TO DIAGONAL");
      while(!backToEdgeStartZone()) {
      }
      // not implementing  states
      delay(500);
      motors.rotateRight();
      delay(700);
      motors.stopMotors();
      delay(500);
      Serial.println("FINAL LINE FOLLOW");
      lineStates = STATE_MOVING_FORWARD;
      while(lineStates != STATE_STOPPED) {
        lineFollow();
      }
      Serial.println("DONE");

      state = TO_CONTACT;
      break;

    case TO_CONTACT:
      delay(1000);
      motors.rotateRight();
      delay(500);
      motors.stopMotors();
      lineStates = STATE_MOVING_FORWARD;
      while(lineStates != STATE_STOPPED) {
        lineFollow();
      }
      // lineFollow();
      // if (C_butt.isPressed()) {
      //   curStateGroup = shootGroup;
      // }
      break;

    case TO_SHOOT:
      // code for assuming starting on left side (A-side), thus strafe left
      motors.strafeLeft();
      delay(3000);
      state = FORWARD_ADJUST;
        // if (L_butt.isPressed()) {
        //   motors.stopMotors();
        //   state = FORWARD_ADJUST;
        // }
      break;

    case FORWARD_ADJUST:
      motors.moveForward();
      delay(1000);
      state = SHOOT_CELEB;
      break;

    case SHOOT_CELEB:
      gateServo.write(180); // open position
      delay(3000);
      gateServo.write(90); // back to blocking
      delay(100);

      Serial.println("celebrating");
      for (int i = 0; i < 3; i++) {
        celebServo.write(180);
        delay(200);
        celebServo.write(0);
        delay(200);
      }
      break;
  }
  while(true){}
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