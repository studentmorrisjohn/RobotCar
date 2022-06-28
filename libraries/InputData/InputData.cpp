#include "Arduino.h"
#include "InputData.h"
#include <LineFollower.h>
#include <FarSensors.h>
#include <Directions.h>
#include <Triggers.h>

InputData::InputData(LineFollower lineFollower, FarSensors farSensor) : 
	_lineFollower(lineFollower), _farSensors(farSensor) {}

void InputData::initialize() {
	_lineFollower.initialize();
	_farSensors.initialize();
}

int InputData::getTrigger() {
	readSensors();
	return _trigger;
}


int InputData::getDirection() {
	return _direction;
}

int InputData::getError() {
	return _error;
}

void InputData::readSensors() {
	_lineFollower.readSensors();
	_farSensors.readSensors();

	if (_farSensors.hasLeft() || _farSensors.hasRight()) {
		_trigger = TURN;
		return;
	}

	if (_lineFollower.noLine()) {
		_trigger = TURN;
		_direction = BACK;
		return;
	}

	// Implement this after adding color sensor
	//if (_colorSensor.hasRed()) {
	//	return;
	//}

	//if (_colorSensor.hasBlue()) {
	//	return;
	//}

	_trigger = FOLLOW_LINE;
	_error = _lineFollower.getError();
}