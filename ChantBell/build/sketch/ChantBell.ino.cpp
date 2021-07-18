#include <Arduino.h>
#line 1 "e:\\Arduino\\Arduino Git\\ChantBell\\ChantBell.ino"
#include <Wire.h>
#include "RTClib.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <LiquidCrystal_I2C.h>
#include "LcdKeypad.h"
#include "MenuManager.h"
#include "MenuData.h"
#include "Config.h"

#define AMP A3
#define STA_PIN 5

int ampDelay = 5000;
unsigned long timrAmp, timrLCD, timr_Time, timrMNU;
SoftwareSerial mySoftwareSerial(2, 3); // TX, RX
DFRobotDFPlayerMini myDFPlayer;
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;
MenuManager Menu1(sampleMenu_Root, menuCount(sampleMenu_Root));
Config config;

byte alarmMode = 0;
byte playSong = 1;
byte btn;
char strbuf[LCD_COLS + 1];

String months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

enum AppModeValues
{
    APP_NORMAL_MODE,
    APP_MENU_MODE,
    APP_PROCESS_MENU_CMD,
    APP_CHANT_MODE,
    APP_DEMO_MODE
};
byte appMode = APP_NORMAL_MODE;

void refreshMenuDisplay(byte refreshMode);
byte getNavAction();
void PlayChant(short hr);
byte processMenuCommand(byte cmdId);
void showTime();

#line 46 "e:\\Arduino\\Arduino Git\\ChantBell\\ChantBell.ino"
void setup();
#line 91 "e:\\Arduino\\Arduino Git\\ChantBell\\ChantBell.ino"
void loop();
#line 46 "e:\\Arduino\\Arduino Git\\ChantBell\\ChantBell.ino"
void setup()
{
    Serial.begin(9600);
    pinMode(AMP, OUTPUT);
    pinMode(STA_PIN, INPUT_PULLUP);
    Wire.begin();
    lcd.init();
    lcd.backlight();
    lcd.setCursor(5, 0);
    lcd.print(F("WELCOME"));
    lcd.setCursor(6, 1);
    lcd.print(F("QTRON"));
    delay(2000);
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Loading...");
    config.load();
    while (!rtc.begin())
    {
        lcd.clear();
        lcd.print(F("ERR 01")); // Couldn't find RTC
    }

    if (rtc.lostPower())
    {
        lcd.clear();
        lcd.print(F("ERR 02"));
        delay(2000);
    }
    // Initialize DF Player...............
     {
         mySoftwareSerial.begin(9600);
         delay(500);
         while (!myDFPlayer.begin(mySoftwareSerial))
         {
             lcd.setCursor(0, 0);
             lcd.print(F("ERR 03"));
         }
         myDFPlayer.setTimeOut(500);
         myDFPlayer.volume(config.vol);
         myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
     }
  lcd.clear();    
  timrLCD = millis();  
}
void loop()
{
    btn = getButton();
    if (btn)
        {
          timrLCD = millis();
          timrMNU = millis();
          lcd.backlight();
        }
    switch (appMode)
    {
    case APP_NORMAL_MODE:
        if (btn == BUTTON_SELECT_LONG_PRESSED)
        {
            appMode = APP_MENU_MODE;
            refreshMenuDisplay(REFRESH_DESCEND);
            timrMNU = millis();
        }
        if (rtc.now().minute() == 0 && rtc.now().second() == 0)
        {
            int hr = rtc.now().hour();
            if (hr >= config.startTime && hr <= config.endTime) // chek time
            {
                digitalWrite(AMP, HIGH);
                timrAmp = millis();
                appMode = APP_CHANT_MODE;
                lcd.clear();
            }
        }
        if (millis() - timr_Time > 1000)
        {
            timr_Time = millis();
            showTime();
        }
        if (millis() - timrLCD > (config.backLightDur * 1000L))
        {
            lcd.noBacklight();
        }
        break;
    case APP_CHANT_MODE:
    {
        lcd.backlight();
        lcd.setCursor(0,1);
        lcd.print("CHANT PLAYING...");
        if (millis() >= timrAmp + ampDelay)
            PlayChant(rtc.now().hour());
    }
    case APP_MENU_MODE:
    {
        if (millis() - timrMNU > 10000)
        {
          Menu1.reset();
          appMode = APP_NORMAL_MODE;
          lcd.clear();
        }
           
        if (Menu1.getCurrentItemCmdId() == 1)
        {
            StDate = rtc.now().day();
            StMonth = rtc.now().month();
            StYear = rtc.now().year();
            StHr = rtc.now().hour();
            StMin = rtc.now().minute();
        }
        if (btn == BUTTON_LEFT_LONG_PRESSED)
        {
            Menu1.reset();
            appMode = APP_NORMAL_MODE;
            lcd.clear();
        }
        byte menuMode = Menu1.handleNavigation(getNavAction, refreshMenuDisplay);

        if (menuMode == MENU_EXIT)
        {
            lcd.clear();
            appMode = APP_NORMAL_MODE;
        }
        else if (menuMode == MENU_INVOKE_ITEM)
        {
            appMode = APP_PROCESS_MENU_CMD;
            if (Menu1.getCurrentItemCmdId())
            {
                lcd.setCursor(0, 1);
                strbuf[0] = 0b01111110; // forward arrow representing input prompt.
                strbuf[1] = 0;
                lcd.print(strbuf);
            }
        }
        break;
    }
    case APP_PROCESS_MENU_CMD:
    {
        byte processingComplete = processMenuCommand(Menu1.getCurrentItemCmdId());
        if (millis() - timrMNU > 10000)
        {
          Menu1.reset();
          appMode = APP_NORMAL_MODE;
          lcd.clear();
        }      
        if (processingComplete)
        {
            appMode = APP_MENU_MODE;
            // clear forward arrow
            lcd.setCursor(0, 1);
            strbuf[0] = ' '; // clear forward arrow
            strbuf[1] = 0;
            lcd.print(strbuf);
        }
        break;
    }
    case APP_DEMO_MODE:
    {
        lcd.setCursor(0, 0);
        lcd.print(F("  RUNNING DEMO  "));
        lcd.setCursor(0, 1);
        lcd.print("DEMO TIME : " + String(demoTime) + String(demoTime >= 12 ? " PM" : " AM"));
        digitalWrite(AMP, HIGH);
        delay(1000);
        PlayChant(demoTime);
        if (btn == BUTTON_DOWN_PRESSED)
        {
          playSong = 9;
          myDFPlayer.stop();
        }
    }
    }
}

