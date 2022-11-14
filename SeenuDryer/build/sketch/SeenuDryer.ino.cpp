#include <Arduino.h>
#line 1 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino"
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include "LcdKeypad.h"
#include "MenuManager.h"
// #include "MenuData.h"
#include "Config.h"

#define SENSOR1 2
#define SENSOR2 3
#define SENSOR3 4
#define SENSOR4 5
#define SENSOR5 6
#define SENSOR6 7

#define RELAY1 9
#define RELAY2 10
#define RELAY3 11
#define RELAY4 9
#define RELAY5 9
#define RELAY6 9

LiquidCrystal_I2C lcd(0x27, 20, 4);
MenuManager Menu1;
Config config;

char strbuf[LCD_COLS + 1];
char intbuf[8];

enum AppModeValues
{
    APP_NORMAL_MODE,
    APP_MENU_MODE,
    APP_PROCESS_MENU_CMD,
    APP_CHANT_MODE,
};
byte appMode = APP_NORMAL_MODE;

void refreshMenuDisplay(byte refreshMode);
byte getNavAction();
byte processMenuCommand(byte cmdId);

#line 42 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino"
double readCelsius(uint8_t cs);
#line 61 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino"
void setup();
#line 85 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino"
void loop();
#line 42 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino"
double readCelsius(uint8_t cs) {
    uint16_t v;

    digitalWrite(cs, LOW);
    v = SPI.transfer(0x00);
    v <<= 8;
    v |= SPI.transfer(0x00);
    digitalWrite(cs, HIGH);

    if (v & 0x4) {
        // uh oh, no thermocouple attached!
        return NAN; 
    }

    v >>= 3;

    return v*0.25;
}

void setup()
{
    SPI.begin();
	pinMode(SENSOR1, OUTPUT);
    pinMode(SENSOR2, OUTPUT);
    pinMode(SENSOR3, OUTPUT);
    pinMode(SENSOR4, OUTPUT);
    pinMode(SENSOR5, OUTPUT);
    pinMode(SENSOR6, OUTPUT);
    digitalWrite(SENSOR1, HIGH);
    digitalWrite(SENSOR2, HIGH);
    digitalWrite(SENSOR3, HIGH);
    digitalWrite(SENSOR4, HIGH);
    digitalWrite(SENSOR5, HIGH);
    digitalWrite(SENSOR6, HIGH);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(5, 0);
    lcd.print(F("WELCOME"));
    delay(100);
    lcd.clear();
    config.load();
}

void loop()
{

    switch (appMode)
    {
    case APP_NORMAL_MODE:
        lcd.setCursor(0,0);
        lcd.print("SUJAHHTRONICS");
        lcd.setCursor(0,1);
        lcd.print(fmt(strbuf, 2,"DRY 1: ",  inttostr(intbuf, config.Sensor1_Value)));
        lcd.setCursor(11,1);
        lcd.print(fmt(strbuf, 2,"DRY 2: ",  inttostr(intbuf, config.Sensor2_Value)));
        lcd.setCursor(0,2);
        lcd.print(fmt(strbuf, 2,"DRY 3: ",  inttostr(intbuf, config.Sensor3_Value)));
        lcd.setCursor(11,2);
        lcd.print(fmt(strbuf, 2,"DRY 4: ",  inttostr(intbuf, config.Sensor4_Value)));
        lcd.setCursor(0,3);
        lcd.print(fmt(strbuf, 2,"DRY 5: ",  inttostr(intbuf, config.Sensor5_Value)));
        lcd.setCursor(11,3);
        lcd.print(fmt(strbuf, 2,"DRY 6: ",  inttostr(intbuf, config.Sensor6_Value)));
        break;
    
    default:
        break;
    }

	
}

