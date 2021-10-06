#include <LiquidCrystal.h>

#include <Wire.h> 
// #include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <RTClib.h>

#define BUT1    2
#define BUT2    3
#define BUT3    4
#define BUT4    5
#define MOTOR   9
#define SPRAY   10
#define BUZZR   11
#define LCDBacklight 6
// LiquidCrystal_I2C lcd(0x27,20,4);
LiquidCrystal lcd(12, 13, A0, A1, A2, A3);
RTC_DS3231 rtc;

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
      init();
    }
    void setHoldDelay(unsigned int _holddelay)
    {
      holdDelay = _holddelay;
    }
    void init()
    {
      pinMode(pin, INPUT_PULLUP);
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

MySwitch Button1(BUT1);
MySwitch Button2(BUT2);
MySwitch Button3(BUT3);
MySwitch Button4(BUT4);

int curPos = 1;
int setDT[10];
int backLight_dur = 10000;
DateTime now;
DateTime tmpDate, remainTime;
String timeNow;
int menu = 0, level = 0;
unsigned long tim_sprTime, tim_sprInter, tim_lcdStat, tmr_beep;
bool sprayActive = false, sprayStrat = false, Pow_on = false, beep=false;

enum BUTT
{
  But1, But2, But3, But4, EXIT, SETTING, _null, LEFT  
};

enum MECSTAT
{
  MOD1RUN, 
  MOD2RUN, 
  MOD3RUN, 
  MECSTOP
};

struct Option
{
  int m1MotTim;
  int m1SprIntr;
  int m1SprTim;
  int m2MotTim;
  int m2SprIntr;
  int m2SprTim;
  int m2SprDly;
  int m3MotTim;
  MECSTAT CUR_STAT;
  DateTime MotrStopTime;
  DateTime SprayStarTime;
};

enum MENU
{
  NON,
  ROOT,
  DATTIM,
  DATTIM_SET,
  MOD1,
  MOD1_MOT_TIM_SET,
  MOD1_SPR_INTR_SET,
  MOD1_SPR_TIM_SET,
  MOD2,
  MOD2_MOT_TIM_SET,
  MOD2_SPR_INTR_SET,
  MOD2_SPR_TIM_SET,
  MOD2_SPR_STRT_SET,
  MOD3,
  MOD3_MOT_TIM_SET,
};

Option options;
BUTT curtBut;
MENU CUR_MNU = NON;
MENU lastMNU;
MECSTAT MechState;
int lstmenu;
String months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
int tmpMOD1_Mot_tim, 
    tmpMOD1_Spra_int, 
    tmpMOD1_Spra_tim,
    tmpMOD2_Mot_tim, 
    tmpMOD2_Spra_int, 
    tmpMOD2_Spra_tim,
    tmpMOD2_Spra_Strt,
    tmpMOD3_Mot_tim;

void showTime();
void setDateTime();
String doubledigit(int v);

void setup()
{
  Serial.begin(9600);
  pinMode(BUT1, INPUT_PULLUP);
  pinMode(BUT2, INPUT_PULLUP);
  pinMode(BUT3, INPUT_PULLUP);
  pinMode(BUT4, INPUT_PULLUP);
  pinMode(MOTOR, OUTPUT);
  pinMode(SPRAY, OUTPUT);
  pinMode(BUZZR, OUTPUT);
  pinMode(LCDBacklight, OUTPUT);
  lcd.begin(20,4);
  // lcd.backlight();  
  digitalWrite(LCDBacklight, HIGH);
  lcd.setCursor(4,1);
  lcd.print(F("SUJHATRONICS"));
  delay(1000);
  lcd.clear();
  EEPROM.begin();
  loadSettings();
  
  while (!rtc.begin())
  {
    lcd.clear();
    lcd.print(F("ERR 01")); // Couldn't find RTC
  }

  if (rtc.lostPower())
  {
    lcd.clear();
    lcd.print(F("ERR 02"));
    delay(2000);
  }
  
  

  if (options.CUR_STAT == MECSTOP)
    Pow_on = false;
  else
    Pow_on = true;
  
  
  Serial.println(Pow_on);
}

void loadSettings()
{
  if (EEPROM.read(1023) == 100)
  {
    EEPROM.get(0,options);
  }else
  {
    options.m1MotTim = 480;
    options.m1SprIntr = 15;
    options.m1SprTim = 10;
    options.m2MotTim = 480;
    options.m2SprDly = 120;
    options.m2SprIntr = 15;
    options.m2SprTim = 10;
    options.m3MotTim = 480;
    options.CUR_STAT = MECSTOP;
    EEPROM.put(0,options);
    EEPROM.write(1023, 100);
  }
}

void loop()
{
  static unsigned long tmr_lcdlight = millis();
  static unsigned long timr = millis();

  if (Pow_on == true)
  {
    // lcd.backlight(); 
    digitalWrite(LCDBacklight, HIGH);
    tmr_lcdlight = millis();
    digitalWrite(BUZZR, HIGH);
    delay(300);
    digitalWrite(BUZZR, LOW);
    delay(100);
    lcd.setCursor(0,0);
    lcd.print(F(" READY TO CONTINUE"));
    lcd.setCursor(0,2);
    lcd.print(F("LONG PRESS STOP BUTT"));
    lcd.setCursor(0,3);
    lcd.print(F("  TO STOP MECHINE   "));
    Button4.update();
    if (Button4.isHold())
    {
      Button4.clearValue();
      Pow_on = false;
      options.CUR_STAT = MECSTOP;
      EEPROM.put(0,options);
    }
    if(millis() > 30000)
    {
      Pow_on = false;
      switch (options.CUR_STAT)
      {   
      case MOD1RUN:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(F("MODE 1 CONTINUE"));
        lcd.setCursor(0,1);
        delay(1000);
        digitalWrite(MOTOR, HIGH);
        tim_sprInter = millis(); 
        break;
      case MOD2RUN:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(F("MODE 2 CONTINUE"));
        lcd.setCursor(0,1);
        delay(1000);
        digitalWrite(MOTOR, HIGH);
        break;
      case MOD3RUN:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(F("MODE 3 CONTINUE"));
        lcd.setCursor(0,1);
        delay(1000);
        digitalWrite(MOTOR, HIGH);
        break;
      }
    }
  }

  // Show LCD Time Date
  if (millis() - timr > 500 && Pow_on == false)
  {
    timr = millis();
    if (level == 0)
      showTime();
  }
  delay(100);

  
  if (millis() >= tmr_lcdlight + backLight_dur)
  {
    // lcd.noBacklight();
    digitalWrite(LCDBacklight, LOW);
  }

  if (digitalRead(BUT1) == LOW) {digitalWrite(LCDBacklight, HIGH); tmr_lcdlight = millis();}
  if (digitalRead(BUT2) == LOW) {digitalWrite(LCDBacklight, HIGH); tmr_lcdlight = millis();}
  if (digitalRead(BUT3) == LOW) {digitalWrite(LCDBacklight, HIGH); tmr_lcdlight = millis();}
  if (digitalRead(BUT4) == LOW) {digitalWrite(LCDBacklight, HIGH); tmr_lcdlight = millis();}

  // Set But State
  {
    curtBut = _null;
    Button1.update();
    if (Button1.isHold())
    {
      Button1.clearValue();
      curtBut = SETTING;
    }else if (Button1.isPressed())
    {
      Button1.clearValue();
      curtBut = But1; 
    }

    Button2.update();
    if (Button2.isHold())
    {
      Button2.clearValue();
    }else if (Button2.isPressed())
    {
      Button2.clearValue();
      curtBut = But2; 
    }

    Button3.update();
    if (Button3.isHold())
    {
      curtBut = LEFT;
      Button3.clearValue();
    }else if (Button3.isPressed())
    {
      Button3.clearValue();
      curtBut = But3; 
    }

    Button4.update();
    if (Button4.isHold())
    {
      Button4.clearValue();
      curtBut = EXIT;
    }else if (Button4.isPressed())
    {
      Button4.clearValue();
      curtBut = But4; 
    }
  }

  if (CUR_MNU == NON && Pow_on == false) 
  {
    switch (curtBut) {
      case But1:
        if (options.CUR_STAT == MECSTOP)
        {
          beep = false;
          digitalWrite(MOTOR, HIGH);
          options.MotrStopTime = rtc.now() + TimeSpan(0,(options.m1MotTim / 60),(options.m1MotTim % 60),0);
          options.CUR_STAT = MOD1RUN;
          tim_sprInter = millis(); 
          lcd.clear();
          Serial.println("Mod 1 Stop time : " + options.MotrStopTime.timestamp());
          EEPROM.put(0,options);
        } else 
        {
          checkMode();
        }
      break;
      case But2:
        if (options.CUR_STAT == MECSTOP)
          {
            beep = false;
            digitalWrite(MOTOR, HIGH);
            options.MotrStopTime = rtc.now() + TimeSpan(0,(options.m2MotTim / 60),(options.m2MotTim % 60),0);
            options.SprayStarTime = now + TimeSpan(0,(options.m2SprDly / 60),(options.m2SprDly % 60),0);
            options.CUR_STAT = MOD2RUN;
            lcd.clear();
            Serial.println("Mod 2 Stop time : " + options.MotrStopTime.timestamp());
            EEPROM.put(0,options);
          } else 
          {
            checkMode();
          }
      break;
      case But3:
        if (options.CUR_STAT == MECSTOP)
        {
          beep = false;
          digitalWrite(MOTOR, HIGH);
          options.MotrStopTime = rtc.now() + TimeSpan(0,(options.m3MotTim / 60),(options.m3MotTim % 60),0);
          options.CUR_STAT = MOD3RUN;
          lcd.clear();
          Serial.println("Mod 3 Stop time : " + options.MotrStopTime.timestamp());
          EEPROM.put(0,options);
        } else 
        {
          checkMode();
        }
      break;
      case But4:
      break;
      case EXIT:
        options.CUR_STAT = MECSTOP;
        digitalWrite(MOTOR, LOW);
        digitalWrite(SPRAY, LOW);
        sprayActive = false;
        EEPROM.put(0,options);
        lcd.setCursor(0,2);
        lcd.clear();
        lcd.print("  MECHINE STOPPED   ");
        delay(500);
      break;
    }
  }

  if (options.CUR_STAT == MOD1RUN && Pow_on == false)
  {
    if (millis() > tim_sprInter + ((options.m1SprIntr) * 60UL * 1000UL))
    {
      if (sprayActive == false)
      {
        Serial.println("Spray ON : " + now.timestamp());
        digitalWrite(SPRAY, HIGH);
        tim_sprTime = millis();
        sprayActive = true;
      }
    }
    if (millis() > tim_sprTime + (options.m1SprTim * 1000))
    {
      if (sprayActive == true)
      {
        Serial.println("Spray OFF : " + now.timestamp());
        digitalWrite(SPRAY, LOW);
        tim_sprInter = millis(); 
        sprayActive = false;
      }      
    } 
    if (now > options.MotrStopTime)
    {
      Serial.println("Motor Stop : " + now.timestamp());
      digitalWrite(MOTOR, LOW);
      options.CUR_STAT = MECSTOP;
      sprayActive = false;
      EEPROM.put(0,options);
      lcd.clear();
      beep = true;
      tmr_beep = millis();
      digitalWrite(LCDBacklight, HIGH); tmr_lcdlight = millis();
    }   
  }

  if (options.CUR_STAT == MOD2RUN && Pow_on == false)
  {
    if (now >= options.SprayStarTime && sprayStrat == false)
    {
      tim_sprInter = millis();
      sprayStrat = true; 
    }
    
    if (millis() > tim_sprInter + ((options.m2SprIntr) * 60UL * 1000UL) && sprayStrat == true)
    {
      if (sprayActive == false)
      {
        digitalWrite(LCDBacklight, HIGH); tmr_lcdlight = millis();
        Serial.println("Spray ON : " + now.timestamp());
        digitalWrite(SPRAY, HIGH);
        tim_sprTime = millis();
        sprayActive = true;
      }
    }
    if (millis() > tim_sprTime + (options.m2SprTim * 1000) && sprayStrat == true)
    {
      if (sprayActive == true)
      {
        Serial.println("Spray OFF : " + now.timestamp());
        digitalWrite(SPRAY, LOW);
        tim_sprInter = millis(); 
        sprayActive = false;
      }      
    } 
    if (now > options.MotrStopTime)
    {
      Serial.println("Motor Stop : " + now.timestamp());
      digitalWrite(MOTOR, LOW);
      options.CUR_STAT = MECSTOP;
      sprayActive = false;
      EEPROM.put(0,options);
      lcd.clear();
      beep = true;
      tmr_beep = millis();
      digitalWrite(LCDBacklight, HIGH); tmr_lcdlight = millis();
    }   
  }

  if (options.CUR_STAT == MOD3RUN && Pow_on == false)
  {
    if (now > options.MotrStopTime)
    {
      Serial.println("Motor Stop : " + now.timestamp());
      digitalWrite(MOTOR, LOW);
      options.CUR_STAT = MECSTOP;
      sprayActive = false;
      EEPROM.put(0,options);
      beep = true;
      lcd.clear();
      tmr_beep = millis();
      digitalWrite(LCDBacklight, HIGH); tmr_lcdlight = millis();
    }   
  }

  if (beep == true)
  {
    if (millis() < tmr_beep + 20000)
    {
      digitalWrite(BUZZR,HIGH);
    }
    else
    {
      digitalWrite(BUZZR,LOW);
      beep = false;
    }    
  }
  else
  {
    digitalWrite(BUZZR,LOW);
  }


  switch (CUR_MNU)
  {
    case DATTIM_SET:
      setDatTimUpDown(curtBut);
      break;   
    case MOD1_MOT_TIM_SET:
      setMod1MotTime(curtBut);
      break;
    case MOD1_SPR_INTR_SET:
      setMod1SprIntr(curtBut);
      break;
    case MOD1_SPR_TIM_SET:
      setMod1SprTime(curtBut);
      break;
    case MOD2_MOT_TIM_SET:
      setMod2MotTime(curtBut);
      break;
    case MOD2_SPR_INTR_SET:
      setMod2SprIntr(curtBut);
      break;
    case MOD2_SPR_TIM_SET:
      setMod2SprTime(curtBut);
      break;            
    case MOD2_SPR_STRT_SET:
      setMod2SprStrt(curtBut);
      break;
    case MOD3_MOT_TIM_SET:
      setMod3MotTime(curtBut);
      break;                        
    default :
    switch (curtBut)
    {
      case SETTING:
        switch (level)
        {
        case 0:
          level++;
          menu = 1;
          CUR_MNU = ROOT;
          showMenu_Level1();
          break;
        default:
          level = 0;
          lcd.clear();
          CUR_MNU = NON;
          lcd.setCursor(7,1);
          lcd.print(F("STRON"));
          break;
        }
      break;
      case But2:
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
        }
      break;
      case But1:
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
        }
      break;
      case But3:
        switch (level)
        {
        
        case 1:
          level++;
          execute_Level1();
          break;
        case 2:
          execute_Level2();
          break;
        }
      break;
      case But4:
        switch (level)
        {
        case 1:
          level--;
          lcd.clear();
          lcd.setCursor(7,1);
          lcd.print(F("STRON"));
          CUR_MNU = NON;
          break;
        case 2:
          level--;
          restorePreMenu();
          showMenu_Level1();
          break;
        }
      break;
    }
  }


}

