#ifndef carmovement
#define carmovement

#if (ARDUINO > 100 )
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Motor.h>
#include <PIDController.h>

class CarMovement {
public:
    // Constructor
    CarMovement(Motor leftMotor, Motor rightMotor, PIDController alignPID, PIDController turnPID);

    // Methods
    void initialize();
    void stopCar();
    void align(int error);
    void turn(int direction);
    void changeAlignConstants(float Kp, float Ki, float Kd);
    void changeTurnConstants(float Kp, float Ki, float Kd);
    void setFollowSpeed(float followSpeed);
    void setTurnSpeed(float turnSpeed);
    void setTurnSideDelay(float turnSideDelay);
    void setTurnBackDelay(float turnBackDelay);
    void setForwardDelay(float forwardDelay);

private:
    Motor _leftMotor, _rightMotor;
    PIDController _alignPID, _turnPID;
    float _followSpeed, _turnSpeed;
    float _turnSideDelay, _turnBackDelay, _forwardDelay;
    float _leftMotorSpeed, _rightMotorSpeed;
};

#endif