# 1 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
# 2 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 2
# 3 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 2
# 4 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 2
# 5 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 2
# 6 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 2
# 7 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 2
# 8 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 2
# 9 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 2
# 10 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 2
# 19 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
long CurTime;
byte Light1ON = 0, Light2ON = 0, PlayerONMor = 0, PlayerONEve = 0;
int ampDelay = 5000;
unsigned long timrAmp, timrLCD, timr_Time, timrMNU, timrTrigger;
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

bool ButLongPress = false;
byte alarmMode = 0;
byte btn;
char strbuf[16 + 1];

String months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

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

enum AppModeValues
{
    APP_NORMAL_MODE,
    APP_MENU_MODE,
    APP_PROCESS_MENU_CMD,
    APP_CHANT_MODE,
    APP_DEMO_MODE,
    APP_PLAYER_MODE,
    APP_SETHR_MODE,
    APP_SETMIN_MODE
};

enum BlinkMode
{
    setHr,
    setMin,
    setDate,
    setMonth,
    setYear
};
byte blinkMode = setHr;
byte appMode = APP_NORMAL_MODE;

void refreshMenuDisplay(byte refreshMode);
byte getNavAction();
void PlayChant(short hr);
byte processMenuCommand(byte cmdId);
void showTime();

template <typename T, size_t N>
size_t ArraySize(T (&)[N]) { return N; }

void setup()
{
    randomSeed(analogRead(A0));
    char intstr[5];
    pinMode(12, 0x1);
    pinMode(10, 0x1);
    pinMode(A1, 0x1);
    pinMode(A0, 0x1);
    pinMode(11, 0x1);
    pinMode(4, 0x2);
    pinMode(5, 0x2);
    pinMode(6, 0x2);
    pinMode(7, 0x2);
    pinMode(8, 0x2);
    pinMode(A2, 0x2);
    Wire.begin();
    config.load();
    lcd.init();
    lcd.backlight();
    lcd.setCursor(5, 0);
    lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 111 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 111 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
             "QTRON"
# 111 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
             ); &__c[0];}))
# 111 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
             )));
    lcd.setCursor(0, 1);
    lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 113 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 113 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
             "DEVOTIONAL CLOCK"
# 113 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
             ); &__c[0];}))
