String command;

void setup() {
  Serial.begin(9600); 
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() {
  if(Serial.available() > 0) {
    command = Serial.readStringUntil('\n');
    processCommand();
  }
}

void processCommand() {
  if(command == "LightsOn") { digitalWrite(13, HIGH); } 
  else if(command == "LightsOff") { digitalWrite(13, LOW); }
}


// Lights
#define ONE_SECOND 1000
#define DS_PIN 8
#define STCP_PIN 9
#define SHCP_PIN 10
boolean lights[8];

// Motors
#define STEERING_A 2
#define STEERING_B 3
#define MOVE_A 4
#define MOVE_B 5

// Ping Sensors
#include <NewPing.h>
#define SONAR_NUM     1 // Number or sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 100 // 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).
#define TRIGGER_PIN 6
#define ECHO_PIN 7

unsigned long pingTimer[SONAR_NUM];   // Holds the times when the next ping should happen for each sensor.
unsigned int pingRangesCm[SONAR_NUM]; // Where the ping distances are stored.
uint8_t currentSensor = 0;            // Keeps track of which sensor is active.

NewPing sonar[SONAR_NUM] = {                    // Sensor object array.
  NewPing(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE) // TriggerPin, EchoPin, MaxDistance
};

// Logic
#define START_DELAY 3000
#define MIN_COLLISION_DISTANCE 30 // In centimeters 
#define EVASIVE_MANEUVERS_OFFSET_DISTANCE 10 // In centimeters 
#define GO_LEFT 1
#define GO_RIGHT 2
#define BACKUP_TIME 1000
boolean runningEvasiveManeuvers = false;

void setup() {
  // Lights
  pinMode(DS_PIN, OUTPUT);
  pinMode(STCP_PIN, OUTPUT);
  pinMode(SHCP_PIN, OUTPUT);

  // Motors
  pinMode(STEERING_A, OUTPUT);
  pinMode(STEERING_B, OUTPUT);
  pinMode(MOVE_A, OUTPUT);
  pinMode(MOVE_B, OUTPUT);
  
  // Ping Sensors
  Serial.begin(9600);   // opens serial port, sets data rate to 9600 bps
  pingTimer[0] = millis() + 75;             // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) { // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
  }    

  testLights();
  testMotors(); 
  delay(START_DELAY);
  noTurn();
  moveForward();
}

void loop() { 
  readPingSensors();
  displayStatus();
  if (runningEvasiveManeuvers) {
    evasiveManeuversCheck();
  } else if (collisionCheck()) {
    evasiveManeuvers();
  }
}

//##########################################
// Logic
//##########################################

boolean collisionCheck() {
  return pingRangesCm[0] < MIN_COLLISION_DISTANCE;
}

void evasiveManeuvers() {
  runningEvasiveManeuvers = true;
  stopMoving();
  randomDirection() == GO_LEFT ? turnLeft() : turnRight();
  reverseLightsOn();
  moveBackward();
}

void evasiveManeuversCheck() {
  if (pingRangesCm[0] >= (MIN_COLLISION_DISTANCE + EVASIVE_MANEUVERS_OFFSET_DISTANCE)) {
    runningEvasiveManeuvers = false;
    noTurn();
    reverseLightsOff();
    moveForward();
  }
}

int randomDirection() {
  return random(10) % 2 == 0 ? GO_LEFT : GO_RIGHT;
}

//##########################################
//##########################################

//##########################################
// Lights
//##########################################
void testLights() {
  allLightsOff();
  delay(ONE_SECOND);
  lightsOn();
  delay(ONE_SECOND);
  lightsOff();
  delay(ONE_SECOND);
  reverseLightsOn();
  delay(ONE_SECOND);
  reverseLightsOff();
//  delay(ONE_SECOND);
//  LeftTurnLightOn(); 
//  delay(ONE_SECOND); 
//  RightTurnLightOn();
//  delay(ONE_SECOND);
//  RightTurnLightOff();
}

