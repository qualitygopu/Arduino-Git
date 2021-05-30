#include <Wire.h> 
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define BUT1    12
#define BUT2    13
#define BUT3    14

LiquidCrystal_I2C lcd(0x27,20,4);

void setup()
{
    pinMode(BUT1, INPUT_PULLUP);
    pinMode(BUT2, INPUT_PULLUP);
    pinMode(BUT3, INPUT_PULLUP);


    lcd.init();
    lcd.backlight();
    lcd.setCursor(3,0);
    lcd.print(F("Hello, world!"));
    lcd.clear();
}

void loop()
{
  if (digitalRead(BUT1) == LOW)
  {
    lcd.setCursor(0,0);
    lcd.print("Button 1   ");
  }
  else if (digitalRead(BUT2) == LOW)
  {
    lcd.setCursor(0,0);
    lcd.print("Button 2   ");
  }
  else if (digitalRead(BUT3) == LOW)
  {
    lcd.setCursor(0,0);
    lcd.print("Button 3   ");
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("Not Press  ");
  }

	
}
