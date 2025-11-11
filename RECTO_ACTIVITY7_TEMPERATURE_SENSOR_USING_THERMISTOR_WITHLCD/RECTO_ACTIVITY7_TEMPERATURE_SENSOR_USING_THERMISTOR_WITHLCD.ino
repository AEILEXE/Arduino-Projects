#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RT0 10000
#define B 3977
#define R 10000  

float RT, VR, ln, TX, T0, VRT;

LiquidCrystal_I2C lcd(0x27,  16, 2);

void setup() {
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.setCursor(0,1);                                                                                                                                                                                             

  Serial.begin(9600);

  T0 = 25 + 273.15;

  delay(700);
}
void loop() {

  lcd.clear();

  VRT = (5.00 / 1023.00) * analogRead(A0);
  
  VR = 5.00 - VRT;

  RT = (R * VRT) / VR;
  ln = log (RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0)));

  TX = TX - 273.15;
  
  
  lcd.setCursor(0, 0);  
  lcd.print(TX, 0);  
  lcd.print("C ");
  
  lcd.print(TX + 273.15, 2);  
  lcd.print("K ");
  
  float tempF = ((TX * 1.8) + 32);  
  lcd.print(TX, 0);  
  lcd.print("F ");

  Serial.print("Temperature: ");
  Serial.print(TX);                  
  Serial.print("C ");
  
  Serial.print(TX + 273.15, 2);
  Serial.print("K ");

  Serial.print((TX * 1.8) + 32);
  Serial.println("F ");
  
  delay(700);
  }