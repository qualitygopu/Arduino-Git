# 1 "e:\\Arduino\\Arduino Git\\Home_Voice_Clock\\sevensegment\\sevensegment.ino"
# 2 "e:\\Arduino\\Arduino Git\\Home_Voice_Clock\\sevensegment\\sevensegment.ino" 2
# 3 "e:\\Arduino\\Arduino Git\\Home_Voice_Clock\\sevensegment\\sevensegment.ino" 2
# 4 "e:\\Arduino\\Arduino Git\\Home_Voice_Clock\\sevensegment\\sevensegment.ino" 2
# 5 "e:\\Arduino\\Arduino Git\\Home_Voice_Clock\\sevensegment\\sevensegment.ino" 2
# 6 "e:\\Arduino\\Arduino Git\\Home_Voice_Clock\\sevensegment\\sevensegment.ino" 2
# 7 "e:\\Arduino\\Arduino Git\\Home_Voice_Clock\\sevensegment\\sevensegment.ino" 2
# 17 "e:\\Arduino\\Arduino Git\\Home_Voice_Clock\\sevensegment\\sevensegment.ino"
SoftwareSerial mySoftwareSerial(2, 3); // TX, RX
DFRobotDFPlayerMini myDFPlayer;
IRrecv irrecv(9);
decode_results results;

const int dataPin = 8; // 74HC595 pin 8 DS
const int latchPin = 7; // 74HC595 pin 9 STCP
const int clockPin = 6; // 74HC595 pin 10 SHCP
byte D1, D2, D3, D4, DOT;
bool dec = true;
RTC_DS1307 rtc;
unsigned long timrAmp, timrLCD, timr_Time, timrMNU;
byte playSong = 1;
struct Config
{
  short MorSongTime;
  short EveSongTime;
  short MorSongNo;
  short EveSongNo;
  short SlogamNo;
  short StartTime;
  short EndTime;
  short SongCount;
  short SlogamCount;
  short Volume;
};
Config config;

/*  COMMON CATHODE  */
// byte num[] = {//  .GFEDCBA
//                 0b00111111, // 0
//                 0b00000110, // 1
//                 0b01011011, // 2
//                 0b01001111, // 3
//                 0b01100110, // 4
//                 0b01101101, // 5
//                 0b01111101, // 6
//                 0b00000111, // 7
//                 0b01111111, // 8
//                 0b01101111  // 9
//                 };
// byte digit[] = {
//                 0b00000111, // digit 1
//                 0b00001011, // digit 2
//                 0b00001101, // digit 3
//                 0b00001110  // digit 4
//                 };

/*  COMMON ANODE  */
byte num[] = {
    //  .GFEDCBA
    0b11000000, // 0
    0b11111001, // 1
    0b10100100, // 2
    0b10110000, // 3
    0b10011001, // 4
    0b10010010, // 5
    0b10000010, // 6
    0b11111000, // 7
    0b10000000, // 8
    0b10010000 // 9
};
byte digit[] = {
    0b00001000, // digit 1
    0b00000100, // digit 2
    0b00000010, // digit 3
    0b00000001, // digit 4
};
enum ButtonValues
{
  BUTTON_NULL = 0,
  BUTTON_TIMESET,
  BUTTON_MENU,
  BUTTON_LEFT,
  BUTTON_RIGHT,
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_OK,
  BUTTON_0
};
byte btn = BUTTON_NULL;
enum AppModeValues
{
  APP_NORMAL_MODE,
  APP_MENU_MODE,
  APP_CHANT_MODE,
  APP_DEMO_MODE,
  APP_SETHR_MODE,
  APP_SETMIN_MODE
};
byte appMode = APP_NORMAL_MODE;

enum MenuMode
{
  MENU_MOR_TIME,
  MENU_MOR_SONG,
  MENU_EVE_TIME,
  MENU_EVE_SONG,
  MENU_SLOGAM,
  MENU_START_TIME,
  MENU_END_TIME,
  MENU_SLOGAM_COUNT,
  MENU_SONG_COUNT
};
short mnu = MENU_MOR_TIME;

bool blink = true;

