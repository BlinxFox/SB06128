
#include "sb06128.h"

#define ADDR_BAR_L 0x07
#define ADDR_BAR_R 0x06

//  AA
// F  B
// F  B
//  GG
// E  C
// E  C
//  DD
//
//  AFEDBGCx
static const uint8_t digits[10] = {
  0b11111010, // 0
  0b00001010, // 1
  0b10111100, // 2
  0b10011110, // 3
  0b01001110, // 4
  0b11010110, // 5
  0b11110110, // 6
  0b10001010, // 7
  0b11111110, // 8
  0b11011110, // 9
};

#define DIGIT_MINUS 0b00000100
#define DIGIT_SPACE 0b00000000

Sb06128::Sb06128(int data, int wr, int cs):
  _pin_data(data), _pin_wr(wr), _pin_cs(cs) {}

void Sb06128::begin() {
  digitalWrite(_pin_cs, HIGH);
  digitalWrite(_pin_data, HIGH);
  digitalWrite(_pin_wr, HIGH);

  pinMode(_pin_cs, OUTPUT);
  pinMode(_pin_data, OUTPUT);
  pinMode(_pin_wr, OUTPUT);

  sendCommand(0b001010010);
  sendCommand(0b000110000);
  sendCommand(0b000001010);
  sendCommand(0b000001000);
  sendCommand(0b000000010);
  sendCommand(0b000000110);
}

void Sb06128::shiftOutBits(int count, uint16_t value) {
  for (int x = count; x > 0; x--) {
    bool val = (bit(x - 1) & value) != 0;
    digitalWrite(_pin_wr, LOW);
    digitalWrite(_pin_data, val);
    digitalWrite(_pin_wr, HIGH);
  }
}

void Sb06128::sendCommand(uint16_t cmd) {
  digitalWrite(_pin_cs, LOW);
  shiftOutBits(3, 0b100);
  shiftOutBits(9, cmd);
  digitalWrite(_pin_cs, HIGH);
}

void Sb06128::sendData8(uint8_t address, uint8_t value) {
  if (address >= sizeof(_displayData) / sizeof(_displayData[0])) {
    return;
  }
  _displayData[address] = value;
  digitalWrite(_pin_cs, LOW);
  shiftOutBits(3, 0b101);
  shiftOutBits(6, address * 2);
  shiftOutBits(8, value);
  digitalWrite(_pin_cs, HIGH);
}

void Sb06128::setSymbol(uint8_t symbol, bool enable) {
  uint8_t addr = symbol >> 3;
  uint8_t bitnum = symbol & 0x07;
  uint8_t mask = ~bit(bitnum);
  uint8_t tmp = _displayData[addr];
  tmp = tmp & mask;
  if (enable) {
    tmp = tmp | bit(bitnum);
  }
  sendData8(addr, tmp);
}

bool Sb06128::getSymbol(uint8_t symbol) {
  uint8_t addr = symbol >> 3;
  uint8_t bitnum = symbol & 0x07;
  uint8_t tmp = _displayData[addr];
  return (tmp & bit(bitnum)) != 0;
}

void Sb06128::setBarL(uint8_t value) {
  if (value > 4) {
    return;
  }
  uint8_t bits[] = {0, 0b10000000, 0b10001000, 0b10001100, 0b10001110};
  uint8_t tmp = _displayData[ADDR_BAR_L];
  tmp = tmp & 0b01110001;
  tmp = tmp | bits[value];
  sendData8(ADDR_BAR_L, tmp);
}

void Sb06128::setBarR(uint8_t value) {
  if (value > 4) {
    return;
  }
  uint8_t bits[] = {0, 0b00000001, 0b00001001, 0b00001101, 0b00001111};
  uint8_t tmp = _displayData[ADDR_BAR_R];
  tmp = tmp & 0b11110000;
  tmp = tmp | bits[value];
  sendData8(ADDR_BAR_R, tmp);
}

void Sb06128::setNumberL(int value) {
  setNumber(0, value);
}
void Sb06128::setNumberR(int value) {
  setNumber(3, value);
}
void Sb06128::setNumber(uint8_t offset, int value) {
  if (value < -99 || value > 999) {
    return;
  }
  bool isNeg = value < 0;
  if (isNeg) {
    value = -value;
  }
  int x;
  for (x = 0; x < 3; x++) {
    uint8_t addr = offset + 2 - x;
    updateDigit(addr, digits[value % 10]);
    value = value / 10;
    if (value == 0) {
      break;
    }
  }
  if (isNeg) {
    x++;
    uint8_t addr = offset + 2 - x;
    updateDigit(addr, DIGIT_MINUS);
  }
  for (int y = x + 1; y < 3; y++) {
    uint8_t addr = offset + 2 - y;
    updateDigit(addr, DIGIT_SPACE);
  }
}

void Sb06128::updateDigit(uint8_t addr, uint8_t upperBits) {
  uint8_t tmp = _displayData[addr];
  tmp = tmp & 0x01;
  tmp = tmp | upperBits;
  sendData8(addr, tmp);
}

void Sb06128::clear() {
  test(0x00);
}

void Sb06128::test(uint8_t value) {
  for (uint8_t x = 0; x < sizeof(_displayData) / sizeof(_displayData[0]); x++) {
    sendData8(x, value);
  }
}

