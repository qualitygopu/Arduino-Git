#include <EEPROM.h>
#include <HCSR04.h>


UltraSonicDistanceSensor distanceSensor(2, 3);  // Initialize sensor that uses digital pins 13 and 12.
int dis;
long starttime = 0, Duar;
byte stat = false;
int HandIn();
#define Valv 4
#define flash 9
int fls=0;
int spray=200, xx = 0;
void setup () 
{
//    Serial.begin(9600);
//    Serial.println("Strated....");
    pinMode(Valv,OUTPUT);
    pinMode(flash,OUTPUT);
    stat = false;
    digitalWrite(Valv,LOW);
    Duar = 0;
    delay(500);
    dis = distanceSensor.measureDistanceCm();
    delay(50);
    if ((dis > 0) && (dis <= 10))
    {
        for (size_t i = 0; i < 50; i++)
        {
            digitalWrite(flash,digitalRead(flash) ^ 1);
            delay(50);
        }
        while ((distanceSensor.measureDistanceCm() <= 10) && (distanceSensor.measureDistanceCm() > 0))
        {
           digitalWrite(flash,HIGH);
           delay(500);
           digitalWrite(flash,LOW);
           delay(300);
           xx +=50;
           if (xx > 600)
           {
            xx = 300;
            break;
           }
//      Serial.println(xx);
        }
        xx -=50;
//        Serial.println(xx);
        EEPROM.put(1,xx);
        for (size_t i = 0; i < 50; i++)
        {
            digitalWrite(flash,digitalRead(flash) ^ 1);
            delay(20);
        }
    }
    EEPROM.get(1,spray);
//    Serial.print("Spray Amount : ");
//    Serial.println(spray);
    
}

void loop ()
{
    dis = distanceSensor.measureDistanceCm();
    delay(100);
    if ((dis > 0) && (dis <= 10)){
            if (stat == false){
                digitalWrite(Valv,HIGH);
                // Serial.println("valve on...");        
                stat = true;
                starttime = millis();
            }
            else if (stat) {
            Duar = (millis() - starttime);
            if (Duar > spray) {
                digitalWrite(Valv,LOW);
                Duar=0;
                //delay(500);
            }
            digitalWrite(flash,digitalRead(flash) ^ 1);
        }
    }
    else if ((dis < 0) or (dis > 10)) {
        digitalWrite(Valv,LOW);
        digitalWrite(flash,HIGH);
        delay(500);
        stat = false;
        Duar=0;
    }
}      
