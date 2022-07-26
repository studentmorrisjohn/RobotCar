#include "Motor.h"
#include "Arduino.h"


#if defined(ARDUINO_ARCH_ESP32)
  #include "esp32-hal-ledc.h"
#endif

Motor::Motor(int pinA, int pinB, int pinPWM, int channel) {
  _motorPinA = pinA;
  _motorPinB = pinB;
  _motorPinPWM = pinPWM;
  _channel = channel;
}

void Motor::initialize() {
  pinMode(_motorPinA, OUTPUT);
  pinMode(_motorPinB, OUTPUT);

  

  #if defined(ARDUINO_ARCH_ESP32)
    ledcAttachPin(_motorPinPWM, _channel);
    ledcSetup(_channel, 30000, 8);
  #else
    pinMode(_motorPinPWM, OUTPUT);
  #endif
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

  if (abs(motorSpeed) > 255) {
    motorSpeed = 255; 
  }

  #if defined(ARDUINO_ARCH_ESP32)
    ledcWrite(_channel, motorSpeed);
  #else
    analogWrite(_motorPinPWM, motorSpeed);
  #endif
  
}

void Motor::stopMotor() {
  setMotorSpeed(0, 0);
}
