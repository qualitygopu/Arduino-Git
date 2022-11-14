# 1 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
# 2 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 2
# 3 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 2
# 4 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 2
# 5 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 2
# 6 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 2
# 7 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 2
# 8 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 2
# 9 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 2
# 10 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 2
# 19 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
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
bool blink = true, serviceMode = false;

MenuManager Menu1;
Config config;

bool ButLongPress = false;
byte alarmMode = 0;
byte playSong = 1;
byte btn;
char strbuf[16 + 1];

String months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};


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
    char intstr[5];
    // Serial.begin(9600);
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
# 99 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 99 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
             "QTRON"
# 99 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
             ); &__c[0];}))
# 99 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
             )));
    lcd.setCursor(0, 1);
    lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 101 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 101 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
             "DEVOTIONAL CLOCK"
# 101 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
             ); &__c[0];}))
# 101 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
             )));
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 105 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 105 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
             "Version : 3.1"
# 105 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
             ); &__c[0];}))
# 105 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
             )));
    lcd.setCursor(0,1);
    inttostr(intstr, config.SerialNo);
    lcd.print(fmt(strbuf, 1, "Shop Demo"));
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
# 126 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 126 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
                 "ERR 01"
# 126 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                 ); &__c[0];}))
# 126 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
                 ))); // Couldn't find RTC
    }

    if (!rtc.isrunning())
    {
        lcd.clear();
        lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 132 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 132 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
                 "ERR 02"
# 132 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                 ); &__c[0];}))
# 132 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
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
# 147 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 147 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
                     "ERR 03"
# 147 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                     ); &__c[0];}))
# 147 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
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
# 180 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 180 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
                         "  AUTO PLAY ON  "
# 180 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                         ); &__c[0];}))
# 180 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
                         )));
            }
            else
            {
                config.AutoPlay = 0;
                lcd.setCursor(0, 1);
                lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 186 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 186 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
                         " AUTO PLAY OFF  "
# 186 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                         ); &__c[0];}))
# 186 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
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
# 203 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 203 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
                         " AUTO LIGHT ON  "
# 203 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                         ); &__c[0];}))
# 203 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
                         )));
            }
            else
            {
                config.AutoLight = 0;
                lcd.setCursor(0, 1);
                lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 209 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 209 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
                         " AUTO LIGHT OFF "
# 209 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                         ); &__c[0];}))
# 209 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
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
    if (millis() - timrLCD > 500)
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
            Menu1.attach(normalMenu_Root, (sizeof(normalMenu_Root) / sizeof(MenuItem)));
            appMode = APP_MENU_MODE;
            refreshMenuDisplay(REFRESH_DESCEND);
            timrMNU = millis();
        }
        if (btn == (1 | (1 << 6)) /*65*/)
        {
            Menu1.attach(demoMenu_Root, (sizeof(demoMenu_Root) / sizeof(MenuItem)));
            appMode = APP_MENU_MODE;
            refreshMenuDisplay(REFRESH_DESCEND);
            timrMNU = millis();
        }


        if (btn == (4 | (0 << 6)))
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
        if (btn == (4 | (2 << 6)))
        {
            if (!ButLongPress)
            {
                appMode = APP_SETHR_MODE;
                timrMNU = millis();
            }
        }
        break;
    case APP_SETHR_MODE:
    {
        if (millis() - timr_Time > 300)
        {
            if (blink)
            {
                showTime();
                blink = false;
            }
            else
            {
                lcd.setCursor(2, 0);
                lcd.print("  ");
                blink = true;
            }
            timr_Time = millis();
        }

        if (btn == (2 | (0 << 6)))
        {
            rtc.adjust(DateTime(rtc.now().year(),
                                rtc.now().month(),
                                rtc.now().day(),
                                rtc.now().hour() > 23 ? 0 : rtc.now().hour() + 1,
                                rtc.now().minute(),
                                rtc.now().second()));
        }
        if (btn == (3 | (0 << 6)))
        {
            rtc.adjust(DateTime(rtc.now().year(),
                                rtc.now().month(),
                                rtc.now().day(),
                                rtc.now().hour() < 0 ? 23 : rtc.now().hour() - 1,
                                rtc.now().minute(),
                                rtc.now().second()));
        }
        if (btn == (1 | (0 << 6)) /*1*/ || millis() - timrMNU > 10000)
        {
            appMode = APP_NORMAL_MODE;
            lcd.clear();
        }
        if (btn == (4 | (0 << 6)))
        {
            appMode = APP_SETMIN_MODE;
            timrMNU = millis();
        }
        break;
    }
    case APP_SETMIN_MODE:
    {
        if (millis() - timr_Time > 300)
        {
            if (blink)
            {
                showTime();
                blink = false;
            }
            else
            {
                lcd.setCursor(5, 0);
                lcd.print("  ");
                blink = true;
            }
            timr_Time = millis();
        }

        if (btn == (2 | (0 << 6)))
        {
            rtc.adjust(DateTime(rtc.now().year(),
                                rtc.now().month(),
                                rtc.now().day(),
                                rtc.now().hour(),
                                rtc.now().minute() > 59 ? 0 : rtc.now().minute() + 1,
                                rtc.now().second()));
        }
        if (btn == (3 | (0 << 6)))
        {
            rtc.adjust(DateTime(rtc.now().year(),
                                rtc.now().month(),
                                rtc.now().day(),
                                rtc.now().hour(),
                                rtc.now().minute() < 0 ? 59 : rtc.now().minute() - 1,
                                rtc.now().second()));
        }
        if (btn == (1 | (0 << 6)) /*1*/ || btn == (4 | (0 << 6)) || millis() - timrMNU > 10000)
        {
            appMode = APP_NORMAL_MODE;
            lcd.clear();
        }
        break;
    }
    case APP_CHANT_MODE:
    {
        lcd.backlight();
        lcd.setCursor(0, 1);
        lcd.print("CHANT PLAYING...");
        if (millis() >= timrAmp + ampDelay)
            PlayChant();
        break;
    }
    case APP_MENU_MODE:
    {
        // if (millis() - timrMNU > 30000)
        // {
        //     if (!digitalRead(STA_PIN))
        //     {
        //         stopPreSong();
        //     }
        //     Menu1.reset();
        //     appMode = APP_NORMAL_MODE;
        //     lcd.clear();
        // }

        switch (Menu1.getCurrentItemCmdId())
        {
        case mnuCmdSetDateTime:
            StDate = rtc.now().day();
            StMonth = rtc.now().month();
            StYear = rtc.now().year();
            StHr = rtc.now().hour();
            StMin = rtc.now().minute();
            break;
        case mnuCmdSetTime:
        case mnuCmdLt1OnTime:
        case mnuCmdLt1OffTime:
        case mnuCmdPlrMorOnTime:
        case mnuCmdPlrMorDur:
        case mnuCmdPlrEveOnTime:
        case mnuCmdPlrEveDur:
            blinkMode = setHr;
            break;
        case mnuCmdSetDate:
            blinkMode = setDate;
            break;
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
        // if (millis() - timrMNU > 30000)
        // {
        //     if (!digitalRead(STA_PIN))
        //     {
        //         stopPreSong();
        //     }
        //     Menu1.reset();
        //     appMode = APP_NORMAL_MODE;
        //     lcd.clear();
        // }
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
        lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 527 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 527 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
                 "  RUNNING DEMO  "
# 527 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                 ); &__c[0];}))
# 527 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
                 )));
        lcd.setCursor(0, 1);
        lcd.print("DEMO TIME : " + String(demoTime > 12 ? demoTime - 12 : demoTime) + String(demoTime >= 12 ? "PM" : "AM"));
        digitalWrite(12, 0x1);
        delay(1000);
        bell = DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), demoTime, 0, 0);
        PlayChant();
        if (btn == (3 | (0 << 6)))
        {
            playSong = 9;
            myDFPlayer.stop();
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
# 558 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 558 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
                 " PLAYER MODE ON "
# 558 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                 ); &__c[0];}))
# 558 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
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
                StYear = --StYear < 2050 ? 2020 : StYear;
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
            config.SlogamCount = ++config.SlogamCount;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.SlogamCount = --config.SlogamCount < 0 ? 100 : config.SlogamCount;
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
    case mnuCmdBakLitDur:
    {
        configChanged = true;
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.backLightDur = ++config.backLightDur > 60 ? 0 : config.backLightDur;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.backLightDur = --config.backLightDur < 0 ? 60 : config.backLightDur;
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
    case mnuCmdMorSong2Time:
    {
        configChanged = true;
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.MorSong2Time = ++config.MorSong2Time > 8 ? 3 : config.MorSong2Time;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.MorSong2Time = --config.MorSong2Time < 3 ? 8 : config.MorSong2Time;
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
    case mnuCmdEveSong2Time:
    {
        configChanged = true;
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.EveSong2Time = ++config.EveSong2Time > 8 ? 3 : config.EveSong2Time;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.EveSong2Time = --config.EveSong2Time < 3 ? 8 : config.EveSong2Time;
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
    case mnuCmdSongCount:
    {
        configChanged = true;
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.SongCount = ++config.SongCount;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.SongCount = --config.SongCount < 0 ? 100 : config.SongCount;
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

    case mnuCmdVoiceDemo:
    {
        configChanged = true;
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            voiceNo = ++voiceNo > voiceCount-1 ? 0 : voiceNo;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            voiceNo = --voiceNo < 0 ? voiceCount-1 : voiceNo;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            startPreSong(10,voiceNo+1);
        }
        else if (btn == (4 | (0 << 6)))
        {
            stopPreSong();
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdSongsDemo:
    {
        configChanged = true;
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            demoSongNo = ++demoSongNo > demoSongCount-1 ? 0 : demoSongNo;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            demoSongNo = --demoSongNo < 0 ? demoSongCount-1 : demoSongNo;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            startPreSong(demoSongNo+11, random(0,20));
        }
        else if (btn == (4 | (0 << 6)))
        {
            stopPreSong();
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
# 1379 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1379 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
                             "  "
# 1379 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                             ); &__c[0];}))
# 1379 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
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
# 1390 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1390 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
                             "  "
# 1390 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino" 3
                             ); &__c[0];}))
# 1390 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\ChantBellV3.1_Demo.ino"
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
    switch (playSong)
    {
    case 1: // Temple Name
        if (digitalRead(4))
        {
            myDFPlayer.playMp3Folder(0);
            delay(1000);
            playSong = 2;
        }
        break;
    case 2:
        if (digitalRead(4)) // Time
        {
            myDFPlayer.playFolder(1, bell.hour());
            delay(1000);
            playSong = 3;
        }
        break;
    case 3: // English Month
        if (digitalRead(4))
        {
            myDFPlayer.playFolder(47, bell.month());
            delay(1000);
            playSong = 4;
        }
        break;
    case 4: // Englidh Date
        if (digitalRead(4))
        {
            myDFPlayer.playFolder(48, bell.day());
            delay(1000);
            playSong = 5;
        }
        break;
    case 5: // Day of Week
        if (digitalRead(4))
        {
            myDFPlayer.playFolder(49, bell.dayOfTheWeek() + 1);
            delay(1000);
            playSong = 6;
        }
        break;
    case 6: // Tamil Date
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
                playSong = 8;
            }
            else
            {
                playSong = 8;
            }
        }
        break;
    case 8:
        if (digitalRead(4))
        {
            if (config.MorSong1Time == bell.hour() && config.MorSong1Time != 3)
            {
                myDFPlayer.playFolder(2, config.MorSongNo);
                delay(1000);
            }
            else if (config.MorSong2Time == bell.hour() && config.MorSong2Time != 3)
            {
                myDFPlayer.playFolder(4, config.SongOrder);
                config.SongOrder++;
                if (config.SongOrder > config.SongCount)
                    config.SongOrder = 1;
                config.save();
                delay(1000);
            }
            else if ((config.EveSong1Time + 12) == bell.hour() && (config.EveSong1Time != 3))
            {
                myDFPlayer.playFolder(2, config.EveSongNo);
                delay(1000);
            }
            else if ((config.EveSong2Time + 12) == bell.hour() && (config.EveSong2Time != 3))
            {
                myDFPlayer.playFolder(4, config.SongOrder);
                config.SongOrder++;
                if (config.SongOrder > config.SongCount)
                    config.SongOrder = 1;
                config.save();
                delay(1000);
            }
            else
            {
                myDFPlayer.playMp3Folder(config.SlogamOrder);
                config.SlogamOrder++;
                if (config.SlogamOrder > config.SlogamCount)
                    config.SlogamOrder = 1;
                config.save();
                delay(1000);
            }
            playSong = 9;
        }
        break;
    case 9:
        if (digitalRead(4))
        {
            playSong = 1;
            lcd.clear();
            appMode = APP_NORMAL_MODE;
            timrLCD = millis();
            digitalWrite(12, 0x0);
        }
        break;
    }
    // Serial.println(String(hr));
}
