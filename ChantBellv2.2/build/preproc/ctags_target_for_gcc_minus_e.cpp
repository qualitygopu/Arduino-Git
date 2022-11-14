# 1 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino"
# 2 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 2
# 3 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 2
# 4 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 2
# 5 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 2
# 6 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 2
# 7 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 2
# 8 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 2
# 9 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 2
# 10 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 2




long ampDelay = 5000, menuTimeOut = 30000;
unsigned long timrAmp, timrLCD, timr_Time, timrMNU;
SoftwareSerial mySoftwareSerial(2, 3); // TX, RX
DFRobotDFPlayerMini myDFPlayer;
LiquidCrystal_I2C lcd(0x27, 16, 2);
short upButPreCount = 0, selButPreCount = 0, downButPreCount = 0;
// RTC_DS3231 rtc;
RTC_DS1307 rtc;
DateTime bell;
// blink time
bool blink = true, serviceMode = false, testMode = false;

MenuManager Menu1;
Config config;

byte alarmMode = 0;

byte btn;
char strbuf[16 + 1];
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
    pinMode(12, 0x1);
    pinMode(4, 0x2);
    pinMode(5, 0x2);
    pinMode(6, 0x2);
    pinMode(7, 0x2);
    pinMode(8, 0x2);
    Wire.begin();
    config.load();
    lcd.init();
    lcd.backlight();
    lcd.setCursor(5, 0);
    lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 93 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 3
             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 93 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino"
             "QTRON"
# 93 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 3
             ); &__c[0];}))
# 93 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino"
             )));
    lcd.setCursor(0, 1);
    lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 95 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 3
             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 95 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino"
             "DEVOTIONAL CLOCK"
# 95 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 3
             ); &__c[0];}))
# 95 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino"
             )));
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

    if (!digitalRead(5))
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
        lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 122 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 122 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino"
                 "ERR 01"
# 122 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 3
                 ); &__c[0];}))
# 122 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino"
                 ))); // Couldn't find RTC
    }

    // if (rtc.lostPower())

    if (!rtc.isrunning())
    {
        lcd.clear();
        lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 130 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 130 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino"
                 "ERR 02"
# 130 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 3
                 ); &__c[0];}))
# 130 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino"
                 )));
        delay(2000);
    }
    // Initialize DF Player...............
    // if (digitalRead(BACK_BUT))
    {
        mySoftwareSerial.begin(9600);
        delay(500);
        while (!myDFPlayer.begin(mySoftwareSerial))
        {
            lcd.setCursor(0, 0);
            lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 141 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 141 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino"
                     "ERR 03"
# 141 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 3
                     ); &__c[0];}))
# 141 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino"
                     )));
        }
        myDFPlayer.setTimeOut(500);
        myDFPlayer.volume(config.vol);
        myDFPlayer.outputDevice(2);
    }
    lcd.clear();
    timrLCD = millis();

    if (serviceMode)
        Menu1.attach(sampleMenu_Root, (sizeof(sampleMenu_Root) / sizeof(MenuItem)));
    else
        Menu1.attach(normalMenu_Root, (sizeof(normalMenu_Root) / sizeof(MenuItem)));

    if (!digitalRead(6) && !digitalRead(7))
    {
        Menu1.attach(configMenu_Root, (sizeof(configMenu_Root) / sizeof(MenuItem)));
        appMode = APP_MENU_MODE;
        refreshMenuDisplay(REFRESH_DESCEND);
    }
    if (!digitalRead(5) && !digitalRead(8))
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
        if (btn == (1 | (2 << 6)) /*128*/)
        {
            appMode = APP_MENU_MODE;
            refreshMenuDisplay(REFRESH_DESCEND);
            timrMNU = millis();
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
            if (btn == (2 | (0 << 6)) && testMode)
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
                    digitalWrite(12, 0x1);
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
        if (btn == (4 | (1 << 6)) && testMode)
        {
            myDFPlayer.stop();
        }
        break;
    }
    case APP_DEMO_MODE:
    {
        lcd.setCursor(0, 0);
        lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 262 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 262 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino"
                 "  RUNNING DEMO  "
# 262 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 3
                 ); &__c[0];}))
