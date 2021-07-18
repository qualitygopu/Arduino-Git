#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <EEPROM.h>

#define UP 16736925
#define DOWN 16754775
#define RIGHT 16761405
#define LEFT 16720605
#define SELECT 16712445
#define SETTING 16732845
#define AMP A3

LiquidCrystal_I2C lcd(0x27, 16, 2);
IRrecv irrecv(11);
decode_results results;
RTC_DS1307 rtc;

void setup()
{
    EEPROM.begin();
    irrecv.enableIRIn();
    lcd.init();
    lcd.backlight();
    if (!rtc.begin())
    {
        lcd.clear();
        lcd.print(F("ERR 01")); // Couldn't find RTC
        abort();
    }
    if (!rtc.isrunning())
    {
        lcd.clear();
        lcd.print(F("ERR 02"));
        delay(2000);
    }
}

void loop()
{
    if (irrecv.decode(&results))
    {
        Serial.println(results.value);
        delay(200);
        irrecv.resume();
    }
}