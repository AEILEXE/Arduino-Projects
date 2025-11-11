#define RT0 10000
#define B 3977
#define R 10000  

float RT, VR, ln, TX, T0, VRT;

void setup() {
  Serial.begin(9600);
  T0 = 1 + 273.15;

}
void loop() {
  VRT = (5.00 / 1023.00) * analogRead(T0);
  
  VR = 5.00 - VRT;

  RT = (R * VRT) / VR;
  ln = log (RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0)));

  TX = TX - 273.15;
  
  Serial.print("Temperature: ");
  Serial.print(TX);                  
  Serial.print("C ");

  Serial.print(TX + 273.15, 2);
  Serial.print("K ");

  Serial.print((TX * 1.8) + 32);
  Serial.println("F ");
  
  delay(1000);
  }