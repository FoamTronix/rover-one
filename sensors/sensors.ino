char command[4];
int lightOnePin = 3; // 0
int lightTwoPin = 4; // 1
int lightPinOffset = 3;
int lights[2] = {0, 0};

void setup() {
  Serial.begin(9600);   // opens serial port, sets data rate to 9600 bps
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
  int lightPos = -1;
  if (command[0] == 'L') {
    lightPos = toInt(command[1]);
    lights[lightPos] = toInt(command[2]);
    if (lights[lightPos] == 1) {
      digitalWrite(lightPos + lightPinOffset, HIGH);
    } else {
      digitalWrite(lightPos + lightPinOffset, LOW);
    }
  } else {
    Serial.println("Discarded unknown command");
  }
}

void displayLights() {
  for(int i = 0; i < 2; i++) {
    Serial.print("Light ");
    Serial.print(i); 
    Serial.print(": ");
    Serial.print(lights[i]);
    if (lights[i] == 0) {
      Serial.println(" (Off)");
    } else {
      Serial.println(" (On)");
    }
  }
  Serial.println("******");
}

int toInt(char cmd) {
  return cmd - '0';
}

