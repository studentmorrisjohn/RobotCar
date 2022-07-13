#include "Motor.h"
#include "PIDController.h"
#include "LineFollower.h"
#include "States.h"


int mode = 0;

int forwardDelay = 0;
int turnDelay = 500;

Motor leftMotor(19, 21, 2, 1);
Motor rightMotor(5, 18, 4, 2);

// Setup Line Sensor Pins
LineFollower lfArray(25,33,32,35,34, 14, 27);

// For Timer
const unsigned long lineSensorInterval = 5;
unsigned long previousTime = 0;
unsigned long currentTime = millis();

// For PID Control
PIDController pid(3, 0, 0);
float error=0;


void pidControl() {
  float pidValue = pid.calculatePIDValue();
  float leftMotorSpeed = 210 + pidValue;
  float rightMotorSpeed = abs(-210 + pidValue);

  leftMotor.setMotorSpeed(leftMotorSpeed, 0);
  rightMotor.setMotorSpeed(rightMotorSpeed, 1);
}

void updateMotors() {
  switch (mode) {
    case NO_LINE:
      stopCar;
      break;
    case STOPPED:
      stopCar();
      break;
    case FOLLOWING_LINE:
      pidControl();
      
      break;
     case TURNING_LEFT:
        turnLeft();
      
      break;
  }
}

void turnLeft() {
  Serial.println("turning left");
  stopCar();
  delay(1000);
  leftMotor.setMotorSpeed(200, 0);
  rightMotor.setMotorSpeed(200, 1);

  delay(forwardDelay);

  
  leftMotor.setMotorSpeed(0, 0);
  rightMotor.setMotorSpeed(200, 1);

  delay(turnDelay);
  
  stopCar();
  delay(1000);
  mode = STOPPED;
}

void updateTime() {
  currentTime = millis();
}

void readSensors() {
  if (currentTime - previousTime >= lineSensorInterval) {
    
    
    error = lfArray.getError();
    mode = lfArray.getMode();
    pid.updateError(error);
    
    previousTime = currentTime;

    Serial.println("updating motors");
  }

}

void stopCar() {
  leftMotor.stopMotor();
  rightMotor.stopMotor();
}

void setup() {
  leftMotor.initialize();
  rightMotor.initialize();

  lfArray.initialize();

  stopCar();
  delay(3000);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  readSensors();
  updateMotors();
  updateTime();
  
  
}