void checkMode()
{
  tim_lcdStat = millis();
  if (options.CUR_STAT == MOD1RUN)
  {
    lcd.setCursor(0,2);
    lcd.print(F(">> MODE 1 RUNNING <<"));
  }
  else if (options.CUR_STAT == MOD2RUN)
  {
    lcd.setCursor(0,2);
    lcd.print(F(">> MODE 2 RUNNING <<"));
  }
  else if (options.CUR_STAT == MOD3RUN)
  {
    lcd.setCursor(0,2);
    lcd.print(F(">> MODE 3 RUNNING <<"));
  }
}

void showTime()
{
  lcd.setCursor(0,0);
  // lcd.print(F("     MODEL EXIM     "));

  now = rtc.now();
  timeNow = doubledigit(now.hour()) + ":" + doubledigit(now.minute()) + ":" + doubledigit(now.second());
            // + " " + String(now.isPM() ? "PM" : "AM");
  lcd.print(doubledigit(now.day()) + "/" + doubledigit(now.month()) + "/" + String(now.year()) + "  " + timeNow);

  lcd.setCursor(0, 1);
  lcd.print("--------------------");

  if (options.CUR_STAT == MOD1RUN)
  {
    lcd.setCursor(0,2);
    lcd.print(F("   MODE 1 RUNNING   "));
    remainTime = options.MotrStopTime - TimeSpan(0,now.hour(),now.minute(),0);
    lcd.setCursor(0,3);
    lcd.print("REMAINING TIME " + doubledigit(remainTime.hour()) + ":" + doubledigit(remainTime.minute()) );
  }
  else if (options.CUR_STAT == MOD2RUN)
  {
    lcd.setCursor(0,2);
    lcd.print(F("   MODE 2 RUNNING   "));
    remainTime = options.MotrStopTime - TimeSpan(0,now.hour(),now.minute(),0);
    lcd.setCursor(0,3);
    lcd.print("REMAINING TIME " + doubledigit(remainTime.hour()) + ":" + doubledigit(remainTime.minute()) );
  }
  else if (options.CUR_STAT == MOD3RUN)
  {
    lcd.setCursor(0,2);
    lcd.print(F("   MODE 3 RUNNING   "));
    remainTime = options.MotrStopTime - TimeSpan(0,now.hour(),now.minute(),0);
    lcd.setCursor(0,3);
    lcd.print("REMAINING TIME " + doubledigit(remainTime.hour()) + ":" + doubledigit(remainTime.minute()) );
  }
  else
  {
    lcd.setCursor(0,2);
    lcd.print(F("     MODEL EXIM     "));
    // lcd.setCursor(0,3);
    // lcd.print(F("                    "));
  }

}

