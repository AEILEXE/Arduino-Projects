#include <SoftwareSerial.h>


SoftwareSerial sim(10, 11);

int _timeout;
String _buffer;

String moduleNumber = "639763226980";          
String recipientNumber = "+639485957423";      


const int irSensorPin = 2;
const int isd1820PlayPin = 3;
const int isd1820RecPin = 4;

bool messageSent = false;
unsigned long motionStartTime = 0;
const unsigned long motionThreshold = 10000; 

void setup() {
  Serial.begin(9600);
  sim.begin(9600);
  _buffer.reserve(50);
  delay(1000);

  pinMode(irSensorPin, INPUT);
  pinMode(isd1820PlayPin, OUTPUT);
  pinMode(isd1820RecPin, OUTPUT);
  digitalWrite(isd1820PlayPin, LOW);
  digitalWrite(isd1820RecPin, LOW);

  Serial.println("System Initialized...");

  checkNetworkStatus();
}

void loop() {
  int irSensorState = digitalRead(irSensorPin);

  if (irSensorState == HIGH && !messageSent) {
    if (motionStartTime == 0) {
      motionStartTime = millis();  
      Serial.println("Motion detected! Starting 10-second timer...");
    } else if (millis() - motionStartTime >= motionThreshold) {
      Serial.println("Motion confirmed for 10 seconds.");
      playAlarm();
      SendMessage();
      messageSent = true;
      motionStartTime = 0; 
    }
  } else {
    motionStartTime = 0; 
        if (irSensorState == LOW) {
      messageSent = false; 
    }
  }

  if (sim.available()) {
    Serial.write(sim.read());
  }

  delay(100);
}

void playAlarm() {
  Serial.println("Playing custom alarm...");
  digitalWrite(isd1820PlayPin, HIGH);
  delay(1000); // Match to actual audio duration
  digitalWrite(isd1820PlayPin, LOW);
}

void SendMessage() {
  Serial.println("Sending SMS...");

  sim.println("AT+CMGF=1");  
  delay(500);
  sim.println("AT+CMGS=\"" + recipientNumber + "\"");
  delay(1000);

  sim.print("Someone is at the door. Motion has been detected.");
  delay(500);
  sim.println((char)26);  
  delay(5000);            

  _buffer = _readSerial();
  Serial.println("Response: " + _buffer);
}

String _readSerial() {
  _timeout = 0;
  while (!sim.available() && _timeout < 12000) {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
  return "";
}

void checkNetworkStatus() {
  Serial.println("Checking signal strength...");
  sim.println("AT+CSQ");
  delay(1000);
  while (sim.available()) Serial.write(sim.read());

  delay(1000);
  Serial.println("\nChecking network registration...");
  sim.println("AT+CREG?");
  delay(1000);
  while (sim.available()) Serial.write(sim.read());

  delay(1000);
  Serial.println("\nChecking SMSC number...");
  sim.println("AT+CSCA?");
  delay(1000);
  while (sim.available()) Serial.write(sim.read());
}
