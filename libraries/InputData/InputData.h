#ifndef inputdata
#define inputdata

#if (ARDUINO > 100 )
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <LineFollower.h>
#include <FarSensors.h>
#include <Adafruit_TCS34725.h>

#define BLUE_MAX (int[]){350, 400, 400}
#define BLUE_MIN (int[]){150, 200, 200}
#define RED_MAX (int[]){600, 500, 500}
#define RED_MIN (int[]){350, 300, 300}

#define RED 0
#define BLUE 1
#define OTHERS 2

class InputData {
public:
    // Constructor
    InputData(LineFollower lineFollower, FarSensors farSensor, Adafruit_TCS34725 tcs);

    // Methods
    void initialize();
    void readSensors();
    int getTrigger();
    int getDirection();
    int getError();
    int readColor();
    bool withinThreshold(float value, float min, float max);
    bool hasRed();
    bool noLine();
    bool hasIntersection();

private:
    LineFollower _lineFollower;
    FarSensors _farSensors;
    int _trigger, _direction, _error;
    Adafruit_TCS34725 _colorSensor;
};

#endif