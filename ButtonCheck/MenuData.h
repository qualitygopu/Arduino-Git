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
    // mnuCmdSetYear, 
    // mnuCmdSetHour, 
    // mnuCmdSetMin,
  mnuCmdBellSet,
    mnuCmdStartTime, 
    mnuCmdEndTime, 
    mnuCmdSlogamCount, 
    mnuCmdBakLitDur, 
    mnuCmdVolume,
    mnuCmdSchlZone,
    mnuCmdPlayDemo,
      mnuCmdselectTime, 
      mnuCmdPlay,
    mnuCmdSongSetting,
      mnuCmdMorSong1Time, 
      mnuCmdMorSong2Time, 
      mnuCmdEveSong1Time, 
      mnuCmdEveSong2Time, 
      mnuCmdSongCount, 
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

PROGMEM const char sampleMenu_2_7_1[] = "MOR SONG 1 TIME";
PROGMEM const char sampleMenu_2_7_2[] = "MOR SONG 1 No.";
PROGMEM const char sampleMenu_2_7_3[] = "EVE SONG 1 TIME";
PROGMEM const char sampleMenu_2_7_4[] = "EVE SONG 1 No.";
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
                                            

PROGMEM const char sampleMenu_2_1[] = "START TIME";
PROGMEM const char sampleMenu_2_2[] = "END TIME";
PROGMEM const char sampleMenu_2_3[] = "SLOGAM COUNT";
PROGMEM const char sampleMenu_2_4[] = "BACKLIGHT DUR";
PROGMEM const char sampleMenu_2_5[] = "VOLUME";
PROGMEM const char sampleMenu_2_6[] = "PLAY DEMO";
PROGMEM const char sampleMenu_2_7[] = "SONG SETTING";
PROGMEM const char sampleMenu_2_8[] = "SCHOOL ZONE";

PROGMEM const MenuItem sampleMenu_List_2[] = {{mnuCmdStartTime, sampleMenu_2_1}, 
                                              {mnuCmdEndTime, sampleMenu_2_2}, 
                                              {mnuCmdSlogamCount, sampleMenu_2_3}, 
                                              {mnuCmdBakLitDur, sampleMenu_2_4}, 
                                              {mnuCmdVolume, sampleMenu_2_5}, 
                                              {mnuCmdSchlZone, sampleMenu_2_8},
                                              {mnuCmdPlayDemo, sampleMenu_2_6, sampleMenu_List_2_6, menuCount(sampleMenu_List_2_6)}, 
                                              {mnuCmdSongSetting, sampleMenu_2_7, sampleMenu_List_2_7, menuCount(sampleMenu_List_2_7)},
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

PROGMEM const MenuItem configMenu_Root[] = {{mnuCmdSerial, configMenu_1}, {mnuCmdYear, configMenu_2}, {mnuCmdBack, sampleMenu_exit}};                                            

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
