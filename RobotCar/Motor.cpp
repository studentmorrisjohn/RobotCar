#include "Motor.h"
#include "Arduino.h"
#include "esp32-hal-ledc.h"

Motor::Motor(int pinA, int pinB, int pinPWM, int channel) {
  _motorPinA = pinA;
  _motorPinB = pinB;
  _motorPinPWM = pinPWM;
  _channel = channel;
}

void Motor::initialize() {
  pinMode(_motorPinA, OUTPUT);
  pinMode(_motorPinB, OUTPUT);

  ledcAttachPin(_motorPinPWM, _channel);
  ledcSetup(_channel, 30000, 8);
}

void Motor::setMotorSpeed(float motorSpeed, int dir) {
  if (dir == 0) {
    digitalWrite(_motorPinA, LOW);
    digitalWrite(_motorPinB, HIGH);
  }
  else {
    digitalWrite(_motorPinA, HIGH);
    digitalWrite(_motorPinB, LOW);
  }

  if (abs(motorSpeed) < 200) {
    motorSpeed = 0; 
  }

  ledcWrite(_channel, motorSpeed);
}

void Motor::stopMotor() {
  setMotorSpeed(0, 0);
}
