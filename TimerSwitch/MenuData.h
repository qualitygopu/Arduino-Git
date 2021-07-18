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
  mnuCmdBellSet,
  mnuCmdStartTime,
  mnuCmdEndTime,
  mnuCmdSongCount,
  mnuCmdBakLitDur,
  mnuCmdVolume,
  mnuCmdPlayDemo,
  mnuCmdselectTime,
  mnuCmdPlay
};

PROGMEM const char sampleMenu_back[] = "BACK";
PROGMEM const char sampleMenu_exit[] = "EXIT";

PROGMEM const char sampleMenu_2_6_1[] = "1 SELECT TIME";
PROGMEM const char sampleMenu_2_6_2[] = "2 PLAY";
PROGMEM const MenuItem sampleMenu_List_2_6[] = {{mnuCmdselectTime, sampleMenu_2_6_1}, {mnuCmdPlay, sampleMenu_2_6_2}, {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_1_1[] = "1 SET DATE";
PROGMEM const char sampleMenu_1_2[] = "2 SET MONTH";
PROGMEM const char sampleMenu_1_3[] = "3 SET YEAR";
PROGMEM const char sampleMenu_1_4[] = "4 SET HRS";
PROGMEM const char sampleMenu_1_5[] = "5 SET MINS";
PROGMEM const MenuItem sampleMenu_List_1[] = {{mnuCmdSetDate, sampleMenu_1_1}, {mnuCmdSetMonth, sampleMenu_1_2}, {mnuCmdSetYear, sampleMenu_1_3}, {mnuCmdSetHour, sampleMenu_1_4}, {mnuCmdSetMin, sampleMenu_1_5}, {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_2_1[] = "1 START TIME";
PROGMEM const char sampleMenu_2_2[] = "2 END TIME";
PROGMEM const char sampleMenu_2_3[] = "3 SONG COUNT";
PROGMEM const char sampleMenu_2_4[] = "4 BACKLIGHT DUR";
PROGMEM const char sampleMenu_2_5[] = "5 VOLUME";
PROGMEM const char sampleMenu_2_6[] = "6 PLAY DEMO";
PROGMEM const MenuItem sampleMenu_List_2[] = {{mnuCmdStartTime, sampleMenu_2_1}, {mnuCmdEndTime, sampleMenu_2_2}, {mnuCmdSongCount, sampleMenu_2_3}, {mnuCmdBakLitDur, sampleMenu_2_4}, {mnuCmdVolume, sampleMenu_2_5}, {mnuCmdPlayDemo, sampleMenu_2_6, sampleMenu_List_2_6, menuCount(sampleMenu_List_2_6)}, {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_1[] = "1 SET DATE TIME";
PROGMEM const char sampleMenu_2[] = "2 BELL SETTING";
PROGMEM const MenuItem sampleMenu_Root[] = {{mnuCmdSetDateTime, sampleMenu_1, sampleMenu_List_1, menuCount(sampleMenu_List_1)}, {mnuCmdBellSet, sampleMenu_2, sampleMenu_List_2, menuCount(sampleMenu_List_2)}, {mnuCmdBack, sampleMenu_exit}};

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
case mnuCmdStartTime :
	break;
case mnuCmdEndTime :
	break;
case mnuCmdSongCount :
	break;
case mnuCmdBakLitDur :
	break;
case mnuCmdVolume :
	break;
case mnuCmdselectTime :
	break;
case mnuCmdPlay :
	break;
*/

/*
<?xml version="1.0"?>
<RootMenu xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xmlns:xsd="http://www.w3.org/2001/XMLSchema">
  <Config IdPrefix="mnuCmd" VarPrefix="sampleMenu" UseNumbering="true" IncludeNumberHierarchy="false" MaxNameLen="15" MenuBackFirstItem="false" BackText="Back" ExitText="Exit" AvrProgMem="true"/>
  <MenuItems>
    <Item Id="SetDateTime" Name="SET DATE TIME">
      <MenuItems>
        <Item Id="SetDate" Name="SET DATE"/>
        <Item Id="SetMonth" Name="SET MONTH"/>
        <Item Id="SetYear" Name="SET YEAR"/>
        <Item Id="SetHour" Name="SET HRS(0-24)"/>
        <Item Id="SetMin" Name="SET MINS"/>
      </MenuItems>
    </Item>
    <Item Id="BellSet" Name="BELL SETTING">
      <MenuItems>
        <Item Id="StartTime" Name="START TIME"/>
        <Item Id="EndTime" Name="END TIME"/>
        <Item Id="SongCount" Name="SONG COUNT"/>
        <Item Id="BakLitDur" Name="BACKLIGHT DUR"/>
        <Item Id="Volume" Name="VOLUME"/>
        <Item Id="PlayDemo" Name="PLAY DEMO">
          <MenuItems>
            <Item Id="selectTime" Name="SELECT TIME"/>
            <Item Id="Play" Name="PLAY"/>
          </MenuItems>
        </Item>
      </MenuItems>
    </Item>
  </MenuItems>
</RootMenu>

*/
#endif
