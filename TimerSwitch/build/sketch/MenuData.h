#line 1 "e:\\Arduino\\Arduino Git\\TimerSwitch\\MenuData.h"
#ifndef _sampleMenu_
#define _sampleMenu_
#include "MenuManager.h"
#include <avr/pgmspace.h>


enum sampleMenuCommandId
{
  mnuCmdBack = 0,
  mnuCmdSetTime,
  mnuCmdTimer1Set,
  mnuCmdOnTime,
  mnuCmdOffTime
};

PROGMEM const char sampleMenu_back[] = "Back";
PROGMEM const char sampleMenu_exit[] = "Exit";

PROGMEM const char sampleMenu_2_1[] = "1 ON TIME";
PROGMEM const char sampleMenu_2_2[] = "2 OFF TIME";
PROGMEM const MenuItem sampleMenu_List_2[] = {{mnuCmdOnTime, sampleMenu_2_1}, 
                                              {mnuCmdOffTime, sampleMenu_2_2}, 
                                              {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_1[] = "SET TIME";
PROGMEM const char sampleMenu_2[] = "TIMER SETTING";
PROGMEM const MenuItem sampleMenu_Root[] = {{mnuCmdSetTime, sampleMenu_1}, 
                                            {mnuCmdTimer1Set, sampleMenu_2, sampleMenu_List_2, menuCount(sampleMenu_List_2)}, 
                                            {mnuCmdBack, sampleMenu_exit}};


#endif
