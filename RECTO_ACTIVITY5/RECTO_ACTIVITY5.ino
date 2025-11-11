#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Replaced '$' with '#' and added '>' at the end of the line

LiquidCrystal_I2C lcd(0x27, 16, 2); //Remove exceess variable as the supposed arguments for lcd() is lcd(address, cols, rows);

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

int x=2; //button is connected to digital pin 2
int val=0,led_num=0; // Added ';' at the end of the line

void setup() // Replaced 'l' with i
{
  lcd.begin(16,2);
  lcd.backlight();

  lcd.createChar(0, Heart);
  lcd.createChar(1, Bell);
  lcd.createChar(2, Alien);
}

void loop() // Replaced '0' with 'o'
{
  //read the value of the button when pressed
  val=digitalRead(x);

  if(val==1){ //button is pressed
    led_num=led_num+1; //add 1 to the led_num variable
  }

  if (led_num==1){
    lcd.clear();
    lcd.setCursor(0,0); // Replaced the word 'led' with 'lcd' and Added ';' at the end of the line
    lcd.write(0); // Replaced the word 'led' with 'lcd' and Added ')' and ';' at the end of the line
  }
  else if (led_num==2) { // Added '{' at the end of the line
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write(1);
  }
  else if (led_num==3){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write(2); // Added ')' and ';' at the end of the line

    //reset the led_num variable to 0
    led_num=0; // Replaced 'o' with '0' and added ';' at the end of the line
  }
  //wait 300 ms for the next reading
  delay(300); // Added ')' and ';' at the end of the line
 
}