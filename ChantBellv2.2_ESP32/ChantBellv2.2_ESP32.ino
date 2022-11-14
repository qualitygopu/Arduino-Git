#include <Wire.h>
#include "RTClib.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <LiquidCrystal_I2C.h>
#include "LcdKeypad.h"
#include "MenuManager.h"
#include "MenuData.h"
#include "Config.h"
#include "Settings.h"



long ampDelay = 5000, menuTimeOut = 30000;
unsigned long timrAmp, timrLCD, timr_Time, timrMNU;
SoftwareSerial mySoftwareSerial(DF_Tx, DF_Rx); // TX, RX
DFRobotDFPlayerMini myDFPlayer;
LiquidCrystal_I2C lcd(0x27, 16, 2);
short upButPreCount = 0, selButPreCount = 0, downButPreCount = 0;
RTC_DS1307 rtc;
DateTime bell;
// blink time
bool blink = true, serviceMode = false, testMode = false;

MenuManager Menu1;
Config config;

byte alarmMode = 0;

byte btn;
char strbuf[LCD_COLS + 1];
enum PlaySong
{
    Starting_Song,
    Time,
    English_Month,
    English_Date,
    Day_of_Week,
    Tamil_Date,
    Song_1,
    Song_2,
    End_of_Song
};

byte playSong = Starting_Song;

enum BlinkMode
{
    setHr,
    setMin,
    setDate,
    setMonth,
    setYear
};
byte blinkMode = setHr;

String months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

enum AppModeValues
{
    APP_NORMAL_MODE,
    APP_MENU_MODE,
    APP_PROCESS_MENU_CMD,
    APP_CHANT_MODE,
    APP_DEMO_MODE,
    APP_SETHR_MODE,
    APP_SETMIN_MODE
};
byte appMode = APP_NORMAL_MODE;

void refreshMenuDisplay(byte refreshMode);
byte getNavAction();
void PlayChant(short hr);
byte processMenuCommand(byte cmdId);
void showTime();

