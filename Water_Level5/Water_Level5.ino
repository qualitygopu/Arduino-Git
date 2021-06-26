#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define STATUSLED   6
#define START       2
#define STOP        3
#define ONBUT       A0
#define OFFBUT      A1
#define On_Det      4
#define LEV1        9
#define LEV2        10
#define LEV3        11
#define LEV4        12
#define LEV5        13
#define DRY         8

LiquidCrystal_I2C lcd(0x27,20,2);
enum MOTORSTATE {ON, OFF};

int waterLevel, waterflow, a;
long tim_on, tim_off, buttonTimer = 0, longPressTime = 1000, tim_setlow = 0, tim_progres=0, tim_lowOn=0;
bool Pow_on = false, lowOn = false;
int AutoMode=0, LowLevel = 20;
MOTORSTATE MotorState;

class MySwitch {
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
          } else {
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

byte Bar[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

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
    pinMode(LEV1, INPUT_PULLUP);
    pinMode(LEV2, INPUT_PULLUP);
    pinMode(LEV3, INPUT_PULLUP);
    pinMode(LEV4, INPUT_PULLUP);
    pinMode(LEV5, INPUT_PULLUP);
    pinMode(DRY, INPUT);

    loadSettings();
    
    lcd.init();
	  lcd.backlight();
    lcd.createChar(0, Bar);
    lcd.setCursor(5,0);
    lcd.print("QTRON");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    String Ams = (AutoMode == 1) ? "ON  " : "OFF ";
    lcd.println("AUTO MODE : " + Ams);
    lcd.setCursor(0, 1);
    lcd.println("LOW LEVEL: " + String(LowLevel)+"%   ");
    delay(1000);
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
  delay(100);
    waterLevel = 0;
    if (digitalRead(LEV1) == 0) waterLevel = 20;
    if (digitalRead(LEV2) == 0) waterLevel = 40;
    if (digitalRead(LEV3) == 0) waterLevel = 60;
    if (digitalRead(LEV4) == 0) waterLevel = 80;
    if (digitalRead(LEV5) == 0) waterLevel = 100;
    if (digitalRead(DRY) == 0) waterflow = 60; else waterflow = 0;


    if (Pow_on == true)
    {
        if(millis() > 60000)
        {
            Pow_on = false;
            MotorOn();
        }
    }
    if (millis() > tim_on + 1000) digitalWrite(START, LOW);
    if (millis() > tim_off + 5000) digitalWrite(STOP, LOW);

    if (digitalRead(On_Det) == LOW){
        MotorState = ON;
        EEPROM.write(2, MotorState);
    }else if (digitalRead(On_Det) == HIGH) {
        MotorState = OFF;
        EEPROM.write(2, MotorState);
    }
    onSw.update();
    if (onSw.isHold() == true) {
        onSw.clearValue();
        setLowLevel();
        }
    else if (onSw.isPressed() == true) {
        onSw.clearValue();
        MotorOn();
        }

    offSw.update();
    if (offSw.isHold() == true) {
        offSw.clearValue();
        changeAutomode();
        }
    else if (offSw.isPressed() == true) {
        offSw.clearValue();
        MotorOff();
    }

    if (waterLevel >=100 && AutoMode == 1)
      {
        MotorOff();
      }
      if (waterLevel <= LowLevel && AutoMode == 1)
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
      lcd.setCursor(0, 0);
      lcd.print(lowOn == true ? "WATER LEVEL LOW " : "WATER LEVEL     ");
      LevelBar(waterLevel/10);
      lcd.setCursor(10,1);
      lcd.print(">>" + String(waterLevel) + "% ");
      if (MotorState == ON )
      {
        if (millis() > tim_progres + 200)
        {
          tim_progres = millis();
          if (((waterLevel/10) + a) > 10) 
          {
            a = 0;
            lcd.setCursor((waterLevel/10), 1);
            lcd.print(strSpace(10 - (waterLevel/10)));
          }
          lcd.setCursor(((waterLevel/10) + a-1), 1);
          lcd.write(0);
          a++;
        }
      } 
      else if (MotorState == OFF)
      {
        a=0;
        lcd.setCursor(waterLevel/10, 1);
        lcd.print(strSpace(10 - (waterLevel/10)));
      }
}

void MotorOff()
{
  if (MotorState != OFF)
  {
    tim_off = millis();
    digitalWrite(STOP, HIGH);
    lcd.clear();
    lcd.print("  MOTOR STOPED");
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
      lcd.clear();
      lcd.print(" MOTOR STARTED");
      //MotorState = ON;
      // EEPROM.write(2, MotorState);
    }
}

void LevelBar(int perc)
{
  for (int i=0; i <= perc-1; i++)
  {
    lcd.setCursor(i, 1);
    lcd.write(0);
  }
}

void changeAutomode()  // Auto Mode ON/OFF
{
  if (AutoMode == 1)
  {
    AutoMode = 0;
    EEPROM.write(3, AutoMode);
    lcd.clear();
    lcd.print(F(" AUTO MODE OFF"));
    delay(1000);
  }
  else 
  {
    AutoMode = 1;
    EEPROM.write(3, AutoMode);
    lcd.clear();
    lcd.print(F(" AUTO MODE ON"));
    delay(1000);
  }
}

void setLowLevel()
{
  lcd.clear();
  tim_setlow = millis();  
  while(true)
  {
    if (millis() - tim_setlow > 10000) break;
    lcd.setCursor(0,0);
    lcd.print("SET LOW LEVEL");
    lcd.setCursor(3,1);
    lcd.print("<  " + String(LowLevel) + "%   >  ");

    onSw.update();
    if (onSw.isHold() == true) 
    {
      onSw.clearValue();
      EEPROM.write(4, LowLevel);
      lcd.clear();
      lcd.print("Low Level Stored...");
      break;
    }
    else if (onSw.isPressed() == true) 
    {
      onSw.clearValue();
      LowLevel -=20;
      if(LowLevel < 0) LowLevel = 80;
      tim_setlow = millis();
    }
    
    offSw.update();
    if (offSw.isPressed() == true) 
    {
      offSw.clearValue();
      LowLevel +=20;
      if(LowLevel > 80) LowLevel = 0;
      tim_setlow = millis();
    }
  }
    
}

String strSpace(int g)
{
  switch (g) {
    case 0:
    return "";
    break;
  case 1:
    return " ";
    break;
  case 2:
    return "  ";
    break;
  case 3:
    return "   ";
    break;
  case 4:
    return "    ";
    break;
  case 5:
    return "     ";
    break;
  case 6:
    return "      ";
    break;
  case 7:
    return "       ";
    break;
  case 8:
    return "        ";
    break;
  case 9:
    return "         ";
    break;
  case 10:
    return "          ";
    break;
  }
}

