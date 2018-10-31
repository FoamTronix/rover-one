#define ONE_SECOND 1000

// Lights
// ##########################################
#define DS_PIN 11
#define STCP_PIN 12
#define SHCP_PIN 13

#define RED_LED1 0
#define RED_LED2 1
#define RED_LED3 2
#define YELLOW_LED4 3
#define YELLOW_LED5 4
#define YELLOW_LED6 5
#define GREEN_LED7 6
#define GREEN_LED8 7
#define GREEN_LED9 8 // start at 0 on second Shift Register
#define BLUE_LED10 9
#define BLUE_LED11 10
#define BLUE_LED12 11
#define WHITE_LED13 12
#define WHITE_LED14 13
#define INFRARED_LED15 14
#define INFRARED_LED16 15
#define LIGHT_COUNT 16
#define SIGNAL_DELAY 500 // milliseconds
boolean signalLeft;
boolean signalRight;
boolean lights[LIGHT_COUNT];

//// Motors
//// ##########################################
//#include <Servo.h> 
//#define MOVE_A 5        // L9110 chip :: Input A
//#define MOVE_B 4        // L9110 chip :: Input B
//#define STEERING_PIN 6  // Pin 6 is a PWM pin for the servo.
//Servo steeringServo; 
//
//// Ping Sensors
//// ##########################################
//#include <NewPing.h>
//#define SONAR_NUM     4  // Number or sensors.
//#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
//#define PING_INTERVAL 100 // 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).
//
//// Using only one ping for both the Trigger and Ping.  The NewPing library takes care of 
//// toggling the pin's input/output state.
//#define PING_SENSOR_1_PIN 7 
//#define PING_SENSOR_2_PIN 8 
//#define PING_SENSOR_3_PIN 9 
//#define PING_SENSOR_4_PIN 10 
//
//unsigned long pingTimer[SONAR_NUM];   // Holds the times when the next ping should happen for each sensor.
//unsigned int pingRangesCm[SONAR_NUM]; // Where the ping distances are stored.
//uint8_t currentSensor = 0;            // Keeps track of which sensor is active.
//
//NewPing sonar[SONAR_NUM] = {                                   // Sensor object array.
//  NewPing(PING_SENSOR_1_PIN, PING_SENSOR_1_PIN, MAX_DISTANCE), // TriggerPin, EchoPin, MaxDistance
//  NewPing(PING_SENSOR_2_PIN, PING_SENSOR_2_PIN, MAX_DISTANCE),
//  NewPing(PING_SENSOR_3_PIN, PING_SENSOR_3_PIN, MAX_DISTANCE),
//  NewPing(PING_SENSOR_4_PIN, PING_SENSOR_4_PIN, MAX_DISTANCE)
//};

// Temperature
// ##########################################

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#define ONE_WIRE_PIN 2
OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature sensors(&oneWire);
float lastTempValue = 0.0;

// LDR
// ##########################################
#define LDR_SENSOR_PIN A0
int lastLDRValue;

// Commands from serial
// ##########################################
String command; // Used to process commands from RaspberryPi

void setup() {
  Serial.begin(9600);
  initLights();
//  initMotors();
//  initPingSensors();
//  initTemperatureSensor();
//  initLDRSensor();
  // performSystemTests();
}

void loop() {
//  if(Serial.available() > 0) {
//    command = Serial.readStringUntil('\n');
//    processCommand();
//  }
//  signalCheck();
//  readPingSensors();
}

void processCommand() {
  if(command == "LightsOn") { allLightsOn(); } 
  else if(command == "LightsOff") { allLightsOff(); }

  else if(command == "RedLightsOn") { redLightsOn(); }
  else if(command == "RedLightsOff") { redLightsOff(); }

  else if(command == "GreenLightsOn") { greenLightsOn(); }
  else if(command == "GreenLightsOff") { greenLightsOff(); }

  else if(command == "YellowLightsOn") { yellowLightsOn(); }
  else if(command == "YellowLightsOff") { yellowLightsOff(); }

  else if(command == "BlueLightsOn") { blueLightsOn(); }
  else if(command == "BlueLightsOff") { blueLightsOff(); }

  else if(command == "WhiteLightsOn") { whiteLightsOn(); }
  else if(command == "WhiteLightsOff") { whiteLightsOff(); }

  else if(command == "InfraredOn") { infraredOn(); }
  else if(command == "InfraredOff") { infraredOff(); }

//  else if(command == "MoveForward") { moveForward(); }
//  else if(command == "MoveBackward") { moveBackward(); }
//  else if(command == "MoveStop") { stopMoving(); }

//  else if(command == "RedLightsOn") { RedLightsOn(); }
//  else if(command == "TurnRight") { turnRight(); }
//  else if(command == "TurnCenter") { turnCenter(); }

//  else if(command == "Sensors") { sensorReadings(); }
}

