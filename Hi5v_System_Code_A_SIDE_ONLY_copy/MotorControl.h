#ifndef MotorControl_h
#define MotorControl_h
#include <Arduino.h>
#include <L298NX2.h>

class MotorControl {
  public:
    MotorControl(); // Constructor
    void moveForwardSlow();
    void rotateLeftSlow();
    void rotateRightSlow();
    void moveForward();
    void moveBackward();
    void rotateLeft();
    void rotateRight();
    void strafeLeft();
    void strafeRight();
    void stopMotors();

  public:
    L298NX2 latMotors;
    L298NX2 longMotors;
};

#endif