void PlayChant(short hr)
{
    switch (playSong)
    {
    case 1:
        if (digitalRead(STA_PIN))
        {
            myDFPlayer.playMp3Folder(0);
            delay(1000);
            playSong = 2;
        }
        break;
    case 2:
        if (digitalRead(STA_PIN))
        {
            myDFPlayer.playFolder(1, hr);
            delay(1000);
            playSong = 3;
        }
        break;
    case 3:
        if (digitalRead(STA_PIN))
        {
            myDFPlayer.playMp3Folder(config.SongOrder);
            config.SongOrder++;
            if (config.SongOrder > config.SongCount)
                config.SongOrder = 1;
            config.save();
            delay(1000);
            playSong = 9;
        }
        break;
    case 9:
        if (digitalRead(STA_PIN))
        {
            playSong = 1;
            lcd.clear();
            appMode = APP_NORMAL_MODE;
            timrLCD = millis();
            digitalWrite(AMP, LOW);
        }
        break;
    }
}

void showTime()
{
    char intbuf[8];
    char h[3], m[3], s[3], dat[3], mon[4], year[5];

    inttostr(intbuf, rtc.now().twelveHour());
    lpad(h, intbuf, '0', 2);
    inttostr(intbuf, rtc.now().minute());
    lpad(m, intbuf, '0', 2);
    inttostr(intbuf, rtc.now().second());
    lpad(s, intbuf, '0', 2);

    lcd.setCursor(2, 0);
    lcd.print(fmt(strbuf, 6, h, ":", m, ":", s, rtc.now().isPM() ? " PM" : " AM"));

    inttostr(intbuf, rtc.now().day());
    lpad(dat, intbuf, '0', 2);
    inttostr(intbuf, rtc.now().year());
    lpad(year, intbuf, '0', 4);
    strcpy(mon, months[rtc.now().month() - 1].c_str());

    lcd.setCursor(2, 1);
    lcd.print(fmt(strbuf, 5, dat, "-", mon, "-", year));
}

