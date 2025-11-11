#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Bonezegei_DS3231.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set I2C address and LCD size
Bonezegei_DS3231 rtc(0x68);

void setup() {
  lcd.begin(16, 2);  // Fix: Add rows and columns
  lcd.backlight();
  
  rtc.begin();
  rtc.setFormat(12);
  rtc.setAMPM(1);
  rtc.setTime("10:26:05");  
  rtc.setDate("04/02/25");  
}

void loop() {
  if (rtc.getTime()) {
    lcd.clear();

    lcd.setCursor(3, 0);
    lcd.print(twoDigit(rtc.getMonth()));
    lcd.print("/");
    lcd.print(twoDigit(rtc.getDate()));
    lcd.print("/20");
    lcd.print(rtc.getYear());

    lcd.setCursor(4, 1);
    lcd.print(twoDigit(rtc.getHour()));
    lcd.print(":");
    lcd.print(twoDigit(rtc.getMinute()));
    lcd.print(":");
    lcd.print(twoDigit(rtc.getSeconds()));

  }
  delay(1000);
}

String twoDigit(int number) {
  return (number < 10) ? "0" + String(number) : String(number);
}

