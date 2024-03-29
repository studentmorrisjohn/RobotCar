#include "Arduino.h"
#include "CarMovement.h"
#include <Directions.h>
#include <Motor.h>
#include <PIDController.h>

CarMovement::CarMovement(Motor leftMotor, Motor rightMotor, PIDController alignPID, PIDController turnPID) :
	_leftMotor(leftMotor), _rightMotor(rightMotor), _alignPID(alignPID), _turnPID(turnPID) {
	_followSpeed = 205;
	_turnSpeed = 200;
	_turnSideDelay = 500;
	_turnBackDelay = 500;
	_forwardDelay = 300;
	_backwardDelay = 300;
	_decisionDelay = 300;
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

	_rightMotor.setMotorSpeed(_rightMotorSpeed, 1);
	_leftMotor.setMotorSpeed(_leftMotorSpeed, 0);

	
	Serial.println(pidValue);
}

void CarMovement::waitasec() {
	stopCar();
	delay(_decisionDelay);
}

void CarMovement::moveAnInch() {
	// waitasec();
	_leftMotor.setMotorSpeed(200, 0);
	_rightMotor.setMotorSpeed(200, 1);
	delay(_forwardDelay);
	waitasec();
}

void CarMovement::goBackALittle() {
	waitasec();
	_leftMotor.setMotorSpeed(200, 1);
	_rightMotor.setMotorSpeed(200, 0);
	delay(_backwardDelay);
	waitasec();
}

void CarMovement::turnBackALittle() {
	_leftMotor.setMotorSpeed(_turnSpeed, 0);
	_rightMotor.setMotorSpeed(150, 0);

	delay(10);
}

void CarMovement::turnLeftALittle(int count) {

	_rightMotor.setMotorSpeed(_turnSpeed, 1);

	delay(10);
}

void CarMovement::turnRightALittle(int count) {

	_leftMotor.setMotorSpeed(_turnSpeed, 0);
	_rightMotor.setMotorSpeed(100, 1);

	delay(10);
}

void CarMovement::turnLeft() {
	moveAnInch();

	float totalTime = 0;
	float currentTime = millis();
	float previousTime = millis();
	float error = 0;

	
	while (true) {

		error = currentTime - previousTime;
		_turnPID.updateError(error);
		float pidValue = _turnPID.calculatePIDValue();

		if (error > _turnSideDelay) {
			break;
		}

		_rightMotorSpeed = _turnSpeed + pidValue;
		_rightMotor.setMotorSpeed(_rightMotorSpeed, 1);

		
		currentTime = millis();
	}
}

void CarMovement::turnRight() {

	float totalTime = 0;
	float currentTime = millis();
	float previousTime = millis();
	float error = 0;

	
	while (true) {

		error = currentTime - previousTime;
		_turnPID.updateError(error);
		float pidValue = _turnPID.calculatePIDValue();

		if (error > _turnSideDelay) {
			break;
		}

		_leftMotorSpeed = _turnSpeed + pidValue;
		_leftMotor.setMotorSpeed(_leftMotorSpeed, 0);
		_rightMotor.setMotorSpeed(100, 1);

		
		currentTime = millis();
	}
}

void CarMovement::turnBack() {
	moveAnInch();

	float totalTime = 0;
	float currentTime = millis();
	float previousTime = millis();
	float error = 0;

	
	while (true) {

		error = currentTime - previousTime;
		_turnPID.updateError(error);
		float pidValue = _turnPID.calculatePIDValue();

		if (error > _turnBackDelay) {
			break;
		}

		_leftMotorSpeed = _turnSpeed + pidValue;
		_leftMotor.setMotorSpeed(_leftMotorSpeed, 0);
		_rightMotor.setMotorSpeed(_leftMotorSpeed, 0);

		
		currentTime = millis();
	}
}

void CarMovement::turn(int direction) {
	if (direction == BACK) {
		turnBack();
	} else if (direction == LEFT) {
		turnLeft();
	} else if (direction == RIGHT) {
		turnRight();
	} else if (direction == STRAIGHT) {
		moveAnInch();
	}
}