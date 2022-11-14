#include <Arduino.h>
#line 1 "e:\\Arduino\\Arduino Git\\Water_Level_7Segment\\Water_Level_7Segment.ino"
#include <TM1637Display.h>
#include <EEPROM.h>

#define STATUSLED A3
#define START 7
#define STOP 6
#define ONBUT A0
#define OFFBUT A1
#define BUZR A2
#define On_Det 5
#define LEV1 9
#define LEV2 10
#define LEV3 11
#define LEV4 12
#define LEV5 13
#define DRY 8

const int CLK = 3;
const int DIO = 4;
TM1637Display display(CLK, DIO);

enum MOTORSTATE
{0
  ON,
  OFF
};
uint8_t data[4] = {0, 0};
uint8_t full[4] = {B01110001, B00111110, B00111000, B00111000};
int i = 0;
int waterLevel, a, OldLevel = 0;
bool waterflow, flowFailure;
long tim_on, tim_off, tim_setlow = 0, tim_progres = 0, tim_lowOn = 0, tim_buzr = 0, timr_rota, timr_wflow;
bool Pow_on = false, lowOn = false, disON = false, highOff = false;
int AutoMode = 0, LowLevel = 20;
byte MotorState;
// uint8_t numberToPrint[2];
uint8_t numberB[] = {
    B00111111, //0
    B00000110, //1
    B01011011, //2
    B01001111, //3
    B01100110, //4
    B01101101, //5
    B01111101, //6
    B00000111, //7
    B01111111, //8
    B01101111, //9
};

class MySwitch
{
  private:
    byte pin;
    bool Key_Down = false, Key_Hold = false, Key_Up = false;
    bool butAct, longAct;
    long butTimer, holdDelay = 3000;
    byte state;
    // int x = 0, y;

  public:
    MySwitch(byte pin)
    {
      this->pin = pin;
      Key_Down = false;
      Key_Up = false;
      Key_Hold = false;
      Serial.println("Switch Init");
      init();
    }
    void init()
    {
      pinMode(pin, INPUT_PULLUP);
      //update();
      Serial.println("Switch Init2222");
    }
    void clearValue()
    {
      Key_Up = false;
      Key_Down = false;
      Key_Hold = false;
    }
    void update()
    {
      if (digitalRead(pin) == LOW)
      {
        if (butAct == false)
        {
          butAct = true;
          butTimer = millis();
        }
        if ((millis() - butTimer > holdDelay) && (longAct == false))
        {
          longAct = true;
          // long press
          Key_Hold = true;
        }
      }
      else
      {
        if (butAct == true)
        {
          if (longAct == true)
          {
            longAct = false;
          }
          else
          {
            // short press
            Key_Up = true;
          }
          butAct = false;
        }
      }
    }

    bool isHold()
    {
      return Key_Hold;
    }
    bool isPressed()
    {
      return Key_Up;
    }
};

MySwitch onSw(ONBUT);
MySwitch offSw(OFFBUT);

