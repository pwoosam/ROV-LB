#include "Arduino.h"
#include "Thruster.h"

Thruster::Thruster(int pwmPin, int dirPin) {
  this->speed = 0;
  this->direction = forward;
  this->speedPin = pwmPin;
  this->directionPin = dirPin;
  pinMode(this->speedPin, OUTPUT);
  pinMode(this->directionPin, OUTPUT);
}

int Thruster::getSpeed() {
  return this->speed;
}

int Thruster::getDirection() {
  return this->direction;
}

void Thruster::setSpeed(int speed) {
  analogWrite(this->speedPin, speed);
  this->speed = speed;
}

void Thruster::setDirection(ThrusterDirection dir) {
  digitalWrite(this->directionPin, dir);
  this->direction = dir;
}

void Thruster::setFromMessage(String message) {
  int speed = message.toInt();
  ThrusterDirection direction = up;
  
  if (speed > 255) {
    speed = 255;
  } else if (speed < -255) {
    speed = 255;
    direction = down;
  } else if (speed < 0) {
    speed = abs(speed);
    direction = down;
  }

  this->setDirection(direction);
  this->setSpeed(speed);
}
