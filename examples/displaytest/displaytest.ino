/**
   Little test sketch for testing the
   GS-SB06128TTWU-A/R LCD display
   salvaged from an
   UPS (PowerWalker VI 650 LCD)

   The display is based on the HT1621


   Wire this modul up like this:

   Connector CN2 LCD     Pin Arduino
   1  ???
   2  5V                     5V
   3  CS                     D3
   4  !WR                    D2
   5  Data                   D5
   6  GND                    GND
   7  (nc)
   8  -> CN1
   9  -> CN1
*/

#include "sb06128.h"

#define PIN_CS 3
#define PIN_DATA 5
#define PIN_WR 2

Sb06128 display(PIN_DATA, PIN_WR, PIN_CS);

void setup() {
  display.begin();
  display.test();
  delay(1000);
  display.clear();
  delay(1000);
  display.test();
  delay(1000);
}

void loop1() {
  for (int x = 0; x < 210; x++) {
    display.setNumberR(x - 20);
    display.setNumberL(5 * x - 95);
    delay(150);
  }
}

void loop2() {
  for (int x = 0; x < 9; x++) {
    display.setSymbol(SYMBOL_INPUT, x == 0);
    display.setSymbol(SYMBOL_VAC_LEFT, x == 1);
    display.setSymbol(SYMBOL_VAC_RIGHT, x == 2);

    display.setSymbol(SYMBOL_OUTPUT, x == 3);
    display.setSymbol(SYMBOL_BATTMODE, x == 4);
    display.setSymbol(SYMBOL_ACMODE, x == 5);

    display.setSymbol(SYMBOL_BATT, x == 6);
    display.setSymbol(SYMBOL_BULB, x == 7);
    display.setSymbol(SYMBOL_DIVIDER, x == 8);
    delay(800);
  }
}

void loop3() {
  for (int x = 0; x < 5; x++) {
    display.setBarL(x);
    display.setBarR(x);
    delay(800);
  }
}

void loop() {
  loop1();
  loop2();
  loop3();
}

