#include "LineFollower.h"
#include "Arduino.h"

LineFollower::LineFollower(int sensorPin0, int sensorPin1, int sensorPin2, int sensorPin3, int sensorPin4) {
  _sensor0 = sensorPin0;
  _sensor1 = sensorPin1;
  _sensor2 = sensorPin2;
  _sensor3 = sensorPin3;
  _sensor4 = sensorPin4;
}

void LineFollower::initialize() {
  pinMode(_sensor0, INPUT_PULLUP);
  pinMode(_sensor1, INPUT_PULLUP);
  pinMode(_sensor2, INPUT_PULLUP);
  pinMode(_sensor3, INPUT_PULLUP);
  pinMode(_sensor4, INPUT_PULLUP);
}

float LineFollower::getError() {
  float error = 0;

  if((_LFSensors[0]== 0 )&&(_LFSensors[1]== 0 )&&(_LFSensors[2]== 0 )&&(_LFSensors[3]== 0 )&&(_LFSensors[4]== 1 ))  {error = 4;}
  else if((_LFSensors[0]== 0 )&&(_LFSensors[1]== 0 )&&(_LFSensors[2]== 0 )&&(_LFSensors[3]== 1 )&&(_LFSensors[4]== 1 ))  {error = 3;}
  else if((_LFSensors[0]== 0 )&&(_LFSensors[1]== 0 )&&(_LFSensors[2]== 0 )&&(_LFSensors[3]== 1 )&&(_LFSensors[4]== 0 ))  {error = 2;}
  else if((_LFSensors[0]== 0 )&&(_LFSensors[1]== 0 )&&(_LFSensors[2]== 1 )&&(_LFSensors[3]== 1 )&&(_LFSensors[4]== 0 ))  {error = 1;}
  else if((_LFSensors[0]== 0 )&&(_LFSensors[1]== 0 )&&(_LFSensors[2]== 1 )&&(_LFSensors[3]== 0 )&&(_LFSensors[4]== 0 ))  {error = 0;}
  else if((_LFSensors[0]== 0 )&&(_LFSensors[1]== 1 )&&(_LFSensors[2]== 1 )&&(_LFSensors[3]== 0 )&&(_LFSensors[4]== 0 ))  {error =- 1;}
  else if((_LFSensors[0]== 0 )&&(_LFSensors[1]== 1 )&&(_LFSensors[2]== 0 )&&(_LFSensors[3]== 0 )&&(_LFSensors[4]== 0 ))  {error = -2;}
  else if((_LFSensors[0]== 1 )&&(_LFSensors[1]== 1 )&&(_LFSensors[2]== 0 )&&(_LFSensors[3]== 0 )&&(_LFSensors[4]== 0 ))  {error = -3;}
  else if((_LFSensors[0]== 1 )&&(_LFSensors[1]== 0 )&&(_LFSensors[2]== 0 )&&(_LFSensors[3]== 0 )&&(_LFSensors[4]== 0 ))  {error = -4;}
  return error;
}

void LineFollower::readSensors() {
    _LFSensors[0] = digitalRead(_sensor0);
    _LFSensors[1] = digitalRead(_sensor1);
    _LFSensors[2] = digitalRead(_sensor2);
    _LFSensors[3] = digitalRead(_sensor3);
    _LFSensors[4] = digitalRead(_sensor4);
}

bool LineFollower::noLine() {
    return (_LFSensors[0] == 0) && (_LFSensors[1] == 0) && (_LFSensors[2] == 0) && (_LFSensors[3] == 0) && (_LFSensors[4] == 0);
}

bool LineFollower::hasIntersection() {
    return (_LFSensors[0] == 1) && (_LFSensors[1] == 1) && (_LFSensors[2] == 1) && (_LFSensors[3] == 1) && (_LFSensors[4] == 1);
}