void loadSettings()
{
  if (EEPROM.read(1023) == 100)
  {
    EEPROM.get(0, config);
  }
  else
  {
    config.MorSongTime = 6;
    config.MorSongNo = 1;
    config.EveSongTime = 6;
    config.EveSongNo = 2;
    config.SlogamNo = 1;
    config.StartTime = 5;
    config.EndTime = 10;
    config.SongCount = 10;
    config.SlogamCount = 5;
    config.Volume = 20;
    EEPROM.put(0, config);
    EEPROM.write(1023, 100);
  }
}

void setup()
{
  randomSeed(analogRead(A0));
  pinMode(5, 0x2);
  pinMode(latchPin, 0x1);
  pinMode(clockPin, 0x1);
  pinMode(dataPin, 0x1);
  EEPROM.begin();
  Wire.begin();
  D1 = 0x42;
  D2 = 0xC0;
  D3 = 0x8C;
  D4 = 0xC1;
  display();
  delay(2000);
  loadSettings();
  while (!rtc.begin())
  {
    D1 = 0x86;
    D2 = 0xBB;
    D3 = 192;
    D4 = 249;
  }
  if (!rtc.isrunning())
  {
    D1 = 0x86;
    D2 = 0xBB;
    D3 = 192;
    D4 = 164;
  }
  // Initialize DF Player...............
  {
      mySoftwareSerial.begin(9600);
      delay(500);
      while (!myDFPlayer.begin(mySoftwareSerial))
      {
          D1=0x86; D2=0xBB; D3=192; D4=176;
          display();
      }
      myDFPlayer.setTimeOut(500);
      myDFPlayer.volume(20);
      myDFPlayer.outputDevice(2);
  }

  irrecv.enableIRIn();
}

void display()
{
  digitalWrite(latchPin, 0x0);
  shiftOut(dataPin, clockPin, 1, D4);
  shiftOut(dataPin, clockPin, 1, D3);
  shiftOut(dataPin, clockPin, 1, D2);
  shiftOut(dataPin, clockPin, 1, D1);
  digitalWrite(latchPin, 0x1);
}

