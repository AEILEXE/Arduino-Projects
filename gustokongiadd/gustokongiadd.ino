void setup() {
  pinMode(12, OUTPUT);  // Sets pin 12 as an output pin
}

void loop() {
  digitalWrite(12, HIGH);  // Turns the LED on
  delay(1000);             // Waits for 1 second (1000 ms)
  digitalWrite(12, LOW);   // Turns the LED off
  delay(1000);             // Waits for 1 second
}
