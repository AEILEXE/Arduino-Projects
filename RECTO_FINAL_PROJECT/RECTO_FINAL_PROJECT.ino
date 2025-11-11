#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial sim(10, 11); // RX, TX

int _timeout;
String _buffer;

String moduleNumber = "639763226980";
String recipientNumber = "+639098789482";

const int irSensorPin = 2;
const int buzzerPin = 8;

bool messageSent = false;

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  Serial.begin(9600);
  sim.begin(9600);
  _buffer.reserve(50);
  delay(1000);

  pinMode(irSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("<ANNYEONG>");
  lcd.setCursor(0, 1);
  lcd.print("YOU CAN GET HERE");

  Serial.println("System Initialized...");
  checkNetworkStatus();
}

void loop() {
  if (!messageSent && digitalRead(irSensorPin) == HIGH) {
    Serial.println("Motion started.");

    unsigned long startTime = millis();
    bool sustained = true;

    while (millis() - startTime < 2000) {
      if (digitalRead(irSensorPin) == LOW) {
        sustained = false;
        break;
      }
      delay(100);
    }

    if (sustained) {
      Serial.println("Motion sustained. Triggering alarm and SMS.");
      playDoorbellTone();
      SendMessage();
      messageSent = true;
    }
  }

  if (digitalRead(irSensorPin) == LOW) {
    messageSent = false;
  }

  if (sim.available()) {
    Serial.write(sim.read());
  }

  delay(200);
}

void playDoorbellTone() {
  Serial.println("Playing doorbell tone...");

  tone(buzzerPin, 1047);
  delay(300);
  noTone(buzzerPin);
  delay(100);
  tone(buzzerPin, 784);
  delay(700);
  tone(buzzerPin, 1047);
  delay(300);
  noTone(buzzerPin);
  delay(100);
  tone(buzzerPin, 784);
  delay(700);
  tone(buzzerPin, 1047);
  delay(300);
  noTone(buzzerPin);
  delay(100);
  tone(buzzerPin, 784);
  delay(700);
  noTone(buzzerPin);
}

void SendMessage() {
  Serial.println("Sending SMS...");

  sim.println("AT+CMGF=1");
  delay(500);
  sim.println("AT+CMGS=\"" + recipientNumber + "\"");
  delay(1000);
  sim.print("A person just passed by the sensor.");
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
  delay(1000);
  while (sim.available()) Serial.write(sim.read());

  delay(1000);
  Serial.println("\nChecking network registration...");
  delay(1000);
  while (sim.available()) Serial.write(sim.read());

  delay(1000);
  Serial.println("\nChecking SMSC number...");
  delay(1000);
  while (sim.available()) Serial.write(sim.read());
}
