#include <Arduino.h>
#include "LcdKeypad.h"
#include "MenuManager.h"
#include "MenuData.h"
#include "Config.h"
#include <Wire.h>
#include <RTClib.h>
#include "SoftwareSerial.h"
#include <LiquidCrystal_I2C.h>
#include "DFRobotDFPlayerMini.h"
#include <SPI.h>
void setup() {
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(2, 1);
  delay(1000);
  digitalWrite(2, 0);
  delay(1000);
}