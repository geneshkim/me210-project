#ifndef MotorControl_h
#define MotorControl_h
#include <Arduino.h>
#include <L298NX2.h>

class Motor {
  public:
    MotorControl(); // Constructor
    void moveForward();
    void moveBackward();
    void rotateLeft();
    void rotateRight();
    void strafeLeft();
    void strafeRight();
    void stopMotors();

  private:
    L298NX2 latMotors;
    L298NX2 longMotors;
};

#endif
