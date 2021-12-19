# 1 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
# 2 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 2
# 3 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 2
# 4 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 2
# 5 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 2
# 6 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 2
# 7 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 2
# 8 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 2
# 9 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 2
# 10 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 2
# 21 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
long CurTime;
byte Light1ON = 0, Light2ON = 0, PlayerONMor = 0, PlayerONEve = 0;
int ampDelay = 5000;
unsigned long timrAmp, timrLCD, timr_Time, timrMNU, timrTrigger;
SoftwareSerial mySoftwareSerial(2, 3); // TX, RX
DFRobotDFPlayerMini myDFPlayer;
LiquidCrystal_I2C lcd(0x27, 16, 2);

short upButPreCount = 0,selButPreCount = 0, downButPreCount = 0;
// RTC_DS3231 rtc;
RTC_DS1307 rtc;

//blink time 
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
    // Serial.begin(9600);
    pinMode(A0, 0x1);
    pinMode(13, 0x1);
    pinMode(A2, 0x1);
    pinMode(A1, 0x1);
    pinMode(11, 0x1);
    pinMode(12, 0x1);
    pinMode(13, 0x1);
    pinMode(5, 0x2);
    pinMode(6, 0x2);
    pinMode(7, 0x2);
    pinMode(8, 0x2);
    pinMode(9, 0x2);
    Wire.begin();
    lcd.init();
    lcd.backlight();
    lcd.setCursor(5, 0);
    lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 88 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 88 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
             "WELCOME"
# 88 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
             ); &__c[0];}))
# 88 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
             )));
    lcd.setCursor(6, 1);
    lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 90 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 90 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
             "QTRON"
# 90 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
             ); &__c[0];}))
# 90 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
             )));
    delay(2000);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Loading...");
    config.load();
    if (!digitalRead(6)) {
      lcd.setCursor(0, 1);
      lcd.print("Service Mode");
      serviceMode = true;
    }
    else {
      serviceMode = false;
    }
    while (!rtc.begin())
    {
        lcd.clear();
        lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 107 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 107 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
                 "ERR 01"
# 107 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                 ); &__c[0];}))
# 107 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
                 ))); // Couldn't find RTC
    }

    if (!rtc.isrunning())
    {
        lcd.clear();
        lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 113 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 113 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
                 "ERR 02"
# 113 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                 ); &__c[0];}))
# 113 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
                 )));
        delay(2000);
        serviceMode = true;
    }
    lcd.clear();

    // Initialize DF Player...............
    if (digitalRead(9))
    {
        mySoftwareSerial.begin(9600);
        delay(500);
        while (!myDFPlayer.begin(mySoftwareSerial))
        {
            lcd.setCursor(0, 0);
            lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 127 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 127 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
                     "ERR 03"
# 127 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                     ); &__c[0];}))
# 127 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
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

    //   rtc.adjust(DateTime(2021,07,18,17,59,57));
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
# 160 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 160 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
                         "  AUTO PLAY ON  "
# 160 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                         ); &__c[0];}))
# 160 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
                         )));
            }
            else
            {
                config.AutoPlay = 0;
                lcd.setCursor(0, 1);
                lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 166 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 166 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
                         " AUTO PLAY OFF  "
# 166 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                         ); &__c[0];}))
# 166 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
                         )));
            }
            digitalWrite(A1, config.AutoPlay);
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
# 183 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 183 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
                         " AUTO LIGHT ON  "
