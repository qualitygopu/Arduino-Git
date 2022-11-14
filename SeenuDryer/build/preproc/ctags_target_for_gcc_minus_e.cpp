# 1 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino"
# 2 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino" 2
# 3 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino" 2
# 4 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino" 2
# 5 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino" 2
// #include "MenuData.h"
# 7 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino" 2
# 22 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino"
LiquidCrystal_I2C lcd(0x27, 20, 4);
MenuManager Menu1;
Config config;

char strbuf[20 + 1];
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

double readCelsius(uint8_t cs) {
    uint16_t v;

    digitalWrite(cs, 0x0);
    v = SPI.transfer(0x00);
    v <<= 8;
    v |= SPI.transfer(0x00);
    digitalWrite(cs, 0x1);

    if (v & 0x4) {
        // uh oh, no thermocouple attached!
        return 
# 53 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino" 3
              __builtin_nan("")
# 53 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino"
                 ;
    }

    v >>= 3;

    return v*0.25;
}

void setup()
{
    SPI.begin();
 pinMode(2, 0x1);
    pinMode(3, 0x1);
    pinMode(4, 0x1);
    pinMode(5, 0x1);
    pinMode(6, 0x1);
    pinMode(7, 0x1);
    digitalWrite(2, 0x1);
    digitalWrite(3, 0x1);
    digitalWrite(4, 0x1);
    digitalWrite(5, 0x1);
    digitalWrite(6, 0x1);
    digitalWrite(7, 0x1);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(5, 0);
    lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 79 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino" 3
             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 79 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino"
             "WELCOME"
# 79 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino" 3
             ); &__c[0];}))
# 79 "e:\\Arduino\\Arduino Git\\SeenuDryer\\SeenuDryer.ino"
             )));
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
        lcd.print(fmt(strbuf, 2,"DRY 1: ", inttostr(intbuf, config.Sensor1_Value)));
        lcd.setCursor(11,1);
        lcd.print(fmt(strbuf, 2,"DRY 2: ", inttostr(intbuf, config.Sensor2_Value)));
        lcd.setCursor(0,2);
        lcd.print(fmt(strbuf, 2,"DRY 3: ", inttostr(intbuf, config.Sensor3_Value)));
        lcd.setCursor(11,2);
        lcd.print(fmt(strbuf, 2,"DRY 4: ", inttostr(intbuf, config.Sensor4_Value)));
        lcd.setCursor(0,3);
        lcd.print(fmt(strbuf, 2,"DRY 5: ", inttostr(intbuf, config.Sensor5_Value)));
        lcd.setCursor(11,3);
        lcd.print(fmt(strbuf, 2,"DRY 6: ", inttostr(intbuf, config.Sensor6_Value)));
        break;

    default:
        break;
    }


}