void performSystemTests() {  
  testLights();
  // testMotors();
}

//void sensorReadings() {
//  fetchTemperature();
//  fetchLDRValue();
//  String data = "{";
//  data += "\"ping1\":" + String(pingRangesCm[0], 3) + ",";
//  data += "\"ping2\":" + String(pingRangesCm[1], 3) + ",";
//  data += "\"ping3\":" + String(pingRangesCm[2], 3) + ",";
//  data += "\"ping4\":" + String(pingRangesCm[3], 3) + ",";
//  data += "\"temp\":" + String(lastTempValue, 3) + ",";
//  data += "\"light\":" + String(lastLDRValue);
//  data += "}";
//  Serial.println(data);
//}

//##########################################
// Lights
//##########################################
void initLights() {
//  signalLeft = false;
//  signalRight = false;

  pinMode(DS_PIN, OUTPUT);
  pinMode(STCP_PIN, OUTPUT);
  pinMode(SHCP_PIN, OUTPUT);

  testLights();
}

void testLights() {
  allLightsOff();
  delay(ONE_SECOND);
  allLightsOn();
  delay(ONE_SECOND);
  
  redLightsOn();
  delay(ONE_SECOND);
  redLightsOff();
  delay(ONE_SECOND);

  yellowLightsOn();
  delay(ONE_SECOND);
  yellowLightsOff();
  delay(ONE_SECOND);
  
  greenLightsOn();
  delay(ONE_SECOND);
  greenLightsOff();
  delay(ONE_SECOND);
  
  blueLightsOn();
  delay(ONE_SECOND);
  blueLightsOff();
  delay(ONE_SECOND);
  
  whiteLightsOn();
  delay(ONE_SECOND);
  whiteLightsOff();
  delay(ONE_SECOND);
  
//  delay(ONE_SECOND);
//  lightsOff();
//  delay(ONE_SECOND);
//  reverseLightsOn();
//  delay(ONE_SECOND);
//  reverseLightsOff();
//  delay(ONE_SECOND);
//  leftSignalLightOn(); 
//  delay(ONE_SECOND); 
//  rightSignalLightOn();
//  delay(ONE_SECOND);
}

void allLightsOff() {
  lights[RED_LED1] = LOW;
  lights[RED_LED2] = LOW;
  lights[RED_LED3] = LOW;
  lights[YELLOW_LED4] = LOW;
  lights[YELLOW_LED5] = LOW;
  lights[YELLOW_LED6] = LOW;
  lights[GREEN_LED7] = LOW;
  lights[GREEN_LED8] = LOW;
  lights[GREEN_LED9] = LOW;
  lights[BLUE_LED10] = LOW;
  lights[BLUE_LED11] = LOW;
  lights[BLUE_LED12] = LOW;
  lights[WHITE_LED13] = LOW;
  lights[WHITE_LED14] = LOW;
  lights[INFRARED_LED15] = LOW;
  lights[INFRARED_LED16] = LOW;
  updateLights(); 
}

void allLightsOn() {  
  lights[RED_LED1] = HIGH;
  lights[RED_LED2] = HIGH;
  lights[RED_LED3] = HIGH;
  lights[YELLOW_LED4] = HIGH;
  lights[YELLOW_LED5] = HIGH;
  lights[YELLOW_LED6] = HIGH;
  lights[GREEN_LED7] = HIGH;
  lights[GREEN_LED8] = HIGH;
  lights[GREEN_LED9] = HIGH;
  lights[BLUE_LED10] = HIGH;
  lights[BLUE_LED11] = HIGH;
  lights[BLUE_LED12] = HIGH;
  lights[WHITE_LED13] = HIGH;
  lights[WHITE_LED14] = HIGH;  
  updateLights();  
}

void redLightsOff() {
  lights[RED_LED1] = LOW;
  lights[RED_LED2] = LOW;
  lights[RED_LED3] = LOW;
  updateLights();  
}

void redLightsOn() {
  lights[RED_LED1] = HIGH;
  lights[RED_LED2] = HIGH;
  lights[RED_LED3] = HIGH;
  updateLights();  
}

void yellowLightsOff() {
  lights[YELLOW_LED4] = LOW;
  lights[YELLOW_LED5] = LOW;
  lights[YELLOW_LED6] = LOW;
  updateLights();  
}

void yellowLightsOn() {
  lights[YELLOW_LED4] = HIGH;
  lights[YELLOW_LED5] = HIGH;
  lights[YELLOW_LED6] = HIGH;
  updateLights();  
}

void greenLightsOff() {
  lights[GREEN_LED7] = LOW;
  lights[GREEN_LED8] = LOW;
  lights[GREEN_LED9] = LOW;
  updateLights();  
}

void greenLightsOn() {
  lights[GREEN_LED7] = HIGH;
  lights[GREEN_LED8] = HIGH;
  lights[GREEN_LED9] = HIGH;
  updateLights();  
}

