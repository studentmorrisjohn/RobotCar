#include "Arduino.h"
#include "CarMovement.h"
#include <Directions.h>
#include <Motor.h>
#include <PIDController.h>

CarMovement::CarMovement(Motor leftMotor, Motor rightMotor, PIDController alignPID, PIDController turnPID) :
	_leftMotor(leftMotor), _rightMotor(rightMotor), _alignPID(alignPID), _turnPID(turnPID) {
	_followSpeed = 200;
	_turnSpeed = 200;
	_turnSideDelay = 500;
	_turnBackDelay = 600;
	_forwardDelay = 0;
}

void CarMovement::initialize() {
	_leftMotor.initialize();
	_rightMotor.initialize();
}

void CarMovement::changeAlignConstants(float Kp, float Ki, float Kd) {
	_alignPID.changeConstants(Kp, Ki, Kd);
}

void CarMovement::changeTurnConstants(float Kp, float Ki, float Kd) {
	_turnPID.changeConstants(Kp, Ki, Kd);
}

void CarMovement::setFollowSpeed(float followSpeed) {
	_followSpeed = followSpeed;
}

void CarMovement::setTurnSpeed(float turnSpeed) {
	_turnSpeed = turnSpeed;
}

void CarMovement::setTurnSideDelay(float turnSideDelay) {
	_turnSideDelay = turnSideDelay;
}

void CarMovement::setTurnBackDelay(float turnBackDelay) {
	_turnBackDelay = turnBackDelay;
}

void CarMovement::setForwardDelay(float forwardDelay) {
	_forwardDelay = forwardDelay;
}

void CarMovement::stopCar() {
	_leftMotor.stopMotor();
	_rightMotor.stopMotor();
}

void CarMovement::align(int error) {
	_alignPID.updateError(error);
	float pidValue = _alignPID.calculatePIDValue();

	_leftMotorSpeed = _followSpeed + pidValue;
	_rightMotorSpeed = abs(-_followSpeed + pidValue);

	_leftMotor.setMotorSpeed(_leftMotorSpeed, 0);
	_rightMotor.setMotorSpeed(_rightMotorSpeed, 1);

	Serial.println(pidValue);
}

void CarMovement::moveAnInch() {
	delay(_forwardDelay);
}

void CarMovement::turn(int direction) {
	stopCar();
	delay(1000);
	float totalTime = 0;
	float currentTime = millis();
	float previousTime = millis();
	float error = 0;

	

	while (true) {
		error = currentTime - previousTime;
		_turnPID.updateError(error);
		float pidValue = _turnPID.calculatePIDValue();

		if (direction == BACK) {
			// if (error > _turnBackDelay) {
			// 	stopCar();
			// 	break;
			// }

			if (error > _turnBackDelay) {
				Serial.println(_turnBackDelay);
				stopCar();
				break;
			}

			_leftMotorSpeed = _turnSpeed + pidValue;
			_leftMotor.setMotorSpeed(_leftMotorSpeed, 0);
			_rightMotor.setMotorSpeed(_leftMotorSpeed, 0);
		} else {
			if (error > _turnSideDelay) {
				stopCar();
				break;
			}

			if (direction == LEFT) {
				_rightMotorSpeed = _turnSpeed + pidValue;
				_rightMotor.setMotorSpeed(_rightMotorSpeed, 1);
;			}

			if (direction == RIGHT) {
				_leftMotorSpeed = _turnSpeed + pidValue;
				_leftMotor.setMotorSpeed(_leftMotorSpeed, 0);
			}
		}

		currentTime = millis();
	}

	delay(1000);
}