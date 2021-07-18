#line 1 "e:\\Arduino\\Arduino Git\\ChantBell\\Config.h"
#ifndef CONFIG_H_
#define CONFIG_H_
#include "Arduino.h"
const char NameAndVersion[] = "ChantBell V2.0";
extern char *toTimeStr(char *buf, long timeval, char separator = ':');
extern char *toDateStr(char *buf, char separator = '/');
extern long addToTime(short delta, long timeval, long minval = 0, long maxval = 1439);
extern short StDate, StMonth, StYear, StHr, StMin, demoTime;
class Config
{
    public:
    char appNameAndVersion[sizeof(NameAndVersion)];
    short startTime, endTime; 
    short backLightDur, vol;
    int SongOrder,SongCount;
    char *getFormattedStr(byte cmdId);
    void save();
    void load();
    void setDefaults();
};


#endif