void blueLightsOff() {
  lights[BLUE_LED10] = LOW;
  lights[BLUE_LED11] = LOW;
  lights[BLUE_LED12] = LOW;
  updateLights();  
}

void blueLightsOn() {
  lights[BLUE_LED10] = HIGH;
  lights[BLUE_LED11] = HIGH;
  lights[BLUE_LED12] = HIGH;
  updateLights();  
}

void whiteLightsOff() {
  lights[WHITE_LED13] = LOW;
  lights[WHITE_LED14] = LOW;
  updateLights();  
}

void whiteLightsOn() {
  lights[WHITE_LED13] = HIGH;
  lights[WHITE_LED14] = HIGH;  
  updateLights();  
}

void infraredOff() {
  lights[INFRARED_LED15] = LOW; 
  lights[INFRARED_LED16] = LOW;    
  updateLights(); 
}

void infraredOn() {
  lights[INFRARED_LED15] = HIGH; 
  lights[INFRARED_LED16] = HIGH;  
  updateLights(); 
}

void updateLights() {
  ////Pin connected to ST_CP of 74HC595
  //int latchPin = 8;
  ////Pin connected to SH_CP of 74HC595
  //int clockPin = 12;
  //////Pin connected to DS of 74HC595
  //int dataPin = 11;
  //
  ////  digitalWrite(STCP_PIN, LOW);
  ////  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  ////  digitalWrite(STCP_PIN, HIGH);

  digitalWrite(STCP_PIN, LOW);
  for (int i = LIGHT_COUNT-1; i >= 0; i--) {
    digitalWrite(SHCP_PIN, LOW);
    digitalWrite(DS_PIN, lights[i]);
    digitalWrite(SHCP_PIN, HIGH);
  }
  digitalWrite(STCP_PIN, HIGH);   
}
//##########################################
//##########################################

////##########################################
//// Motors
////##########################################
//void initMotors() {
//  steeringServo.attach(STEERING_PIN); 
//  pinMode(MOVE_A, OUTPUT);
//  pinMode(MOVE_B, OUTPUT);
//}
//
//void testMotors() {
//  stopMoving();  
//  delay(ONE_SECOND);
//  moveForward();
//  delay(ONE_SECOND);
//  moveBackward();
//  delay(ONE_SECOND);
//  turnLeft();
//  delay(ONE_SECOND);
//  turnRight();
//  delay(ONE_SECOND);
//  turnCenter();
//  stopMoving();  
//}
//
//void moveForward() {
//  reverseLightsOff();
//  digitalWrite(MOVE_A, HIGH);
//  digitalWrite(MOVE_B, LOW);
//}
//
//void moveBackward() {  
//  reverseLightsOn();
//  digitalWrite(MOVE_A, LOW);
//  digitalWrite(MOVE_B, HIGH);
//}
//
//void turnLeft() {
//  leftSignalLightOn();
//  steeringServo.write(0);
//}
//
//void turnRight() {
//  rightSignalLightOn();
//  steeringServo.write(180);
//}
//
//void turnCenter() {
//  signalLightsOff();
//  steeringServo.write(90);
//}
//
//void stopMoving() {
//  reverseLightsOff();
//  digitalWrite(MOVE_A, LOW);
//  digitalWrite(MOVE_B, LOW);  
//}
////##########################################
////##########################################

////##########################################
//// Ping sensor
////##########################################
//void initPingSensors() {
//  pingTimer[0] = millis() + 75;                      // First ping starts at 75ms, gives time for the Arduino to chill before starting.
//  for (uint8_t i = 1; i < SONAR_NUM; i++) {          // Set the starting time for each sensor.
//    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
//  }     
//}
//
//void readPingSensors() {
//  for (uint8_t i = 0; i < SONAR_NUM; i++) {       // Loop through all the sensors.
//    if (millis() >= pingTimer[i]) {               // Is it this sensor's time to ping?
//      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
//      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
//      currentSensor = i;                          // Sensor being accessed.
//      pingRangesCm[currentSensor] = 0;            // Make distance zero in case there's no ping echo for this sensor.
//      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
//    }
//  } 
//}
//
//void echoCheck() { 
//  if (sonar[currentSensor].check_timer()) { // If ping received, set the sensor distance to array.
//    pingRangesCm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
//  }
//}
////##########################################
////##########################################

//##########################################
// Temperature Sensor
//##########################################
void initTemperatureSensor() {
  sensors.begin();  
}

void fetchTemperature() {
  sensors.requestTemperatures();  
  lastTempValue = sensors.getTempCByIndex(0);
}
//##########################################
//##########################################

//##########################################
// LDR (light dependent resistor) Sensor
//##########################################
void initLDRSensor() {
  lastLDRValue = 0;
}

void fetchLDRValue() {
  lastLDRValue = analogRead(LDR_SENSOR_PIN);  
}
//##########################################
//##########################################
