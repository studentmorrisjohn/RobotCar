#include <LineFollower.h>
#include <FarSensors.h>
#include <InputData.h>
#include <Directions.h>
#include <Triggers.h>
#include <Motor.h>
#include <CarMovement.h>
#include <PIDController.h>

LineFollower lineFollower(25,33,32,35,34);
FarSensors farSensors(14, 27);
InputData inputData(lineFollower, farSensors);
Motor leftMotor(19, 23, 2, 1);
Motor rightMotor(5, 18, 4, 2);
PIDController alignPID(3, 0, 0);
PIDController turnPID(0.0001, 0, 0);

CarMovement carMovement(leftMotor, rightMotor, alignPID, turnPID);

int trigger, error, turnDirection;
bool searchIsDone = false;

// First Pass
void searchLoop() {
  while (true) {
    trigger = inputData.getTrigger();

    if (trigger == FOLLOW_LINE) {
      error = inputData.getError();
      carMovement.align(error);
      continue;
    }

    if (trigger == TURN) {
      turnDirection = inputData.getDirection();
      carMovement.turn(turnDirection);
      continue; 
    }

    if (trigger == RED_DETECTED) {
      carMovement.stopCar();
      searchIsDone = true;
      break;
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
}

void loop() {
  // put your main code here, to run repeatedly:
  trigger = inputData.getTrigger();
  
  if (trigger == BLUE_DETECTED && !searchIsDone) {
    searchLoop();
  }
  if (trigger == BLUE_DETECTED && searchIsDone) {
    solveLoop();
  }
}