void restorePreMenu()
{
  if (CUR_MNU == DATTIM)
  {  
    CUR_MNU = ROOT;
    menu = 1;
  }
  if (CUR_MNU == MOD1)
  {  
    CUR_MNU = ROOT;
    menu = 2;
  }
  if (CUR_MNU == MOD2)
  {  
    CUR_MNU = ROOT;
    menu = 3;
  }
  if (CUR_MNU == MOD3)
  {  
    CUR_MNU = ROOT;
    menu = 4;
  }
  if (CUR_MNU == DATTIM_SET)
  {
    CUR_MNU = DATTIM;
    menu = 1;
  }
  if (CUR_MNU == MOD1_MOT_TIM_SET || CUR_MNU == MOD1_SPR_INTR_SET || CUR_MNU == MOD1_SPR_TIM_SET)
  {
    CUR_MNU = MOD1;
    menu = 1;
  }
  if (CUR_MNU == MOD2_MOT_TIM_SET || CUR_MNU == MOD2_SPR_INTR_SET || CUR_MNU == MOD2_SPR_TIM_SET || CUR_MNU == MOD2_SPR_STRT_SET)
  {
    CUR_MNU = MOD2;
    menu = 1;
  }
  if (CUR_MNU == MOD3_MOT_TIM_SET)
  {
    CUR_MNU = MOD3;
    menu = 1;
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
    lcd.print(F(">SET DATE & TIME"));
    lcd.setCursor(0, 1);
    lcd.print(F(" MODE 1"));
    lcd.setCursor(0, 2);
    lcd.print(F(" MODE 2"));
    lcd.setCursor(0, 3);
    lcd.print(F(" MODE 3"));
    break;
  case 2:
    lcd.clear();
    lcd.print(F(" SET DATE & TIME"));
    lcd.setCursor(0, 1);
    lcd.print(F(">MODE 1"));
    lcd.setCursor(0, 2);
    lcd.print(F(" MODE 2"));
    lcd.setCursor(0, 3);
    lcd.print(F(" MODE 3"));
    break;
  case 3:
    lcd.clear();
    lcd.print(F(" SET DATE & TIME"));
    lcd.setCursor(0, 1);
    lcd.print(F(" MODE 1"));
    lcd.setCursor(0, 2);
    lcd.print(F(">MODE 2"));
    lcd.setCursor(0, 3);
    lcd.print(F(" MODE 3"));
    break;
  case 4:
    lcd.clear();
    lcd.print(F(" SET DATE & TIME"));
    lcd.setCursor(0, 1);
    lcd.print(F(" MODE 1"));
    lcd.setCursor(0, 2);
    lcd.print(F(" MODE 2"));
    lcd.setCursor(0, 3);
    lcd.print(F(">MODE 3"));
    break;
  case 5:
    menu = 5;
    break;
  }
}

