#include "Storage.hpp"
#include <EEPROM.h>
#include "DisplayConstants.h"

char const* Storage::getP1Name() {
  char name[MAX_NAME_LEN];
  for (int i = P1_NAME_ADDRESS; i < P2_NAME_ADDRESS; i++) {
    name[i] = EEPROM.read(P1_NAME_ADDRESS + i);
  }

  return name;
}

char const* Storage::getP2Name() {
  char name[MAX_NAME_LEN];
  for (int i = P2_NAME_ADDRESS; i < MATRIX_BRIGHTNESS_ADDRESS; i++) {
    name[i] = EEPROM.read(P1_NAME_ADDRESS + i);
  }

  return name;
}

uint8_t Storage::getMatrixBrightnessLv() {
  return EEPROM.read(MATRIX_BRIGHTNESS_ADDRESS);
}


uint8_t Storage::getLCDBrightnessLv() {
  return EEPROM.read(LCD_BRIGHTNESS_ADDRESS);
}

uint16_t Storage::getMaxHP() {
  uint16_t result;
  EEPROM.get(MAX_HP_ADDRESS, result);
  return result;
}

uint8_t Storage::getMaxBlocks() {
  return EEPROM.read(MAX_BLOCKS_ADDRESS);
}

uint32_t Storage::getRoundTime() {
  uint32_t result;
  EEPROM.get(ROUND_TIME_ADDRESS, result);
  return result;
}

void Storage::init() {
  EEPROM.update(P1_NAME_ADDRESS, 'P');
  EEPROM.update(P1_NAME_ADDRESS + 1, '1');
  EEPROM.update(P1_NAME_ADDRESS + 2, NULL);

  EEPROM.update(P2_NAME_ADDRESS, 'P');
  EEPROM.update(P2_NAME_ADDRESS + 1, '2');
  EEPROM.update(P2_NAME_ADDRESS + 2, NULL);

  EEPROM.update(MATRIX_BRIGHTNESS_ADDRESS, DEFAULT_BRIGHTNESS_LV);
  EEPROM.update(LCD_BRIGHTNESS_ADDRESS, DEFAULT_BRIGHTNESS_LV);
  EEPROM.update(MAX_HP_ADDRESS, DEFAULT_MAX_HP);
  EEPROM.update(MAX_BLOCKS_ADDRESS, DEFAULT_MAX_BLOCKS);
  EEPROM.update(ROUND_TIME_ADDRESS, DEFAULT_ROUND_TIME);
}