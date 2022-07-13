#include <LineFollower.h>
#include <FarSensors.h>
#include <InputData.h>
#include <Directions.h>
#include <Triggers.h>
#include <Motor.h>
#include <CarMovement.h>
#include <PIDController.h>
#include <IkotIkotlang.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>

LineFollower lineFollower(25,33,32,35,34);
FarSensors farSensors(14, 27);
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
InputData inputData(lineFollower, farSensors, tcs);

Motor leftMotor(19, 23, 2, 1);
Motor rightMotor(5, 18, 4, 2);
PIDController alignPID(3, 0, 0);
PIDController turnPID(0.0001, 0, 0);
IkotIkotlang alaAla;
CarMovement carMovement(leftMotor, rightMotor, alignPID, turnPID);

int error, trigger;
float currentTime, previousTime, timeInterval;
bool searchIsDone = false;


void searchLoop() {
  currentTime = millis();
  previousTime = millis();
  timeInterval = 10;

    
  while (true) {
    
    currentTime = millis();

    if (currentTime - previousTime > timeInterval) {
      previousTime = currentTime;
      Serial.println("searching");
      trigger = inputData.getTrigger();

      if (trigger == RED_DETECTED) {
        Serial.println("red detected");
        carMovement.stopCar();
        delay(2000);
        continue;
//        searchIsDone = true;
//        break;
      }


      if (trigger == FOLLOW_LINE) {
        error = inputData.getError();
        carMovement.align(error);                                                                                                      
        continue;
      }

      if (inputData.noLine() || inputData.hasIntersection()) {
        carMovement.align(0);  
      } 
      
      
      
    }

  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  inputData.initialize();
  carMovement.initialize();

  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
    searchLoop();


}
