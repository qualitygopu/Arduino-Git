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
    mnuCmdBakLitDur, 
    mnuCmdVolume,
    mnuCmdSchlZone,
    mnuCmdPlayDemo,
      mnuCmdselectTime, 
      mnuCmdPlay,
    mnuCmdSongSetting,
      mnuCmdMorSong1Time, 
      mnuCmdEveSong1Time, 
      mnuCmdMorSongNo, 
      mnuCmdEveSongNo,
    mnuCmdReset,
  mnuCmdLightSet,
    mnuCmdLt1OnTime,
    mnuCmdLt1OffTime,

  mnuCmdPlayerSet,  
    mnuCmdPlrMorOnTime,  
    mnuCmdPlrMorDur,
    mnuCmdPlrEveOnTime,  
    mnuCmdPlrEveDur,
  mnuCmdClockSet,  
  mnuCmdYear,
  mnuCmdSerial
};

PROGMEM const char sampleMenu_back[] = "BACK";
PROGMEM const char sampleMenu_exit[] = "EXIT";

PROGMEM const char sampleMenu_2_6_1[] = "SELECT TIME";
PROGMEM const char sampleMenu_2_6_2[] = "PLAY";
PROGMEM const MenuItem sampleMenu_List_2_6[] = {{mnuCmdselectTime, sampleMenu_2_6_1}, 
                                                {mnuCmdPlay, sampleMenu_2_6_2}, 
                                                {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_2_7_1[] = "MOR SONG TIME";
PROGMEM const char sampleMenu_2_7_2[] = "MOR SONG No.";
PROGMEM const char sampleMenu_2_7_3[] = "EVE SONG TIME";
PROGMEM const char sampleMenu_2_7_4[] = "EVE SONG No.";
PROGMEM const MenuItem sampleMenu_List_2_7[] = {{mnuCmdMorSong1Time, sampleMenu_2_7_1},
                                                {mnuCmdMorSongNo, sampleMenu_2_7_2},
                                                {mnuCmdEveSong1Time, sampleMenu_2_7_3}, 
                                                {mnuCmdEveSongNo, sampleMenu_2_7_4},
                                                {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_1_1[] = "SET DATE";
PROGMEM const char sampleMenu_1_2[] = "SET TIME";
PROGMEM const MenuItem sampleMenu_List_1[] = {{mnuCmdSetDate, sampleMenu_1_1}, 
                                              {mnuCmdSetTime, sampleMenu_1_2}, 
                                              {mnuCmdBack, sampleMenu_back}};
                                            
PROGMEM const char sampleMenu_2_1[] = "START TIME";
PROGMEM const char sampleMenu_2_2[] = "END TIME";
PROGMEM const char sampleMenu_2_3[] = "SONG COUNT";
PROGMEM const char sampleMenu_2_5[] = "VOLUME";
PROGMEM const char sampleMenu_2_6[] = "PLAY DEMO";
PROGMEM const char sampleMenu_2_7[] = "SONG SETTING";
PROGMEM const char sampleMenu_2_8[] = "SCHOOL ZONE";
PROGMEM const char sampleMenu_2_9[] = "ALARM ON/OFF";
PROGMEM const char sampleMenu_2_10[] = "PANCHANGAM TIME";

PROGMEM const MenuItem sampleMenu_List_2[] = {{mnuCmdStartTime, sampleMenu_2_1}, 
                                              {mnuCmdEndTime, sampleMenu_2_2}, 
                                              {mnuCmdSlogamCount, sampleMenu_2_3}, 
                                              {mnuCmdVolume, sampleMenu_2_5}, 
                                              {mnuCmdSchlZone, sampleMenu_2_8},
                                              {mnuCmdPlayDemo, sampleMenu_2_6, sampleMenu_List_2_6, menuCount(sampleMenu_List_2_6)}, 
                                              {mnuCmdSongSetting, sampleMenu_2_7, sampleMenu_List_2_7, menuCount(sampleMenu_List_2_7)},
                                              {mnuCmdAlarmTime, sampleMenu_2_9},
                                              {mnuCmdPanchangamTime, sampleMenu_2_10},
                                              {mnuCmdBack, sampleMenu_back}};                                      

PROGMEM const char sampleMenu_3_1[] = "LIGHT ON TIME";
PROGMEM const char sampleMenu_3_2[] = "LIGHT OFF TIME";
PROGMEM const MenuItem sampleMenu_List_3[] = {{mnuCmdLt1OnTime, sampleMenu_3_1}, 
                                              {mnuCmdLt1OffTime, sampleMenu_3_2}, 
                                              {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_4_1[] = "PLYR MOR ON TIM";
PROGMEM const char sampleMenu_4_2[] = "PLYR MOR DUR";
PROGMEM const char sampleMenu_4_3[] = "PLYR EVE ON TIM";
PROGMEM const char sampleMenu_4_4[] = "PLYR EVE DUR";
PROGMEM const MenuItem sampleMenu_List_4[] = {{mnuCmdPlrMorOnTime, sampleMenu_4_1},
                                              {mnuCmdPlrMorDur, sampleMenu_4_2},
                                              {mnuCmdPlrEveOnTime, sampleMenu_4_3},
                                              {mnuCmdPlrEveDur, sampleMenu_4_4},
                                              {mnuCmdBack, sampleMenu_back}};

PROGMEM const MenuItem sampleMenu_List_5[] = {{mnuCmdStartTime, sampleMenu_2_1}, 
                                              {mnuCmdEndTime, sampleMenu_2_2}, 
                                              {mnuCmdPlayDemo, sampleMenu_2_6, sampleMenu_List_2_6, menuCount(sampleMenu_List_2_6)}, 
                                              {mnuCmdVolume, sampleMenu_2_5},
                                              {mnuCmdSchlZone, sampleMenu_2_8},
                                              {mnuCmdBack, sampleMenu_back}};    

PROGMEM const char sampleMenu_1[] = "SET DATE TIME";
PROGMEM const char sampleMenu_2[] = "BELL SETTING";
PROGMEM const char sampleMenu_3[] = "LIGHTS SETTING";
PROGMEM const char sampleMenu_4[] = "PLAYER SETTING";
PROGMEM const char sampleMenu_5[] = "CLOCK SETTING";
PROGMEM const char sampleMenu_6[] = "RESET SETTING";
PROGMEM const char configMenu_1[] = "SET SERIAL";
PROGMEM const char configMenu_2[] = "SET YEAR";
PROGMEM const MenuItem serviceMenu_Root[] = {{mnuCmdSetDateTime, sampleMenu_1, sampleMenu_List_1, menuCount(sampleMenu_List_1)}, 
                                            {mnuCmdBellSet, sampleMenu_2, sampleMenu_List_2, menuCount(sampleMenu_List_2)}, 
                                            {mnuCmdLightSet, sampleMenu_3, sampleMenu_List_3, menuCount(sampleMenu_List_3)},
                                            {mnuCmdPlayerSet, sampleMenu_4, sampleMenu_List_4, menuCount(sampleMenu_List_4)},
                                            {mnuCmdReset, sampleMenu_6}, 
                                            {mnuCmdBack, sampleMenu_exit}};

PROGMEM const MenuItem normalMenu_Root[] = {{mnuCmdLightSet, sampleMenu_3, sampleMenu_List_3, menuCount(sampleMenu_List_3)},
                                            {mnuCmdPlayerSet, sampleMenu_4, sampleMenu_List_4, menuCount(sampleMenu_List_4)},
                                            {mnuCmdSetDateTime, sampleMenu_1, sampleMenu_List_1, menuCount(sampleMenu_List_1)},
                                            {mnuCmdClockSet, sampleMenu_5, sampleMenu_List_5, menuCount(sampleMenu_List_5)},
                                            {mnuCmdBack, sampleMenu_exit}};

PROGMEM const MenuItem configMenu_Root[] = {{mnuCmdYear, configMenu_2}, {mnuCmdBack, sampleMenu_exit}};                                            

#endif
