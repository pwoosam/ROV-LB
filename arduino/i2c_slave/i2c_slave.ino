#include <Wire.h>
#include <Servo.h>
#include "Thruster.h"

// The i2c address of the Arduino
#define SLAVE_ADDRESS 0x2f

// Pins used for thrusters
const int pwmLeftThruster = 5;
const int dirLeftThruster = 4;

// Create register and objects for servo1 (This is here as an example for moving servos)
#define SERVO_1 0x00
Servo servo1;
int servo1loc = 90;  // This is the starting position for the servo

// Create register and object for elevatorThruster
#define ELEVATOR_THRUSTER 0x10
Thruster elevatorThruster = Thruster(3, 7);

// Create register and object for leftThruster
#define LEFT_THRUSTER 0x11
Thruster leftThruster = Thruster(pwmLeftThruster, dirLeftThruster);

// These variables keep track of which messages were sent and to whom
byte recentMessageRegister;
String recentMessage;

void setup() {
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(recvMessage);
  Wire.onRequest(sendMessage);
  Serial.begin(9600);
  servo1.attach(2);
}

void loop() {
  servo1.write(servo1loc);
  delay(100);
}

void recvMessage(int byteLength) {
  // Single byte messages contain no message
  if (byteLength <= 1) return;
  
  byte messageRegister = Wire.read();  // First byte sent is a register
  recentMessageRegister = messageRegister;
  String message;
  while (Wire.available())  // The remaining bytes are the message
    message += (char)Wire.read();
  recentMessage = message;

  switch (messageRegister) {
    case SERVO_1:
      servo1loc = message.toInt();
      if (servo1loc > 180) {
        servo1loc = 180;
      } else if (servo1loc < 0) {
        servo1loc = 0;
      }
      break;
    case ELEVATOR_THRUSTER:
      elevatorThruster.setFromMessage(message);
      break;
    default:
      break;
  }
  
  Serial.print("Value: ");
  Serial.println(message);
  Serial.print("Message register: ");
  Serial.println(messageRegister, HEX);
  Serial.println();
}

void sendMessage() {
  // We can only send 32 bytes at a time
  int padStart = 0;
  String message;
  switch (recentMessageRegister) {
    case SERVO_1:
      message += "Servo 1 Position is: ";
      message += servo1loc;
      padStart = message.length();
      Wire.write(message.c_str());
      break;
    case ELEVATOR_THRUSTER:
      message = "Elevator speed=";
      message += elevatorThruster.getSpeed();
      message += ", dir=";
      message += elevatorThruster.getDirection();
      padStart = message.length();
      Wire.write(message.c_str());
      break;
    default:
      break;
  }

  // Fill the remaining bytes with spaces
  for (int i = padStart; i < 32; i++)
    Wire.write(' ');
}
