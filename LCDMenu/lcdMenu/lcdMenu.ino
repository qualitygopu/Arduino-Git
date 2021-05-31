#include <Wire.h>
#include "RTClib.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <EEPROM.h>

#define UP 16736925
#define DOWN 16754775
#define RIGHT 16761405
#define LEFT 16720605
#define SELECT 16712445
// #define BACK 12583043
#define SETTING 16732845
//#define REM1 16738455
//#define REM2 16750695
#define REMAMP 16728765
//#define RECV_PIN 9
#define AMP A3
#define STA_PIN 5

int ampDelay = 5000;
int lcdbcklit = 20000;
int songOrder = 1;
int songCount = 30;

SoftwareSerial mySoftwareSerial(2, 3); // TX, RX
DFRobotDFPlayerMini myDFPlayer;
LiquidCrystal_I2C lcd(0x27, 16, 2);
IRrecv irrecv(7);
decode_results results;
RTC_DS1307 rtc;
int vol = 20;
String timeNow;
int i = 1;
uint8_t dd = 0;
uint8_t mm = 0;
uint16_t yy = 0;
DateTime tmpDate;

int curPos = 1;
int setDT[6];
int setTM[5];
int alarmMode = 0;
int songMode = 0;
int playsong = 1;

enum MENU
{
  ROOT,
  DAT,
  DAT_SET,
  DAT_SAVE,
  TIM,
  TIM_SET,
  TIM_SAVE
};

MENU CUR_MNU = ROOT;
DateTime now;

String months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
int menu = 0, level = 0;

void showTime()
{
  now = rtc.now();
  {
    timeNow = String(now.twelveHour() / 10) + String(now.twelveHour() % 10) + ":" +
              String(now.minute() / 10) + String(now.minute() % 10) + ":" +
              String(now.second() / 10) + String(now.second() % 10) + " " +
              String(now.isPM() ? "PM" : "AM");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   " + timeNow);
    lcd.setCursor(0, 1);
    if (alarmMode == 1)
    {
      lcd.print("    ALARM ON    ");
    }
    else if (songMode == 1)
    {
      lcd.print("    SONG ON    ");
    }
    else
    {
      lcd.print("   " + doubledigit(now.day()) + "-" + months[now.month() - 1] + "-" + String(now.year()));
    }
  }
}

void setup()
{
  pinMode(AMP, OUTPUT);
  pinMode(STA_PIN, INPUT_PULLUP);
  EEPROM.begin();
  Wire.begin();
  //Serial.begin(115200);
  lcd.backlight();
  lcd.init();
  lcd.setCursor(0, 0);
  lcd.print("    WELCOME    ");
  lcd.setCursor(0, 1);
  lcd.print("     QTRON      ");
  delay(2000);
  lcd.clear();
  if (!rtc.begin())
  {
    // Serial.println("Couldn't find RTC");
    lcd.clear();
    lcd.print(F("ERR 01")); // Couldn't find RTC
    // Serial.flush();
    abort();
  }

  if (!rtc.isrunning())
  {
    lcd.clear();
    lcd.print(F("ERR 02"));
    delay(2000);
  }
  vol = EEPROM.read(10);
  songOrder = EEPROM.read(15);
  if (songOrder > songCount)
        songOrder = 1;
  lcd.setCursor(0, 1);
  lcd.print("   VOLUME : " + doubledigit(vol) + "    ");
  vol = vol > 30 ? 20 : vol;
  // Initialize DF Player...............
  {
    mySoftwareSerial.begin(9600);
    delay(500);
    if (!myDFPlayer.begin(mySoftwareSerial))
    { //Use softwareSerial to communicate with mp3.
      lcd.clear();
      lcd.print(F("ERR 03"));
      // Serial.println(F("Unable to begin:"));
      // Serial.println(F("1.Please recheck the connection!"));
      // Serial.println(F("2.Please insert the SD card!"));
      while (true)
        ;
    }
    // Serial.println(F("DFPlayer Mini online."));
    myDFPlayer.setTimeOut(500);
    myDFPlayer.volume(vol);
    myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  }
  now = rtc.now();
  tmpDate = now;
  irrecv.enableIRIn();
  // Serial.println(now.timestamp());
}