#line 129 "e:\\Arduino\\Arduino Git\\Water_Level_7Segment\\Water_Level_7Segment.ino"
void loadSettings();
#line 148 "e:\\Arduino\\Arduino Git\\Water_Level_7Segment\\Water_Level_7Segment.ino"
void rotDisplay();
#line 198 "e:\\Arduino\\Arduino Git\\Water_Level_7Segment\\Water_Level_7Segment.ino"
void setup();
#line 225 "e:\\Arduino\\Arduino Git\\Water_Level_7Segment\\Water_Level_7Segment.ino"
void loop();
#line 424 "e:\\Arduino\\Arduino Git\\Water_Level_7Segment\\Water_Level_7Segment.ino"
void MotorOff();
#line 433 "e:\\Arduino\\Arduino Git\\Water_Level_7Segment\\Water_Level_7Segment.ino"
void MotorOn();
#line 444 "e:\\Arduino\\Arduino Git\\Water_Level_7Segment\\Water_Level_7Segment.ino"
void changeAutomode();
#line 464 "e:\\Arduino\\Arduino Git\\Water_Level_7Segment\\Water_Level_7Segment.ino"
void setLowLevel();
#line 518 "e:\\Arduino\\Arduino Git\\Water_Level_7Segment\\Water_Level_7Segment.ino"
String doubledigit(int v);
#line 129 "e:\\Arduino\\Arduino Git\\Water_Level_7Segment\\Water_Level_7Segment.ino"
void loadSettings()
{
  if (EEPROM.read(1023) == 'T')
  {
    MotorState = EEPROM.read(2);
    AutoMode = EEPROM.read(3);
    LowLevel = EEPROM.read(4);
  }
  else
  {
    MotorState = OFF;
    EEPROM.write(2, MotorState);
    AutoMode = 1;
    EEPROM.write(3, AutoMode);
    LowLevel = 10;
    EEPROM.write(4, LowLevel);
    EEPROM.write(1023, 'T');
  }
}
void rotDisplay()
{

  timr_rota = millis();
  display.setSegments(data);
  if (i == 3)
  {
    data[0] = B00001000;
    data[1] = 0;
  }
  else if (i <= 3)
  {
    data[1] = data[1] << 1;
  }
  else if (i <= 5)
  {
    data[0] = data[0] << 1;
  }
  if (i == 6)
  {
    data[0] = 1;
  }
  i++;
  if (i > 7)
  {
    data[0] = 0;
    data[1] = 1;
    i = 0;
  }
  if (millis() > tim_progres + 100 && flowFailure)
  {
    tim_progres = millis();
    if (disON == false)
    {
      data[2] = numberB[waterLevel / 10];
      data[3] = numberB[waterLevel % 10];
      disON = true;
    }
    else
    {
      display.clear();
      disON = false;
    }
  }
  else
  {
    data[2] = numberB[waterLevel / 10];
    data[3] = numberB[waterLevel % 10];
  }
}
void setup()
{
  display.setBrightness(0x0f);
  uint8_t data[] = {0xff, 0xff, 0xff, 0xff};
  display.setSegments(data);
  delay(1000);

  //Serial.begin(9600);
  pinMode(STATUSLED, OUTPUT);
  pinMode(START, OUTPUT);
  pinMode(STOP, OUTPUT);
  pinMode(ONBUT, INPUT_PULLUP);
  pinMode(OFFBUT, INPUT_PULLUP);
  pinMode(On_Det, INPUT_PULLUP);
  pinMode(LEV1, INPUT_PULLUP);
  pinMode(LEV2, INPUT_PULLUP);
  pinMode(LEV3, INPUT_PULLUP);
  pinMode(LEV4, INPUT_PULLUP);
  pinMode(LEV5, INPUT_PULLUP);
  pinMode(DRY, INPUT_PULLUP);
  pinMode(BUZR, OUTPUT);
  loadSettings();
  if (MotorState == ON)
    Pow_on = true;
  else
    Pow_on = false;
}
void loop()
{
  static unsigned long timr = millis();
  // Show LCD Time Date
  if (millis() - timr > 500)
  {
    timr = millis();
    digitalWrite(STATUSLED, !digitalRead(STATUSLED));
  }
  if (millis() > timr_rota + 100 && MotorState == ON && !highOff)
  {
    rotDisplay();
  }
  //Check Water Level
  {
    waterLevel = 0;
    if (digitalRead(LEV1) == 0)
      waterLevel = 20;
    if (digitalRead(LEV2) == 0)
      waterLevel = 40;
    if (digitalRead(LEV3) == 0)
      waterLevel = 60;
    if (digitalRead(LEV4) == 0)
      waterLevel = 80;
    if (digitalRead(LEV5) == 0)
      waterLevel = 100;
    if (digitalRead(DRY) == 0)
      waterflow = true;
    else
      waterflow = false;
  }

  if (Pow_on == true)
  {
    if (millis() > 30000)
    {
      Pow_on = false;
      MotorOn();
    }
  }

  if (millis() > tim_on + 1000)
  {
    digitalWrite(START, LOW);
    digitalWrite(BUZR, LOW);
  }
  if (millis() > tim_off + 2000)
    digitalWrite(STOP, LOW);

  if (digitalRead(On_Det) == LOW)
  {
    MotorState = ON;
    EEPROM.write(2, MotorState);
  }
  else if (digitalRead(On_Det) == HIGH)
  {
    MotorState = OFF;
    EEPROM.write(2, MotorState);
    digitalWrite(BUZR, LOW);
  }

  // update switch status
  {
    onSw.update();
    if (onSw.isHold() == true)
    {
      onSw.clearValue();
      setLowLevel();
    }
    else if (onSw.isPressed() == true)
    {
      onSw.clearValue();
      MotorOn();
    }

    offSw.update();
    if (offSw.isHold() == true)
    {
      offSw.clearValue();
      changeAutomode();
    }
    else if (offSw.isPressed() == true)
    {
      offSw.clearValue();
      MotorOff();
    }
  }
  // Check High Level and OFF Motor after 30 Sec.
  {
    if (waterLevel >= 100 && AutoMode == 1 && MotorState == ON)
    {
      if (highOff == false)
      {
        tim_lowOn = millis();
        highOff = true;
      }
    }
    else
    {
      highOff = false;
    }
    if (millis() > tim_lowOn + 25000 && highOff == true)
    {
      digitalWrite(BUZR, HIGH);
    }
    if (millis() > tim_lowOn + 30000 && highOff == true)
    {
      MotorOff();
      highOff = false;
    }
  }
  // Check Low Level and ON Motor after 30 Sec.
  {
    if (waterLevel < LowLevel && AutoMode == 1 && MotorState == OFF)
    {
      if (lowOn == false)
      {
        tim_lowOn = millis();
        lowOn = true;
      }
    }
    else
    {
      lowOn = false;
    }
    if (millis() > tim_lowOn + 30000 && lowOn == true)
    {
      MotorOn();
      lowOn = false;
    }
  }

  // Find Water Flow -----------------------------------------------------
  {
    if (MotorState == ON)
    {
      if (!waterflow)
      {
        if (!flowFailure)
        {
          timr_wflow = millis();
          flowFailure = true;
        }
      }
      else
      {
        flowFailure = false;
      }
    }

    if (millis() > timr_wflow + 60000L && flowFailure)
    {
      MotorOff();
    }
  }
  if (MotorState == OFF)
  {
    if (OldLevel != waterLevel && lowOn == false)
    {
      display.showNumberDec(waterLevel, false);
      OldLevel = waterLevel;
    }
    if (lowOn)
    {
      if (millis() > tim_progres + 300)
      {
        tim_progres = millis();
        if (disON == false)
        {
          display.showNumberDec(waterLevel, false);
          disON = true;
        }
        else
        {
          display.clear();
          disON = false;
        }
      }
    }
    else
      display.showNumberDec(waterLevel, false);
  }

  if (MotorState == ON && highOff && millis() > tim_progres + 300)
  {
    tim_progres = millis();
    if (disON == false)
    {
      display.setSegments(full);
      disON = true;
    }
    else
    {
      display.clear();
      disON = false;
    }
  }
}

