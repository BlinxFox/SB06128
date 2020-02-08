#ifndef __SB06128_h
#define __SB06128_h

#include "Arduino.h"

#define SYMBOL_INPUT 0x00
#define SYMBOL_VAC_LEFT 0x10
#define SYMBOL_VAC_RIGHT 0x28
#define SYMBOL_OUTPUT 0x18

#define SYMBOL_BAR_R0 0x30
#define SYMBOL_BAR_R1 0x33
#define SYMBOL_BAR_R2 0x32
#define SYMBOL_BAR_R3 0x31

#define SYMBOL_BATTMODE 0x34
#define SYMBOL_ACMODE 0x3D
#define SYMBOL_DIVIDER 0x37
#define SYMBOL_BULB 0x38
#define SYMBOL_BATT 0x3C

#define SYMBOL_BAR_L0 0x3F
#define SYMBOL_BAR_L1 0x3B
#define SYMBOL_BAR_L2 0x3A
#define SYMBOL_BAR_L3 0x39

class Sb06128 {
  public:
    Sb06128(int data, int wr, int cs);
    void begin();
    void setSymbol(uint8_t symbol, bool enable);
    bool getSymbol(uint8_t symbol);
    void setBarL(uint8_t value);
    void setBarR(uint8_t value);
    void setNumberL(int value);
    void setNumberR(int value);
    void clear();
    void test(uint8_t value = 0xff);
  private:
    int _pin_data;
    int _pin_wr;
    int _pin_cs;
    uint8_t _displayData[8];

    void shiftOutBits(int count, uint16_t value);
    void sendCommand(uint16_t cmd);
    void sendData8(uint8_t address, uint8_t value);
    void setNumber(uint8_t offset, int value);
    void updateDigit(uint8_t addr, uint8_t upperBits);
};

#endif
