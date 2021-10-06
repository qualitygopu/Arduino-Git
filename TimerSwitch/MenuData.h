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
  mnuCmdSetMonth,
  mnuCmdSetYear,
  mnuCmdSetHour,
  mnuCmdSetMin,
  mnuCmdTimer1Set,
  mnuCmdOnTime,
  mnuCmdt1onHour,
  mnuCmdt1onMin,
  mnuCmdOffTime,
  mnuCmdt1offHour,
  mnuCmdt1offMin
};

PROGMEM const char sampleMenu_back[] = "Back";
PROGMEM const char sampleMenu_exit[] = "Exit";

PROGMEM const char sampleMenu_2_1_1[] = "1 SET HRS";
PROGMEM const char sampleMenu_2_1_2[] = "2 SET MINS";
PROGMEM const MenuItem sampleMenu_List_2_1[] = {{mnuCmdt1onHour, sampleMenu_2_1_1}, {mnuCmdt1onMin, sampleMenu_2_1_2}, {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_2_2_1[] = "1 SET HRS";
PROGMEM const char sampleMenu_2_2_2[] = "2 SET MINS";
PROGMEM const MenuItem sampleMenu_List_2_2[] = {{mnuCmdt1offHour, sampleMenu_2_2_1}, {mnuCmdt1offMin, sampleMenu_2_2_2}, {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_1_1[] = "1 SET DATE";
PROGMEM const char sampleMenu_1_2[] = "2 SET MONTH";
PROGMEM const char sampleMenu_1_3[] = "3 SET YEAR";
PROGMEM const char sampleMenu_1_4[] = "4 SET HRS";
PROGMEM const char sampleMenu_1_5[] = "5 SET MINS";
PROGMEM const MenuItem sampleMenu_List_1[] = {{mnuCmdSetDate, sampleMenu_1_1}, {mnuCmdSetMonth, sampleMenu_1_2}, {mnuCmdSetYear, sampleMenu_1_3}, {mnuCmdSetHour, sampleMenu_1_4}, {mnuCmdSetMin, sampleMenu_1_5}, {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_2_1[] = "1 ON TIME";
PROGMEM const char sampleMenu_2_2[] = "2 OFF TIME";
PROGMEM const MenuItem sampleMenu_List_2[] = {{mnuCmdOnTime, sampleMenu_2_1, sampleMenu_List_2_1, menuCount(sampleMenu_List_2_1)}, {mnuCmdOffTime, sampleMenu_2_2, sampleMenu_List_2_2, menuCount(sampleMenu_List_2_2)}, {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_1[] = "1 SET DATE TIME";
PROGMEM const char sampleMenu_2[] = "2 TIMR1 SETTING";
PROGMEM const MenuItem sampleMenu_Root[] = {{mnuCmdSetDateTime, sampleMenu_1, sampleMenu_List_1, menuCount(sampleMenu_List_1)}, {mnuCmdTimer1Set, sampleMenu_2, sampleMenu_List_2, menuCount(sampleMenu_List_2)}, {mnuCmdBack, sampleMenu_exit}};

/*
case mnuCmdSetDate :
	break;
case mnuCmdSetMonth :
	break;
case mnuCmdSetYear :
	break;
case mnuCmdSetHour :
	break;
case mnuCmdSetMin :
	break;
case mnuCmdt1onHour :
	break;
case mnuCmdt1onMin :
	break;
case mnuCmdt1offHour :
	break;
case mnuCmdt1offMin :
	break;
*/


#endif
