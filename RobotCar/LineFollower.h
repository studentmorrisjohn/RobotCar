#ifndef linefollower
#define linefollower

#if (ARDUINO > 100 )
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class LineFollower {
  public:
    // Constructor
    LineFollower(int sensorPin0, int sensorPin1, int sensorPin2, int sensorPin3, int sensorPin4, int farLeftSensorPin, int farRightSensorPin);

    // Methods
    void initialize();
    float getError();
    int getMode();
    
    void stopMotor();
    bool hasMiddle();
  private:
    int _LFSensors[5] = {0,0,0,0,0};
    int _leftSensorValue = 0;
    int _rightSensorValue = 0;
    int _sensor0;
    int _sensor1;
    int _sensor2;
    int _sensor3;
    int _sensor4;
    int _farLeftSensor;
    int _farRightSensor;
    int _mode;
};

#endif