# 183 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                         ); &__c[0];}))
# 183 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
                         )));
            }
            else
            {
                config.AutoLight = 0;
                lcd.setCursor(0, 1);
                lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 189 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 189 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
                         " AUTO LIGHT OFF "
# 189 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                         ); &__c[0];}))
# 189 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
                         )));
            }
            digitalWrite(A2, config.AutoLight);
            delay(1000);
            // lcd.clear();
            config.save();
            ButLongPress = true;
        }
    }
    if (btn == (3 | (3 << 6)) || btn == (2 | (3 << 6)) || btn == (4 | (3 << 6)))
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
        selButPreCount =0;
        upButPreCount=0;
    }

    if (millis() - timrTrigger > 1000)
    {
        timrTrigger = millis();
        if (config.AutoLight)
            TriggerLights();
    }
    digitalWrite(A2, config.AutoLight);
    digitalWrite(A1, config.AutoPlay);

    switch (appMode)
    {
    case APP_NORMAL_MODE:
        digitalWrite(A0, 0x0);
        digitalWrite(13, 0x0);
        if (btn == (1 | (2 << 6)) /*128*/)
        {
            appMode = APP_MENU_MODE;
            refreshMenuDisplay(REFRESH_DESCEND);
            timrMNU = millis();
        }

        if (btn == (1 | (0 << 6)) /*1*/) {
          selButPreCount +=1;
          if (selButPreCount >= 4)
          {
            demoTime = random(6,18);
            appMode = APP_DEMO_MODE;
            lcd.clear();
          }
        }

        if (rtc.now().minute() == 0 && rtc.now().second() == 0 && millis() > timrAmp + 61000L)
        {
            int hr = rtc.now().hour();
            if (hr >= config.startTime && hr <= config.endTime) // chek time
            {
                digitalWrite(A0, 0x1);
                timrAmp = millis();
                appMode = APP_CHANT_MODE;
                lcd.clear();
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
        if (btn == (2 | (0 << 6)))
        {
            upButPreCount += 1;
            if (upButPreCount >= 2)
            {
                appMode = APP_PLAYER_MODE;
                lcd.clear();
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
                lcd.setCursor(2,0);
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
                lcd.setCursor(5,0);
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
                                rtc.now().minute() > 59 ? 0 : rtc.now().minute()+1,
                                rtc.now().second()));
        }
        if (btn == (3 | (0 << 6)))
        {
            rtc.adjust(DateTime(rtc.now().year(),
                                rtc.now().month(),
                                rtc.now().day(),
                                rtc.now().hour(),
                                rtc.now().minute() < 0 ? 59 : rtc.now().minute()-1,
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
            PlayChant(rtc.now().hour());
        break;
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
        if (millis() - timrMNU > 30000)
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
        lcd.print((reinterpret_cast<const __FlashStringHelper *>(
# 457 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 457 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
                 "  RUNNING DEMO  "
# 457 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                 ); &__c[0];}))
# 457 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
                 )));
        lcd.setCursor(0, 1);
        lcd.print("DEMO TIME : " + String(demoTime > 12 ? demoTime - 12 : demoTime) + String(demoTime >= 12 ? "PM" : "AM"));
        digitalWrite(A0, 0x1);
        delay(1000);
        PlayChant(demoTime);
        if (btn == (3 | (0 << 6)))
        {
            playSong = 9;
            myDFPlayer.stop();
        }
        break;
    }
    case APP_PLAYER_MODE:
    {
        digitalWrite(A0, 0x1);
        digitalWrite(13, 0x1);
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
# 487 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 487 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
                 " PLAYER MODE ON "
# 487 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino" 3
                 ); &__c[0];}))
# 487 "e:\\Arduino\\Arduino Git\\ChantBellV3SingleLight\\ChantBellV3SingleLight.ino"
                 )));
        if (btn == (2 | (0 << 6)))
        {
            upButPreCount += 1;
            if (upButPreCount >= 2)
            {
                appMode = APP_NORMAL_MODE;
                PlayerONMor = 0;
                PlayerONEve = 0;
                lcd.clear();
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
    // Validate Light 2
    if (config.Light2On != 0 && config.Light2Off != 0)
    {
        if (config.Light2Off > config.Light2On)
        {
            if (CurTime >= config.Light2On && CurTime < config.Light2Off)
            {
                if (Light2ON == 0)
                {
                    digitalWrite(12, 0x1);
                    Light2ON = 1;
                }
            }
            else
            {
                if (Light2ON == 1)
                {
                    digitalWrite(12, 0x0);
                    Light2ON = 0;
                }
            }
        }
        else
        {
            if (CurTime < config.Light2On && CurTime >= config.Light2Off)
            {
                if (Light2ON == 1)
                {
                    digitalWrite(12, 0x0);
                    Light2ON = 0;
                }
            }
            else
            {
                if (Light2ON == 0)
                {
                    digitalWrite(12, 0x1);
                    Light2ON = 1;
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
                digitalWrite(A0, 0x0);
                digitalWrite(13, 0x0);
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

    if (btn == (1 | (0 << 6)) /*1*/)
    {
        complete = true;
    }
    switch (cmdId)
    {
    case mnuCmdSetDate:
    {
        configChanged = true;
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            StDate = ++StDate > 31 ? 1 : StDate;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            StDate = --StDate < 1 ? 31 : StDate;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            StMonth = ++StMonth > 12 ? 1 : StMonth;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            StMonth = --StMonth < 1 ? 12 : StMonth;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            StYear = ++StYear > 2050 ? 1 : StYear;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            StYear = --StYear < 1 ? 2020 : StYear;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            StHr = ++StHr > 23 ? 0 : StHr;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            StHr = --StHr < 0 ? 23 : StHr;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            StMin = ++StMin > 59 ? 0 : StMin;
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            StMin = --StMin < 0 ? 59 : StMin;
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
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
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdLt1OnHr:
    {
        configChanged = true;
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.Light1On = addToTime(60, config.Light1On);
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.Light1On = addToTime(-60, config.Light1On);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            config.save();
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdLt1OnMin:
    {
        configChanged = true;
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.Light1On = addToTime(1, config.Light1On);
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.Light1On = addToTime(-1, config.Light1On);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            config.save();
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdLt1OffHr:
    {
        configChanged = true;
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.Light1Off = addToTime(60, config.Light1Off);
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.Light1Off = addToTime(-60, config.Light1Off);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            config.save();
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdLt1OffMin:
    {
        configChanged = true;
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.Light1Off = addToTime(1, config.Light1Off);
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.Light1Off = addToTime(-1, config.Light1Off);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            config.save();
        }
        else
        {
            configChanged = false;
        }
        break;
    }

    case mnuCmdPlrMorHr:
    {
        configChanged = true;
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.PlayerMorTime = addToTime(60, config.PlayerMorTime);
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.PlayerMorTime = addToTime(-60, config.PlayerMorTime);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            config.save();
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdPlrMorMin:
    {
        configChanged = true;
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.PlayerMorTime = addToTime(1, config.PlayerMorTime);
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.PlayerMorTime = addToTime(-1, config.PlayerMorTime);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            config.save();
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdPlrMorDur:
    {
        configChanged = true;
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.PlayerMorDur = addToTime(5, config.PlayerMorDur);
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.PlayerMorDur = addToTime(-5, config.PlayerMorDur);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            config.save();
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdPlrEveHr:
    {
        configChanged = true;
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.PlayerEveTime = addToTime(60, config.PlayerEveTime);
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.PlayerEveTime = addToTime(-60, config.PlayerEveTime);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            config.save();
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdPlrEveMin:
    {
        configChanged = true;
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.PlayerEveTime = addToTime(1, config.PlayerEveTime);
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.PlayerEveTime = addToTime(-1, config.PlayerEveTime);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            config.save();
        }
        else
        {
            configChanged = false;
        }
        break;
    }
    case mnuCmdPlrEveDur:
    {
        configChanged = true;
        if (btn == (2 | (0 << 6)) || btn == (2 | (2 << 6)))
        {
            config.PlayerEveDur = addToTime(5, config.PlayerEveDur);
        }
        else if (btn == (3 | (0 << 6)) || btn == (3 | (2 << 6)))
        {
            config.PlayerEveDur = addToTime(-5, config.PlayerEveDur);
        }
        else if (btn == (1 | (0 << 6)) /*1*/)
        {
            config.save();
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

int n = 1;
void PlayChant(short hr)
{
    // Serial.println(String(hr));
    switch (playSong)
    {
    case 1:
        if (digitalRead(5))
        {
            myDFPlayer.playMp3Folder(0);
            delay(1000);
            playSong = 2;
        }
        break;
    case 2:
        if (digitalRead(5))
        {
            myDFPlayer.playFolder(1, hr);
            delay(1000);
            playSong = 3;
        }
        break;
    case 3:
        if (digitalRead(5))
        {
            if (config.MorSong1Time == hr && config.MorSong1Time != 3)
            {
                myDFPlayer.playFolder(2, config.MorSongNo);
                delay(1000);
            }
            else if (config.MorSong2Time == hr && config.MorSong2Time != 3)
            {
                myDFPlayer.playFolder(4, config.SongOrder);
                config.SongOrder++;
                if (config.SongOrder > config.SongCount)
                    config.SongOrder = 1;
                config.save();
                delay(1000);
            }
            else if ((config.EveSong1Time + 12) == hr && (config.EveSong1Time != 3))
            {
                myDFPlayer.playFolder(2, config.EveSongNo);
                delay(1000);
            }
            else if ((config.EveSong2Time + 12) == hr && (config.EveSong2Time != 3))
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
        if (digitalRead(5))
        {
            playSong = 1;
            lcd.clear();
            appMode = APP_NORMAL_MODE;
            timrLCD = millis();
            digitalWrite(A0, 0x0);
        }
        break;
    }
}
