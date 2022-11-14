#ifndef _sampleMenu_
#define _sampleMenu_
#include "MenuManager.h"
#include <avr/pgmspace.h>

/*

Generated using LCD Menu Builder at https://lcd-menu-bulder.cohesivecomputing.co.uk/
For more information, visit https://www.cohesivecomputing.co.uk/hackatronics/arduino-lcd-menu-library/

All our hackatronics projects are free for personal use. If you find them helpful or useful, please consider
making a small donation to our hackatronics fund using the donate buttons on our web pages. Thank you.
		
*/

enum sampleMenuCommandId
{
  mnuCmdBack = 0,
  mnuCmdSetDateTime,
    mnuCmdSetDate, 
    mnuCmdSetTime, 
  mnuCmdBellSet,
  mnuCmdStartTime,
  mnuCmdEndTime,
  mnuCmdAlarmTime,
  mnuCmdPanchangamTime,
  mnuCmdSlogamCount,
  mnuCmdVolume,
  mnuCmdPlayDemo,
  mnuCmdselectTime,
  mnuCmdPlay,
  mnuCmdSongSetting,
  mnuCmdMorSong1Time,
  mnuCmdEveSong1Time,
  mnuCmdMorSongNo,
  mnuCmdEveSongNo,
  mnuCmdMorSong2Time,
  mnuCmdEveSong2Time,
  mnuCmdSongCount,
  mnuCmdSchlZone,
  mnuCmdClockSet,
  mnuCmdReset,
  mnuCmdYear,
  mnuCmdSerial
};

PROGMEM const char sampleMenu_back[] = "BACK";
PROGMEM const char sampleMenu_exit[] = "EXIT";

PROGMEM const char sampleMenu_2_6_1[] = "1 SELECT TIME";
PROGMEM const char sampleMenu_2_6_2[] = "2 PLAY";
PROGMEM const MenuItem sampleMenu_List_2_6[] = {{mnuCmdselectTime, sampleMenu_2_6_1}, 
                                                {mnuCmdPlay, sampleMenu_2_6_2}, 
                                                {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_2_7_1[] = "MORNING SONG TIME";
PROGMEM const char sampleMenu_2_7_2[] = "MORNING SONG No.";
PROGMEM const char sampleMenu_2_7_3[] = "EVENING SONG TIME";
PROGMEM const char sampleMenu_2_7_4[] = "EVENING SONG No.";
PROGMEM const char sampleMenu_2_7_5[] = "MOR SONG 2 TIME";
PROGMEM const char sampleMenu_2_7_6[] = "EVE SONG 2 TIME";
PROGMEM const char sampleMenu_2_7_7[] = "SONG COUNT";
PROGMEM const MenuItem sampleMenu_List_2_7[] = {{mnuCmdMorSong1Time, sampleMenu_2_7_1},
                                                {mnuCmdMorSongNo, sampleMenu_2_7_2},
                                                {mnuCmdEveSong1Time, sampleMenu_2_7_3}, 
                                                {mnuCmdEveSongNo, sampleMenu_2_7_4},
                                                {mnuCmdMorSong2Time, sampleMenu_2_7_5}, 
                                                {mnuCmdEveSong2Time,sampleMenu_2_7_6},
                                                {mnuCmdSongCount, sampleMenu_2_7_7},
                                                {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_1_1[] = "SET DATE";
PROGMEM const char sampleMenu_1_2[] = "SET TIME";
PROGMEM const MenuItem sampleMenu_List_1[] = {{mnuCmdSetDate, sampleMenu_1_1}, 
                                              {mnuCmdSetTime, sampleMenu_1_2}, 
                                              {mnuCmdBack, sampleMenu_back}};


PROGMEM const char sampleMenu_2_1[] = "ALARM START TIME";
PROGMEM const char sampleMenu_2_2[] = "ALARM END TIME";
PROGMEM const char sampleMenu_2_3[] = "SONG1 COUNT";
PROGMEM const char sampleMenu_2_5[] = "VOLUME";
PROGMEM const char sampleMenu_2_6[] = "PLAY DEMO";
PROGMEM const char sampleMenu_2_7[] = "SONG SETTING";
PROGMEM const char sampleMenu_2_8[] = "SCHOOL ZONE";
PROGMEM const char sampleMenu_2_9[] = "RESET SETTING";
PROGMEM const char sampleMenu_2_10[] = "ALARM ON/OFF";
PROGMEM const char sampleMenu_2_11[] = "PANCHANGAM TIME";
PROGMEM const char configMenu_1[] = "SET SERIAL";
PROGMEM const char configMenu_2[] = "SET YEAR";
PROGMEM const MenuItem sampleMenu_List_2[] = {{mnuCmdStartTime, sampleMenu_2_1}, 
                                              {mnuCmdEndTime, sampleMenu_2_2}, 
                                              {mnuCmdSlogamCount, sampleMenu_2_3}, 
                                              {mnuCmdVolume, sampleMenu_2_5}, 
                                              {mnuCmdPlayDemo, sampleMenu_2_6, sampleMenu_List_2_6, menuCount(sampleMenu_List_2_6)}, 
                                              {mnuCmdSongSetting, sampleMenu_2_7, sampleMenu_List_2_7, menuCount(sampleMenu_List_2_7)},
                                              {mnuCmdSchlZone, sampleMenu_2_8},
                                              {mnuCmdAlarmTime, sampleMenu_2_10},
                                              {mnuCmdPanchangamTime, sampleMenu_2_11},
                                              {mnuCmdReset, sampleMenu_2_9}, 
                                              {mnuCmdBack, sampleMenu_back}};
                                             

PROGMEM const char sampleMenu_1[] = "SET DATE TIME";
PROGMEM const char sampleMenu_2[] = "BELL SETTING";

PROGMEM const MenuItem sampleMenu_Root[] = {{mnuCmdSetDateTime, sampleMenu_1, sampleMenu_List_1, menuCount(sampleMenu_List_1)}, 
                                            {mnuCmdBellSet, sampleMenu_2, sampleMenu_List_2, menuCount(sampleMenu_List_2)}, 
                                            {mnuCmdBack, sampleMenu_exit}};

PROGMEM const MenuItem normalMenu_Root[] = {{mnuCmdSetDate, sampleMenu_1_1},
                                            {mnuCmdSetTime, sampleMenu_1_2}, 
                                            {mnuCmdStartTime, sampleMenu_2_1},
                                            {mnuCmdEndTime, sampleMenu_2_2}, 
                                            {mnuCmdSchlZone, sampleMenu_2_8},
                                            {mnuCmdVolume, sampleMenu_2_5},
                                            {mnuCmdBack, sampleMenu_exit}};


PROGMEM const MenuItem configMenu_Root[] = {{mnuCmdYear, configMenu_2}, {mnuCmdBack, sampleMenu_exit}};

#endif
