#include <PCF8574.h>
#include <Wire.h>
#include <EEPROM.h>

#define STATUSLED 6
#define START 2
#define STOP 3
#define ONBUT A0
#define OFFBUT A1
#define BUZR A2
#define On_Det 4
#define LEV1 9
#define LEV2 10
#define LEV3 11
#define LEV4 12
#define LEV5 13
#define DRY 8

enum MOTORSTATE
{
  ON,
  OFF
};
PCF8574 pcf(0x20);
int waterLevel, waterflow, a, OldLevel = 0;
long tim_on, tim_off, tim_setlow = 0, tim_progres = 0, tim_lowOn = 0, tim_buzr = 0;
bool Pow_on = false, lowOn = false, disON = false, highOff = false;
int AutoMode = 0, LowLevel = 20;
MOTORSTATE MotorState;
int latchPin = 5;
int clockPin = 6;
int dataPin = 7;
uint8_t numberToPrint[2];
uint8_t numberB[] = {
    B01111011, //0
    B00001001, //1
    B10110011, //2
    B10011011, //3
    B11001001, //4
    B11011010, //5
    B11111010, //6
    B00001011, //7
    B11111011, //8
    B11001011, //9
    B01111011, //0
};
uint8_t alphaB[] = {
    B11101011, //A
    B11111000, //b
    B01110010, //C
    B10111001, //d
    B11110010, //E
    B11100010, //F
    B11011011, //G
    B11101000, //H
    B01100000, //I
    B01101011  //n
};

uint8_t mON[] = {B01111011, B01101011};
uint8_t mOFF[] = {B01111011, B11100010};
uint8_t dispOFF[] = {B0000000, B0000000};

class MySwitch
{
private:
  byte pin;
  bool Key_Down = false, Key_Hold = false, Key_Up = false;
  bool butAct, longAct;
  long butTimer, holdDelay = 3000;
  byte state;
  int x = 0, y;

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

void setup()
{
  Serial.begin(9600);
  pinMode(STATUSLED, OUTPUT);
  pinMode(START, OUTPUT);
  pinMode(STOP, OUTPUT);
  pinMode(ONBUT, INPUT_PULLUP);
  pinMode(OFFBUT, INPUT_PULLUP);
  pinMode(On_Det, INPUT_PULLUP);
  // pinMode(LEV1, INPUT_PULLUP);
  // pinMode(LEV2, INPUT_PULLUP);
  // pinMode(LEV3, INPUT_PULLUP);
  // pinMode(LEV4, INPUT_PULLUP);
  // pinMode(LEV5, INPUT_PULLUP);
  // pinMode(DRY, INPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(BUZR, OUTPUT);
  loadSettings();
  pcf.begin();
  Serial.println("Welcome");
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
    Serial.println(waterLevel);
  }
  // delay(100);
  //Check Water Level
  {
    waterLevel = 0;
    if (pcf.readButton(1) == 0)
      waterLevel = 20;
    if (pcf.readButton(2) == 0)
      waterLevel = 40;
    if (pcf.readButton(3) == 0)
      waterLevel = 60;
    if (pcf.readButton(4) == 0)
      waterLevel = 80;
    if (pcf.readButton(5) == 0)
      waterLevel = 100;
    if (pcf.readButton(0) == 0)
      waterflow = 60;
    else
      waterflow = 0;
  }
  
  if (Pow_on == true)
  {
    if (millis() > 60000)
    {
      Pow_on = false;
      MotorOn();
    }
  }
  
  if (millis() > tim_on + 1000)
    {digitalWrite(START, LOW); noTone(BUZR);}
  if (millis() > tim_off + 5000)
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
    noTone(BUZR);
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
      tone(BUZR,500);
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

  
  waterLevel = waterLevel == 100 ? 99 : waterLevel;
  if (waterLevel == 0)
  {
    numberToPrint[0] = B0000000;
    numberToPrint[1] = numberB[(waterLevel % 10)];
  }
  else
  {
    numberToPrint[0] = numberB[(waterLevel / 10)];
    numberToPrint[1] = numberB[(waterLevel % 10)];
  }

  if (MotorState == OFF)
  {
    if (OldLevel != waterLevel && lowOn == false)
    {
      Serial.println("update");
      updateDisp(dispOFF);
      updateDisp(numberToPrint);
      OldLevel = waterLevel;
    }
    if (lowOn)
    {
      if (millis() > tim_progres + 200)
      {
        tim_progres = millis();
        if (disON == false)
        {
          updateDisp(dispOFF);
          updateDisp(numberToPrint);
          disON = true;
        }
        else
        {
          updateDisp(dispOFF);
          disON = false;
        }
      }
    } else updateDisp(numberToPrint);
  }

  if (MotorState == ON)
  {
    if (!highOff)
    {
      if (millis() > tim_progres + 500)
      {
        tim_progres = millis();

        if (!disON)
        {
          if ((a & 2) == 0)
          {
            updateDisp(dispOFF);
            updateDisp(mON);
            disON = true;
          }
        }
        else
        {
          updateDisp(numberToPrint);
          disON = false;
        }
        if (a >= 10)
          a = 0;
        a++;
      }
    }
    else
    {
      if (millis() > tim_progres + 200)
      {
        tim_progres = millis();
        if (disON == false)
        {
          updateDisp(dispOFF);
          updateDisp(numberToPrint);
          disON = true;
        }
        else
        {
          updateDisp(dispOFF);
          disON = false;
        }
      }
    }
  }
}

void MotorOff()
{
  if (MotorState != OFF)
  {
    tim_off = millis();
    digitalWrite(STOP, HIGH);
    updateDisp(numberToPrint);
    //MotorState = OFF;
    // EEPROM.write(2, MotorState);
  }
}

void MotorOn()
{
  if (MotorState != ON)
  {
    tim_on = millis();
    digitalWrite(START, HIGH);
    updateDisp(numberToPrint);
    tone(BUZR,500);
    //MotorState = ON;
    // EEPROM.write(2, MotorState);
  }
}

void changeAutomode() // Auto Mode ON/OFF
{
  if (AutoMode == 1)
  {
    AutoMode = 0;
    EEPROM.write(3, AutoMode);
    uint8_t dis[] = {B11101011, B00001001};
    updateDisp(dis);
    delay(1000);
  }
  else
  {
    AutoMode = 1;
    EEPROM.write(3, AutoMode);
    uint8_t dis[] = {B11101011, B01111011};
    updateDisp(dis);
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
        uint8_t numLow[] = {numberB[(LowLevel / 10)], numberB[(LowLevel % 10)]};
        updateDisp(numLow);
        xx = true;
      }
      else
      {
        updateDisp(dispOFF);
        xx = false;
      }
    }

    onSw.update();
    if (onSw.isHold() == true)
    {
      onSw.clearValue();
      EEPROM.write(4, LowLevel);
      uint8_t numLow[] = {numberB[(LowLevel / 10)], numberB[(LowLevel % 10)]};
      updateDisp(numLow);
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

void updateDisp(uint8_t *out)
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, out[0]);
  shiftOut(dataPin, clockPin, MSBFIRST, out[1]);
  digitalWrite(latchPin, HIGH);
}

