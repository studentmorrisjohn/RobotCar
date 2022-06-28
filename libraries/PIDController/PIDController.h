#ifndef pidcontrol
#define pidcontrol

#if (ARDUINO > 100 )
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class PIDController {
  public:
    // Constructor
    PIDController(float Kp, float Ki, float Kd);

    // Methods
    float calculatePIDValue();
    void resetValues();
    void updateError(float error);
    void changeConstants(float Kp, float Ki, float Kd);
    float getKp();
    float getKi();
    float getKd();
    
  private:
    float _Kp;
    float _Ki;
    float _Kd;
    float _error;
    float _Pval;
    float _Ival;
    float _Dval;
    float _previousError;
    float _previousI;
};

#endif
