#include <Wire.h>
#include <LiquidCrystal_I2C.h> 

LiquidCrystal_I2C lcd(0x27, 16, 2); 

byte Heart[8] = {
0b00000,
0b01010,
0b11111,
0b11111,
0b01110,
0b00100,
0b00000,
0b00000
};

byte Bell[8] = {
0b00100,
0b01110,
0b01110,
0b01110,
0b11111,
0b00000,
0b00100,
0b00000
};

byte Alien[8] = {
0b11111,
0b10101,
0b11111,
0b11111,
0b01110,
0b01010,
0b11011,
0b00000

};

int x=2; 
int val=0,led_num=0; 

void setup() {
  lcd.begin(16,2);
  lcd.backlight();

  lcd.createChar(0, Heart);
  lcd.createChar(1, Bell);
  lcd.createChar(2, Alien);
}

void loop() 
{
  
  val=digitalRead(x);

  if(val==1){
    led_num=led_num+1; 
  }

  if (led_num==1){
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.write(0); 
  }
  else if (led_num==2) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write(1);
  }
  else if (led_num==3){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write(2); 
  
    led_num=0; 
  }
  delay(300);
 
}