void loop()
{
  display();
  if (btn)
  {
    timrMNU = millis();
  }

  switch (appMode)
  {
  case APP_NORMAL_MODE:
  {
    ShowTime();

    if (btn == BUTTON_TIMESET)
    {
      appMode = APP_SETHR_MODE;
      timrMNU = millis();
    }
    if (btn == BUTTON_MENU)
    {
      appMode = APP_MENU_MODE;
      timrMNU = millis();
    }
    if (btn == BUTTON_OK)
    {
      appMode = APP_DEMO_MODE;
    }

    if (rtc.now().minute() == 0 && rtc.now().second() == 0 && millis() > timrAmp + 61000L)
    {
      int hr = rtc.now().hour();
      if (hr >= config.StartTime && hr <= config.EndTime + 12) // chek time
      {
        timrAmp = millis();
        appMode = APP_CHANT_MODE;
      }
    }
    break;
  }
  case APP_CHANT_MODE:
  {
    ShowTime();
    PlayChant(rtc.now().hour());
    if (btn == BUTTON_0)
    {
      playSong = 9;
      myDFPlayer.stop();
    }
    break;
  }
  case APP_SETHR_MODE:
  {
    if (millis() - timr_Time > 300)
    {
      if (blink)
      {
        D1 = rtc.now().twelveHour() / 10 == 0 ? 255 : num[rtc.now().twelveHour() / 10];
        D2 = num[rtc.now().twelveHour() % 10];
        D3 = rtc.now().isPM() ? 0x8C : 0x88;
        D4 = 0xFF;
        blink = false;
      }
      else
      {
        D1 = 255;
        D2 = 255;
        blink = true;
      }
      timr_Time = millis();
    }
    if (btn == BUTTON_UP)
    {
      rtc.adjust(DateTime(rtc.now().year(),
                          rtc.now().month(),
                          rtc.now().day(),
                          rtc.now().hour() > 23 ? 0 : rtc.now().hour() + 1,
                          rtc.now().minute(),
                          rtc.now().second()));
    }
    if (btn == BUTTON_DOWN)
    {
      rtc.adjust(DateTime(rtc.now().year(),
                          rtc.now().month(),
                          rtc.now().day(),
                          rtc.now().hour() < 0 ? 23 : rtc.now().hour() - 1,
                          rtc.now().minute(),
                          rtc.now().second()));
    }
    if (btn == BUTTON_TIMESET || millis() - timrMNU > 10000)
      appMode = APP_NORMAL_MODE;

    if (btn == BUTTON_OK)
      appMode = APP_SETMIN_MODE;

    break;
  }
  case APP_SETMIN_MODE:
  {
    if (millis() - timr_Time > 300)
    {
      if (blink)
      {
        D1 = rtc.now().twelveHour() / 10 == 0 ? 255 : num[rtc.now().twelveHour() / 10];
        D2 = num[rtc.now().twelveHour() % 10];
        D3 = num[rtc.now().minute() / 10];
        D4 = num[rtc.now().minute() % 10];
        blink = false;
      }
      else
      {
        D3 = 255;
        D4 = 255;
        blink = true;
      }
      timr_Time = millis();
    }
    if (btn == BUTTON_UP)
    {
      rtc.adjust(DateTime(rtc.now().year(),
                          rtc.now().month(),
                          rtc.now().day(),
                          rtc.now().hour(),
                          rtc.now().minute() > 59 ? 0 : rtc.now().minute() + 1,
                          rtc.now().second()));
    }
    if (btn == BUTTON_DOWN)
    {
      short min = rtc.now().minute();
      min = --min < 0 ? 59 : min;
      rtc.adjust(DateTime(rtc.now().year(),
                          rtc.now().month(),
                          rtc.now().day(),
                          rtc.now().hour(),
                          min,
                          rtc.now().second()));
    }
    if (btn == BUTTON_OK || btn == BUTTON_TIMESET || millis() - timrMNU > 10000)
      appMode = APP_NORMAL_MODE;
    break;
  }
  case APP_MENU_MODE:
  {
    switch (mnu)
    {
    case MENU_MOR_TIME:
    {
      D1 = 0x8E;
      D2 = 0xF9;
      if (config.MorSongTime == 3)
      {
        D3 = 0xC0;
        D4 = 0x8E;
      }
      else
      {
        D3 = config.MorSongTime < 10 ? 0xFF : num[config.MorSongTime / 10];
        D4 = num[config.MorSongTime % 10];
      }
      if (btn == BUTTON_UP)
      {
        config.MorSongTime = ++config.MorSongTime > 10 ? 3 : config.MorSongTime;
        EEPROM.put(0, config);
      }
      if (btn == BUTTON_DOWN)
      {
        config.MorSongTime = --config.MorSongTime < 3 ? 10 : config.MorSongTime;
        EEPROM.put(0, config);
      }
      break;
    }
    case MENU_MOR_SONG:
    {
      D1 = 0x8E;
      D2 = 0xA4;
      D3 = num[config.MorSongNo / 10];
      D4 = num[config.MorSongNo % 10];
      switch (btn)
      {
      case BUTTON_UP:
        config.MorSongNo = ++config.MorSongNo > config.SongCount ? 0 : config.MorSongNo;
        EEPROM.put(0, config);
        myDFPlayer.playFolder(2, config.MorSongNo);
        delay(500);
        break;
      case BUTTON_DOWN:
        config.MorSongNo = --config.MorSongNo < 0 ? config.SongCount : config.MorSongNo;
        EEPROM.put(0, config);
        myDFPlayer.playFolder(2, config.MorSongNo);
        delay(500);
        break;
      }
      break;
    }
    case MENU_EVE_TIME:
    {
      D1 = 0x8E;
      D2 = 0xB0;
      if (config.EveSongTime == 3)
      {
        D3 = 0xC0;
        D4 = 0x8E;
      }
      else
      {
        D3 = 0xFF;
        D4 = num[config.EveSongTime];
      }
      if (btn == BUTTON_UP)
      {
        config.EveSongTime = ++config.EveSongTime > 9 ? 3 : config.EveSongTime;
        EEPROM.put(0, config);
      }
      if (btn == BUTTON_DOWN)
      {
        config.EveSongTime = --config.EveSongTime < 3 ? 9 : config.EveSongTime;
        EEPROM.put(0, config);
      }
      break;
    }
    case MENU_EVE_SONG:
    {
      D1 = 0x8E;
      D2 = 0x99;
      D3 = num[config.EveSongNo / 10];
      D4 = num[config.EveSongNo % 10];
      switch (btn)
      {
      case BUTTON_UP:
        config.EveSongNo = ++config.EveSongNo > config.SongCount ? 0 : config.EveSongNo;
        EEPROM.put(0, config);
        myDFPlayer.playFolder(2, config.EveSongNo);
        delay(500);
        break;
      case BUTTON_DOWN:
        config.EveSongNo = --config.EveSongNo < 0 ? config.SongCount : config.EveSongNo;
        EEPROM.put(0, config);
        myDFPlayer.playFolder(2, config.EveSongNo);
        delay(500);
        break;
      }
      break;
    }
    case MENU_SLOGAM:
    {
      D1 = 0x8E;
      D2 = 0x92;
      D3 = num[config.SlogamNo / 10];
      D4 = num[config.SlogamNo % 10];
      switch (btn)
      {
      case BUTTON_UP:
        config.SlogamNo = ++config.SlogamNo > config.SlogamCount ? 0 : config.SlogamNo;
        EEPROM.put(0, config);
        myDFPlayer.playFolder(3, config.SlogamNo);
        delay(500);
        break;
      case BUTTON_DOWN:
        config.SlogamNo = --config.SlogamNo < 0 ? config.SlogamCount : config.SlogamNo;
        EEPROM.put(0, config);
        myDFPlayer.playFolder(3, config.SlogamNo);
        delay(500);
        break;
      }
      break;
    }
    case MENU_START_TIME:
    {
      D1 = 0x8E;
      D2 = 0x82;
      D3 = config.StartTime < 10 ? 0xFF : num[config.StartTime / 10];
      D4 = num[config.StartTime % 10];
      if (btn == BUTTON_UP)
      {
        config.StartTime = ++config.StartTime > 10 ? 4 : config.StartTime;
        EEPROM.put(0, config);
      }
      if (btn == BUTTON_DOWN)
      {
        config.StartTime = --config.StartTime < 4 ? 10 : config.StartTime;
        EEPROM.put(0, config);
      }
      break;
    }
    case MENU_END_TIME:
    {
      D1 = 0x8E;
      D2 = 0xF8;
      D3 = config.EndTime < 10 ? 0xFF : num[config.EndTime / 10];
      D4 = num[config.EndTime % 10];
      if (btn == BUTTON_UP)
      {
        config.EndTime = ++config.EndTime > 10 ? 5 : config.EndTime;
        EEPROM.put(0, config);
      }
      if (btn == BUTTON_DOWN)
      {
        config.EndTime = --config.EndTime < 5 ? 10 : config.EndTime;
        EEPROM.put(0, config);
      }
      break;
    }
    case MENU_SLOGAM_COUNT:
    {
      D1 = 0x8E;
      D2 = 0x80;
      D3 = num[config.SlogamCount / 10];
      D4 = num[config.SlogamCount % 10];
      switch (btn)
      {
      case BUTTON_UP:
        config.SlogamCount = ++config.SlogamCount > 99 ? 0 : config.SlogamCount;
        EEPROM.put(0, config);
        break;
      case BUTTON_DOWN:
        config.SlogamCount = --config.SlogamCount < 0 ? 99 : config.SlogamCount;
        EEPROM.put(0, config);
        break;
      }
      break;
    }
    case MENU_SONG_COUNT:
    {
      D1 = 0x8E;
      D2 = 0x98;
      D3 = num[config.SongCount / 10];
      D4 = num[config.SongCount % 10];
      switch (btn)
      {
      case BUTTON_UP:
        config.SongCount = ++config.SongCount > 99 ? 0 : config.SongCount;
        EEPROM.put(0, config);
        break;
      case BUTTON_DOWN:
        config.SongCount = --config.SongCount < 0 ? 99 : config.SongCount;
        EEPROM.put(0, config);
        break;
      }
      break;
    }
    }
    if (btn == BUTTON_RIGHT)
    {

      myDFPlayer.stop();
      mnu = ++mnu > 8 ? 0 : mnu;
    }

    if (btn == BUTTON_LEFT)
    {
      myDFPlayer.stop();
      mnu = --mnu < 0 ? 8 : mnu;
    }

    if (btn == BUTTON_MENU || millis() - timrMNU > 30000)
    {
      appMode = APP_NORMAL_MODE;
      mnu = MENU_MOR_TIME;
      myDFPlayer.stop();
    }
    break;
  }
  case APP_DEMO_MODE:
  {
    ShowTime();
    PlayChant(rtc.now().hour());
    if (btn == BUTTON_OK)
    {
      playSong = 9;
      myDFPlayer.stop();
    }
    if (btn == BUTTON_UP)
    {
      config.Volume = ++config.Volume > 30 ? 1 : config.Volume;
      myDFPlayer.volume(config.Volume);
      D1 = 0xC1;
      D2 = 0xC7;
      D3 = num[config.Volume / 10];
      D4 = num[config.Volume % 10];
    }
    if (btn == BUTTON_DOWN)
    {
      config.Volume = --config.Volume < 1 ? 30 : config.Volume;
      myDFPlayer.volume(config.Volume);
      D1 = 0xC1;
      D2 = 0xC7;
      D3 = num[config.Volume / 10];
      D4 = num[config.Volume % 10];
    }
    break;
  }
  }
  btn = BUTTON_NULL;
  if (irrecv.decode(&results))
  {
    switch (results.value)
    {
    case 16728765:
      btn = BUTTON_TIMESET;
      break;
    case 16732845:
      btn = BUTTON_MENU;
      break;
    case 16720605:
      btn = BUTTON_LEFT;
      break;
    case 16761405:
      btn = BUTTON_RIGHT;
      break;
    case 16736925:
      btn = BUTTON_UP;
      break;
    case 16754775:
      btn = BUTTON_DOWN;
      break;
    case 16712445:
      btn = BUTTON_OK;
      break;
    case 16730805: // mini remote 0
      btn = BUTTON_0;
      break;
    case 12582912:
      rtc.adjust(DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), rtc.now().hour(), 59, 55));
    default:
      btn = BUTTON_NULL;
      break;
    }
    delay(100);
    irrecv.resume();
  }
}

