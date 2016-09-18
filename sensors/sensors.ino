// Ping Sensors
#include <NewPing.h>
#define SONAR_NUM     1 // Number or sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 100 // 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[SONAR_NUM];   // Holds the times when the next ping should happen for each sensor.
unsigned int pingRangesCm[SONAR_NUM]; // Where the ping distances are stored.
uint8_t currentSensor = 0;            // Keeps track of which sensor is active.

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(8, 7, MAX_DISTANCE)    // TriggerPin, EchoPin, MaxDistance
};


// Servos
#include <Servo.h>
Servo servoPan;  // Servo 0 on pin 9
Servo servoTilt; // Servo 1 on pin 10
int servos[2] = { 90, 90 }; // Pan, Title

// Lights
char command[6];
int lightOnePin = 3; // 0
int lightTwoPin = 4; // 1
int lightPinOffset = 3;
int lights[2] = { 0, 0 };

void setup() {
  Serial.begin(9600);   // opens serial port, sets data rate to 9600 bps

  // Ping Sensors
  pingTimer[0] = millis() + 75;             // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) { // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
  }  

  // Servos
  servoPan.attach(9);
  servoTilt.attach(10);

  // Lights
  pinMode(lightOnePin, OUTPUT);
  pinMode(lightTwoPin, OUTPUT);
  digitalWrite(lightOnePin, LOW);
  digitalWrite(lightTwoPin, LOW);
}

// Commands are sent using a property followed by its value represented in two bytes.
// Multiple commands can be sent at the same time, but only two bytes at a time will be used.
// [Property][Position][Value][Property][Value][Position][Property][Value]
void loop() {
  readPingSensors();      // Always read and store the ping sensor values
  displayPingSensors();
  while(Serial.available() >= 6) {
    Serial.readBytes(command, 6);    
    Serial.readString(); // Clear out anything else.
    processCommand();
    displayStats();
  }
}

void processCommand() {
  int sensorPos = -1;
  if (command[0] == 'P') {
    String pingRanges = "";
    for(unsigned int i = 0; i < sizeof(pingRangesCm); i++) {
       pingRanges += printf("%03d", pingRangesCm[i]);
    }
    Serial.print(pingRanges);
  } else if (command[0] == 'S') {
    sensorPos = toInt(command[1]);
    servos[sensorPos] = (toInt(command[2]) * 100) + (toInt(command[3]) * 10) + toInt(command[4]); // 0 to 180
    if (sensorPos == 0) { // Pan
      servoPan.write(servos[sensorPos]); 
    } else if (sensorPos == 1) { // Tilt
      servoTilt.write(servos[sensorPos]);
    }   
  } else if (command[0] == 'L') {
    sensorPos = toInt(command[1]);
    lights[sensorPos] = toInt(command[2]); // 0 or 1
    if (lights[sensorPos] == 1) {
      digitalWrite(sensorPos + lightPinOffset, HIGH);
    } else {
      digitalWrite(sensorPos + lightPinOffset, LOW);
    }
  } else {
    Serial.println("Discarded unknown command");
  }
}

void readPingSensors() {
  for (uint8_t i = 0; i < SONAR_NUM; i++) {       // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {               // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      // if (i == 0 && currentSensor == SONAR_NUM - 1) displayPingSensors(); // Sensor ping cycle complete, do something with the results.
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      pingRangesCm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  } 
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer()) {
    pingRangesCm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
  }
}

void displayStats() {
  displayCommand();
  displayPingSensors();
  displayServos();
  displayLights();  
  clearTerminal();
}

void clearTerminal() {
  for(int i = 0; i< 20; i++) {
    Serial.println('\n');
  }
}

void displayCommand() {
  Serial.print("Command Received: ");
  for(int i = 0; i < 5; i++) {
    Serial.print(command[i]);
  }
  Serial.println();
  Serial.println("******");
}

void displayPingSensors() { // Sensor ping cycle complete, do something with the results.
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    Serial.print(i);
    Serial.print("=");
    Serial.print(pingRangesCm[i]);
    Serial.print("cm ");
  }
  Serial.println();
}

void displayServos() {
  Serial.print("Servo Pan (S0): ");
  Serial.println(servos[0]);
  Serial.print("Servo Tilt (S1): ");
  Serial.println(servos[1]);
  Serial.println("******");
}

void displayLights() {
  Serial.print("Light 1 (L0): ");
  Serial.println(lights[0] == 0 ? " (OFF)" : " (On)");
  Serial.print("Light 2 (L1): ");
  Serial.println(lights[1] == 0 ? " (OFF)" : " (On)");  
  Serial.println("******");
}

int toInt(char cmd) {
  return cmd - '0';
}

