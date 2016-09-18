// Ping Sensors
#include <NewPing.h>
#define SONAR_NUM     1 // Number or sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 100 // 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[SONAR_NUM];   // Holds the times when the next ping should happen for each sensor.
unsigned int pingRangesCm[SONAR_NUM]; // Where the ping distances are stored.
uint8_t currentSensor = 0;            // Keeps track of which sensor is active.

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(8, 9, MAX_DISTANCE)    // TriggerPin, EchoPin, MaxDistance
};

// Lights
int lightFrontRight = 2;
int lightFrontLeft = 3;
int lightRearRight = 4;
int lightRearLeft = 5;
int lightReverseRight = 6;
int lightReverseLeft = 7;

void setup() {
  pinMode(lightFrontRight, OUTPUT);
  pinMode(lightFrontLeft, OUTPUT);
  pinMode(lightRearRight, OUTPUT);
  pinMode(lightRearLeft, OUTPUT);
  pinMode(lightReverseRight, OUTPUT);
  pinMode(lightReverseLeft, OUTPUT);
  digitalWrite(lightFrontRight, HIGH);
  digitalWrite(lightFrontLeft, HIGH);
  digitalWrite(lightRearRight, HIGH);
  digitalWrite(lightRearLeft, HIGH);
  digitalWrite(lightReverseRight, HIGH);
  digitalWrite(lightReverseLeft, HIGH);

  // Ping Sensors
  pingTimer[0] = millis() + 75;             // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) { // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
  }    
}

void loop() {
  readPingSensors();
  displayStatus();    
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

void displayStatus() {
//  Serial.print(i);
//  Serial.print("=");
//  Serial.print(pingRangesCm[i]);
//  Serial.print("cm ");
}

