#include "PIDController.h"
#include "Arduino.h"

PIDController::PIDController(int Kp,int Ki, int Kd) {
  _Kp = Kp;
  _Ki = Ki;
  _Kd = Kd;
  _error=0;
  _Pval = 0;
  _Ival = 0;
  _Dval = 0;
  _previousError = 0;
  _previousI = 0;
}

void PIDController::updateError(float error) {
  _error = error;
}

float PIDController::calculatePIDValue() {
  _Pval = _error;
  _Ival = _Ival + _error;
  _Dval = _error - _previousError;
  _previousError = _error;
  return (_Kp*_Pval) + (_Ki*_Ival) + (_Kd*_Dval);
}
