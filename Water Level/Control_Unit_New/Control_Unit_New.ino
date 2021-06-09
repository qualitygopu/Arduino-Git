#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define SSerialRX        3  //Serial Receive pin
#define SSerialTX        2  //Serial Transmit pin
#define SSerialTxControl 4   //RS485 Direction control
#define STATUSLED        6
#define RS485Transmit    HIGH
#define RS485Receive     LOW
#define START            13
#define STOP             12
#define ONBUT            A0
#define OFFBUT           A1
#define On_Det           10
SoftwareSerial RS485(SSerialRX, SSerialTX);
LiquidCrystal_I2C lcd(0x27,20,2);

int byteReceived, a = 0;
int waterLevel, waterflow;
int SendStat;
long tim_on, tim_off, buttonTimer = 0, longPressTime = 1000, tim_setlow = 0, tim_progres=0, tim_lowOn=0;
bool Pow_on = false, lowOn = false;
int TankHight, AutoMode=0, LowLevel = 10;
enum MOTORSTATE {ON, OFF};
MOTORSTATE MotorState;
byte tx[4];
int counter = 0;

//dat

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

void setup()  
{
  Serial.begin(9600);
  while (!Serial);
  pinMode(STATUSLED, OUTPUT);
  pinMode(START, OUTPUT);
  pinMode(STOP, OUTPUT);
  pinMode(ONBUT, INPUT_PULLUP);
  pinMode(OFFBUT, INPUT_PULLUP);
  pinMode(On_Det, INPUT_PULLUP);

  pinMode(SSerialTxControl, OUTPUT);
  RS485.begin(9600);
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
  digitalWrite(SSerialTxControl, RS485Receive);
  if (MotorState == ON)
    Pow_on = true;
  else
    Pow_on = false;
}

void loadSettings()
{
  if (EEPROM.read(1023) == 'T')
  {
    TankHight = EEPROM.read(1);
    MotorState = EEPROM.read(2);
    AutoMode = EEPROM.read(3);
    LowLevel = EEPROM.read(4);
  }
  else
  {
    TankHight = 200;
    EEPROM.write(1, TankHight);
    MotorState = OFF;
    EEPROM.write(2, MotorState);
    AutoMode = 1;
    EEPROM.write(3, AutoMode);
    LowLevel = 10;
    EEPROM.write(4, LowLevel);
    EEPROM.write(1023, 'T');
  }
}

void loop()  
{
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
 
  if (RS485.available())  //Look for data from other Arduino
   {
    digitalWrite(STATUSLED, HIGH);  
    RS485.readBytes(tx,4);   
    waterLevel = tx[0];
    waterflow = tx[1];
    SendStat = tx[3];
    Serial.println(String(waterLevel));
    digitalWrite(STATUSLED, LOW); 
   }
    
    if (SendStat == 100)
    {
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
    else {
      lcd.clear();
      lcd.print(F("TANK UNIT ERR"));
    }
  
  //Clear Serial Buffer
  if (RS485.overflow())
  {
    Serial.println("Serial Overflow");
    while(RS485.read() >=0);
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
      LowLevel -=10;
      if(LowLevel < 0) LowLevel = 80;
      tim_setlow = millis();
    }
    
    offSw.update();
    if (offSw.isPressed() == true) 
    {
      offSw.clearValue();
      LowLevel +=10;
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

