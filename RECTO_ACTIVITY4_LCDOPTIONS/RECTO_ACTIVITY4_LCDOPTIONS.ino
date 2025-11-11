#include <Wire.h>
#include <LiquidCrystal_I2C.h> 

LiquidCrystal_I2C lcd(0x27, 16, 2); 

int x=2; 
int val=0,led_num=0;

void setup() 
{
  lcd.begin(16,2);
  lcd.backlight();
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
    lcd.print(" Mode 1 ");
  }
  else if (led_num==2) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Mode 2 ");
  }
  else if (led_num==3){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Mode 3 "); 

    led_num=0;
  }

  delay(300);
}