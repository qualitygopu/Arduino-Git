#ifndef CONFIG_H_
#define CONFIG_H_
#include "Arduino.h"
const char NameAndVersion[] = "Dryer V1.0";
extern char *toTimeStr(char *buf, long timeval, char separator = ':');
extern char *toDurStr(char *buf, long timeval, char separator = ':');
extern char *toDateStr(char *buf, char separator = '/');
extern long addToTime(short delta, long timeval, long minval = 0, long maxval = 1439);
extern short StDate, StMonth, StYear, StHr, StMin, demoTime;
class Config
{
    public:
    char appNameAndVersion[sizeof(NameAndVersion)];
    short Sensor1_Value, Sensor2_Value, Sensor3_Value, Sensor4_Value, Sensor5_Value, Sensor6_Value;
    char *getFormattedStr(byte cmdId);
    void save();
    void load();
    void setDefaults();
};


#endif