void loop()
{
  static unsigned long timr = millis();
  static unsigned long timr1 = millis();
  static unsigned long timramp = millis();
  static unsigned long timrlcd = millis();
  static unsigned int lngpress = 1;
  // Show LCD Time Date
  if (millis() - timr > 1000)
  {
    timr = millis();
    if (level == 0)
      showTime();
  }

  if (millis() >= timrlcd + lcdbcklit)
  {
    lcd.noBacklight();
    lngpress = -1;
  }

  // Check Alaram.............
  if (now.minute() == 0 && now.second() == 0)
  {
    int hr = now.hour();
    if (hr >= 5 && hr <= 22) // chek time
    {
      digitalWrite(AMP, HIGH);
      alarmMode = 1;
      timramp = millis();
    }
  }

  if (alarmMode == 1)
  {
    lcd.backlight();
    timrlcd = millis();
    if (millis() >= timramp + ampDelay)
      playTemple(now.hour());
  }

  if (songMode == 1)
  {
    lcd.backlight();
    timrlcd = millis();
    if (millis() >= timramp + ampDelay)
      playSongMode(lngpress);
  }

  if (irrecv.decode(&results))
  {
    //serialPrintUint64(results.value, DEC);
    // Serial.println(results.value);
    lcd.backlight();
    timrlcd = millis();
    switch (CUR_MNU)
    {
    case DAT_SET:
      setDateUpDown();
      break;
    case TIM_SET:
      setTimeUpDown();
      break;
    default:
      switch (results.value)
      {
      case SETTING:
        switch (level)
        {
        case 0:
          level++;
          menu = 1;
          showMenu_Level1();
          break;
        default:
          level = 0;
          lcd.clear();
          lcd.print("     QTRON      ");
          break;
        }
        break;
      case DOWN:
        switch (level)
        {
        case 1:
          menu++;
          showMenu_Level1();
          break;
        case 2:
          menu++;
          showMenu_Level2();
          break;
        default:
          if (vol > 5)
          {
            vol--;
            myDFPlayer.volume(vol);
            EEPROM.write(10, vol);
          }
          lcd.clear();
          lcd.print("VOLUME : " + String(vol));
          break;
        }
        break;
      case 12583001:
        if (vol > 5)
        {
          vol--;
          myDFPlayer.volume(vol);
          EEPROM.write(10, vol);
        }
        lcd.clear();
        lcd.print("VOLUME : " + String(vol));
        break;
      case UP:
        switch (level)
        {
        case 1:
          menu--;
          showMenu_Level1();
          break;
        case 2:
          menu--;
          showMenu_Level2();
          break;
        default:
          if (vol < 30)
          {
            vol++;
            myDFPlayer.volume(vol);
            EEPROM.write(10, vol);
          }
          lcd.clear();
          lcd.print("VOLUME : " + String(vol));
          break;
        }
        break;
      case 12583000:
        if (vol < 30)
        {
          vol++;
          myDFPlayer.volume(vol);
          EEPROM.write(10, vol);
        }
        lcd.clear();
        lcd.print("VOLUME : " + String(vol));
        break;
      case SELECT:
        switch (level)
        {
        case 1:
          level++;
          execute_Level1();
          break;
        case 2:
          execute_Level2();
          break;
        default:
          if (lngpress >= 1 && lngpress <= 9)
          {
            digitalWrite(AMP, HIGH);
            songMode = 1;
            timramp = millis();
          }
          else if (lngpress == 0)
          {
            if (songMode == 1)
              myDFPlayer.stop();
          }
          break;
        }

        break;
      case LEFT:
        switch (level)
        {
        case 1:
          level--;
          lcd.clear();
          lcd.print("     QTRON      ");
          break;
        case 2:
          level--;
          menu = 1;
          restorePreMenu();
          showMenu_Level1();
          break;
        case 3:
          if (CUR_MNU != DAT_SET && CUR_MNU != TIM_SET)
          {
            // Serial.println("Go Back");
            level--;
            menu = 1;
            restorePreMenu();
            showMenu_Level2();
          }
          break;
        }
        break;
      case REMAMP:
        digitalWrite(AMP, (!digitalRead(AMP)));
        if (digitalRead(AMP))
        {
          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print(F("    AMP ON    "));
        }
        else
        {
          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print(F("    AMP OFF    "));
          
        }
        break;
      case 12582912:
        //playTemple(random(5, 21));
        rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), 59, 55));
        break;
      case 16738455: // 1
        lngpress = 1;
        break;
      case 16750695: // 2
        lngpress = 2;
        break;
      case 16756815: // 3
        lngpress = 3;
        break;
      case 16724175: // 4
        lngpress = 4;
        break;
      case 16718055: // 5
        lngpress = 5;
        break;
      case 16743045: // 6
        lngpress = 6;
        break;
      case 16716015: // 7
        lngpress = 7;
        break;
      case 16726215: // 8
        lngpress = 8;
        break;
      case 16734885: // 9
        lngpress = 9;
        break;
      case 16730805: //0
        lngpress = 0;
        break;
      }
      break;
    }

    delay(300);
    irrecv.resume();
  }
}

