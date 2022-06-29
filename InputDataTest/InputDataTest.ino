#include <LineFollower.h>
#include <FarSensors.h>
#include <InputData.h>
#include <Directions.h>
#include <Triggers.h>
#include <Motor.h>
#include <CarMovement.h>
#include <PIDController.h>
#include <IkotIkotlang.h>

LineFollower lineFollower(25,33,32,35,34);
FarSensors farSensors(14, 27);
InputData inputData(lineFollower, farSensors);
Motor leftMotor(19, 23, 2, 1);
Motor rightMotor(5, 18, 4, 2);
PIDController alignPID(3, 0, 0);
PIDController turnPID(0.0001, 0, 0);
IkotIkotlang alaAla;

CarMovement carMovement(leftMotor, rightMotor, alignPID, turnPID);

int trigger, error, turnDirection;
float currentTime, previousTime, timeInterval;
bool searchIsDone = false;

// First Pass
void searchLoop() {
  currentTime = millis();
  previousTime = millis();
  timeInterval = 1000;

    
  while (true) {
    currentTime = millis();

    if (currentTime - previousTime > timeInterval) {
      previousTime = currentTime;
      trigger = inputData.getTrigger();

      if (trigger == FOLLOW_LINE) {
        error = inputData.getError();
        carMovement.align(error);                                                                                                      
        continue;
      }
      
      if (trigger == TURN) {
        turnDirection = inputData.getDirection();
        alaAla.addDirection(turnDirection);
        Serial.println(alaAla.getPath());
        continue; 
      }
      
      if (trigger == RED_DETECTED) {
        Serial.println("red detected");
        continue;
      }
      
      if (trigger == BLUE_DETECTED) {
        Serial.println("blue detected");
        continue;
      }
    }

    
  }
}

// Second Pass
void solveLoop() {
  while (true) {
    trigger = inputData.getTrigger();

    if (trigger == FOLLOW_LINE) {
      error = inputData.getError();
      carMovement.align(error);
      continue;
    }

    if (trigger == TURN) {
      continue; 
    }

    if (trigger == RED_DETECTED) {
      carMovement.stopCar();
      searchIsDone = true;
      break;
    }
    
  }
}

void setup() {
  // put your setup code here, to run once:
  inputData.initialize();
  carMovement.initialize();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  searchLoop();
}