# 113 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
             )));
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    inttostr(intstr, config.DataYear);
    lcd.print(fmt(strbuf, 2, "Ver : 3.2/", intstr));
    lcd.setCursor(0, 1);
    inttostr(intstr, config.SerialNo);
    lcd.print(fmt(strbuf, 2, "Serial No : 0", intstr));
    delay(2000);
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
# 139 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 139 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                 "ERR 01"
# 139 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                 ); &__c[0];}))
# 139 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                 ))); // Couldn't find RTC
    }

    if (!rtc.isrunning())
    {
        lcd.clear();
        lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 145 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 145 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                 "ERR 02"
# 145 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                 ); &__c[0];}))
# 145 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                 )));
        delay(2000);
        serviceMode = true;
    }

    // Initialize DF Player...............
    if (digitalRead(8))
    {
        mySoftwareSerial.begin(9600);
        delay(500);
        while (!myDFPlayer.begin(mySoftwareSerial))
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 159 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 159 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                     "ERR 03"
# 159 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                     ); &__c[0];}))
# 159 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                     )));
        }
        myDFPlayer.setTimeOut(500);
        myDFPlayer.volume(config.vol);
        myDFPlayer.outputDevice(2);
    }
    lcd.clear();
    timrLCD = millis();

    if (serviceMode)
        Menu1.attach(serviceMenu_Root, (sizeof(serviceMenu_Root) / sizeof(MenuItem)));
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
    if (btn == (2 | (2 << 6)))
    {
        if (!ButLongPress)
        {
            if (config.AutoPlay == 0)
            {
                config.AutoPlay = 1;
                lcd.setCursor(0, 1);
                lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 203 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 203 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                         "  AUTO PLAY ON  "
# 203 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                         ); &__c[0];}))
# 203 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                         )));
            }
            else
            {
                config.AutoPlay = 0;
                lcd.setCursor(0, 1);
                lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 209 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 209 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                         " AUTO PLAY OFF  "
# 209 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                         ); &__c[0];}))
# 209 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                         )));
            }
            digitalWrite(A0, config.AutoPlay);
            delay(1000);
            // lcd.clear();
            config.save();
            ButLongPress = true;
        }
    }
    if (btn == (3 | (2 << 6)))
    {
        if (!ButLongPress)
        {
            if (config.AutoLight == 0)
            {
                config.AutoLight = 1;
                lcd.setCursor(0, 1);
                lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 226 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 226 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                         " AUTO LIGHT ON  "
# 226 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                         ); &__c[0];}))
# 226 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                         )));
            }
            else
            {
                config.AutoLight = 0;
                lcd.setCursor(0, 1);
                lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 232 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 232 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                         " AUTO LIGHT OFF "
# 232 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                         ); &__c[0];}))
# 232 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                         )));
            }
            digitalWrite(A1, config.AutoLight);
            delay(1000);
            // lcd.clear();
            config.save();
            ButLongPress = true;
        }
    }
    if (btn == (3 | (3 << 6)) || btn == (2 | (3 << 6)) || btn == (4 | (3 << 6)) || btn == (5 | (3 << 6)))
    {
        ButLongPress = false;
    }

    if (btn == (3 | (0 << 6)) && (appMode == APP_NORMAL_MODE || appMode == APP_PLAYER_MODE))
    {
        downButPreCount += 1;
        if (downButPreCount >= 2)
        {
            digitalWrite(11, !digitalRead(11));
            Light1ON = !Light1ON;
        }
    }
    if (millis() - timrLCD > 300)
    {
        downButPreCount = 0;
        selButPreCount = 0;
        upButPreCount = 0;
    }

    if (millis() - timrTrigger > 500 && config.AutoLight)
    {
        timrTrigger = millis();
        TriggerLights();
    }
    digitalWrite(A1, config.AutoLight);
    digitalWrite(A0, config.AutoPlay);

    switch (appMode)
    {
    case APP_NORMAL_MODE:
        digitalWrite(12, 0x0);
        digitalWrite(10, 0x0);
        if (btn == (1 | (2 << 6)) /*128*/)
        {
            appMode = APP_MENU_MODE;
            refreshMenuDisplay(REFRESH_DESCEND);
            timrMNU = millis();
        }

        if (btn == (1 | (0 << 6)) /*1*/)
        {
            selButPreCount += 1;
            if (selButPreCount >= 4)
            {
                demoTime = rtc.now().minute() > 30 ? rtc.now().hour() + 1 : rtc.now().hour();
                appMode = APP_DEMO_MODE;
                lcd.clear();
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
                    lcd.setCursor(0, 0);
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
        if (millis() - timr_Time > 1000)
        {
            timr_Time = millis();
            showTime();
            CurTime = (rtc.now().hour() * 60) + rtc.now().minute();
            if (config.AutoPlay)
                TriggerPlayer();
        }
        if (millis() - timrLCD > (config.backLightDur * 1000L))
        {
            lcd.noBacklight();
        }
        if (btn == (5 | (2 << 6)))
        {
            if (!ButLongPress)
            {
                appMode = APP_PLAYER_MODE;
                lcd.clear();
                ButLongPress = true;
            }
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
# 367 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 367 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                 "  RUNNING DEMO  "
# 367 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                 ); &__c[0];}))
# 367 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
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
        if (millis() - timrMNU > 30000)
        {
            if (!digitalRead(4))
            {
                stopPreSong();
            }
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
        if (Menu1.getCurrentItemCmdId() == mnuCmdSetTime ||
            Menu1.getCurrentItemCmdId() == mnuCmdLt1OnTime ||
            Menu1.getCurrentItemCmdId() == mnuCmdLt1OffTime ||
            Menu1.getCurrentItemCmdId() == mnuCmdPlrMorOnTime ||
            Menu1.getCurrentItemCmdId() == mnuCmdPlrMorDur ||
            Menu1.getCurrentItemCmdId() == mnuCmdPlrEveOnTime ||
            Menu1.getCurrentItemCmdId() == mnuCmdPlrEveDur)
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
            if (!digitalRead(4))
            {
                stopPreSong();
            }
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
        if (millis() - timrMNU > 30000)
        {
            if (!digitalRead(4))
            {
                stopPreSong();
            }
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

    case APP_PLAYER_MODE:
    {
        digitalWrite(12, 0x1);
        digitalWrite(10, 0x1);
        if (millis() - timr_Time > 1000)
        {
            timr_Time = millis();
            showTime();
            CurTime = (rtc.now().hour() * 60) + rtc.now().minute();
            if (config.AutoPlay)
                TriggerPlayer();
        }
        if (millis() - timrLCD > (config.backLightDur * 1000L))
        {
            lcd.noBacklight();
        }
        lcd.setCursor(0, 1);
        lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 513 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 513 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                 " PLAYER MODE ON "
# 513 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                 ); &__c[0];}))
# 513 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                 )));
        if (btn == (5 | (2 << 6)))
        {
            if (!ButLongPress)
            {
                appMode = APP_NORMAL_MODE;
                PlayerONMor = 0;
                PlayerONEve = 0;
                lcd.clear();
                ButLongPress = true;
            }
        }
        break;
    }
    }
}

void TriggerLights()
{
    // Validate Light 1
    if (config.Light1On != 0 && config.Light1Off != 0)
    {
        if (config.Light1Off > config.Light1On)
        {
            if (CurTime >= config.Light1On && CurTime < config.Light1Off)
            {
                if (Light1ON == 0)
                {
                    digitalWrite(11, 0x1);
                    Light1ON = 1;
                }
            }
            else
            {
                if (Light1ON == 1)
                {
                    digitalWrite(11, 0x0);
                    Light1ON = 0;
                }
            }
        }
        else
        {
            if (CurTime < config.Light1On && CurTime >= config.Light1Off)
            {
                if (Light1ON == 1)
                {
                    digitalWrite(11, 0x0);
                    Light1ON = 0;
                }
            }
            else
            {
                if (Light1ON == 0)
                {
                    digitalWrite(11, 0x1);
                    Light1ON = 1;
                }
            }
        }
    }
}

void TriggerPlayer()
{
    // Validate Player Morning
    if (CurTime >= config.PlayerMorTime && CurTime <= config.PlayerMorTime + config.PlayerMorDur && config.PlayerMorDur != 0)
    {
        if (PlayerONMor == 0)
        {
            PlayerONMor = 1;
            appMode = APP_PLAYER_MODE;
            lcd.clear();
        }
    }
    // Validate Player Evening
    else if (CurTime >= config.PlayerEveTime && CurTime <= config.PlayerEveTime + config.PlayerEveDur && config.PlayerEveDur != 0)
    {
        if (PlayerONEve == 0)
        {
            PlayerONEve = 1;
            appMode = APP_PLAYER_MODE;
            lcd.clear();
        }
    }
    else
    {
        if (appMode == APP_PLAYER_MODE)
        {
            if (PlayerONEve == 1 || PlayerONMor == 1)
            {
                digitalWrite(12, 0x0);
                digitalWrite(10, 0x0);
                PlayerONMor = 0;
                PlayerONEve = 0;
                lcd.clear();
                appMode = APP_NORMAL_MODE;
            }
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

    lcd.setCursor(0, 0);
    lcd.print(fmt(strbuf, 7, "  ", h, ":", m, ":", s, rtc.now().isPM() ? " PM   " : " AM   "));

    inttostr(intbuf, rtc.now().day());
    lpad(dat, intbuf, '0', 2);
    inttostr(intbuf, rtc.now().year());
    lpad(year, intbuf, '0', 4);
    strcpy(mon, months[rtc.now().month() - 1].c_str());

    lcd.setCursor(0, 1);
    lcd.print(fmt(strbuf, 7, "  ", dat, "-", mon, "-", year, "   "));
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
        // Set School
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
            startPreSong(2, config.MorSongNo);
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.MorSongNo = --config.MorSongNo < 1 ? 20 : config.MorSongNo;
            startPreSong(2, config.MorSongNo);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            stopPreSong();
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
            startPreSong(2, config.EveSongNo);
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.EveSongNo = --config.EveSongNo < 1 ? 20 : config.EveSongNo;
            startPreSong(2, config.EveSongNo);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            stopPreSong();
            config.save();
            complete = true;
        }
        else
        {
            configChanged = false;
        }
        break;
    }

    case mnuCmdLt1OnTime:
    {
        if (btn == (2 | (0 << 6)))
        {
            if (blinkMode == setHr)
                config.Light1On = addToTime(60, config.Light1On);
            else if (blinkMode == setMin)
                config.Light1On = addToTime(1, config.Light1On);
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            if (blinkMode == setHr)
                config.Light1On = addToTime(-60, config.Light1On);
            else if (blinkMode == setMin)
                config.Light1On = addToTime(-1, config.Light1On);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            if (blinkMode == setHr)
            {
                blinkMode = setMin;
            }
            else if (blinkMode == setMin)
            {
                config.save();
                complete = true;
                blink = true;
            }
        }

        break;
    }
    case mnuCmdLt1OffTime:
    {
        if (btn == (2 | (0 << 6)))
        {
            if (blinkMode == setHr)
                config.Light1Off = addToTime(60, config.Light1Off);
            else if (blinkMode == setMin)
                config.Light1Off = addToTime(1, config.Light1Off);
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            if (blinkMode == setHr)
                config.Light1Off = addToTime(-60, config.Light1Off);
            else if (blinkMode == setMin)
                config.Light1Off = addToTime(-1, config.Light1Off);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            if (blinkMode == setHr)
            {
                blinkMode = setMin;
            }
            else if (blinkMode == setMin)
            {
                config.save();
                complete = true;
                blink = true;
            }
        }
        break;
    }
    case mnuCmdPlrMorOnTime:
    {
        if (btn == (2 | (0 << 6)))
        {
            if (blinkMode == setHr)
                config.PlayerMorTime = addToTime(60, config.PlayerMorTime);
            else if (blinkMode == setMin)
                config.PlayerMorTime = addToTime(1, config.PlayerMorTime);
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            if (blinkMode == setHr)
                config.PlayerMorTime = addToTime(-60, config.PlayerMorTime);
            else if (blinkMode == setMin)
                config.PlayerMorTime = addToTime(-1, config.PlayerMorTime);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            if (blinkMode == setHr)
            {
                blinkMode = setMin;
            }
            else if (blinkMode == setMin)
            {
                config.save();
                complete = true;
                blink = true;
            }
        }
        break;
    }
    case mnuCmdPlrMorDur:
    {
        if (btn == (2 | (0 << 6)))
        {
            if (blinkMode == setHr)
                config.PlayerMorDur = addToTime(60, config.PlayerMorDur);
            else if (blinkMode == setMin)
                config.PlayerMorDur = addToTime(5, config.PlayerMorDur);
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            if (blinkMode == setHr)
                config.PlayerMorDur = addToTime(-60, config.PlayerMorDur);
            else if (blinkMode == setMin)
                config.PlayerMorDur = addToTime(-5, config.PlayerMorDur);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            if (blinkMode == setHr)
            {
                blinkMode = setMin;
            }
            else if (blinkMode == setMin)
            {
                config.save();
                complete = true;
                blink = true;
            }
        }
        break;
    }
    case mnuCmdPlrEveOnTime:
    {
        if (btn == (2 | (0 << 6)))
        {
            if (blinkMode == setHr)
                config.PlayerEveTime = addToTime(60, config.PlayerEveTime);
            else if (blinkMode == setMin)
                config.PlayerEveTime = addToTime(1, config.PlayerEveTime);
        }
        else if (btn == (3 | (0 << 6)))
        {
            if (blinkMode == setHr)
                config.PlayerEveTime = addToTime(-60, config.PlayerEveTime);
            else if (blinkMode == setMin)
                config.PlayerEveTime = addToTime(-1, config.PlayerEveTime);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            if (blinkMode == setHr)
            {
                blinkMode = setMin;
            }
            else if (blinkMode == setMin)
            {
                config.save();
                complete = true;
                blink = true;
            }
        }
        break;
    }
    case mnuCmdPlrEveDur:
    {
        if (btn == (2 | (0 << 6)))
        {
            if (blinkMode == setHr)
                config.PlayerEveDur = addToTime(60, config.PlayerEveDur);
            else if (blinkMode == setMin)
                config.PlayerEveDur = addToTime(5, config.PlayerEveDur);
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            if (blinkMode == setHr)
                config.PlayerEveDur = addToTime(-60, config.PlayerEveDur);
            else if (blinkMode == setMin)
                config.PlayerEveDur = addToTime(-5, config.PlayerEveDur);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            if (blinkMode == setHr)
            {
                blinkMode = setMin;
            }
            else if (blinkMode == setMin)
            {
                config.save();
                complete = true;
                blink = true;
            }
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
                if (cmdId == mnuCmdSetTime ||
                    cmdId == mnuCmdLt1OnTime ||
                    cmdId == mnuCmdLt1OffTime ||
                    cmdId == mnuCmdPlrMorOnTime ||
                    cmdId == mnuCmdPlrMorDur ||
                    cmdId == mnuCmdPlrEveOnTime ||
                    cmdId == mnuCmdPlrEveDur)
                {
                    if (blinkMode == setHr)
                        lcd.setCursor(1, 1);
                    if (blinkMode == setMin)
                        lcd.setCursor(4, 1);
                    lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 1238 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1238 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                             "  "
# 1238 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                             ); &__c[0];}))
# 1238 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
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
# 1249 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1249 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
                             "  "
# 1249 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino" 3
                             ); &__c[0];}))
# 1249 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\ChantBellV3.2.ino"
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
    byte cmdId;
    cmdId = Menu1.getCurrentItemCmdId();
    lcd.setCursor(0, 0);
    if (Menu1.currentItemHasChildren())
    {
        rpad(strbuf, Menu1.getCurrentItemName(nameBuf));
        strbuf[16 - 1] = 0b01111110; // Display forward arrow if this menu item has children.
        lcd.print(strbuf);
        if (cmdId == mnuCmdLt1OnTime ||
            cmdId == mnuCmdLt1OffTime ||
            cmdId == mnuCmdPlrMorOnTime ||
            cmdId == mnuCmdPlrEveOnTime)
        {
            lcd.setCursor(0, 1);
            lcd.print(" ");
            lcd.print(rpad(strbuf, config.getFormattedStr(cmdId)));
        }
        else
        {
            lcd.setCursor(0, 1);
            lcd.print(rpad(strbuf, EmptyStr)); // Clear config value in display
        }
    }
    else
    {
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
            lcd.print(strbuf);
            lcd.setCursor(0, 1);
            lcd.print(" ");
            lcd.print(rpad(strbuf, config.getFormattedStr(cmdId)));
        }
    }
}

void startPreSong(byte fol, byte song)
{
    digitalWrite(12, 0x1);
    myDFPlayer.playFolder(fol, song);
    delay(500);
}
void stopPreSong()
{
    digitalWrite(12, 0x0);
    myDFPlayer.stop();
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
                myDFPlayer.playMp3Folder(random(1, config.Song1_Count));
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
            myDFPlayer.playFolder(6, random(1, config.Song2_Count));
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