void ShowTime()
{
  if (millis() - timr_Time > 500)
  {
    timr_Time = millis();
    D1 = rtc.now().twelveHour() / 10 == 0 ? 255 : num[rtc.now().twelveHour() / 10];
    D2 = num[rtc.now().twelveHour() % 10];
    D3 = num[rtc.now().minute() / 10];
    D4 = num[rtc.now().minute() % 10];
    if (dec)
    {
      D1 = D1 & 0b01111111;
      dec = false;
    }
    else
    {
      D1 = rtc.now().twelveHour() / 10 == 0 ? 255 : num[rtc.now().twelveHour() / 10];
      dec = true;
    }
  }
}

void PlayChant(short hr)
{
  switch (playSong)
  {
  case 1:
    if (digitalRead(5))
    {
      if (config.SlogamNo != 0)
      {
        myDFPlayer.playFolder(3, config.SlogamNo);
        delay(1000);
      }
      playSong = 2;
    }
    break;
  case 2:
    if (digitalRead(5))
    {
      myDFPlayer.playFolder(1, hr);
      delay(1000);
      playSong = 3;
    }
    break;
  case 3:
    if (digitalRead(5))
    {
      if (config.MorSongTime == hr && config.MorSongTime != 3)
      {
        if(config.MorSongNo == 0)
          myDFPlayer.playFolder(2, random(0, config.SongCount));
        else
          myDFPlayer.playFolder(2, config.MorSongNo);
        delay(1000);
      }
      else if ((config.EveSongTime + 12) == hr && (config.EveSongTime != 3))
      {
        if(config.MorSongNo == 0)
          myDFPlayer.playFolder(2, random(0, config.SongCount));
        else
          myDFPlayer.playFolder(2, config.EveSongNo);
        delay(1000);
      }
      playSong = 9;
    }
    break;
  case 9:
    if (digitalRead(5))
    {
      playSong = 1;
      appMode = APP_NORMAL_MODE;
      timrLCD = millis();
    }
    break;
  }
}