void allLightsOff() {
  lights[0] = LOW; // Front left
  lights[1] = LOW; // Front right
  lights[2] = LOW; // Rear left
  lights[3] = LOW; // Rear right  
  lights[4] = LOW; // Reverse left
  lights[5] = LOW; // Reverse right  
  lights[6] = LOW; // Turn left
  lights[7] = LOW; // Turn right  
  updateLights(); 
}

void lightsOn() {
  lights[0] = HIGH; // Front left
  lights[1] = HIGH; // Front right
  lights[2] = HIGH; // Rear left
  lights[3] = HIGH; // Rear right
  updateLights();  
}

void lightsOff() {
  lights[0] = LOW; // Front left
  lights[1] = LOW; // Front right
  lights[2] = LOW; // Rear left
  lights[3] = LOW; // Rear right
  updateLights();  
}

void reverseLightsOn() {
  lights[4] = HIGH; // Reverse left
  lights[5] = HIGH; // Reverse right
  updateLights();  
}

void reverseLightsOff() {
  lights[4] = LOW; // Reverse left
  lights[5] = LOW; // Reverse right
  updateLights();  
}

//void LeftTurnLightOn() {
//  lights[6] = HIGH; // Turn left
//  lights[7] = LOW;  // Turn right
//  updateLights();  
//}
//
//void LeftTurnLightOff() {
//  lights[6] = LOW; // Turn left
//  updateLights();  
//}
//
//void RightTurnLightOn() {
//  lights[6] = LOW; // Turn left
//  lights[7] = HIGH; // Turn right
//  updateLights();  
//}
//
//void RightTurnLightOff() {
//  lights[7] = LOW; // Turn right
//  updateLights();  
//}

void updateLights() {
  digitalWrite(STCP_PIN, LOW);
  for (int i = 7; i >= 0; i--) {
    digitalWrite(SHCP_PIN, LOW);
    digitalWrite(DS_PIN, lights[i]);
    digitalWrite(SHCP_PIN, HIGH);
  }
  digitalWrite(STCP_PIN, HIGH);   
}
//##########################################
//##########################################

//##########################################
// Motors
//##########################################
void testMotors() {
  stopMoving();  
  delay(ONE_SECOND);
  moveForward();
  delay(ONE_SECOND);
  moveBackward();
  delay(ONE_SECOND);
  turnLeft();
  delay(ONE_SECOND);
  turnRight();
  delay(ONE_SECOND);
  noTurn();
  stopMoving();  
}

void moveForward() {
  digitalWrite(MOVE_A, HIGH);
  digitalWrite(MOVE_B, LOW);
}

void moveBackward() {  
  digitalWrite(MOVE_A, LOW);
  digitalWrite(MOVE_B, HIGH);
}

void noTurn() {
  digitalWrite(STEERING_A, LOW);
  digitalWrite(STEERING_B, LOW);
}

void turnLeft() {
  digitalWrite(STEERING_A, HIGH);
  digitalWrite(STEERING_B, LOW);
}

void turnRight() {
  digitalWrite(STEERING_A, LOW);
  digitalWrite(STEERING_B, HIGH);  
}

void stopMoving() {
  digitalWrite(MOVE_A, LOW);
  digitalWrite(MOVE_B, LOW);  
  digitalWrite(STEERING_A, LOW);
  digitalWrite(STEERING_B, LOW); 
}
//##########################################
//##########################################

//##########################################
// Ping sensor
//##########################################
void readPingSensors() {
  for (uint8_t i = 0; i < SONAR_NUM; i++) {       // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {               // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      //pingRangesCm[currentSensor] = 0;            // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  } 
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer()) {
    pingRangesCm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
  }
}

void displayStatus() {
  Serial.print("Ping =");
  Serial.print(pingRangesCm[0]);
  Serial.print("cm ");
  Serial.println("\n");
}
//##########################################
//##########################################
