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

//LineFollower lineFollower(25,33,32,35,34);
//FarSensors farSensors(14, 27);
//Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
//InputData inputData(lineFollower, farSensors, tcs);

//Motor leftMotor(19, 23, 2, 1);
//Motor rightMotor(5, 18, 4, 2);
//PIDController alignPID(3, 0, 0);
//PIDController turnPID(0.0001, 0, 0);
IkotIkotlang alaAla;
//CarMovement carMovement(leftMotor, rightMotor, alignPID, turnPID);

char path[] = {'S', 'R', 'L', 'B', 'S', 'B', 'L', 'L', 'L'};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.println("tangina naman init");

  Serial.println("tangina naman");
  

  delay(3000);

  alaAla.simplifyRoute(path, 9);
  Serial.println(alaAla.getPath());
  Serial.println("putangina");
  Serial.println(1&1&0);
}

void loop() {
  // put your main code here, to run repeatedly:
 
}
