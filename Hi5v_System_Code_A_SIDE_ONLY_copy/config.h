// HC_SR04 pins
#define TRIG_PIN A3
#define ECHO_PIN 4

// Button pins
#define ORIENT_PIN 5
#define BUMP_PIN_L 2
#define BUMP_PIN_C 12
#define BUMP_PIN_R 13

// Servos
#define GATE_SERVO_PIN 3 // PWM pin
#define CELEB_SERVO_PIN 9 // PWM pin

//define constants and analog pins to read IR sensor values/enable pin to turn on/off IR sensing
#define LIGHT_THRESHOLD   475  //empirically determined: if IR reading is higher than threshold, that sensor is on the tape.
#define LEFT_IR_PIN       A0   
#define MID_IR_PIN        A1   
#define RIGHT_IR_PIN      A2   
#define ENABLE_PIN        5   