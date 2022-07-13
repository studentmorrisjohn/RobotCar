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
PIDController alignPID(3, 0, 0);
PIDController turnPID(0.0001, 0, 0);
IkotIkotlang alaAla;
CarMovement carMovement(leftMotor, rightMotor, alignPID, turnPID);

bool searching = false;
int trigger, error, turnDirection;
float currentTime, previousTime, timeInterval;
const char* SSID  = "Wifi Sa Taas";
const char* PWD = "pinkcharger";
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

}

void setup_routing() {
  server.enableCORS(true);
  server.on("/", test);
  server.on("/start", startSearch);
  server.on("/stop", stopSearch);
  server.on("/left", HTTP_POST, leftTest);
  server.on("/right", HTTP_POST, rightTest);
  server.on("/back", HTTP_POST, backTest);
  server.on("/turnpid", HTTP_POST, changeTurnPIDConstants);
  server.on("/alignpid", HTTP_POST, changeAlignPIDConstants);
  server.on("/turnspeed", HTTP_POST, changeInitialTurnSpeed);
  server.on("/alignspeed", HTTP_POST, changeInitialAlignSpeed);

  server.begin();
}

void test() {
  server.send(200, "text/plain", "Hello there");
}

void startSearch() {
  searching = true;
  server.send(200, "text/plain", "Searching Started");
}

void stopSearch() {
  searching = false;
  server.send(200, "text/plain", "Searching Stopped");
}

void leftTest() {
  if (server.hasArg("plain") == false) {
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  float turnDelay = jsonDocument["turn_delay"];
  float forwardDelay = jsonDocument["forward_delay"];

  Serial.println("data recieved");
  Serial.print("turn delay: "); Serial.print(turnDelay);

  carMovement.setTurnSideDelay(turnDelay);
  carMovement.setForwardDelay(forwardDelay);
  carMovement.turn(LEFT);

  server.send(200, "text/plain", "Finished Turning Left");
}

void rightTest() {
  if (server.hasArg("plain") == false) {
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  float turnDelay = jsonDocument["turn_delay"];
  float forwardDelay = jsonDocument["forward_delay"];

  Serial.println("data recieved");
  Serial.print("turn delay: "); Serial.print(turnDelay);

  carMovement.setTurnSideDelay(turnDelay);
  carMovement.setForwardDelay(forwardDelay);
  carMovement.turn(RIGHT);

  server.send(200, "text/plain", "Finished Turning Right");
}

void backTest() {
  if (server.hasArg("plain") == false) {
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  float turnDelay = jsonDocument["delay"];
  Serial.println(turnDelay);

  carMovement.setTurnBackDelay(turnDelay);
  carMovement.turn(BACK);

  server.send(200, "text/plain", "Finished Turning Back");
}

void changeTurnPIDConstants() {
  if (server.hasArg("plain") == false) {
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  float Kp = jsonDocument["proportional"];
  float Ki = jsonDocument["integral"];
  float Kd = jsonDocument["differential"];

  carMovement.changeTurnConstants(Kp, Ki, Kd);

  server.send(200, "text/plain", "Turn Constants Changed");
}

void changeAlignPIDConstants() {
  if (server.hasArg("plain") == false) {
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  float Kp = jsonDocument["proportional"];
  float Ki = jsonDocument["integral"];
  float Kd = jsonDocument["differential"];

  carMovement.changeAlignConstants(Kp, Ki, Kd);

  server.send(200, "text/plain", "Align Constants Changed");
}

void changeInitialTurnSpeed() {
  if (server.hasArg("plain") == false) {
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  float _speed = jsonDocument["speed"];

  carMovement.setTurnSpeed(_speed);

  server.send(200, "text/plain", "Initial Turn Speed Changed");
}

void changeInitialAlignSpeed() {
  if (server.hasArg("plain") == false) {
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  float _speed = jsonDocument["speed"];

  carMovement.setFollowSpeed(_speed);

  server.send(200, "text/plain", "Initial Follow Speed Changed");
}

void searchLoop() {
  currentTime = millis();
  previousTime = millis();
  timeInterval = 10;
  while (true) {
    server.handleClient();
    if (!searching) {
      break;
    }

    currentTime = millis();

    if (currentTime - previousTime > timeInterval) {
      trigger = inputData.getTrigger();
      // inputData.readSensors();
//      error = inputData.getError();
//      carMovement.align(error);


      if (trigger == FOLLOW_LINE) {
        error = inputData.getError();
        carMovement.align(error);
        continue;
      }

      if (trigger == TURN) {
        turnDirection = inputData.getDirection();

        if (turnDirection == MOVEALITTLE) {
          carMovement.stopCar();
          delay(1000);
          
          carMovement.moveAnInch();
          trigger = inputData.getTrigger();
          

          if (inputData.noLine()) {
            carMovement.turn(RIGHT);
            continue;
          }

          continue;
        }
        carMovement.turn(turnDirection);
        continue;
      }

      if (trigger == RED_DETECTED) {
        carMovement.stopCar();
        searching = false;
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

  carMovement.stopCar();
  delay(3000);

  connectToWiFi();
  setup_routing();

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();

  if (searching) {
    searchLoop();
  }
}
