#line 1 "e:\\Arduino\\Arduino Git\\ChantBellV3.3\\Config.h"
#ifndef CONFIG_H_
#define CONFIG_H_
#include "Arduino.h"
const char NameAndVersion[] = "ChantBell V3.1";
extern char *toTimeStr(char *buf, long timeval, char separator = ':');
extern char *toDurStr(char *buf, long timeval, char separator = ':');
extern char *toDateStr(char *buf, char separator = '/');
extern long addToTime(short delta, long timeval, long minval = 0, long maxval = 1439);
extern short StDate, StMonth, StYear, StHr, StMin, demoTime,AlarmTimeCursor, PanchangamCursor;
class Config
{
    public:
    char appNameAndVersion[sizeof(NameAndVersion)];
    short startTime, endTime; 
    short backLightDur, vol;
    byte AutoPlay, AutoLight, SchlZone;
    int Song1_Count, Song2_Count, DataYear, SerialNo;
    short MorSong1Time, MorSong2Time, EveSong1Time, EveSong2Time, MorSongNo, EveSongNo;
    long Light1On, Light1Off, Light2On, Light2Off, PlayerMorTime, PlayerMorDur, PlayerEveTime, PlayerEveDur;
    char *getFormattedStr(byte cmdId);
    bool AlarmTime[24];
    bool PanchangamTime[7];
    void save();
    void load();
    void setDefaults();
};


#endif
