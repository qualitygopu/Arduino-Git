# 1 "e:\\Arduino\\Arduino Git\\Water_Level_7Segment\\Water_Level_7Segment.ino"
# 2 "e:\\Arduino\\Arduino Git\\Water_Level_7Segment\\Water_Level_7Segment.ino" 2
# 3 "e:\\Arduino\\Arduino Git\\Water_Level_7Segment\\Water_Level_7Segment.ino" 2
# 18 "e:\\Arduino\\Arduino Git\\Water_Level_7Segment\\Water_Level_7Segment.ino"
const int CLK = 3;
const int DIO = 4;
TM1637Display display(CLK, DIO);

enum MOTORSTATE
{0
  ON,
  OFF
};
uint8_t data[4] = {0, 0};
uint8_t full[4] = {113, 62, 56, 56};
int i = 0;
int waterLevel, a, OldLevel = 0;
bool waterflow, flowFailure;
long tim_on, tim_off, tim_setlow = 0, tim_progres = 0, tim_lowOn = 0, tim_buzr = 0, timr_rota, timr_wflow;
bool Pow_on = false, lowOn = false, disON = false, highOff = false;
int AutoMode = 0, LowLevel = 20;
byte MotorState;
// uint8_t numberToPrint[2];
uint8_t numberB[] = {
    63, //0
    6, //1
    91, //2
    79, //3
    102, //4
    109, //5
    125, //6
    7, //7
    127, //8
    111, //9
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
      pinMode(pin, 0x2);
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
      if (digitalRead(pin) == 0x0)
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

MySwitch onSw(A0);
MySwitch offSw(A1);

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
    data[0] = 8;
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
  pinMode(A3, 0x1);
  pinMode(7, 0x1);
  pinMode(6, 0x1);
  pinMode(A0, 0x2);
  pinMode(A1, 0x2);
  pinMode(5, 0x2);
  pinMode(9, 0x2);
  pinMode(10, 0x2);
  pinMode(11, 0x2);
  pinMode(12, 0x2);
  pinMode(13, 0x2);
  pinMode(8, 0x2);
  pinMode(A2, 0x1);
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
    digitalWrite(A3, !digitalRead(A3));
  }
  if (millis() > timr_rota + 100 && MotorState == ON && !highOff)
  {
    rotDisplay();
  }
  //Check Water Level
  {
    waterLevel = 0;
    if (digitalRead(9) == 0)
      waterLevel = 20;
    if (digitalRead(10) == 0)
      waterLevel = 40;
    if (digitalRead(11) == 0)
      waterLevel = 60;
    if (digitalRead(12) == 0)
      waterLevel = 80;
    if (digitalRead(13) == 0)
      waterLevel = 100;
    if (digitalRead(8) == 0)
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
    digitalWrite(7, 0x0);
    digitalWrite(A2, 0x0);
  }
  if (millis() > tim_off + 2000)
    digitalWrite(6, 0x0);

  if (digitalRead(5) == 0x0)
  {
    MotorState = ON;
    EEPROM.write(2, MotorState);
  }
  else if (digitalRead(5) == 0x1)
  {
    MotorState = OFF;
    EEPROM.write(2, MotorState);
    digitalWrite(A2, 0x0);
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
      digitalWrite(A2, 0x1);
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
    digitalWrite(6, 0x1);
  }
}

void MotorOn()
{
  if (MotorState != ON)
  {
    flowFailure = false;
    tim_on = millis();
    digitalWrite(7, 0x1);
    digitalWrite(A2, 0x1);
  }
}

void changeAutomode() // Auto Mode ON/OFF
{
  if (AutoMode == 1)
  {
    AutoMode = 0;
    EEPROM.write(3, AutoMode);
    uint8_t dis[] = {119, 248, 63, 113};
    display.setSegments(dis);
    delay(1000);
  }
  else
  {
    AutoMode = 1;
    EEPROM.write(3, AutoMode);
    uint8_t dis[] = {119, 248, 63, 55};
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
