#include <SoftwareSerial.h>

// SIM800L Serial connection
SoftwareSerial sim(10, 11); // RX, TX

int _timeout;
String _buffer;

String moduleNumber = "639763226980";          // SIM module's own number
String recipientNumber = "+639485957423";      // Recipient of the SMS

// IR Sensor and ISD1820 Voice Module
const int irSensorPin = 2;        // IR motion sensor input pin
const int isd1820PlayPin = 3;     // ISD1820 FT (play) pin
const int isd1820RecPin = 4;      // ISD1820 REC pin (optional if you want to record via code)

bool messageSent = false;

void setup() {
  // Setup Serial and GSM
  Serial.begin(9600);
  sim.begin(9600);
  _buffer.reserve(50);
  delay(1000);

  // Setup sensor and ISD1820 pins
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
    Serial.println("Motion detected! Waiting 5 seconds...");
    delay(5000); // Wait 10 seconds

    // Confirm again before triggering
    if (digitalRead(irSensorPin) == HIGH) {
      playAlarm();
      SendMessage();
      messageSent = true;
    }
  }

  if (irSensorState == LOW) {
    messageSent = false; // Reset to allow new detection
  }

  // Print responses from SIM module
  if (sim.available()) {
    Serial.write(sim.read());
  }

  delay(200);
}

// Function to play the recorded voice
void playAlarm() {
  Serial.println("Playing custom alarm...");
  digitalWrite(isd1820PlayPin, HIGH);
  delay(4000); // Play for 3 seconds (adjust as needed)
  digitalWrite(isd1820PlayPin, LOW);
}

// Function to send SMS
void SendMessage() {
  Serial.println("Sending SMS...");

  sim.println("AT+CMGF=1");  // Set text mode
  delay(500);
  sim.println("AT+CMGS=\"" + recipientNumber + "\"");  // Set recipient
  delay(1000);  // Wait for '>' prompt

  sim.print("Someone is at the door. Motion has been detected.");
  delay(500);
  sim.println((char)26);  // CTRL+Z to send
  delay(5000);            // Wait for transmission

  _buffer = _readSerial();
  Serial.println("Response: " + _buffer);
}

// Function to read response from SIM800L
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

// Diagnostics to confirm SIM800L is connected and ready
void checkNetworkStatus() {
  Serial.println("Checking signal strength...");
//  sim.println("AT+CSQ");
  delay(1000);
  while (sim.available()) Serial.write(sim.read());

  delay(1000);
  Serial.println("\nChecking network registration...");
 // sim.println("AT+CREG?");
  delay(1000);
  while (sim.available()) Serial.write(sim.read());

  delay(1000);
  Serial.println("\nChecking SMSC number...");
 // sim.println("AT+CSCA?");
  delay(1000);
  while (sim.available()) Serial.write(sim.read());
}
