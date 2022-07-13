#ifndef motors
#define motors

#if (ARDUINO > 100 )
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class Motor {
  public:
    // Constructor
    Motor(int pinA, int pinB, int pinPWM, int channel);

    // Methods
    void initialize();
    void setMotorSpeed(float motorSpeed, int dir);
    void stopMotor();
  private:
    int _motorPinA, _motorPinB, _motorPinPWM, _channel;
};

#endif
