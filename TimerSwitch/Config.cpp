#include "Config.h"
#include "LcdKeypad.h"
#include "MenuData.h"
#include <avr/eeprom.h>
short StDate, StMonth, StYear, StHr, StMin, demoTime = 5;
static char strbuf[LCD_COLS + 1];
const char NotImp[] = "-----";
char *toTimeStr(char *buf, long timeval, char separator)
{
  char sepstr[] = {separator, 0};

  short hours = timeval / 60;
  short mins = timeval % 60;
  short hrs = hours > 12 ? hours - 12 : hours;
  char intbuf[8];
  char h[3];
  char m[3];

  inttostr(intbuf, hrs);
  lpad(h, intbuf, '0', 2);
  inttostr(intbuf, mins);
  lpad(m, intbuf, '0', 2);

  fmt(buf, 4, h, sepstr, m, (hours >= 12 ? " PM" : " AM"));
  return buf;
}

char *toDateStr(char *buf, char separator)
{
  char sepstr[] = {separator, 0};
  char intbuf[8];
  char dat[3], mon[3], year[5];

  inttostr(intbuf, StDate);
  lpad(dat, intbuf, '0', 2);
  inttostr(intbuf, StMonth);
  lpad(mon, intbuf, '0', 2);
  inttostr(intbuf, StYear);
  lpad(year, intbuf, '0', 4);

  fmt(buf, 5, dat, sepstr, mon, sepstr, year);
  return buf;
}

long addToTime(short delta, long timeval, long minval, long maxval)
{
  long newtime = timeval + delta;

  if (newtime >= minval && newtime <= maxval)
    return newtime;

  return timeval;
}

char *Config::getFormattedStr(byte cmdId)
{
  char intbuf[8];
  char h[3];
  switch (cmdId)
  {
  case mnuCmdSetDate:
  case mnuCmdSetMonth:
  case mnuCmdSetYear:
    toDateStr(strbuf);
    break;
  case mnuCmdSetHour:
  case mnuCmdSetMin:
    toTimeStr(strbuf, ((StHr * 60L) + StMin));
    break;
  case mnuCmdt1onHour:
  case mnuCmdt1onMin:
    toTimeStr(strbuf, timer1On);
    break;
  case mnuCmdt1offHour:
  case mnuCmdt1offMin:
    toTimeStr(strbuf, timer1Off);
    break;
  default:
    strcpy(strbuf, NotImp);
    break;
  }
  return strbuf;
}

void Config::setDefaults()
{
  strcpy(appNameAndVersion, NameAndVersion);
  timer1On = 18 * 60;
  timer1Off = 6 * 60;
}

void Config::save()
{
  eeprom_write_block(this, (void *)0, sizeof(Config));
}

void Config::load()
{
  // Attempt to load config from EEPROM
  eeprom_read_block(this, (void *)0, sizeof(Config));
  if (strcmp(this->appNameAndVersion, NameAndVersion) != 0)
  {
    setDefaults();
  }
}
