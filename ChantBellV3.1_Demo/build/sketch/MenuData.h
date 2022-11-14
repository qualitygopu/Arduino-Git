#line 1 "e:\\Arduino\\Arduino Git\\ChantBellV3.1_Demo\\MenuData.h"
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

  mnuCmdVoiceDemo,
    // mnuCmdAmman,
    // mnuCmdVinayagar,
    // mnuCmdSivan,
    // mnuCmdPerumal,
    // mnuCmdMurugan,
    // mnuCmdVeeran,
    // mnuCmdAyyanar,
  mnuCmdSongsDemo,
    // mnuCmdAmmanPotri,
    // mnuCmdAmmanSongs,
    // mnuCmdVinayagarPotri,
    // mnuCmdVinayagarSongs,
    // mnuCmdSivanPotri,
    // mnuCmdSivanSongs,
    // mnuCmdPerumalSongs,
    // mnuCmdMuruganPotri,
    // mnuCmdMuruganSongs,
    // mnuCmdThirukural,
    // mnuCmdRamarSongs,
    // mnuCmdAbiramiAnthathi
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
                                              {mnuCmdSchlZone, sampleMenu_2_8},
                                              {mnuCmdBack, sampleMenu_back}};    

PROGMEM const char sampleMenu_1[] = "SET DATE TIME";
PROGMEM const char sampleMenu_2[] = "BELL SETTING";
PROGMEM const char sampleMenu_3[] = "LIGHTS SETTING";
PROGMEM const char sampleMenu_4[] = "PLAYER SETTING";
PROGMEM const char sampleMenu_5[] = "CLOCK SETTING";
PROGMEM const char sampleMenu_6[] = "RESET SETTING";

// PROGMEM const char demoMenu_1_1[] = "Amman";
// PROGMEM const char demoMenu_1_2[] = "Vinayagar";
// PROGMEM const char demoMenu_1_3[] = "Sivan";
// PROGMEM const char demoMenu_1_4[] = "Perumal";
// PROGMEM const char demoMenu_1_5[] = "Murugan";
// PROGMEM const char demoMenu_1_6[] = "Veeran";
// PROGMEM const char demoMenu_1_7[] = "Ayyanar";
// PROGMEM const MenuItem demoMenu_List_1[] = {{mnuCmdAmman, demoMenu_1_1},
//                                             {mnuCmdVinayagar, demoMenu_1_2},
//                                             {mnuCmdSivan, demoMenu_1_3},
//                                             {mnuCmdPerumal, demoMenu_1_4},
//                                             {mnuCmdMurugan, demoMenu_1_5},
//                                             {mnuCmdVeeran, demoMenu_1_6},
//                                             {mnuCmdAyyanar, demoMenu_1_7},
//                                             {mnuCmdBack, sampleMenu_back}};

// PROGMEM const char demoMenu_2_1[] = "Amman Potri";                                            
// PROGMEM const char demoMenu_2_2[] = "Amman Songs";                                            
// PROGMEM const char demoMenu_2_3[] = "Vinayagar Potri";                                            
// PROGMEM const char demoMenu_2_4[] = "Vinayagar Songs";                                            
// PROGMEM const char demoMenu_2_5[] = "Sivan Potri";                                            
// PROGMEM const char demoMenu_2_6[] = "Sivan Songs";                                            
// PROGMEM const char demoMenu_2_7[] = "Perumal Songs";                                            
// PROGMEM const char demoMenu_2_8[] = "Murugan Potri";                                            
// PROGMEM const char demoMenu_2_9[] = "Murugan Songs";                                            
// PROGMEM const char demoMenu_2_10[] = "Thirukural";                                            
// PROGMEM const char demoMenu_2_11[] = "Ramar Songs";                                            
// PROGMEM const char demoMenu_2_12[] = "Abirami Anthathi";  
// PROGMEM const MenuItem demoMenu_List_2[] = {{mnuCmdAmmanPotri, demoMenu_2_1},
//                                             {mnuCmdAmmanSongs, demoMenu_2_2},
//                                             {mnuCmdVinayagarPotri, demoMenu_2_3},
//                                             {mnuCmdVinayagarSongs, demoMenu_2_4},
//                                             {mnuCmdSivanPotri, demoMenu_2_5},
//                                             {mnuCmdSivanSongs, demoMenu_2_6},
//                                             {mnuCmdPerumalSongs, demoMenu_2_7},
//                                             {mnuCmdMuruganPotri, demoMenu_2_8},
//                                             {mnuCmdMuruganSongs, demoMenu_2_9},
//                                             {mnuCmdThirukural, demoMenu_2_10},
//                                             {mnuCmdRamarSongs, demoMenu_2_11},
//                                             {mnuCmdAbiramiAnthathi, demoMenu_2_12},
//                                             {mnuCmdBack, sampleMenu_back}};                                          

PROGMEM const char demoMenu_1[] = "VOICE DEMO";
PROGMEM const char demoMenu_2[] = "SONGS DEMO";

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


PROGMEM const MenuItem demoMenu_Root[] = {{mnuCmdVoiceDemo, demoMenu_1 /*, demoMenu_List_1, menuCount(demoMenu_List_1)*/}, 
                                          {mnuCmdSongsDemo, demoMenu_2 /*, demoMenu_List_2, menuCount(demoMenu_List_2)*/}, 
                                          {mnuCmdBack, sampleMenu_exit}};          


#endif
