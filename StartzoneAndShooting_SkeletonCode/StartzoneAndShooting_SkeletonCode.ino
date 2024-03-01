#include "HC_SR04.h"
#include "ezButton.h"
#include "Servo.h"

// HC_SR04 pins
#define TRIG_PIN A3
#define ECHO_PIN 4

// Button pins
#define STOP_PIN 0
#define ORIENT_PIN 5
#define BUMP_PIN_L 2
#define BUMP_PIN_C 12
#define BUMP_PIN_R 13

// Motors
#define GATE_SERVO 3 // PWM pin
#define CELEB_SERVO 9 // PWM pin

// Initialize sensors
// HC_SR04 hc(TRIG_PIN, ECHO_PIN); // not working right now

ezButton STOP_butt(STOP_PIN);
ezButton ORI_butt(ORIENT_PIN);
ezButton L_butt(BUMP_PIN_L);
ezButton C_butt(BUMP_PIN_C);
ezButton R_butt(BUMP_PIN_R);

Servo gateServo;
Servo celebServo;

enum startzone {
  WAVING, // Power on initiation
  WAIT_FOR_BUTT, // Waiting for orient button
  ROTATE_IN_PLACE, // Rotate while orient button pressed
  CROSS_BOX // crossing line of start zone, all 3 IR sensors will trigger
};

enum shooting {
  TO_SHOOT, // driving along wall to shooting zone
  SHOOT, // open gate
  CELEB, // final celebration
  FINISH // end code
};

// tracking which group of states we are in. will definitely grow
enum stateGroup {
  startGroup, // startzone states
  shootGroup   // shooting states
};

// initializing state trackers
startzone startStates = WAVING;
// shooting shootStates = TO_SHOOT;
// stateGroup curStateGroup = startGroup;

// debugging shooting code:
shooting shootStates = SHOOT;
stateGroup curStateGroup = shootGroup;

void setup() {  
  Serial.begin(9600); 
  while(!Serial) continue;
 
  STOP_butt.setDebounceTime(100);
  ORI_butt.setDebounceTime(100);
  L_butt.setDebounceTime(50);
  C_butt.setDebounceTime(50);
  R_butt.setDebounceTime(50);

  gateServo.attach(GATE_SERVO);
  celebServo.attach(CELEB_SERVO);

  gateServo.write(90); //  set to blocking position
}

void loop() {
  STOP_butt.loop();
  ORI_butt.loop();
  L_butt.loop();
  C_butt.loop();
  R_butt.loop();

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
          }
          break;
        case ROTATE_IN_PLACE:
          // ************ INSERT ROTATE CODE ************ 
          
          // continue rotating until button is released
          if(ORI_butt.isReleased()) {
            startStates = CROSS_BOX;
          }
          break;
        case CROSS_BOX:
          // not sure right now, something about the IR sensors all triggering?
          curStateGroup = shootGroup;
          break;
      }

    case shootGroup:
      switch (shootStates) {
        case TO_SHOOT:
          // drive along wall, using ultrasonic sensor
          shootStates = SHOOT;
          break;
        case SHOOT:
          // blocking code until all the balls are released
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
          return; // end code completely
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