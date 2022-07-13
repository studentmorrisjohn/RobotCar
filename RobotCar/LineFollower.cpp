#include "LineFollower.h"
#include "Arduino.h"
#include "States.h"

LineFollower::LineFollower(int sensorPin0, int sensorPin1, int sensorPin2, int sensorPin3, int sensorPin4,  int farLeftSensorPin, int farRightSensorPin) {
  _sensor0 = sensorPin0;
  _sensor1 = sensorPin1;
  _sensor2 = sensorPin2;
  _sensor3 = sensorPin3;
  _sensor4 = sensorPin4;
  _farLeftSensor = farLeftSensorPin;
  _farRightSensor = farRightSensorPin;
}

void LineFollower::initialize() {
  pinMode(_sensor0, INPUT_PULLUP);
  pinMode(_sensor1, INPUT_PULLUP);
  pinMode(_sensor2, INPUT_PULLUP);
  pinMode(_sensor3, INPUT_PULLUP);
  pinMode(_sensor4, INPUT_PULLUP);
  pinMode(_farLeftSensor, INPUT_PULLUP);
  pinMode(_farRightSensor, INPUT_PULLUP);
  
}

float LineFollower::getError() {
  float error = 0;
  _LFSensors[0] = digitalRead(_sensor0);
  _LFSensors[1] = digitalRead(_sensor1);
  _LFSensors[2] = digitalRead(_sensor2);
  _LFSensors[3] = digitalRead(_sensor3);
  _LFSensors[4] = digitalRead(_sensor4);

  _leftSensorValue = digitalRead(_farLeftSensor);
  _rightSensorValue = digitalRead(_farRightSensor);

  if ((_leftSensorValue == 0) && (_rightSensorValue == 0) && (_LFSensors[0]== 0 )&&(_LFSensors[1]== 0 )&&(_LFSensors[2]== 0 )&&(_LFSensors[3]== 0 )&&(_LFSensors[4]== 0 )) {
    _mode = NO_LINE; error = 0;
    return error;
  }

  if ((_leftSensorValue == 1) && (_rightSensorValue == 0)) {
    _mode = TURNING_LEFT; error = 0;
    
    return error;
  }

//  if ((_leftSensorValue == 1) && (_rightSensorValue == 1)) {
//    _mode = TURNING_LEFT; error = 0;
//    return error;
//  }

  
    
  if((_LFSensors[0]== 0 )&&(_LFSensors[1]== 0 )&&(_LFSensors[2]== 0 )&&(_LFSensors[3]== 0 )&&(_LFSensors[4]== 1 ))  {_mode = FOLLOWING_LINE; error = 4;}
  else if((_LFSensors[0]== 0 )&&(_LFSensors[1]== 0 )&&(_LFSensors[2]== 0 )&&(_LFSensors[3]== 1 )&&(_LFSensors[4]== 1 ))  {_mode = FOLLOWING_LINE; error = 3;}
  else if((_LFSensors[0]== 0 )&&(_LFSensors[1]== 0 )&&(_LFSensors[2]== 0 )&&(_LFSensors[3]== 1 )&&(_LFSensors[4]== 0 ))  {_mode = FOLLOWING_LINE; error = 2;}
  else if((_LFSensors[0]== 0 )&&(_LFSensors[1]== 0 )&&(_LFSensors[2]== 1 )&&(_LFSensors[3]== 1 )&&(_LFSensors[4]== 0 ))  {_mode = FOLLOWING_LINE; error = 1;}
  else if((_LFSensors[0]== 0 )&&(_LFSensors[1]== 0 )&&(_LFSensors[2]== 1 )&&(_LFSensors[3]== 0 )&&(_LFSensors[4]== 0 ))  {_mode = FOLLOWING_LINE; error = 0;}
  else if((_LFSensors[0]== 0 )&&(_LFSensors[1]== 1 )&&(_LFSensors[2]== 1 )&&(_LFSensors[3]== 0 )&&(_LFSensors[4]== 0 ))  {_mode = FOLLOWING_LINE; error =- 1;}
  else if((_LFSensors[0]== 0 )&&(_LFSensors[1]== 1 )&&(_LFSensors[2]== 0 )&&(_LFSensors[3]== 0 )&&(_LFSensors[4]== 0 ))  {_mode = FOLLOWING_LINE; error = -2;}
  else if((_LFSensors[0]== 1 )&&(_LFSensors[1]== 1 )&&(_LFSensors[2]== 0 )&&(_LFSensors[3]== 0 )&&(_LFSensors[4]== 0 ))  {_mode = FOLLOWING_LINE; error = -3;}
  else if((_LFSensors[0]== 1 )&&(_LFSensors[1]== 0 )&&(_LFSensors[2]== 0 )&&(_LFSensors[3]== 0 )&&(_LFSensors[4]== 0 ))  {_mode = FOLLOWING_LINE; error = -4;}
  else if((_LFSensors[0]== 1 )&&(_LFSensors[1]== 1 )&&(_LFSensors[2]== 1 )&&(_LFSensors[3]== 1 )&&(_LFSensors[4]== 1 ))  {_mode = STOPPED; error = 0;}
  else if((_LFSensors[0]== 0 )&&(_LFSensors[1]== 0 )&&(_LFSensors[2]== 0 )&&(_LFSensors[3]== 0 )&&(_LFSensors[4]== 0 ))  {_mode = STOPPED; error = 0;}

  return error;
}

bool LineFollower::hasMiddle() {
  _LFSensors[2] = digitalRead(_sensor2);
  return _LFSensors[2]== 1;
}

int LineFollower::getMode() {
  return _mode;
}
