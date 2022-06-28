#ifndef farsensors
#define farsensors

#if (ARDUINO > 100 )
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class FarSensors {
public:
    // Constructor
    FarSensors(int farLeftPin, int farRightPin);

    // Methods
    void initialize();
    void readSensors();
    bool hasLeft();
    bool hasRight();
private:
    int _farLeftSensor, _farRightSensor;
    int _farLeftValue, _farRightValue;
};

#endif