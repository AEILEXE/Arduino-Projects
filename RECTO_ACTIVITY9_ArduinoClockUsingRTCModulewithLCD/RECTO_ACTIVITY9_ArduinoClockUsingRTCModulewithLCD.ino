#include <Bonezegei_DS3231.h>
#include <LiquidCrystal_I2C.h>
#include  <Wire.h>

Bonezegei_DS3231 rtc(0x68);
LiquidCrystal_I2C lcd(0x27,  16, 2);

void setup () {
  Serial.begin(9600); 
  rtc.begin();
  rtc.setFormat("12");    // Set 12 Hours Format
  rtc.setAMPM("1");        //Set AM or PM    0 = AM  1 =PM
  rtc.setTime("06:28:00");  //Set Time    Hour:Minute:Seconds
  rtc.setDate("03/27/25");     //Set Date    Month/Date/Year

    //initialize lcd screen
  lcd.init();
  // turn on the backlight
  lcd.backlight();

}

void loop() {
  if (rtc.getTime()) { 
    lcd.setCursor(0,1);
    lcd.print("   ");
    lcd.print(twoDigit(rtc.getDate()));
    lcd.print("/");
    lcd.print(twoDigit(rtc.getMonth()));
    lcd.print("/");
    lcd.print("2025" );

 lcd.setCursor(0,0);
    lcd.print("   ");
    lcd.print(twoDigit(rtc.getHour()));
    lcd.print(":");
    lcd.print(twoDigit(rtc.getMinute()));
    lcd.print(":");
    lcd.print(twoDigit(rtc.getSeconds()));

    if (rtc.getFormat() == 12) {  // returns 12 or 24 hour format
      if (rtc.getAMPM()) {    //return 0 = AM  1 = PM
        lcd.print(" PM");
      } else {
        lcd.print(" AM");
      }
    } else {
      lcd.print("");  // Print new line if 24-hour format
    }
  }




  delay(1000);
}

String twoDigit(int number) {
  if (number < 10) {  
    return "0" + String(number);  
  } else {
    return String(number);  
  }
}

