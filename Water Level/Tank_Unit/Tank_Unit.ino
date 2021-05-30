#include <SoftwareSerial.h>
#include <Wire.h> 

#define SSerialRX        4  //Serial Receive pin
#define SSerialTX        2  //Serial Transmit pin
#define SSerialTxControl 3   //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW

//Set Level Pins
#define Lev_1 9
#define Lev_2 10
#define Lev_3 11
#define Lev_4 12
#define Lev_5 13
#define Lev_6 A0
#define Lev_7 A1
#define Lev_8 A2
#define Lev_9 A3
#define Lev_10 A4
#define DrySen  8

SoftwareSerial RS485(SSerialRX, SSerialTX);
int waterLevel, waterflow;
int t = 0;
int incr = 1;
byte tx[4];
void setup()  
{
  pinMode(Lev_1, INPUT_PULLUP);
  pinMode(Lev_2, INPUT_PULLUP);
  pinMode(Lev_3, INPUT_PULLUP);
  pinMode(Lev_4, INPUT_PULLUP);
  pinMode(Lev_5, INPUT_PULLUP);
  pinMode(Lev_6, INPUT_PULLUP);
  pinMode(Lev_7, INPUT_PULLUP);
  pinMode(Lev_8, INPUT_PULLUP);
  pinMode(Lev_9, INPUT_PULLUP);
  pinMode(Lev_10, INPUT_PULLUP);
  pinMode(DrySen, INPUT_PULLUP);

  Serial.begin(9600);
  while(!Serial);
  RS485.begin(9600);
  pinMode(SSerialTxControl, OUTPUT);
  digitalWrite(SSerialTxControl, RS485Transmit);  // Init Transceiver   
  Serial.println("Start");
  delay(1000);
}
void loop()  
{
    if (digitalRead(Lev_1) == 0) waterLevel = 10;
    if (digitalRead(Lev_2) == 0) waterLevel = 20;
    if (digitalRead(Lev_3) == 0) waterLevel = 30;
    if (digitalRead(Lev_4) == 0) waterLevel = 40;
    if (digitalRead(Lev_5) == 0) waterLevel = 50;
    if (digitalRead(Lev_6) == 0) waterLevel = 60;
    if (digitalRead(Lev_7) == 0) waterLevel = 70;
    if (digitalRead(Lev_8) == 0) waterLevel = 80;
    if (digitalRead(Lev_9) == 0) waterLevel = 90;
    if (digitalRead(Lev_10) == 0) waterLevel = 100;
    if (digitalRead(DrySen) == 0) waterflow = 1; else waterflow = 0;

    delay(100);
    tx[0] = waterLevel;
    tx[1] = waterflow;
    tx[3] = 100;
    Serial.println(waterLevel);
    digitalWrite(SSerialTxControl, RS485Transmit);   
    RS485.write(tx,4); 
    delay(900);
}