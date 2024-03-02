// HC_SR04 pins
#define TRIG_PIN A3
#define ECHO_PIN 4

// Button pins
#define STOP_PIN 0
#define ORIENT_PIN 5
#define BUMP_PIN_L 2
#define BUMP_PIN_C 12
#define BUMP_PIN_R 13

// Servos
#define GATE_SERVO_PIN 3 // PWM pin
#define CELEB_SERVO_PIN 9 // PWM pin

// Motors
// need two diff enable pins because want these motors to go diff speeds to turn
#define IN1_A1   A5 // in1 and in2 control motor direction (okay for these to be synched)
#define IN2_A1   7
#define EN_A1    5
#define IN1_B1   A5
#define IN2_B1   7
#define EN_B1    6

// these two motors will be used when we want to move sideways to shoot
// (so can be controlled by same pwm because no need to change direction)
#define IN1_A2   A4 // in1 and in2 control motor direction (okay for these to be synched)
#define IN2_A2   8
#define EN_A2    11
#define IN1_B2   A4
#define IN2_B2   8
#define EN_B2    11

//define constants and analog pins to read IR sensor values/enable pin to turn on/off IR sensing
#define LIGHT_THRESHOLD   350  //empirically determined: if IR reading is higher than threshold, that sensor is on the tape.
#define LEFT_IR_PIN       A0   
#define MID_IR_PIN        A1   
#define RIGHT_IR_PIN      A2   
#define ENABLE_PIN        5   