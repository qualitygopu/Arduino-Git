#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include "LcdKeypad.h"
#include "MenuManager.h"
#include "MenuData.h"
#include "Config.h"

#define LOAD A3

LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;
unsigned long timrAmp, timrLCD, timr_Time, timrMNU, timrShowtimr;
MenuManager Menu1(sampleMenu_Root, menuCount(sampleMenu_Root));
Config config;
byte btn;
char strbuf[LCD_COLS + 1];
byte Load1ON = 0;
int CurTime;
bool blink = true;
enum AppModeValues
{
    APP_NORMAL_MODE,
    APP_MENU_MODE,
    APP_PROCESS_MENU_CMD,
    APP_SHOWTIMER_MODE
};

enum BlinkMode
{
    setHr,
    setMin
};
byte blinkMode = setHr;

byte appMode = APP_NORMAL_MODE;

void refreshMenuDisplay(byte refreshMode);
byte getNavAction();
byte processMenuCommand(byte cmdId);
void showTime();

void setup()
{
    pinMode(LOAD, OUTPUT);
    pinMode(SELECT_BUT, INPUT_PULLUP);
    pinMode(UP_BUT, INPUT_PULLUP);
    pinMode(DOWN_BUT, INPUT_PULLUP);
    pinMode(BACK_BUT, INPUT_PULLUP);
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
        lcd.print(("ERR 01")); // Couldn't find RTC
    }

    if (!rtc.isrunning())
    {
        lcd.clear();
        lcd.print(("ERR 02"));
        delay(2000);
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
        if (btn == BUTTON_DOWN_PRESSED)
        {
            appMode = APP_SHOWTIMER_MODE;
            lcd.clear();
            timrShowtimr = millis();
        }
        if (millis() - timr_Time > 1000)
        {
            timr_Time = millis();
            if (btn != BUTTON_DOWN_PRESSED)
            {
                showTime();
            }

            CurTime = (rtc.now().hour() * 60) + rtc.now().minute();
            TriggerLoad();
        }
        if (millis() - timrLCD > (20 * 1000L))
        {
            lcd.noBacklight();
        }
        break;
    case APP_SHOWTIMER_MODE:
    {
        lcd.setCursor(0, 0);
        lcd.print("ON  :");
        lcd.print(rpad(strbuf, toTimeStr(strbuf, config.timer1On)));
        lcd.setCursor(0, 1);
        lcd.print("OFF :");
        lcd.print(rpad(strbuf, toTimeStr(strbuf, config.timer1Off)));
        if (millis() - timrShowtimr > (5 * 1000L))
        {
            appMode = APP_NORMAL_MODE;
            lcd.clear();
        }
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
            StHr = rtc.now().hour();
            StMin = rtc.now().minute();
        }
        if (Menu1.getCurrentItemCmdId() == mnuCmdSetTime || 
            Menu1.getCurrentItemCmdId() == mnuCmdOnTime || 
            Menu1.getCurrentItemCmdId() == mnuCmdOffTime)
        {
            blinkMode = setHr;
        }
        // if (btn == BUTTON_LEFT_LONG_PRESSED)
        // {
        //     Menu1.reset();
        //     appMode = APP_NORMAL_MODE;
        //     lcd.clear();
        // }
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
    }
}

void showTime()
{
    char intbuf[8];
    char h[3], m[3], s[3];

    inttostr(intbuf, rtc.now().twelveHour());
    lpad(h, intbuf, '0', 2);
    inttostr(intbuf, rtc.now().minute());
    lpad(m, intbuf, '0', 2);
    inttostr(intbuf, rtc.now().second());
    lpad(s, intbuf, '0', 2);

    lcd.setCursor(2, 0);

    lcd.print(fmt(strbuf, 6, h, ":", m, ":", s, rtc.now().isPM() ? " PM" : " AM"));

}

void TriggerLoad()
{
    if (config.timer1Off > config.timer1On)
    {
        if (CurTime >= config.timer1On && CurTime < config.timer1Off)
        {
            if (Load1ON == 0)
            {
                digitalWrite(LOAD, HIGH);
                Load1ON = 1;
            }
        }
        else
        {
            if (Load1ON == 1)
            {
                digitalWrite(LOAD, LOW);
                Load1ON = 0;
            }
        }
    }
    else
    {
        if (CurTime < config.timer1On && CurTime >= config.timer1Off)
        {
            if (Load1ON == 1)
            {
                digitalWrite(LOAD, LOW);
                Load1ON = 0;
            }
        }
        else
        {
            if (Load1ON == 0)
            {
                digitalWrite(LOAD, HIGH);
                Load1ON = 1;
            }
        }
    }
}

byte processMenuCommand(byte cmdId)
{
    byte complete = false; // set to true when menu command processing complete.
    byte configChanged = false;

    switch (cmdId)
    {
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
                rtc.adjust(DateTime(2022, 1, 1, StHr, StMin, 0));
                complete = true;
                blink = true;
            }
        }
        break;
    }
    case mnuCmdOnTime:
    {
        if (btn == BUTTON_UP_PRESSED)
        {
            if (blinkMode == setHr)
                config.timer1On = addToTime(60, config.timer1On);
            else if (blinkMode == setMin)
                config.timer1On = addToTime(1, config.timer1On);
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            if (blinkMode == setHr)
                config.timer1On = addToTime(-60, config.timer1On);
            else if (blinkMode == setMin)
                config.timer1On = addToTime(-1, config.timer1On);
        }
        else if (btn == BUTTON_SELECT_PRESSED)
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
    case mnuCmdOffTime:
    {
        if (btn == BUTTON_UP_PRESSED)
        {
            if (blinkMode == setHr)
                config.timer1Off = addToTime(60, config.timer1Off);
            else if (blinkMode == setMin)
                config.timer1Off = addToTime(1, config.timer1Off);
        }
        else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
        {
            if (blinkMode == setHr)
                config.timer1Off = addToTime(-60, config.timer1Off);
            else if (blinkMode == setMin)
                config.timer1Off = addToTime(-1, config.timer1Off);
        }
        else if (btn == BUTTON_SELECT_PRESSED)
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
    }
    if (btn == BUTTON_LEFT_SHORT_RELEASE || complete )
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
                    cmdId == mnuCmdOnTime ||
                    cmdId == mnuCmdOffTime)
                {
                    if (blinkMode == setHr)
                        lcd.setCursor(1,1);
                    if (blinkMode == setMin)
                        lcd.setCursor(4,1);
                    lcd.print(("  "));
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
        // strbuf[LCD_COLS - 1] = 0b01111110; // Display forward arrow if this menu item has children.
        // lcd.print(strbuf);
        // lcd.setCursor(0, 1);
        // lcd.print(rpad(strbuf, EmptyStr)); // Clear config value in display
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
