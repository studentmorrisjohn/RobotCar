#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

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
PIDController alignPID(8, 0, 0);
PIDController turnPID(0.0001, 0, 0);
IkotIkotlang alaAla;
CarMovement carMovement(leftMotor, rightMotor, alignPID, turnPID);

bool carIsRunning = false, doneSearch = false;
int trigger, error, turnDirection;
float currentTime, previousTime, timeInterval;

const char* SSID  = "robot";
const char* PWD = "password";

WebServer server(80);
StaticJsonDocument<250> jsonDocument;

//Server Functions
void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PWD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    // we can even make the ESP32 to sleep
  }

  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());

  Serial.println("Gateway IP address: ");
  Serial.println(WiFi.gatewayIP());

}

void setup_routing() {
  server.enableCORS(true);
  server.on("/", test);
  server.on("/start", startSearch);
  server.on("/stop", stopSearch);
  server.on("/path", getPath);
  server.on("/left", leftTest);
  server.on("/right", rightTest);
  server.on("/back", backTest);
  server.begin();
}

void test() {
  server.send(200, "text/plain", "Hello there");
}

void startSearch() {
  carIsRunning = true;
  server.send(200, "text/plain", "Searching Started");
}

void stopSearch() {
  carIsRunning = false;
  server.send(200, "text/plain", "Searching Stopped");
}

void leftTest() {
  carMovement.turn(LEFT);
  carMovement.stopCar();

  server.send(200, "text/plain", "Finished Turning Left");
}

void rightTest() {
  carMovement.turn(RIGHT);
  carMovement.stopCar();

  server.send(200, "text/plain", "Finished Turning Right");
}

void backTest() {
  carMovement.turn(BACK);
  carMovement.stopCar();

  server.send(200, "text/plain", "Finished Turning Back");
}


void getPath() {
  String path = alaAla.getPath();
  server.send(200, "text/plain", path);
}

void searchLoop() {
  currentTime = millis();
  previousTime = millis();
  timeInterval = 10;
  while (true) {

    server.handleClient();

    if (!carIsRunning) {
      delay(3000);
      break;
    }

    currentTime = millis();

    if (currentTime - previousTime > timeInterval) {
      trigger = inputData.getTrigger();

      if (trigger == FOLLOW_LINE) {
        error = inputData.getError();
        carMovement.align(error);
        continue;
      }

      if (trigger == TURN) {
        turnDirection = inputData.getDirection();

        if (turnDirection == MOVEALITTLE) {        
          carMovement.moveAnInch();
          trigger = inputData.getTrigger();
          

          if (inputData.noLine()) {
            carMovement.turn(RIGHT);
            inputData.readSensors();

  
            while (true) {
              if (inputData.hasMiddle()){
                break;
              }
              carMovement.turnRightALittle(1);
              inputData.readSensors();            
            }
            carMovement.waitasec();
            alaAla.addDirection(RIGHT);
            continue;
          }
                   
          alaAla.addDirection(STRAIGHT);
          continue;
        }

        if (turnDirection == BACK) {
          carMovement.stopCar();

          for (int i; i < 5; i++) {
            if (inputData.readColor()) {
              carIsRunning = false;
              doneSearch = true;
              break;
            }
          }

          if (!carIsRunning) {
            break;
          }

          carMovement.turn(BACK);
          inputData.readSensors();

          while (true) {
            if (inputData.hasMiddle()){
              break;
            }
            carMovement.turnBackALittle();
            inputData.readSensors();            
          }

          carMovement.waitasec();
          alaAla.addDirection(BACK);
          continue;                      
        }

        if (turnDirection == LEFT) {
          
          carMovement.turn(turnDirection);
          inputData.readSensors();

          while (true) {

            if (inputData.hasMiddle()){
              break;
            }
            carMovement.turnLeftALittle(1);
            inputData.readSensors();            
          }

                   
          carMovement.waitasec();
          alaAla.addDirection(turnDirection);
          continue;
          
        }
      }

      if (trigger == RED_DETECTED) {
        carMovement.stopCar();
        carIsRunning = false;
        doneSearch = true;
        break;
      }

      previousTime = currentTime;
    }



  }
}

void solveLoop() {
  currentTime = millis();
  previousTime = millis();
  timeInterval = 10;
  
  while (true) {

    server.handleClient();
    
    if (!carIsRunning) {
      break;
    }
  
    currentTime = millis();
  
    if (currentTime - previousTime > timeInterval) {
      
      trigger = inputData.getTrigger();
  
      if (trigger == FOLLOW_LINE) {
        error = inputData.getError();
        carMovement.align(error);
        continue;
      }
  
      if (trigger == TURN) {
        turnDirection = alaAla.getNextDirection();

        if (inputData.noLine()) {
          if (inputData.hasRed()) {
            carMovement.stopCar();
            carIsRunning = false;
            doneSearch = true;
            break;
          }
        }

        if (turnDirection == RIGHT) {
          carMovement.moveAnInch();

          carMovement.turn(turnDirection);
          inputData.readSensors();

          while (true) {

            if (inputData.hasMiddle()){
              break;
            }
            carMovement.turnRightALittle(1);
            inputData.readSensors();            
          }

                   
          carMovement.waitasec();
          continue;
        }

        if (turnDirection == LEFT) {
          carMovement.turn(turnDirection);
          inputData.readSensors();

          while (true) {

            if (inputData.hasMiddle()){
              break;
            }
            carMovement.turnLeftALittle(1);
            inputData.readSensors();            
          }

                   
          carMovement.waitasec();
          continue;
        }
        
        carMovement.turn(turnDirection);
        continue;
      }
  
      if (trigger == RED_DETECTED) {
        carMovement.stopCar();
        carIsRunning = false;
        break;
      }
  
      previousTime = currentTime;
    }
  
  
  
  }
}

void setup() {
  // put your setup code here, to run once:
  inputData.initialize();
  carMovement.initialize();

  Serial.begin(9600);

  connectToWiFi();
  setup_routing();

  carMovement.stopCar();
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("waiting");
  server.handleClient();

  inputData.readSensors();
  Serial.println(inputData.getError());

  
  
  if (carIsRunning&& !doneSearch) {
    delay(3000);
    searchLoop();
  }

  

  if (carIsRunning && doneSearch) {
    alaAla.simplifyRoute();
    delay(3000);
    solveLoop();
  }
}
