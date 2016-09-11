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
  while(Serial.available() >= 6) {
    Serial.readBytes(command, 6);    
    Serial.readString(); // Clear out anything else.
    processCommand();
    displayStats();
  }
}

void processCommand() {
  int sensorPos = -1;
  if (command[0] == 'S') {
    sensorPos = toInt(command[1]);
    servos[sensorPos] = (toInt(command[2]) * 100) + (toInt(command[3]) * 10) + toInt(command[4]); // 0 to 180
    Serial.print("Servo ");
    Serial.println(sensorPos);
    Serial.println(toInt(command[2]) * 100);
    Serial.println(toInt(command[3]) * 10);
    Serial.println(toInt(command[4]) * 1);
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

void displayStats() {
  displayCommand();
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

