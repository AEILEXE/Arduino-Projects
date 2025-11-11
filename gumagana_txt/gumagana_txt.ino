#include <SoftwareSerial.h>

SoftwareSerial sim(10, 11);
int _timeout;
String _buffer;
String moduleNumber = "639763226980";      // SIM module's own number
String recipientNumber = "+639485957423";  // Number to receive SMS

void setup() {
  Serial.begin(9600);
  sim.begin(9600);
  _buffer.reserve(50);
  delay(1000);

  Serial.println("System Started...");
  checkNetworkStatus();
  delay(1000);
  SendMessage();  // Automatically send SMS on startup
}

void loop() {
  // No need for loop action
}

// --------------------- SEND SMS FUNCTION ---------------------
void SendMessage() {
  Serial.println("Sending SMS...");
  sim.println("AT+CMGF=1");  // Text mode
  delay(200);

  sim.println("AT+CMGS=\"" + recipientNumber + "\"");  // Recipient number
  delay(200);

  sim.print("Hello, how are you? Greetings from SIM800L.");
  delay(200);

  sim.write(26);  // CTRL+Z
  delay(5000);    // Wait for message to send

  _buffer = _readSerial();
  Serial.println("Response: " + _buffer);
}

// --------------------- READ SMS FUNCTION ---------------------
void RecieveMessage() {
  Serial.println("SIM800L Read an SMS");
  sim.println("AT+CMGF=1");
  delay(200);
  sim.println("AT+CNMI=1,2,0,0,0");  // Live message forwarding
  delay(200);
  Serial.println("Unread Message setup done");
}

// --------------------- CALL FUNCTION ---------------------
void callNumber() {
  Serial.println("Dialing number...");
  sim.print(F("ATD"));
  sim.print(moduleNumber);
  sim.print(F(";\r\n"));
  _buffer = _readSerial();
  Serial.println("Call Response: " + _buffer);
}

// --------------------- SERIAL READER ---------------------
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

// --------------------- NETWORK CHECKS ---------------------
void checkNetworkStatus() {
  Serial.println("Checking Signal Strength (AT+CSQ):");
  sim.println("AT+CSQ");
  delay(500);
  while (sim.available()) {
    Serial.write(sim.read());
  }

  delay(500);

  Serial.println("\nChecking Network Registration (AT+CREG?):");
  sim.println("AT+CREG?");
  delay(500);
  while (sim.available()) {
    Serial.write(sim.read());
  }

  delay(500);

  Serial.println("\nChecking SMSC Number (AT+CSCA?):");
  sim.println("AT+CSCA?");
  delay(500);
  while (sim.available()) {
    Serial.write(sim.read());
  }
}
