#include "FarSensors.h"
#include "Arduino.h"

FarSensors::FarSensors(int farLeftPin, int farRightPin) {
	_farLeftSensor = farLeftPin;
	_farRightSensor = farRightPin;
}

void FarSensors::initialize() {
	pinMode(_farLeftSensor, INPUT_PULLUP);
	pinMode(_farRightSensor, INPUT_PULLUP);
}

void FarSensors::readSensors() {
	_farLeftValue = digitalRead(_farLeftSensor);
	_farRightValue = digitalRead(_farRightSensor);
}

bool FarSensors::hasLeft() {
	return _farLeftValue == 1;
}

bool FarSensors::hasRight() {
	return _farRightValue == 1;
}