byte processMenuCommand(byte cmdId)
{
    byte complete = false; // set to true when menu command processing complete.
    byte configChanged = false;

    if (btn == BUTTON_SELECT_PRESSED)
    {
        complete = true;
    }
    switch (cmdId)
    {
    case mnuCmdSetDate:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            StDate = ++StDate > 31 ? 1 : StDate;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            StDate = --StDate < 1 ? 31 : StDate;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            rtc.adjust(DateTime(StYear, StMonth, StDate, StHr, StMin, 0));
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdSetMonth:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            StMonth = ++StMonth > 12 ? 1 : StMonth;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            StMonth = --StMonth < 1 ? 12 : StMonth;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            rtc.adjust(DateTime(StYear, StMonth, StDate, StHr, StMin, 0));
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdSetYear:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            StYear = ++StYear > 2050 ? 1 : StYear;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            StYear = --StYear < 1 ? 2020 : StYear;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            rtc.adjust(DateTime(StYear, StMonth, StDate, StHr, StMin, 0));
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdSetHour:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            StHr = ++StHr > 23 ? 0 : StHr;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            StHr = --StHr < 0 ? 23 : StHr;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            rtc.adjust(DateTime(StYear, StMonth, StDate, StHr, StMin, 0));
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdSetMin:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            StMin = ++StMin > 59 ? 0 : StMin;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            StMin = --StMin < 0 ? 59 : StMin;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            rtc.adjust(DateTime(StYear, StMonth, StDate, StHr, StMin, 0));
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdStartTime:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            config.startTime = ++config.startTime > 23 ? 0 : config.startTime;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            config.startTime = --config.startTime < 0 ? 23 : config.startTime;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            config.save();
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdEndTime:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            config.endTime = ++config.endTime > 23 ? 0 : config.endTime;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            config.endTime = --config.endTime < 0 ? 23 : config.endTime;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            config.save();
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdSongCount:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            config.SongCount = ++config.SongCount;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            config.SongCount = --config.SongCount < 0 ? 100 : config.SongCount;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            config.save();
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdBakLitDur:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            config.backLightDur = ++config.backLightDur > 60 ? 0 : config.backLightDur;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            config.backLightDur = --config.backLightDur < 0 ? 60 : config.backLightDur;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            config.save();
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdVolume:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            config.vol = ++config.vol > 30 ? 1 : config.vol;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            config.vol = --config.vol < 1 ? 30 : config.vol;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            config.save();
            myDFPlayer.volume(config.vol);                                   
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdselectTime:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            demoTime = ++demoTime > 11 ? 4 : demoTime;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            demoTime = --demoTime < 4 ? 11 : demoTime;
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdPlay:
        Menu1.reset();
        appMode = APP_DEMO_MODE;
        lcd.clear();
        break;
    }
    if (configChanged)
    {
        lcd.setCursor(1, 1);
        lcd.print(rpad(strbuf, config.getFormattedStr(cmdId))); // Display config value.
    }
    return complete;
}

byte getNavAction()
{
    byte navAction = 0;
    byte currentItemHasChildren = Menu1.currentItemHasChildren();

    if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        navAction = MENU_ITEM_PREV;
    else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        navAction = MENU_ITEM_NEXT;
    else if (btn == BUTTON_SELECT_PRESSED)
        navAction = MENU_ITEM_SELECT;
    // else if (btn == BUTTON_LEFT_PRESSED)
    //     navAction = MENU_BACK;
    return navAction;
}

const char EmptyStr[] = "";
void refreshMenuDisplay(byte refreshMode)
{
    char nameBuf[LCD_COLS + 1];

    lcd.setCursor(0, 0);
    if (Menu1.currentItemHasChildren())
    {
        rpad(strbuf, Menu1.getCurrentItemName(nameBuf));
        strbuf[LCD_COLS - 1] = 0b01111110; // Display forward arrow if this menu item has children.
        lcd.print(strbuf);
        lcd.setCursor(0, 1);
        lcd.print(rpad(strbuf, EmptyStr)); // Clear config value in display
    }
    else
    {
        byte cmdId;
        rpad(strbuf, Menu1.getCurrentItemName(nameBuf));

        if ((cmdId = Menu1.getCurrentItemCmdId()) == 0)
        {
            strbuf[LCD_COLS - 1] = 0b01111111; // Display back arrow if this menu item ascends to parent.
            lcd.print(strbuf);
            lcd.setCursor(0, 1);
            lcd.print(rpad(strbuf, EmptyStr)); // Clear config value in display.
        }
        else
        {
            lcd.print(strbuf);
            lcd.setCursor(0, 1);
            lcd.print(" ");
            lcd.print(rpad(strbuf, config.getFormattedStr(cmdId)));
        }
    }
}

