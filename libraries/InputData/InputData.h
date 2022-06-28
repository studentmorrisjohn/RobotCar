#ifndef inputdata
#define inputdata

#if (ARDUINO > 100 )
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <LineFollower.h>
#include <FarSensors.h>


class InputData {
public:
    // Constructor
    InputData(LineFollower lineFollower, FarSensors farSensor);

    // Methods
    void initialize();
    void readSensors();
    int getTrigger();
    int getDirection();
    int getError();

private:
    LineFollower _lineFollower;
    FarSensors _farSensors;
    int _trigger, _direction, _error;
};

#endif