void restorePreMenu()
{
  if (CUR_MNU == DAT)
    menu = 1;
  if (CUR_MNU == TIM)
    menu = 2;
  if (CUR_MNU == DAT_SET || CUR_MNU == DAT_SET)
  {
    CUR_MNU = DAT;
    menu = 1;
  }
  if (CUR_MNU == TIM_SAVE || CUR_MNU == TIM_SET)
  {
    CUR_MNU = TIM;
    menu = 2;
  }
}

void showMenu_Level1()
{
  switch (menu)
  {
  case 0:
    menu = 1;
    break;
  case 1:
    lcd.clear();
    lcd.print(">SET DATE");
    lcd.setCursor(0, 1);
    lcd.print(" SET TIME");
    break;
  case 2:
    lcd.clear();
    lcd.print(" SET DATE");
    lcd.setCursor(0, 1);
    lcd.print(">SET TIME");
    break;
  case 3:
    menu = 2;
    break;
  }
}

void showMenu_Level2()
{
  switch (CUR_MNU)
  {
  case DAT:
    switch (menu)
    {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">DATE:" + dt(now));
      lcd.setCursor(0, 1);
      lcd.print(" SAVE");
      break;
    case 2:
      lcd.clear();
      lcd.print(" DATE:" + dt(tmpDate));
      lcd.setCursor(0, 1);
      lcd.print(">SAVE");
      break;
    case 3:
      menu = 2;
      break;
    }
    break;
  case TIM:
    switch (menu)
    {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">TIME: " + tm(now));
      lcd.setCursor(0, 1);
      lcd.print(" SAVE");
      break;
    case 2:
      lcd.clear();
      lcd.print(" TIME: " + tm(tmpDate));
      lcd.setCursor(0, 1);
      lcd.print(">SAVE");
      break;
    case 3:
      menu = 2;
      break;
    }
    break;
  }
}

void execute_Level1()
{
  switch (menu)
  {
  case 1:
    CUR_MNU = DAT;
    menu = 1;
    showMenu_Level2();
    break;
  case 2:
    CUR_MNU = TIM;
    menu = 1;
    showMenu_Level2();
    break;
  }
}

void execute_Level2()
{
  switch (CUR_MNU)
  {
  case DAT:
    switch (menu)
    {
    case 1:
      level++;
      CUR_MNU = DAT_SET;
      // Serial.println("Set Date Now");
      lcd.clear();
      lcd.print(">DATE:" + dt(now));
      lcd.setCursor(0, 1);
      lcd.print("      ^");
      curPos = 0;
      setDT[0] = now.day() / 10;
      setDT[1] = now.day() % 10;
      setDT[2] = now.month() / 10;
      setDT[3] = now.month() % 10;
      setDT[4] = (now.year() % 1000) / 10;
      setDT[5] = (now.year() % 1000) % 10;
      break;
    case 2:
      CUR_MNU = DAT_SAVE;
      rtc.adjust(tmpDate);
      level--;
      menu = 1;
      CUR_MNU = DAT;
      showMenu_Level1();
      break;
    }
    break;
  case TIM:
    switch (menu)
    {
    case 1:
      level++;
      CUR_MNU = TIM_SET;
      // Serial.println("Set Time Now");
      lcd.clear();
      lcd.print(">TIME: " + tm(now));
      lcd.setCursor(0, 1);
      lcd.print("       ^");
      curPos = 0;
      setTM[0] = now.twelveHour() / 10;
      setTM[1] = now.twelveHour() % 10;
      setTM[2] = now.minute() / 10;
      setTM[3] = now.minute() % 10;
      setTM[4] = (now.isPM() ? 1 : 0);
      break;
    case 2:
      // Serial.println("tmpTime : " + tmpDate.timestamp());
      rtc.adjust(tmpDate);
      menu = 2;
      level--;
      CUR_MNU = TIM;
      showMenu_Level1();
      break;
    }
    break;
  }
}

