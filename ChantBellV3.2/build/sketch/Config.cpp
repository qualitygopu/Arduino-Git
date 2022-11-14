#line 1 "e:\\Arduino\\Arduino Git\\ChantBellV3.2\\Config.cpp"
#include "Config.h"
#include "LcdKeypad.h"
#include "MenuData.h"
#include <avr/eeprom.h>
short StDate, StMonth, StYear, StHr, StMin, demoTime = 5;
static char strbuf[LCD_COLS + 1];
const char NotImp[] = "-----";

void Config::setDefaults()
{
  strcpy(appNameAndVersion, NameAndVersion);
  Song1_Count = 100;
  Song2_Count = 100;
  startTime = 5; //5AM
  endTime = 22; // 10PM
  backLightDur = 20; //Sec
  vol = 20;
  MorSong1Time = 3;
  MorSong2Time = 3;
  EveSong2Time = 3;
  EveSong1Time = 3;
  MorSongNo = 1;
  EveSongNo = 2;

  Light1On = 1079;
  Light1Off = 359;
  Light2On = 0;
  Light2Off = 0;
  PlayerMorTime = 300;
  PlayerMorDur= 115;
  PlayerEveTime = 1020;
  PlayerEveDur = 115;

  AutoPlay = 0;
  AutoLight = 0;
  SchlZone = 0;

  SerialNo = 315;
  DataYear = 2022;
}

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

char *toDurStr(char *buf, long timeval, char separator)
{
  char sepstr[] = {separator, 0};

  short hours = timeval / 60;
  short mins = timeval % 60;
  char intbuf[8];
  char h[3];
  char m[3];

  inttostr(intbuf, hours);
  lpad(h, intbuf, '0', 2);
  inttostr(intbuf, mins);
  lpad(m, intbuf, '0', 2);

  fmt(buf, 3, h, sepstr, m);
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
  // case mnuCmdSetMonth:
  // case mnuCmdSetYear:
    toDateStr(strbuf);
    break;
  case mnuCmdSetTime:
  // case mnuCmdSetMin:
    toTimeStr(strbuf, ((StHr*60L) + StMin));
    break;
  case mnuCmdStartTime:
    inttostr(intbuf, (startTime > 12 ? startTime-12 : startTime));
    lpad(h, intbuf, '0', 2);
    fmt(strbuf, 2, h, startTime >= 12 ? ":00 PM" : ":00 AM");
    break;
  case mnuCmdEndTime:
    inttostr(intbuf, (endTime > 12 ? endTime - 12 : endTime));
    lpad(h, intbuf, '0', 2);
    fmt(strbuf, 2, h, endTime >= 12 ? ":00 PM" : ":00 AM");
    break;
  case mnuCmdSlogamCount:
    fmt(strbuf, 2, inttostr(intbuf, Song1_Count), " Songs");
    break;
  case mnuCmdVolume:
    fmt(strbuf, 1, inttostr(intbuf, vol));
    break;
  case mnuCmdselectTime:
    inttostr(intbuf, (demoTime > 12 ? demoTime-12 : demoTime));
    lpad(h, intbuf, '0', 2);
    fmt(strbuf, 2, h, demoTime >= 12 ? ":00 PM" : ":00 AM");
    break;
  case mnuCmdSchlZone:
    fmt(strbuf, 1, SchlZone == 0 ? "OFF" : "ON");
    break;
  case mnuCmdPlay:
    fmt(strbuf, 2, "Play", " Demo >");
    break;
  case mnuCmdMorSong1Time:
    fmt(strbuf, 2, MorSong1Time == 3 ? "OFF" : inttostr(intbuf, MorSong1Time), MorSong1Time == 3 ? "" : " AM");
    break;
  case mnuCmdEveSong1Time:
    fmt(strbuf, 2, EveSong1Time == 3 ? "OFF" : inttostr(intbuf, EveSong1Time), EveSong1Time == 3 ? "" : " PM");
    break;
  case mnuCmdMorSongNo:
    inttostr(intbuf, MorSongNo);
    lpad(h, intbuf, '0', 3);
    fmt(strbuf, 1, h);
    break;
  case mnuCmdEveSongNo:
    inttostr(intbuf, EveSongNo);
    lpad(h, intbuf, '0', 3);
    fmt(strbuf, 1, h);
    break;
  case mnuCmdLt1OnTime:
    toTimeStr(strbuf, Light1On);
    break;
  case mnuCmdLt1OffTime:  
    toTimeStr(strbuf, Light1Off);
    break;
  case mnuCmdPlrMorOnTime:
    toTimeStr(strbuf, PlayerMorTime);
    break;
  case mnuCmdPlrMorDur:
    toDurStr(strbuf, PlayerMorDur);
    break;
  case mnuCmdPlrEveOnTime:
    toTimeStr(strbuf, PlayerEveTime);
    break;
  case mnuCmdPlrEveDur:
    toDurStr(strbuf, PlayerEveDur);
    break;
  case mnuCmdSerial:
    fmt(strbuf, 1, inttostr(intbuf, SerialNo));
    break;
  case mnuCmdYear:
    fmt(strbuf, 1, inttostr(intbuf, DataYear));
    break;    
  case mnuCmdReset:
    strbuf[0] = 0;
    break;
  default:
    strcpy(strbuf, NotImp);
    break;
  }
  return strbuf;
}



void Config::save()
{
  eeprom_write_block(this, (void *)0, sizeof (Config));
}

void Config::load()
{
  // Attempt to load config from EEPROM
  eeprom_read_block(this, (void *)0, sizeof (Config));
  if (strcmp(this->appNameAndVersion, NameAndVersion) != 0)
  {
    setDefaults();
  }
}
