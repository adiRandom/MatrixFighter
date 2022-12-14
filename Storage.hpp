#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <stdint.h>
#include "DisplayConstants.h"


uint16_t const DEFAULT_MAX_HP = 10;
uint32_t const DEFAULT_ROUND_TIME = 200;
uint8_t const DEFAULT_MAX_BLOCKS = 5;
uint8_t const DEFAULT_MATRIX_BRIGHTNESS_LV = 2;
uint8_t const DEFAULT_LCD_BRIGHTNESS_LV = 5;

uint8_t const P1_NAME_ADDRESS = 0;
uint8_t const P2_NAME_ADDRESS = 4;
uint8_t const MATRIX_BRIGHTNESS_ADDRESS = 8;
uint8_t const LCD_BRIGHTNESS_ADDRESS = 9;
uint8_t const MAX_HP_ADDRESS = 10;
uint8_t const MAX_BLOCKS_ADDRESS = 12;
uint8_t const ROUND_TIME_ADDRESS = 13;

class Storage {
public:
  void getP1Name(char buffer[MAX_NAME_LEN]);
  void getP2Name(char buffer[MAX_NAME_LEN]);
  uint8_t getMatrixBrightnessLv();
  uint8_t getLCDBrightnessLv();
  uint16_t getMaxHP();
  uint8_t getMaxBlocks();
  uint32_t getRoundTime();

  void updateP1NameChar(char newValue, uint8_t index);
  void updateP2NameChar(char newValue, uint8_t index);
  void updateMatrixBrightnessLv(uint8_t lv);
  void updateLCDBrightnessLv(uint8_t lv);
  void updateMaxHp(uint16_t maxHp);
  void updateMaxBlocks(uint8_t maxBlocks);
  void updateRoundTime(uint32_t roundTime);

  void init();
};

#endif