#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <stdint.h>

uint16_t const DEFAULT_MAX_HP = 10;
uint32_t const DEFAULT_ROUND_TIME = 200;
uint8_t const DEFAULT_MAX_BLOCKS = 5;
uint8_t const DEFAULT_BRIGHTNESS_LV = 2;

uint8_t const P1_NAME_ADDRESS = 0;
uint8_t const P2_NAME_ADDRESS = 4;
uint8_t const MATRIX_BRIGHTNESS_ADDRESS = 7;
uint8_t const LCD_BRIGHTNESS_ADDRESS = 8;
uint8_t const MAX_HP_ADDRESS = 9;
uint8_t const MAX_BLOCKS_ADDRESS = 11;
uint8_t const ROUND_TIME_ADDRESS = 12;

class Storage {
public:
  char const* getP1Name();
  char const* getP2Name();
  uint8_t getMatrixBrightnessLv();
  uint8_t getLCDBrightnessLv();
  uint16_t getMaxHP();
  uint8_t getMaxBlocks();
  uint32_t getRoundTime();
  void init();
};

#endif