void setup()
{
    randomSeed(analogRead(A0));
    char intstr[5];
    pinMode(AMP, OUTPUT);
    pinMode(STA_PIN, INPUT_PULLUP);
    pinMode(SELECT_BUT, INPUT_PULLUP);
    pinMode(UP_BUT, INPUT_PULLUP);
    pinMode(DOWN_BUT, INPUT_PULLUP);
    pinMode(BACK_BUT, INPUT_PULLUP);
    Wire.begin();
    config.load();
    lcd.init();
    lcd.backlight();
    lcd.setCursor(5, 0);
    lcd.print(F("QTRON"));
    lcd.setCursor(0, 1);
    lcd.print(F("DEVOTIONAL CLOCK"));
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    inttostr(intstr, config.DataYear);
    lcd.print(fmt(strbuf, 2, "Ver : 2.2/", intstr));
    lcd.setCursor(0, 1);
    inttostr(intstr, config.SerialNo);
    lcd.print(fmt(strbuf, 2, "Serial No : 0", intstr));
    delay(1000);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Loading...");

    if (!digitalRead(SELECT_BUT))
    {
        lcd.setCursor(0, 1);
        lcd.print("Service Mode");
        serviceMode = true;
    }
    else
    {
        serviceMode = false;
    }
    while (!rtc.begin())
    {
        lcd.clear();
        lcd.print(F("ERR 01")); // Couldn't find RTC
    }

    // if (rtc.lostPower())

    if (!rtc.isrunning())
    {
        lcd.clear();
        lcd.print(F("ERR 02"));
        delay(2000);
    }
    // Initialize DF Player...............
    if (digitalRead(BACK_BUT))
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

    if (serviceMode)
        Menu1.attach(sampleMenu_Root, menuCount(sampleMenu_Root));
    else
        Menu1.attach(normalMenu_Root, menuCount(normalMenu_Root));

    if (!digitalRead(UP_BUT) && !digitalRead(DOWN_BUT))
    {
        Menu1.attach(configMenu_Root, menuCount(configMenu_Root));
        appMode = APP_MENU_MODE;
        refreshMenuDisplay(REFRESH_DESCEND);
    }
    if (!digitalRead(SELECT_BUT) && !digitalRead(BACK_BUT))
        testMode = true;
    else
        testMode = false;
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
    if (millis() - timrLCD > 300)
    {
        downButPreCount = 0;
        selButPreCount = 0;
        upButPreCount = 0;
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
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            selButPreCount += 1;
            if (selButPreCount >= 4)
            {
                demoTime = rtc.now().minute() > 30 ? rtc.now().hour() + 1 : rtc.now().hour();
                appMode = APP_DEMO_MODE;
                lcd.clear();
            }
        }
        // TEST MODE //   
        {
            if (btn == BUTTON_UP_PRESSED && testMode)
            {
                timrAmp = millis() - 71000L;
                if (rtc.now().minute() < 30)
                    rtc.adjust(DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), rtc.now().hour(), 29, 55));
                else
                    rtc.adjust(DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), rtc.now().hour(), 59, 55));
            }
        }   
        if (rtc.now().minute() == 0 && rtc.now().second() == 0 && millis() > timrAmp + 61000L)
        {
            bell = rtc.now();
            if (bell.hour() >= config.startTime && bell.hour() <= config.endTime) // chek time
            {
                if (config.SchlZone && !bell.dayOfTheWeek() == 0 &&
                    (bell.hour() == 10 || bell.hour() == 11 || bell.hour() == 12 ||
                     bell.hour() == 13 || bell.hour() == 14 || bell.hour() == 15 ||
                     bell.hour() == 16))
                {
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("School Zone : ON");
                    delay(1000);
                    lcd.clear();
                }
                else
                {
                    digitalWrite(AMP, HIGH);
                    timrAmp = millis();
                    appMode = APP_CHANT_MODE;
                    lcd.clear();
                }
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
        lcd.setCursor(0, 1);
        lcd.print("CHANT PLAYING...");
        if (millis() >= timrAmp + ampDelay)
            PlayChant();
        if (btn == BUTTON_LEFT_SHORT_RELEASE && testMode)
        {
            myDFPlayer.stop();
        }
        break;
    }
    case APP_DEMO_MODE:
    {
        lcd.setCursor(0, 0);
        lcd.print(F("  RUNNING DEMO  "));
        lcd.setCursor(0, 1);
        lcd.print("DEMO TIME : " + String(demoTime > 12 ? demoTime - 12 : demoTime) + String(demoTime >= 12 ? "PM" : "AM"));
        digitalWrite(AMP, HIGH);
        // delay(1000);
        bell = DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), demoTime, 0, 0);
        PlayChant();
        if (btn == BUTTON_LEFT_PRESSED)
        {
            playSong = End_of_Song;
            myDFPlayer.stop();
        }
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            config.vol = ++config.vol > 30 ? 10 : config.vol;
            config.save();
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("VOLUME : ");
            lcd.print(config.vol);
            myDFPlayer.volume(config.vol);
            delay(100);
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            config.vol = --config.vol < 10 ? 30 : config.vol;
            config.save();
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("VOLUME : ");
            lcd.print(config.vol);
            myDFPlayer.volume(config.vol);
            delay(100);
        }
        break;
    }
    case APP_MENU_MODE:
    {
        if (millis() - timrMNU > menuTimeOut)
        {
            Menu1.reset();
            appMode = APP_NORMAL_MODE;
            lcd.clear();
        }

        if (Menu1.getCurrentItemCmdId() == 1 ||  
            Menu1.getCurrentItemCmdId() == mnuCmdSetDate || 
            Menu1.getCurrentItemCmdId() == mnuCmdSetTime)
        {
            StDate = rtc.now().day();
            StMonth = rtc.now().month();
            StYear = rtc.now().year();
            StHr = rtc.now().hour();
            StMin = rtc.now().minute();
        }
        if (Menu1.getCurrentItemCmdId() == mnuCmdSetTime)
        {
            blinkMode = setHr;
        }
        else if (Menu1.getCurrentItemCmdId() == mnuCmdSetDate)
        {
            blinkMode = setDate;
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
        if (millis() - timrMNU > menuTimeOut)
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

    // if (btn == BUTTON_SELECT_PRESSED)
    // {
    //     complete = true;
    // }
    switch (cmdId)
    {
    case mnuCmdSetDate:
    {
        if (btn == BUTTON_UP_PRESSED)
        {
            if (blinkMode == setDate)
                StDate = ++StDate > 31 ? 1 : StDate;
            else if (blinkMode == setMonth)
                StMonth = ++StMonth > 12 ? 1 : StMonth;
            else if (blinkMode == setYear)
                StYear = ++StYear > 2050 ? 2020 : StYear;
        }
        else if (btn == BUTTON_DOWN_PRESSED)
        {
            if (blinkMode == setDate)
                StDate = --StDate < 1 ? 31 : StDate;
            else if (blinkMode == setMonth)
                StMonth = --StMonth < 1 ? 12 : StMonth;
            else if (blinkMode == setYear)
                StYear = --StYear < 2020 ? 2050 : StYear;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            if (blinkMode == setDate)
            {
                blinkMode = setMonth;
            }
            else if (blinkMode == setMonth)
            {
                blinkMode = setYear;
            }
            else if (blinkMode == setYear)
            {
                rtc.adjust(DateTime(StYear, StMonth, StDate, StHr, StMin, 0));
                complete = true;
                blink = true;
            }
        }
        break;
    }
    case mnuCmdSetTime:
    {
        if (btn == BUTTON_UP_PRESSED)
        {
            if (blinkMode == setHr)
                StHr = ++StHr > 23 ? 0 : StHr;
            else if (blinkMode == setMin)
                StMin = ++StMin > 59 ? 0 : StMin;
        }
        else if (btn == BUTTON_DOWN_PRESSED)
        {
            if (blinkMode == setHr)
                StHr = --StHr < 0 ? 23 : StHr;
            else if (blinkMode == setMin)
                StMin = --StMin < 0 ? 59 : StMin;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            if (blinkMode == setHr)
            {
                blinkMode = setMin;
            }
            else if (blinkMode == setMin)
            {
                rtc.adjust(DateTime(StYear, StMonth, StDate, StHr, StMin, 0));
                complete = true;
                blink = true;
            }
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
            complete = true;
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
            complete = true;
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdSlogamCount:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            config.Song1_Count = ++config.Song1_Count;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            config.Song1_Count = --config.Song1_Count < 0 ? 100 : config.Song1_Count;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            config.save();
            complete = true;
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
            complete = true;
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
            demoTime = ++demoTime > 23 ? 4 : demoTime;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            demoTime = --demoTime < 4 ? 23 : demoTime;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            complete = true;
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdPlay:
    {
        Menu1.reset();
        appMode = APP_DEMO_MODE;
        lcd.clear();
        break;
    }
    case mnuCmdMorSong1Time:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            config.MorSong1Time = ++config.MorSong1Time > 8 ? 3 : config.MorSong1Time;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            config.MorSong1Time = --config.MorSong1Time < 3 ? 8 : config.MorSong1Time;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            config.save();
             complete = true;
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdEveSong1Time:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            config.EveSong1Time = ++config.EveSong1Time > 8 ? 3 : config.EveSong1Time;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            config.EveSong1Time = --config.EveSong1Time < 3 ? 8 : config.EveSong1Time;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            config.save();
             complete = true;
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdMorSongNo:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            config.MorSongNo = ++config.MorSongNo > 20 ? 1 : config.MorSongNo;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            config.MorSongNo = --config.MorSongNo < 1 ? 20 : config.MorSongNo;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            config.save();
             complete = true;
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdEveSongNo:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            config.EveSongNo = ++config.EveSongNo > 20 ? 1 : config.EveSongNo;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            config.EveSongNo = --config.EveSongNo < 1 ? 20 : config.EveSongNo;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            config.save();
             complete = true;
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdSchlZone:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_DOWN_PRESSED)
        {
            config.SchlZone = !config.SchlZone;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            config.save();
            complete = true;
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdSerial:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            config.SerialNo = ++config.SerialNo;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            config.SerialNo = --config.SerialNo;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            config.save();
            complete = true;
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdYear:
    {
        configChanged = true;
        if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
        {
            config.DataYear = ++config.DataYear;
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            config.DataYear = --config.DataYear < 2022 ? 2022 : config.DataYear;
        }
        else if (btn == BUTTON_SELECT_PRESSED)
        {
            config.save();
            complete = true;
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdReset:
    {
        if (btn == BUTTON_SELECT_LONG_PRESSED)
        {
            config.setDefaults();
            lcd.setCursor(1, 1);
            lcd.print("Defaults loaded");
        }
        else if (btn == BUTTON_SELECT_LONG_RELEASE)
        {
            complete = true;
        }
        break;
    }
    }

    if (btn == BUTTON_LEFT_SHORT_RELEASE || complete)
    {
        complete = true;
        lcd.setCursor(1, 1);
        lcd.print(rpad(strbuf, config.getFormattedStr(cmdId)));
    }
    else if (configChanged)
    {
        lcd.setCursor(1, 1);
        lcd.print(rpad(strbuf, config.getFormattedStr(cmdId)));
    }
    else
    {
        if (millis() - timr_Time > 300)
        {
            if (blink)
            {
                lcd.setCursor(1, 1);
                lcd.print(rpad(strbuf, config.getFormattedStr(cmdId)));
                blink = false;
            }
            else
            {
                if (cmdId == mnuCmdSetTime)
                {
                    if (blinkMode == setHr)
                        lcd.setCursor(1, 1);
                    if (blinkMode == setMin)
                        lcd.setCursor(4, 1);
                    lcd.print(F("  "));
                    blink = true;
                }
                else if (cmdId == mnuCmdSetDate)
                {
                    if (blinkMode == setDate)
                        lcd.setCursor(1, 1);
                    if (blinkMode == setMonth)
                        lcd.setCursor(4, 1);
                    if (blinkMode == setYear)
                        lcd.setCursor(9, 1);
                    lcd.print(F("  "));
                    blink = true;
                }
            }
            timr_Time = millis();
        }
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
    else if (btn == BUTTON_LEFT_SHORT_RELEASE)
        navAction = MENU_BACK;
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
            if (cmdId == mnuCmdSetDate)
            {
                StDate = rtc.now().day();
                StMonth = rtc.now().month();
                StYear = rtc.now().year();
            }
            lcd.print(strbuf);
            lcd.setCursor(0, 1);
            lcd.print(" ");
            lcd.print(rpad(strbuf, config.getFormattedStr(cmdId)));
        }
    }
}

int n = 1;
void PlayChant()
{
    // Serial.println(String(hr));
    switch (playSong)
    {
    case Starting_Song: // Temple Name
        if (digitalRead(STA_PIN))
        {
            myDFPlayer.playMp3Folder(0);
            delay(1000);
            playSong = Time;
        }
        break;
    case Time:
        if (digitalRead(STA_PIN)) // Time
        {
            myDFPlayer.playFolder(1, bell.hour());
            delay(1000);
            playSong = English_Month;
        }
        break;
    case English_Month: // English Month
        if (digitalRead(STA_PIN))
        {
            myDFPlayer.playFolder(47, bell.month());
            delay(1000);
            playSong = English_Date;
        }
        break;
    case English_Date: // Englidh Date
        if (digitalRead(STA_PIN))
        {
            myDFPlayer.playFolder(48, bell.day());
            delay(1000);
            playSong = Day_of_Week;
        }
        break;
    case Day_of_Week: // Day of Week
        if (digitalRead(STA_PIN))
        {
            myDFPlayer.playFolder(49, bell.dayOfTheWeek() + 1);
            delay(1000);
            playSong = Tamil_Date;
        }
        break;
    case Tamil_Date: // Tamil Date
        if (digitalRead(STA_PIN))
        {
            if (bell.hour() == 6 || bell.hour() == 8 || bell.hour() == 10)
            {
                if (bell.year() == config.DataYear)
                {
                    myDFPlayer.playFolder(bell.month() + 50, bell.day()); // current year
                    delay(1000);
                }
                else if (bell.year() == config.DataYear - 1 && bell.month() == 12) // prev year december
                {
                    myDFPlayer.playFolder(50, bell.day());
                    delay(1000);
                }
                playSong = Song_1;
            }
            else
            {
                playSong = Song_1;
            }
        }
        break;

    case Song_1:
        if (digitalRead(STA_PIN))
        {
            if (config.MorSong1Time == bell.hour() && config.MorSong1Time != 3)
            {
                myDFPlayer.playFolder(2, config.MorSongNo);
                delay(1000);
                playSong = End_of_Song;
            }
            else if ((config.EveSong1Time + 12) == bell.hour() && (config.EveSong1Time != 3))
            {
                myDFPlayer.playFolder(2, config.EveSongNo);
                delay(1000);
                playSong = End_of_Song;
            }
            else
            {
                myDFPlayer.playMp3Folder(random(1,config.Song1_Count));
                delay(1000);
                playSong = Song_2;
            }
        }
        break;
    case Song_2:
        // if (digitalRead(STA_PIN))
        // {
        //     myDFPlayer.playFolder(6,random(1,config.Song2_Count));
        //     delay(1000);
            playSong = End_of_Song;
        // }
        break;
    case End_of_Song:
        if (digitalRead(STA_PIN))
        {
            playSong = Starting_Song;
            lcd.clear();
            appMode = APP_NORMAL_MODE;
            timrLCD = millis();
            digitalWrite(AMP, LOW);
        }
        break;
    }
}
