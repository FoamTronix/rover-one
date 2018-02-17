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
  Serial.println(command);
  if(command == "LightsOn") {
    digitalWrite(13, HIGH);
    Serial.println('Lights On');
  } 
  else if(command == "LightsOff") {
    digitalWrite(13, LOW);
    Serial.println('Lights Off');
  }
}