# 262 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino"
                 )));
        lcd.setCursor(0, 1);
        lcd.print("DEMO TIME : " + String(demoTime > 12 ? demoTime - 12 : demoTime) + String(demoTime >= 12 ? "PM" : "AM"));
        digitalWrite(12, 0x1);
        // delay(1000);
        bell = DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), demoTime, 0, 0);
        PlayChant();
        if (btn == (4 | (0 << 6)))
        {
            playSong = End_of_Song;
            myDFPlayer.stop();
        }
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
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
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
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

        if (btn == (4 | (2 << 6)))
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
        if (btn == (2 | (0 << 6)))
        {
            if (blinkMode == setDate)
                StDate = ++StDate > 31 ? 1 : StDate;
            else if (blinkMode == setMonth)
                StMonth = ++StMonth > 12 ? 1 : StMonth;
            else if (blinkMode == setYear)
                StYear = ++StYear > 2050 ? 2020 : StYear;
        }
        else if (btn == (3 | (0 << 6)))
        {
            if (blinkMode == setDate)
                StDate = --StDate < 1 ? 31 : StDate;
            else if (blinkMode == setMonth)
                StMonth = --StMonth < 1 ? 12 : StMonth;
            else if (blinkMode == setYear)
                StYear = --StYear < 2020 ? 2050 : StYear;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (2 | (0 << 6)))
        {
            if (blinkMode == setHr)
                StHr = ++StHr > 23 ? 0 : StHr;
            else if (blinkMode == setMin)
                StMin = ++StMin > 59 ? 0 : StMin;
        }
        else if (btn == (3 | (0 << 6)))
        {
            if (blinkMode == setHr)
                StHr = --StHr < 0 ? 23 : StHr;
            else if (blinkMode == setMin)
                StMin = --StMin < 0 ? 59 : StMin;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.startTime = ++config.startTime > 23 ? 0 : config.startTime;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.startTime = --config.startTime < 0 ? 23 : config.startTime;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.endTime = ++config.endTime > 23 ? 0 : config.endTime;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.endTime = --config.endTime < 0 ? 23 : config.endTime;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.Song1_Count = ++config.Song1_Count;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.Song1_Count = --config.Song1_Count < 0 ? 100 : config.Song1_Count;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.vol = ++config.vol > 30 ? 1 : config.vol;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.vol = --config.vol < 1 ? 30 : config.vol;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            demoTime = ++demoTime > 23 ? 4 : demoTime;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            demoTime = --demoTime < 4 ? 23 : demoTime;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.MorSong1Time = ++config.MorSong1Time > 8 ? 3 : config.MorSong1Time;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.MorSong1Time = --config.MorSong1Time < 3 ? 8 : config.MorSong1Time;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.EveSong1Time = ++config.EveSong1Time > 8 ? 3 : config.EveSong1Time;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.EveSong1Time = --config.EveSong1Time < 3 ? 8 : config.EveSong1Time;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.MorSongNo = ++config.MorSongNo > 20 ? 1 : config.MorSongNo;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.MorSongNo = --config.MorSongNo < 1 ? 20 : config.MorSongNo;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.EveSongNo = ++config.EveSongNo > 20 ? 1 : config.EveSongNo;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.EveSongNo = --config.EveSongNo < 1 ? 20 : config.EveSongNo;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (2 | (0 << 6)) || btn == (3 | (0 << 6)))
        {
            config.SchlZone = !config.SchlZone;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.SerialNo = ++config.SerialNo;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.SerialNo = --config.SerialNo;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.DataYear = ++config.DataYear;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.DataYear = --config.DataYear < 2022 ? 2022 : config.DataYear;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (1 | (2 << 6)) /*128*/)
        {
            config.setDefaults();
            lcd.setCursor(1, 1);
            lcd.print("Defaults loaded");
        }
        else if (btn == (1 | (3 << 6)) /*193*/)
        {
            complete = true;
        }
        break;
    }
    }

    if (btn == (4 | (1 << 6)) || complete)
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
                    lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 794 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 3
                             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 794 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino"
                             "  "
# 794 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 3
                             ); &__c[0];}))
# 794 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino"
                             )));
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
                    lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 805 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 3
                             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 805 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino"
                             "  "
# 805 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino" 3
                             ); &__c[0];}))
# 805 "e:\\Arduino\\Arduino Git\\ChantBellv2.2\\ChantBellv2.2.ino"
                             )));
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

    if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        navAction = MENU_ITEM_PREV;
    else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        navAction = MENU_ITEM_NEXT;
    else if (btn == (1 | (0 << 6)) /*1*/)
        navAction = MENU_ITEM_SELECT;
    else if (btn == (4 | (1 << 6)))
        navAction = MENU_BACK;
    return navAction;
}

const char EmptyStr[] = "";

void refreshMenuDisplay(byte refreshMode)
{
    char nameBuf[16 + 1];

    lcd.setCursor(0, 0);
    if (Menu1.currentItemHasChildren())
    {
        rpad(strbuf, Menu1.getCurrentItemName(nameBuf));
        strbuf[16 - 1] = 0b01111110; // Display forward arrow if this menu item has children.
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
            strbuf[16 - 1] = 0b01111111; // Display back arrow if this menu item ascends to parent.
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
        if (digitalRead(4))
        {
            myDFPlayer.playFolder(0, random(1, 20));
            delay(1000);
            playSong = Time;
        }
        break;
    case Time:
        if (digitalRead(4)) // Time
        {
            myDFPlayer.playFolder(1, bell.hour());
            delay(1000);
            playSong = English_Month;
        }
        break;
    case English_Month: // English Month
        if (digitalRead(4))
        {
            myDFPlayer.playFolder(47, bell.month());
            delay(1000);
            playSong = English_Date;
        }
        break;
    case English_Date: // Englidh Date
        if (digitalRead(4))
        {
            myDFPlayer.playFolder(48, bell.day());
            delay(1000);
            playSong = Day_of_Week;
        }
        break;
    case Day_of_Week: // Day of Week
        if (digitalRead(4))
        {
            myDFPlayer.playFolder(49, bell.dayOfTheWeek() + 1);
            delay(1000);
            playSong = Tamil_Date;
        }
        break;
    case Tamil_Date: // Tamil Date
        if (digitalRead(4))
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
        if (digitalRead(4))
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
                if (config.Song2_Count != 0)
                    playSong = Song_2;
                else
                    playSong = End_of_Song;
            }
        }
        break;
    case Song_2:
         if (digitalRead(4))
         {
            myDFPlayer.playFolder(6,random(1,config.Song2_Count));
            delay(1000);
            playSong = End_of_Song;
         }
        break;
    case End_of_Song:
        if (digitalRead(4))
        {
            playSong = Starting_Song;
            lcd.clear();
            appMode = APP_NORMAL_MODE;
            timrLCD = millis();
            digitalWrite(12, 0x0);
        }
        break;
    }
}
