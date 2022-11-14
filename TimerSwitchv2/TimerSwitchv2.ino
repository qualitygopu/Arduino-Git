#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include "LcdKeypad.h"
#include <EEPROM.h>

#define LOAD A3

LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;

struct Setting
{
    long OnTime, OffTime;
};

Setting setting;
char strbuf[LCD_COLS + 1];
byte btn;
byte Load1ON = 0;
int CurTime;
bool blink = true;
unsigned long timrAmp, timrLCD, timr_Time, timrMNU, timrShowtimr;
enum AppModeValues
{
    APP_NORMAL_MODE,
    APP_SET_TIME_HR_MODE,
    APP_SET_TIME_MIN_MODE,
    APP_SET_ON_HR_MODE,
    APP_SET_ON_MIN_MODE,
    APP_SET_OFF_HR_MODE,
    APP_SET_OFF_MIN_MODE,
};
byte appMode = APP_NORMAL_MODE;
void showTime();

void loadSettings()
{
  if (EEPROM.read(1023) == 'T')
  {
    EEPROM.get(0,setting);
  }
  else
  {
    setting.OnTime = 18 * 60;
    setting.OffTime = 6 * 60;
    EEPROM.put(0,setting);
    EEPROM.write(1023, 'T');
  }
}

void setup()
{
    pinMode(LOAD, OUTPUT);
    pinMode(SELECT_BUT, INPUT_PULLUP);
    pinMode(UP_BUT, INPUT_PULLUP);
    pinMode(DOWN_BUT, INPUT_PULLUP);
    pinMode(BACK_BUT, INPUT_PULLUP);
    EEPROM.begin();
    lcd.init();
    lcd.backlight();
    lcd.setCursor(4, 0);
    lcd.print(F("QTRONIX"));
    lcd.setCursor(2, 1);
    lcd.print(F("90438 10887"));
    delay(2000);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Loading...");
    loadSettings();
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
      {
        if (millis() - timrLCD > (20 * 1000L))
        {
            lcd.noBacklight();
        }
          if (btn == BUTTON_SELECT_LONG_PRESSED)
          {
              appMode = APP_SET_TIME_HR_MODE;
              lcd.clear();
          }
          else if (btn == BUTTON_UP_LONG_PRESSED) 
          {
              appMode = APP_SET_ON_HR_MODE;
              lcd.clear();
          }
          else if (btn == BUTTON_DOWN_LONG_PRESSED) 
          {
              appMode = APP_SET_OFF_HR_MODE;
              lcd.clear();
          }
          else if (btn == BUTTON_SELECT_SHORT_RELEASE)
          {
            lcd.clear();
            showTimers();
            delay(2000);
            lcd.clear();
          }
          
          if (millis() - timr_Time > 1000)
          {
              timr_Time = millis();
              showTime();
              CurTime = (rtc.now().hour() * 60) + rtc.now().minute();
              TriggerLoad();
          }
          break;
      }
      case APP_SET_TIME_HR_MODE:
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

          if (btn == BUTTON_UP_PRESSED)   
          {
              rtc.adjust(DateTime(rtc.now().year(), 
                                  rtc.now().month(), 
                                  rtc.now().day(), 
                                  rtc.now().hour() > 23 ? 0 : rtc.now().hour() + 1,
                                  rtc.now().minute(),
                                  rtc.now().second()));
          }
          if (btn == BUTTON_DOWN_PRESSED)   
          {
              rtc.adjust(DateTime(rtc.now().year(), 
                                  rtc.now().month(), 
                                  rtc.now().day(), 
                                  rtc.now().hour() < 0 ? 23 : rtc.now().hour() - 1,
                                  rtc.now().minute(),
                                  rtc.now().second()));
          }
          if (btn == BUTTON_SELECT_PRESSED || millis() - timrMNU > 10000)
          {
              appMode = APP_SET_TIME_MIN_MODE;
              timrMNU = millis();
          }   
          break;
      }
      case APP_SET_TIME_MIN_MODE:
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

          if (btn == BUTTON_UP_PRESSED)   
          {
              rtc.adjust(DateTime(rtc.now().year(), 
                                  rtc.now().month(), 
                                  rtc.now().day(), 
                                  rtc.now().hour(),
                                  rtc.now().minute() > 59 ? 0 : rtc.now().minute()+1,
                                  rtc.now().second()));
          }
          if (btn == BUTTON_DOWN_PRESSED)   
          {
              rtc.adjust(DateTime(rtc.now().year(), 
                                  rtc.now().month(), 
                                  rtc.now().day(), 
                                  rtc.now().hour(),
                                  rtc.now().minute() < 0 ? 59 : rtc.now().minute()-1,
                                  rtc.now().second()));
          }
          if (btn == BUTTON_SELECT_PRESSED || millis() - timrMNU > 10000)
          {
              appMode = APP_NORMAL_MODE;
              lcd.clear();
          }   
          break;
      }
      case APP_SET_ON_HR_MODE:
      {
          if (millis() - timr_Time > 300)
          {
              if (blink)
              { 
                  lcd.setCursor(0,0);
                  lcd.print("SET ON TIME :");
                  lcd.setCursor(0,1);
                  lcd.print(rpad(strbuf, toTimeStr(strbuf,setting.OnTime)));
                  blink = false;
              }
              else
              {
                  lcd.setCursor(0,1);
                  lcd.print("  ");
                  blink = true;
              }     
              timr_Time = millis();       
          }
          if (btn == BUTTON_UP_PRESSED)   
          {
              setting.OnTime = (setting.OnTime += 60) > 1439 ? 0 : setting.OnTime;
          }
          if (btn == BUTTON_DOWN_PRESSED)   
          {
              setting.OnTime = (setting.OnTime -= 60) < 0 ? 1439 : setting.OnTime;
          }
          if (btn == BUTTON_SELECT_PRESSED || millis() - timrMNU > 10000)
          {
              EEPROM.put(0,setting);
              appMode = APP_SET_ON_MIN_MODE;
          }   
          break;
      }
      case APP_SET_ON_MIN_MODE:
      {
          if (millis() - timr_Time > 300)
          {
              if (blink)
              { 
                  lcd.setCursor(0,0);
                  lcd.print("SET ON TIME :");
                  lcd.setCursor(0,1);
                  lcd.print(rpad(strbuf, toTimeStr(strbuf,setting.OnTime)));
                  blink = false;
              }
              else
              {
                  lcd.setCursor(3,1);
                  lcd.print("  ");
                  blink = true;
              }     
              timr_Time = millis();       
          }
          if (btn == BUTTON_UP_PRESSED)   
          {
              setting.OnTime = (setting.OnTime += 1) > 1439 ? 0 : setting.OnTime;
          }
          if (btn == BUTTON_DOWN_PRESSED)   
          {
              setting.OnTime = (setting.OnTime -= 1) < 0 ? 1439 : setting.OnTime;
          }
          if (btn == BUTTON_SELECT_PRESSED || millis() - timrMNU > 10000)
          {
              EEPROM.put(0,setting);
              appMode = APP_NORMAL_MODE;
              lcd.clear();
          }   
          break;
      }
      case APP_SET_OFF_HR_MODE:
      {
          if (millis() - timr_Time > 300)
          {
              if (blink)
              { 
                  lcd.setCursor(0,0);
                  lcd.print("SET OFF TIME :");
                  lcd.setCursor(0,1);
                  lcd.print(rpad(strbuf, toTimeStr(strbuf,setting.OffTime)));
                  blink = false;
              }
              else
              {
                  lcd.setCursor(0,1);
                  lcd.print("  ");
                  blink = true;
              }     
              timr_Time = millis();       
          }
          if (btn == BUTTON_UP_PRESSED)   
          {
              setting.OffTime = (setting.OffTime += 60) > 1439 ? 0 : setting.OffTime;
          }
          if (btn == BUTTON_DOWN_PRESSED)   
          {
              setting.OffTime = (setting.OffTime -= 60) < 0 ? 1439 : setting.OffTime;
          }
          if (btn == BUTTON_SELECT_PRESSED || millis() - timrMNU > 10000)
          {
              EEPROM.put(0,setting);
              appMode = APP_SET_OFF_MIN_MODE;
          }   
          break;
      }
      case APP_SET_OFF_MIN_MODE:
      {
          if (millis() - timr_Time > 300)
          {
              if (blink)
              { 
                  lcd.setCursor(0,0);
                  lcd.print("SET OFF TIME :");
                  lcd.setCursor(0,1);
                  lcd.print(rpad(strbuf, toTimeStr(strbuf,setting.OffTime)));
                  blink = false;
              }
              else
              {
                  lcd.setCursor(3,1);
                  lcd.print("  ");
                  blink = true;
              }     
              timr_Time = millis();       
          }
          if (btn == BUTTON_UP_PRESSED)   
          {
              setting.OffTime = (setting.OffTime += 1) > 1439 ? 0 : setting.OffTime;
          }
          if (btn == BUTTON_DOWN_PRESSED)   
          {
              setting.OffTime = (setting.OffTime -= 1) < 0 ? 1439 : setting.OffTime;
          }
          if (btn == BUTTON_SELECT_PRESSED || millis() - timrMNU > 10000)
          {
              EEPROM.put(0,setting);
              appMode = APP_NORMAL_MODE;
              lcd.clear();
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

    lcd.setCursor(0, 0);
    lcd.print(fmt(strbuf, 7, "  ", h, ":", m, ":", s, rtc.now().isPM() ? " PM   " : " AM   "));
}

void showTimers()
{
    char intbuf[8];
    lcd.setCursor(0, 0);
    lcd.print(fmt(strbuf, 2, "ON TIM :", toTimeStr(intbuf,setting.OnTime)));
    lcd.setCursor(0, 1);
    lcd.print(fmt(strbuf, 2, "OFF TIM:", toTimeStr(intbuf,setting.OffTime)));
}

void TriggerLoad()
{
    if (setting.OffTime > setting.OnTime)
    {
        if (CurTime >= setting.OnTime && CurTime < setting.OffTime)
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
        if (CurTime < setting.OnTime && CurTime >= setting.OffTime)
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