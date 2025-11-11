#include <SoftwareSerial.h>

// Pin Definitions
const int irSensorPin = 2;         // IR sensor connected to digital pin 2
const int isd1820PlayPin = 3;      // FT pin of ISD1820 (Play Trigger)
const int isd1820RecPin = 4;       // REC pin of ISD1820 (Record Button)

SoftwareSerial sim800(10, 11);     // RX (D10), TX (D11) for SIM800L

bool messageSent = false;          // Flag to avoid sending SMS repeatedly

void setup() {
  // Set up pin modes
  pinMode(irSensorPin, INPUT);         
  pinMode(isd1820PlayPin, OUTPUT);     
  pinMode(isd1820RecPin, OUTPUT);      

  digitalWrite(isd1820PlayPin, LOW);
  digitalWrite(isd1820RecPin, LOW);

  // Serial monitor
  Serial.begin(9600);
  Serial.println("Customizable Alarm System Initialized");

  // SIM800L Serial
  sim800.begin(9600);
  delay(1000);
  sim800.println("AT");            // Initialize SIM800L
  delay(1000);
  sim800.println("AT+CMGF=1");     // Set SMS to Text Mode
  delay(1000);
}

void loop() {
  int irSensorState = digitalRead(irSensorPin);

  if (irSensorState == HIGH) {
    Serial.println("Motion detected! Playing custom alarm...");
    playAlarm();

    if (!messageSent) {
      sendSMS();                   // Send SMS only once per detection
      messageSent = true;
    }
  } else {
    messageSent = false;          // Reset flag when no motion
  }

  delay(200); // Stability
}

void recordAlarm() {
  Serial.println("Recording started. Speak now...");
  digitalWrite(isd1820RecPin, HIGH);
  delay(10000);
  digitalWrite(isd1820RecPin, LOW);
  Serial.println("Recording complete. Ready to play.");
}

void playAlarm() {
  digitalWrite(isd1820PlayPin, HIGH);
  delay(1000); // Adjust to match audio length
  digitalWrite(isd1820PlayPin, LOW);
  delay(2000); // Cooldown
}

void sendSMS() {
  Serial.println("Sending SMS...");
  sim800.println("AT+CMGS=\"+63xxxxxxxxxx\""); // Replace with actual phone number
  delay(1000);
  sim800.print("Motion detected! Please check the area.");
  delay(500);
  sim800.write(26); // ASCII code for CTRL+Z to send SMS
  delay(5000);
  Serial.println("SMS sent.");
}
