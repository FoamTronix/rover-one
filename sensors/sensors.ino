// Servos
#include <Servo.h>
Servo servoPan;  // 0
Servo servoTilt; // 1
int servos[2] = { 90, 90 }; // Pan, Title

// Lights
char command[4];
int lightOnePin = 3; // 0
int lightTwoPin = 4; // 1
int lightPinOffset = 3;
int lights[2] = { 0, 0 };

void setup() {
  Serial.begin(9600);   // opens serial port, sets data rate to 9600 bps

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
  while(Serial.available() >= 4) {
    //clearTerminal();
    Serial.readBytes(command, 4);
    //Serial.readString(); // Clear out anything else.
    displayCommand();
    processCommand();
    displayServos();
    displayLights();
  }
}

void clearTerminal() {
  for(int i = 0; i< 20; i++) {
    Serial.println('\n');
  }
}

void displayCommand() {
  Serial.print("Command Received: ");
  for(int i = 0; i < 3; i++) {
    Serial.print(command[i]);
  }
  Serial.println();
}

void processCommand() {
  int sensorPos = -1;
  if (command[0] == 'S') {
    sensorPos = toInt(command[1]);
    servos[sensorPos] = toInt(command[2]); // 0 to 180
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

void displayServos() {
  Serial.print("Servo Pan (S0): ");
  Serial.print(servos[0]);
  Serial.print("Servo Tilt (S1): ");
  Serial.print(servos[1]);
  Serial.println("******");
}

void displayLights() {
  Serial.print("Light 1 (L0): ");
  Serial.print(lights[0] == 0 ? " (OFF)" : " (On)");
  Serial.print("Light 2 (L1): ");
  Serial.print(lights[1] == 0 ? " (OFF)" : " (On)");  
  Serial.println("******");
}

int toInt(char cmd) {
  return cmd - '0';
}