void setDateUpDown()
{
  switch (results.value)
  {
  case LEFT:
    curPos--;
    if (curPos < 1)
      curPos = 6;
    setCur(curPos);
    delay(300);
    break;
  case RIGHT:
    curPos++;
    if (curPos > 6)
      curPos = 1;
    setCur(curPos);
    delay(300);
    break;
  case UP:
    setDat(1, curPos);
    delay(300);
    break;
  case DOWN:
    setDat(-1, curPos);
    delay(300);
    break;
  case SELECT:
    now = rtc.now();
    tmpDate = DateTime((2000 + (setDT[4] * 10) + setDT[5]), (setDT[2] * 10 + setDT[3]), (setDT[0] * 10 + setDT[1]), now.hour(), now.minute(), now.second());
    CUR_MNU = DAT;
    menu = 2;
    level--;
    showMenu_Level2();
    break;
  }
}

void setTimeUpDown()
{
  switch (results.value)
  {
  case LEFT:
    curPos--;
    if (curPos < 1)
      curPos = 5;
    setCur(curPos);
    delay(300);
    break;
  case RIGHT:
    curPos++;
    if (curPos > 5)
      curPos = 1;
    setCur(curPos);
    delay(300);
    break;
  case UP:
    setTime(1, curPos);
    delay(300);
    break;
  case DOWN:
    setTime(-1, curPos);
    delay(300);
    break;
  case SELECT:
    int thr;
    if (setTM[4] == 0)
    {
      thr = (setTM[0] * 10 + setTM[1]);
      thr = (thr == 12) ? 0 : thr;
    }
    else
    {
      thr = (setTM[0] * 10 + setTM[1]) + 12;
      thr = (thr == 24) ? 12 : thr;
    }
    tmpDate = DateTime(now.year(), now.month(), now.day(), thr, (setTM[2] * 10 + setTM[3]), 0);
    CUR_MNU = TIM;
    menu = 2;
    level--;
    showMenu_Level2();
    break;
  }
}

void setDat(int dirc, int pos)
{

  if (pos == 1)
  {
    setDT[pos - 1] += dirc;
    if (setDT[pos - 1] > 3)
      setDT[pos - 1] = 0;
    if (setDT[pos - 1] < 0)
      setDT[pos - 1] = 3;
  }
  else if (pos == 3)
  {
    setDT[pos - 1] += dirc;
    if (setDT[pos - 1] > 1)
      setDT[pos - 1] = 0;
    if (setDT[pos - 1] < 0)
      setDT[pos - 1] = 1;
  }
  else
  {
    setDT[pos - 1] += dirc;
    if (setDT[pos - 1] > 9)
      setDT[pos - 1] = 0;
    if (setDT[pos - 1] < 0)
      setDT[pos - 1] = 9;
  }

  if (setDT[0] == 3)
  {
    if (setDT[1] > 1)
      setDT[1] = 1;
  }
  if (setDT[2] == 1)
  {
    if (setDT[3] > 2)
      setDT[3] = 2;
  }
  String dis = String(setDT[0]) +
               String(setDT[1]) + "/" +
               String(setDT[2]) +
               String(setDT[3]) + "/20" +
               String(setDT[4]) +
               String(setDT[5]);
  // Serial.println(dis);
  lcd.setCursor(6, 0);
  lcd.print(dis);
}

void setTime(int dirc, int pos)
{

  if (pos == 1)
  {
    setTM[pos - 1] += dirc;
    if (setTM[pos - 1] > 1)
      setTM[pos - 1] = 0;
    if (setTM[pos - 1] < 0)
      setTM[pos - 1] = 1;
  }
  else if (pos == 3)
  {
    setTM[pos - 1] += dirc;
    if (setTM[pos - 1] > 5)
      setTM[pos - 1] = 0;
    if (setTM[pos - 1] < 0)
      setTM[pos - 1] = 5;
  }
  else if (pos == 5)
  {
    setTM[pos - 1] += dirc;
    if (setTM[pos - 1] > 1)
      setTM[pos - 1] = 0;
    if (setTM[pos - 1] < 0)
      setTM[pos - 1] = 1;
  }
  else
  {
    setTM[pos - 1] += dirc;
    if (setTM[pos - 1] > 9)
      setTM[pos - 1] = 0;
    if (setTM[pos - 1] < 0)
      setTM[pos - 1] = 9;
  }

  if (setTM[0] == 1)
  {
    if (setTM[1] > 2)
      setTM[1] = 2;
  }
  if (CUR_MNU == TIM_SET)
  {
    String dis = String(setTM[0]) +
                 String(setTM[1]) + ":" +
                 String(setTM[2]) +
                 String(setTM[3]) + " " +
                 String((setTM[4] == 0) ? "AM" : "PM");
    // Serial.println(dis);
    lcd.setCursor(7, 0);
    lcd.print(dis);
  }
}