void showMenu_Level2()
{
  switch (CUR_MNU)
  {
  case MOD1:
    switch (menu)
    {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(F(">MOTOR TIME"));
      lcd.setCursor(0, 1);
      lcd.print(F(" SPRAY TIME"));
      lcd.setCursor(0, 2);
      lcd.print(F(" SPRAY INTERVAL"));
      break;
    case 2:
      lcd.clear();
      lcd.print(F(" MOTOR TIME"));
      lcd.setCursor(0, 1);
      lcd.print(F(">SPRAY TIME"));
      lcd.setCursor(0, 2);
      lcd.print(F(" SPRAY INTERVAL"));
      break;
    case 3:
      lcd.clear();
      lcd.print(F(" MOTOR TIME"));
      lcd.setCursor(0, 1);
      lcd.print(F(" SPRAY TIME"));
      lcd.setCursor(0, 2);
      lcd.print(F(">SPRAY INTERVAL"));
      break;
    case 4:
      menu=3;
      break;
    }
    break;
  case MOD2:
    switch (menu)
    {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(F(">MOTOR TIME"));
      lcd.setCursor(0, 1);
      lcd.print(F(" SPRAY TIME"));
      lcd.setCursor(0, 2);
      lcd.print(F(" SPRAY INTERVAL"));
      lcd.setCursor(0, 3);
      lcd.print(F(" SPRAY START DELAY"));
      break;
    case 2:
      lcd.clear();
      lcd.print(F(" MOTOR TIME"));
      lcd.setCursor(0, 1);
      lcd.print(F(">SPRAY TIME"));
      lcd.setCursor(0, 2);
      lcd.print(F(" SPRAY INTERVAL"));
      lcd.setCursor(0, 3);
      lcd.print(F(" SPRAY START DELAY"));
      break;
    case 3:
      lcd.clear();
      lcd.print(F(" MOTOR TIME"));
      lcd.setCursor(0, 1);
      lcd.print(F(" SPRAY TIME"));
      lcd.setCursor(0, 2);
      lcd.print(F(">SPRAY INTERVAL"));
      lcd.setCursor(0, 3);
      lcd.print(F(" SPRAY START DELAY"));
      break;
    case 4:
      lcd.clear();
      lcd.print(F(" MOTOR TIME"));
      lcd.setCursor(0, 1);
      lcd.print(F(" SPRAY TIME"));
      lcd.setCursor(0, 2);
      lcd.print(F(" SPRAY INTERVAL"));
      lcd.setCursor(0, 3);
      lcd.print(F(">SPRAY START DELAY"));
      break;
    case 5:
      menu=4;
      break;
    }
    break;
  case MOD3:
    switch (menu)
    {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(F(">MOTOR TIME"));
      break;
    case 2:
      menu=1;
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
    CUR_MNU = DATTIM_SET;
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print(F("DATE      TIME"));
    lcd.setCursor(1,1);
    lcd.print(doubledigit(now.day()) + "/" + doubledigit(now.month()) + "/" + String(now.year()) + " - " + doubledigit(now.hour()) + ":" + doubledigit(now.minute()));
    lcd.setCursor(1, 2);
    lcd.print("^");
    lcd.setCursor(0,3);
    lcd.print(F("UP   DWN   SEL  SAVE"));
    curPos = 1;
    setDT[0] = now.day() / 10;
    setDT[1] = now.day() % 10;
    setDT[2] = now.month() / 10;
    setDT[3] = now.month() % 10;
    setDT[4] = (now.year() % 1000) / 10;
    setDT[5] = (now.year() % 1000) % 10;
    setDT[6] = now.hour() / 10;
    setDT[7] = now.hour() % 10;
    setDT[8] = now.minute() / 10;
    setDT[9] = now.minute() % 10;
    break;
  case 2:
    CUR_MNU = MOD1;
    menu = 1;
    showMenu_Level2();
    break;
  case 3:
    CUR_MNU = MOD2;
    menu = 1;
    showMenu_Level2();
    break;
  case 4:
    CUR_MNU = MOD3;
    menu = 1;
    showMenu_Level2();
    break;
  }
}

void execute_Level2()
{
  switch (CUR_MNU)
  {
  case MOD1:
    level++;
    switch(menu)
    {
      case 1:
        CUR_MNU = MOD1_MOT_TIM_SET;
        lcd.clear();
        lcd.print(F(" MOTOR RUNNING TIME "));
        lcd.setCursor(0,1);
        lcd.print(">SET TIME : " + trebledigit(options.m1MotTim) + " Min");
        lcd.setCursor(12,2);
        lcd.print(F("^"));
        lcd.setCursor(0,3);
        lcd.print(F("UP   DWN   SEL  SAVE"));
        curPos=1;
        tmpMOD1_Mot_tim = options.m1MotTim;
        break;
      case 2:
        CUR_MNU = MOD1_SPR_TIM_SET;
        lcd.clear();
        lcd.print(F("    SPRAYING TIME   "));
        lcd.setCursor(0,1);
        lcd.print(">SET TIME : " + trebledigit(options.m1SprTim) + " Sec");
        lcd.setCursor(12,2);
        lcd.print(F("^"));
        lcd.setCursor(0,3);
        lcd.print(F("UP   DWN   SEL  SAVE"));
        curPos=1;
        tmpMOD1_Spra_tim = options.m1SprTim;
        break;
      case 3:
        CUR_MNU = MOD1_SPR_INTR_SET;
        lcd.clear();
        lcd.print(F("   SPRAY INTERVAL   "));
        lcd.setCursor(0,1);
        lcd.print(">SET TIME : " + trebledigit(options.m1SprIntr) + " Min");
        lcd.setCursor(12,2);
        lcd.print(F("^"));
        lcd.setCursor(0,3);
        lcd.print(F("UP   DWN   SEL  SAVE"));
        curPos=1;
        tmpMOD1_Spra_int = options.m1SprIntr;
        break;
    }
    break;
  case MOD2:
    level++;
    switch(menu)
    {
      case 1:
        CUR_MNU = MOD2_MOT_TIM_SET;
        lcd.clear();
        lcd.print(F(" MOTOR RUNNING TIME "));
        lcd.setCursor(0,1);
        lcd.print(">SET TIME : " + trebledigit(options.m2MotTim) + " Min");
        lcd.setCursor(12,2);
        lcd.print(F("^"));
        lcd.setCursor(0,3);
        lcd.print(F("UP   DWN   SEL  SAVE"));
        curPos=1;
        tmpMOD2_Mot_tim = options.m2MotTim;
        break;
      case 2:
        CUR_MNU = MOD2_SPR_TIM_SET;
        lcd.clear();
        lcd.print(F("    SPRAYING TIME   "));
        lcd.setCursor(0,1);
        lcd.print(">SET TIME : " + trebledigit(options.m2SprTim) + " Sec");
        lcd.setCursor(12,2);
        lcd.print(F("^"));
        lcd.setCursor(0,3);
        lcd.print(F("UP   DWN   SEL  SAVE"));
        curPos=1;
        tmpMOD2_Spra_tim = options.m2SprTim;
        break;
      case 3:
        CUR_MNU = MOD2_SPR_INTR_SET;
        lcd.clear();
        lcd.print(F("   SPRAY INTERVAL   "));
        lcd.setCursor(0,1);
        lcd.print(">SET TIME : " + trebledigit(options.m2SprIntr) + " Min");
        lcd.setCursor(12,2);
        lcd.print(F("^"));
        lcd.setCursor(0,3);
        lcd.print(F("UP   DWN   SEL  SAVE"));
        curPos=1;
        tmpMOD2_Spra_int = options.m2SprIntr;
        break;
      case 4:
        CUR_MNU = MOD2_SPR_STRT_SET;
        lcd.clear();
        lcd.print(F("   SPRAY START DELAY   "));
        lcd.setCursor(0,1);
        lcd.print(">SET TIME : " + trebledigit(options.m2SprDly) + " Min");
        lcd.setCursor(12,2);
        lcd.print(F("^"));
        lcd.setCursor(0,3);
        lcd.print(F("UP   DWN   SEL  SAVE"));
        curPos=1;
        tmpMOD2_Spra_Strt = options.m2SprDly;
        break;
    }
    break;
  case MOD3:
    level++;
    switch(menu)
    {
      case 1:
        CUR_MNU = MOD3_MOT_TIM_SET;
        lcd.clear();
        lcd.print(F(" MOTOR RUNNING TIME "));
        lcd.setCursor(0,1);
        lcd.print(">SET TIME : " + trebledigit(options.m3MotTim) + " Min");
        lcd.setCursor(12,2);
        lcd.print(F("^"));
        lcd.setCursor(0,3);
        lcd.print(F("UP   DWN   SEL  SAVE"));
        curPos=1;
        tmpMOD3_Mot_tim = options.m3MotTim;
        break;
    }
    break;
  }
}

void setDatTimUpDown(BUTT key)
{
  switch (key)
  {
  case But3:
    curPos++;
    if (curPos > 10)
      curPos = 1;
    setCur(curPos);
    break;
  case LEFT:
    curPos--;
    if (curPos < 1)
      curPos = 10;
    setCur(curPos);
    break;
  case But1:
    setDatTim(1, curPos);
    break;
  case But2:
    setDatTim(-1, curPos);
    break;
  case But4:
    //now = rtc.now();
    tmpDate = DateTime((2000 + (setDT[4] * 10) + setDT[5]), (setDT[2] * 10 + setDT[3]), (setDT[0] * 10 + setDT[1]), (setDT[6] * 10 + setDT[7]), (setDT[8] * 10 + setDT[9]), 0);
    rtc.adjust(tmpDate);
    CUR_MNU = DATTIM;
    menu = 1;
    level--;
    showMenu_Level1();
    break;
  case EXIT:
    CUR_MNU = DATTIM;
    menu = 1;
    level--;
    showMenu_Level1();
    break;
  }
}

void setDatTim(int dirc, int pos)
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
  else if (pos == 7)
  {
    setDT[pos - 1] += dirc;
    if (setDT[pos - 1] > 2)
      setDT[pos - 1] = 0;
    if (setDT[pos - 1] < 0)
      setDT[pos - 1] = 2;
  }
  else if (pos == 9)
  {
    setDT[pos - 1] += dirc;
    if (setDT[pos - 1] > 5)
      setDT[pos - 1] = 0;
    if (setDT[pos - 1] < 0)
      setDT[pos - 1] = 5;
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
  if (setDT[6] == 2)
  {
    if (setDT[7] > 3)
      setDT[7] = 0;
  }
  String dis =  String(setDT[0]) +
                String(setDT[1]) + "/" +
                String(setDT[2]) +
                String(setDT[3]) + "/20" +
                String(setDT[4]) +
                String(setDT[5]) + " - " +
                String(setDT[6]) +
                String(setDT[7]) + ":" +
                String(setDT[8]) +
                String(setDT[9]);
  // Serial.println(dis);
  lcd.setCursor(1, 1);
  lcd.print(dis);
}

void setCur(int pos)
{
  lcd.setCursor(0, 2);
  switch (pos)
  {
  case 1:
      lcd.print(" ^                 ");
    break;
  case 2:
      lcd.print("  ^                ");
    break;
  case 3:
      lcd.print("    ^              ");
    break;
  case 4:
      lcd.print("     ^             ");
    break;
  case 5:
      lcd.print("         ^         ");
    break;
  case 6:
      lcd.print("          ^        ");
    break;
  case 7:
      lcd.print("              ^    ");
    break;
  case 8:
      lcd.print("               ^   ");
    break;
  case 9:
      lcd.print("                 ^ ");
    break;
  case 10:
      lcd.print("                  ^");
    break;
  }
}

void setMod1MotTime(BUTT key)
{
  switch(key)
  {
    case But3:
      curPos++;
      if (curPos > 3)
        curPos = 1;
      lcd.setCursor(0,2);
      lcd.print(F("                    "));
      lcd.setCursor((11+curPos),2);
      lcd.print(F("^"));
      break;
    case LEFT:
      curPos--;
      if (curPos < 1)
        curPos = 3;
      lcd.setCursor(0,2);
      lcd.print(F("                    "));
      lcd.setCursor((11+curPos),2);
      lcd.print(F("^"));
      break;
    case But1:
      tmpMOD1_Mot_tim = changeValue(tmpMOD1_Mot_tim, curPos, 1);
      lcd.setCursor(0,1);
      lcd.print(">SET TIME : " + trebledigit(tmpMOD1_Mot_tim) + " Min ");
      break;
    case But2:
      tmpMOD1_Mot_tim = changeValue(tmpMOD1_Mot_tim,curPos,-1);
      lcd.setCursor(0,1);
      lcd.print(">SET TIME : " + trebledigit(tmpMOD1_Mot_tim) + " Min ");
      break;
    case But4:
      CUR_MNU = MOD1;
      options.m1MotTim = tmpMOD1_Mot_tim;
      EEPROM.put(0,options);
      menu = 1;
      level--;
      showMenu_Level2();
      break;
  }
}

void setMod1SprTime(BUTT key)
{
  switch(key)
  {
    case LEFT:
      curPos--;
      if (curPos < 1)
        curPos = 3;
      lcd.setCursor(0,2);
      lcd.print(F("                    "));
      lcd.setCursor((11+curPos),2);
      lcd.print(F("^"));
      break;
    case But3:
      curPos++;
      if (curPos > 3)
        curPos = 1;
      lcd.setCursor(0,2);
      lcd.print(F("                    "));
      lcd.setCursor((11+curPos),2);
      lcd.print(F("^"));
      break;
    case But1:
      tmpMOD1_Spra_tim = changeValue(tmpMOD1_Spra_tim, curPos, 1);
      lcd.setCursor(0,1);
      lcd.print(">SET TIME : " + trebledigit(tmpMOD1_Spra_tim) + " Sec      ");
      break;
    case But2:
      tmpMOD1_Spra_tim = changeValue(tmpMOD1_Spra_tim,curPos,-1);
      lcd.setCursor(0,1);
      lcd.print(">SET TIME : " + trebledigit(tmpMOD1_Spra_tim) + " Sec      ");
      break;
    case But4:
      CUR_MNU = MOD1;
      options.m1SprTim = tmpMOD1_Spra_tim;
      EEPROM.put(0,options);
      menu = 2;
      level--;
      showMenu_Level2();
      break;
  }
}

void setMod1SprIntr(BUTT key)
{
  switch(key)
  {
    case LEFT:
      curPos--;
      if (curPos < 1)
        curPos = 3;
      lcd.setCursor(0,2);
      lcd.print(F("                    "));
      lcd.setCursor((11+curPos),2);
      lcd.print(F("^"));
      break;
    case But3:
      curPos++;
      if (curPos > 3)
        curPos = 1;
      lcd.setCursor(0,2);
      lcd.print(F("                    "));
      lcd.setCursor((11+curPos),2);
      lcd.print(F("^"));
      break;
    case But1:
      tmpMOD1_Spra_int = changeValue(tmpMOD1_Spra_int, curPos, 1);
      lcd.setCursor(0,1);
      lcd.print(">SET TIME : " + trebledigit(tmpMOD1_Spra_int) + " Min ");
      break;
    case But2:
      tmpMOD1_Spra_int = changeValue(tmpMOD1_Spra_int,curPos,-1);
      lcd.setCursor(0,1);
      lcd.print(">SET TIME : " + trebledigit(tmpMOD1_Spra_int) + " Min ");
      break;
    case But4:
      CUR_MNU = MOD1;
      options.m1SprIntr = tmpMOD1_Spra_int;
      EEPROM.put(0,options);      
      menu = 3;
      level--;
      showMenu_Level2();
      break;
  }
}

void setMod2MotTime(BUTT key)
{
  switch(key)
  {
    case LEFT:
      curPos--;
      if (curPos < 1)
        curPos = 3;
      lcd.setCursor(0,2);
      lcd.print(F("                    "));
      lcd.setCursor((11+curPos),2);
      lcd.print(F("^"));
      break;
    case But3:
      curPos++;
      if (curPos > 3)
        curPos = 1;
      lcd.setCursor(0,2);
      lcd.print(F("                    "));
      lcd.setCursor((11+curPos),2);
      lcd.print(F("^"));
      break;
    case But1:
      tmpMOD2_Mot_tim = changeValue(tmpMOD2_Mot_tim, curPos, 1);
      lcd.setCursor(0,1);
      lcd.print(">SET TIME : " + trebledigit(tmpMOD2_Mot_tim) + " Min ");

      break;
    case But2:
      tmpMOD2_Mot_tim = changeValue(tmpMOD2_Mot_tim,curPos,-1);
      lcd.setCursor(0,1);
      lcd.print(">SET TIME : " + trebledigit(tmpMOD2_Mot_tim) + " Min ");
      break;
    case But4:
      CUR_MNU = MOD2;
      menu = 1;
      level--;
      options.m2MotTim = tmpMOD2_Mot_tim;
      EEPROM.put(0,options);
      showMenu_Level2();
      break;
  }
}

void setMod2SprTime(BUTT key)
{
  switch(key)
  {
    case LEFT:
      curPos--;
      if (curPos < 1)
        curPos = 3;
      lcd.setCursor(0,2);
      lcd.print(F("                    "));
      lcd.setCursor((11+curPos),2);
      lcd.print(F("^"));
      break;
    case But3:
      curPos++;
      if (curPos > 3)
        curPos = 1;
      lcd.setCursor(0,2);
      lcd.print(F("                    "));
      lcd.setCursor((11+curPos),2);
      lcd.print(F("^"));
      break;
    case But1:
      tmpMOD2_Spra_tim = changeValue(tmpMOD2_Spra_tim, curPos, 1);
      lcd.setCursor(0,1);
      lcd.print(">SET TIME : " + trebledigit(tmpMOD2_Spra_tim) + " Sec      ");
      break;
    case But2:
      tmpMOD2_Spra_tim = changeValue(tmpMOD2_Spra_tim,curPos,-1);
      lcd.setCursor(0,1);
      lcd.print(">SET TIME : " + trebledigit(tmpMOD2_Spra_tim) + " Sec      ");
      break;
    case But4:
      CUR_MNU = MOD2;
      menu = 2;
      level--;
      options.m2SprTim = tmpMOD2_Spra_tim;
      EEPROM.put(0,options);
      showMenu_Level2();
      break;
  }
}

void setMod2SprIntr(BUTT key)
{
  switch(key)
  {
    case LEFT:
      curPos--;
      if (curPos < 1)
        curPos = 3;
      lcd.setCursor(0,2);
      lcd.print(F("                    "));
      lcd.setCursor((11+curPos),2);
      lcd.print(F("^"));
      break;
    case But3:
      curPos++;
      if (curPos > 3)
        curPos = 1;
      lcd.setCursor(0,2);
      lcd.print(F("                    "));
      lcd.setCursor((11+curPos),2);
      lcd.print(F("^"));
      break;
    case But1:
      tmpMOD2_Spra_int = changeValue(tmpMOD2_Spra_int, curPos, 1);
      lcd.setCursor(0,1);
      lcd.print(">SET TIME : " + trebledigit(tmpMOD2_Spra_int) + " Min ");
      break;
    case But2:
      tmpMOD2_Spra_int = changeValue(tmpMOD2_Spra_int,curPos,-1);
      lcd.setCursor(0,1);
      lcd.print(">SET TIME : " + trebledigit(tmpMOD2_Spra_int) + " Min ");
      break;
    case But4:
      CUR_MNU = MOD2;
      menu = 3;
      level--;
      options.m2SprIntr = tmpMOD2_Spra_int;
      EEPROM.put(0,options);
      showMenu_Level2();
      break;
  }
}

void setMod2SprStrt(BUTT key)
{
  switch(key)
  {
    case LEFT:
      curPos--;
      if (curPos < 1)
        curPos = 3;
      lcd.setCursor(0,2);
      lcd.print(F("                    "));
      lcd.setCursor((11+curPos),2);
      lcd.print(F("^"));
      break;
    case But3:
      curPos++;
      if (curPos > 3)
        curPos = 1;
      lcd.setCursor(0,2);
      lcd.print(F("                    "));
      lcd.setCursor((11+curPos),2);
      lcd.print(F("^"));
      break;
    case But1:
      tmpMOD2_Spra_Strt = changeValue(tmpMOD2_Spra_Strt, curPos, 1);
      lcd.setCursor(0,1);
      lcd.print(">SET TIME : " + trebledigit(tmpMOD2_Spra_Strt) + " Min ");
      break;
    case But2:
      tmpMOD2_Spra_Strt = changeValue(tmpMOD2_Spra_Strt,curPos,-1);
      lcd.setCursor(0,1);
      lcd.print(">SET TIME : " + trebledigit(tmpMOD2_Spra_Strt) + " Min ");
      break;
    case But4:
      CUR_MNU = MOD2;
      menu = 4;
      level--;
      options.m2SprDly = tmpMOD2_Spra_Strt;     
      EEPROM.put(0,options); 
      showMenu_Level2();
      break;
  }
}

void setMod3MotTime(BUTT key)
{
  switch(key)
  {
    case LEFT:
      curPos--;
      if (curPos < 1)
        curPos = 3;
      lcd.setCursor(0,2);
      lcd.print(F("                    "));
      lcd.setCursor((11+curPos),2);
      lcd.print(F("^"));
      break;
    case But3:
      curPos++;
      if (curPos > 3)
        curPos = 1;
      lcd.setCursor(0,2);
      lcd.print(F("                    "));
      lcd.setCursor((11+curPos),2);
      lcd.print(F("^"));
      break;
    case But1:
      tmpMOD3_Mot_tim = changeValue(tmpMOD3_Mot_tim, curPos, 1);
      lcd.setCursor(0,1);
      lcd.print(">SET TIME : " + trebledigit(tmpMOD3_Mot_tim) + " Min ");
      break;
    case But2:
      tmpMOD3_Mot_tim = changeValue(tmpMOD3_Mot_tim,curPos,-1);
      lcd.setCursor(0,1);
      lcd.print(">SET TIME : " + trebledigit(tmpMOD3_Mot_tim) + " Min ");
      break;
    case But4:
      CUR_MNU = MOD3;
      menu = 1;
      level--;
      options.m3MotTim = tmpMOD3_Mot_tim;
      EEPROM.put(0,options);
      showMenu_Level2();
      break;
  }
}

int changeValue(int value, int pos, int chng)
{
  int hu = value /100;
  int te = (value % 100) / 10;
  int on = ((value % 100) % 10);
  switch(pos)
  {
    case 1:
      hu += chng;
      if (hu > 9) hu = 0;
      if (hu < 0) hu = 9;
      break;
    case 2:
      te +=chng;
      if (te > 9) te = 0;
      if (te < 0) te = 9;
      break;
    case 3:
      on +=chng;
      if (on > 9) on = 0;
      if (on < 0) on = 9;
      break;
  }
  return (hu * 100) + (te * 10) + on;
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

String trebledigit(int v)
{
  return  String(v / 100) + 
          String((v % 100) / 10) +
          String((v % 100) % 10);
}