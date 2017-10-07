#ifndef THRUSTER_H
#define THRUSTER_H

#include "Arduino.h"

enum ThrusterDirection {up, down, forward = 0, reverse};

class Thruster {
  private:
    int speedPin;
    int directionPin;
    int speed;
    ThrusterDirection direction;
  public:
    Thruster(int pwmPin, int dirPin);
    int getSpeed();
    int getDirection();
    void setSpeed(int);
    void setDirection(ThrusterDirection);
    void setFromMessage(String);
};

#endif