void setCur(int pos)
{
  lcd.setCursor(0, 1);
  switch (pos)
  {
  case 1:
    if (CUR_MNU == DAT_SET)
      lcd.print("      ^         ");
    if (CUR_MNU == TIM_SET)
      lcd.print("       ^        ");
    break;
  case 2:
    if (CUR_MNU == DAT_SET)
      lcd.print("       ^        ");
    if (CUR_MNU == TIM_SET)
      lcd.print("        ^       ");
    break;
  case 3:
    if (CUR_MNU == DAT_SET)
      lcd.print("         ^      ");
    if (CUR_MNU == TIM_SET)
      lcd.print("          ^     ");
    break;
  case 4:
    if (CUR_MNU == DAT_SET)
      lcd.print("          ^     ");
    if (CUR_MNU == TIM_SET)
      lcd.print("           ^    ");
    break;
  case 5:
    if (CUR_MNU == DAT_SET)
      lcd.print("              ^ ");
    if (CUR_MNU == TIM_SET)
      lcd.print("             ^  ");
    break;
  case 6:
    if (CUR_MNU == DAT_SET)
      lcd.print("               ^");
    break;
  }
}

String dt(DateTime DT)
{
  return String(DT.day() / 10) +
         String(DT.day() % 10) + "/" +
         String(DT.month() / 10) +
         String(DT.month() % 10) + "/" +
         String(DT.year());
}

String tm(DateTime DT)
{
  return String(DT.twelveHour() / 10) +
         String(DT.twelveHour() % 10) + ":" +
         String(DT.minute() / 10) +
         String(DT.minute() % 10) + " " +
         String(DT.isPM() ? "PM" : "AM");
}

String doubledigit(int v)
{
  return String(v / 10) +
         String(v % 10);
}

void playTemple(int t)
{
  switch (playsong)
  {
  case 1:
    if (digitalRead(STA_PIN))
    {
      myDFPlayer.playMp3Folder(0);
      delay(1000);
      playsong = 2;
    }
    break;
  case 2:
    if (digitalRead(STA_PIN))
    {
      myDFPlayer.playFolder(1, t);
      delay(1000);
      playsong = 3;
    }
    break;
  case 3:
    if (digitalRead(STA_PIN))
    {
      // if (t == 5)
      // {
      //   myDFPlayer.playFolder(2, 8);
      //   delay(1000);
      // }
      // else if (t == 6)
      // {
      //   myDFPlayer.playFolder(2, 9);
      //   delay(1000);
      // }
      // else
      // {
      //   myDFPlayer.playMp3Folder(songOrder);
      //   songOrder++;
      //   if (songOrder > songCount)
      //     songOrder = 1;
      //   EEPROM.write(15, songOrder);
      //   delay(1000);
      // }
      myDFPlayer.playMp3Folder(songOrder);
      songOrder++;
      if (songOrder > songCount)
        songOrder = 1;
      EEPROM.write(15, songOrder);
      delay(1000);
      playsong = 4;
    }
    break;
  case 4:
    if (digitalRead(STA_PIN))
    {
      playsong = 1;
      alarmMode = 0;
      digitalWrite(AMP, LOW);
    }
    break;
  }
}
void playSongMode(int s)
{
  switch (playsong)
  {
  case 1:
    if (digitalRead(STA_PIN))
    {
      myDFPlayer.playFolder(2, s);
      delay(1000);
      playsong = 2;
    }
    break;
  case 2:
    if (digitalRead(STA_PIN))
    {
      playsong = 1;
      songMode = 0;
      digitalWrite(AMP, LOW);
    }
    break;
  }
}
