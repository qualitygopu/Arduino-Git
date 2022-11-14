/*
 * 7-segment display with 74HC595 shift register
 * 4-Digit counter example.
 * Common anode 7-segment display is used.
 * This is a free software with NO WARRANTY.
 * https://simple-circuit.com/
 */
#include <TimerOne.h>
// counter button definition
const int dataPin  = 8;  // 74HC595 pin 8 DS
const int latchPin = 9;  // 74HC595 pin 9 STCP
const int clockPin = 10;   // 74HC595 pin 10 SHCP
const int digit0   = 2;   // 7-Segment pin D4
const int digit1   = 3;   // 7-Segment pin D3
const int digit2   = 4;   // 7-Segment pin D2
const int digit3   = 5;   // 7-Segment pin D1 

byte table[]= 
    {   0xA0,  // = 0
        0xBB,  // = 1
        0x62,  // = 2
        0x2A,  // = 3
        0x39,  // = 4
        0x2C,  // = 5
        0x24,  // = 6
        0xBA,  // = 7
        0x20,  // = 8
        0x38,  // = 9
        0x30,  // = A
        0x25,  // = b
        0xE4,  // = C
        0x23,  // = d
        0x64,  // = E
        0x74,  // = F
        0xFF  // blank
    };  //Hex shown
byte digitDP = 32;  // 0x20 - adds this to digit to show decimal point
byte controlDigits[] = { digit0, digit1, digit2, digit3 };  // pins to turn off & on digits
byte displayDigits[] = { 0,0,0,0,0 }; // ie: { 1, 0, 7, 13, 0} == d701 (all values from table array)

int digitDelay = 50;                  // delay between incrementing digits (ms)
int brightness = 90;                  // valid range of 0-100, 100=brightest

void setup()
{
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  for (int x=0; x<4; x++){
      pinMode(controlDigits[x],OUTPUT);
      digitalWrite(controlDigits[x],LOW);  // Turns off the digit  
  }

Timer1.attachInterrupt(DisplaySegments);
    Timer1.start();
 // enable all interrupts
}

void DisplaySegments(){
    for (int x=0; x<4; x++){
        for (int j=0; j<4; j++){
            digitalWrite(controlDigits[j],LOW);    // turn off digits
        }
        digitalWrite(latchPin,LOW);
        if (bitRead(displayDigits[4],x)==1){
            // raw byte value is sent to shift register
            shiftOut(dataPin,clockPin,MSBFIRST,displayDigits[x]);
        } else {
            // table array value is sent to the shift register
            shiftOut(dataPin,clockPin,MSBFIRST,table[displayDigits[x]]);
        }
        
        digitalWrite(latchPin,HIGH);
        digitalWrite(controlDigits[x],HIGH);   // turn on one digit
        delay(1);                              // 1 or 2 is ok
    }
    for (int j=0; j<4; j++){
        digitalWrite(controlDigits[j],LOW);    // turn off digits
    }
}


// main loop
void loop()
{
  
    // DisplaySegments();    
    displayDigits[0] = B10111011;
    displayDigits[1] = B01100010;
    displayDigits[2] = B00101010;
    displayDigits[3] = B00111001;
    displayDigits[4] = B1111;
}


// end of code.