#include "Arduino.h"
#include "InputData.h"
#include <LineFollower.h>
#include <FarSensors.h>
#include <Directions.h>
#include <Triggers.h>
#include <Adafruit_TCS34725.h>

InputData::InputData(LineFollower lineFollower, FarSensors farSensor, Adafruit_TCS34725 tcs) : 
	_lineFollower(lineFollower), _farSensors(farSensor), _colorSensor(tcs) {}

void InputData::initialize() {
	_lineFollower.initialize();
	_farSensors.initialize();

	if (_colorSensor.begin()) {
    Serial.println("Found sensor");
	} else {
		Serial.println("No TCS34725 found ... check your connections");
		while (1);
	}

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

bool InputData::noLine() {
	return _lineFollower.noLine();
}

bool InputData::hasIntersection() {
	return _lineFollower.hasIntersection();
}

bool InputData::withinThreshold(float value, float min, float max) {
	return (value > min) && (value < max);
}


int InputData::readColor() {
	uint16_t red, green, blue, clear;
	
	_colorSensor.getRawData(&red, &green, &blue, &clear);

	uint32_t sum = red;

	sum += blue + green;

	float r, g, b;

	r = red; r /= sum;
	g = green; g /= sum;
	b = blue; b /= sum;
	r *= 256; g *= 256; b *= 256;

	if (r > 255) r = 255;
	if (g > 255) g = 255;
	if (b > 255) b = 255;

	float remove, normalize;
	if ((b < g) && (b < r)) {
		remove = b;
		normalize = max(r-b, g-b);
	} else if ((g < b) && (g < r)) {
		remove = g;
		normalize = max(r-g, b-g);
	} else {
		remove = r;
		normalize = max(b-r, g-r);
	}
	// get rid of minority report
	float rednorm = r - remove;
	float greennorm = g - remove;
	float bluenorm = b - remove;
	// now normalize for the highest number
	rednorm /= normalize;
	greennorm /= normalize;
	bluenorm /= normalize;

	if (rednorm >= 1) {
		return 1;
	}

	return 0;
}

bool InputData::hasRed() {
	return readColor();
}

void InputData::readSensors() {
	_lineFollower.readSensors();
	_farSensors.readSensors();

	if (hasRed()) {
		_trigger = RED_DETECTED;
		_error = 0;
		return;
	}

	if (_farSensors.hasLeft() || _farSensors.hasRight()) {
		_trigger = TURN;
		if (_farSensors.hasLeft() && _farSensors.hasRight()) {
			_direction = LEFT;
		}
		if (_farSensors.hasLeft() && !_farSensors.hasRight()) {
			_direction = LEFT;
		}
		if (!_farSensors.hasLeft() && _farSensors.hasRight()) {
			_direction = STRAIGHT;
		}

		_error = 0;


		return;
	}

	if (_lineFollower.noLine()) {
		_trigger = TURN;
		_direction = BACK;

		_error = 0;
		return;
	}

	_trigger = FOLLOW_LINE;
	_error = _lineFollower.getError();
}