void MotorOff()
{
  if (MotorState != OFF)
  {
    tim_off = millis();
    digitalWrite(STOP, HIGH);
  }
}

void MotorOn()
{
  if (MotorState != ON)
  {
    flowFailure = false;
    tim_on = millis();
    digitalWrite(START, HIGH);
    digitalWrite(BUZR, HIGH);
  }
}

void changeAutomode() // Auto Mode ON/OFF
{
  if (AutoMode == 1)
  {
    AutoMode = 0;
    EEPROM.write(3, AutoMode);
    uint8_t dis[] = {B01110111, B11111000, 63, 113};
    display.setSegments(dis);
    delay(1000);
  }
  else
  {
    AutoMode = 1;
    EEPROM.write(3, AutoMode);
    uint8_t dis[] = {B01110111, B11111000, 63, 55};
    display.setSegments(dis);
    delay(1000);
  }
}

void setLowLevel()
{
  static unsigned long timr_setlow = millis();
  bool xx = false;
  tim_setlow = millis();
  while (true)
  {
    if (millis() - tim_setlow > 10000)
      break;
    if (millis() > timr_setlow + 300)
    {
      timr_setlow = millis();
      if (!xx)
      {
        display.showNumberDec(LowLevel);
        xx = true;
      }
      else
      {
        display.clear();
        xx = false;
      }
    }

    onSw.update();
    if (onSw.isHold() == true)
    {
      onSw.clearValue();
      EEPROM.write(4, LowLevel);
      display.showNumberDec(LowLevel);
      delay(1000);
      break;
    }
    else if (onSw.isPressed() == true)
    {
      onSw.clearValue();
      LowLevel -= 20;
      if (LowLevel < 0)
        LowLevel = 80;
      tim_setlow = millis();
    }

    offSw.update();
    if (offSw.isPressed() == true)
    {
      offSw.clearValue();
      LowLevel += 20;
      if (LowLevel > 80)
        LowLevel = 0;
      tim_setlow = millis();
    }
  }
}

String doubledigit(int v)
{
  return String(v / 10) +
         String(v % 10);
}
