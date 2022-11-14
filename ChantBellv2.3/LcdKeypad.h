#ifndef LCDKEYPAD_H_
#define LCDKEYPAD_H_

#define LCD_KEYPAD_1_0

#include "Arduino.h"
#include <stdarg.h>

#define LCD_ROWS  2
#define LCD_COLS  16

//Button Pins
#define SELECT_BUT  5
#define UP_BUT      6
#define DOWN_BUT    7
#define BACK_BUT    8

// button state indicators
#define BUTTON_PRESSED_IND        (0 << 6)
#define BUTTON_SHORT_RELEASE_IND  (1 << 6)
#define BUTTON_LONG_PRESSED_IND   (2 << 6)
#define BUTTON_LONG_RELEASE_IND   (3 << 6)

// Button values must be assigned in ascending order as the analog values.
// The analog values must be declared in ascending order in buttonValues[] in LcdKeypad.cpp.

#define BUTTON_SELECT_PRESSED        (1 |  BUTTON_PRESSED_IND) //1
#define BUTTON_SELECT_SHORT_RELEASE  (1 |  BUTTON_SHORT_RELEASE_IND) //65
#define BUTTON_SELECT_LONG_PRESSED   (1 |  BUTTON_LONG_PRESSED_IND) //128
#define BUTTON_SELECT_LONG_RELEASE   (1 |  BUTTON_LONG_RELEASE_IND) //193

#define BUTTON_UP_PRESSED         (2 |  BUTTON_PRESSED_IND)
#define BUTTON_UP_SHORT_RELEASE   (2 |  BUTTON_SHORT_RELEASE_IND)
#define BUTTON_UP_LONG_PRESSED    (2 |  BUTTON_LONG_PRESSED_IND)
#define BUTTON_UP_LONG_RELEASE    (2 |  BUTTON_LONG_RELEASE_IND)

#define BUTTON_DOWN_PRESSED         (3 |  BUTTON_PRESSED_IND)
#define BUTTON_DOWN_SHORT_RELEASE   (3 |  BUTTON_SHORT_RELEASE_IND)
#define BUTTON_DOWN_LONG_PRESSED    (3 |  BUTTON_LONG_PRESSED_IND)
#define BUTTON_DOWN_LONG_RELEASE    (3 |  BUTTON_LONG_RELEASE_IND)

#define BUTTON_LEFT_PRESSED         (4 |  BUTTON_PRESSED_IND)
#define BUTTON_LEFT_SHORT_RELEASE   (4 |  BUTTON_SHORT_RELEASE_IND)
#define BUTTON_LEFT_LONG_PRESSED    (4 |  BUTTON_LONG_PRESSED_IND)
#define BUTTON_LEFT_LONG_RELEASE    (4 |  BUTTON_LONG_RELEASE_IND)

#define BUTTON_RIGHT_PRESSED         (5 |  BUTTON_PRESSED_IND)
#define BUTTON_RIGHT_SHORT_RELEASE   (5 |  BUTTON_SHORT_RELEASE_IND)
#define BUTTON_RIGHT_LONG_PRESSED    (5 |  BUTTON_LONG_PRESSED_IND)
#define BUTTON_RIGHT_LONG_RELEASE    (5 |  BUTTON_LONG_RELEASE_IND)

extern void queueButton (byte button);
extern byte getButton ();

extern void buttonHandlerCycle();   // Can call in ISR, but will need to modify body of queueButton() method (see method body).

// Convert integer to string.
extern char *inttostr(char *dest, short integer);
// Apply left padding to string.
extern char *lpad (char *dest, const char *str, char chr = ' ', unsigned char width = LCD_COLS);
// Apply right padding to string.
extern char *rpad (char *dest, const char *str, char chr = ' ', unsigned char width = LCD_COLS);
// Apply string concatenation. argc = number of string arguments to follow.
extern char *fmt (char *dest, unsigned char argc, ... );



#endif
