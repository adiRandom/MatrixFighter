#include "Storage.hpp"
#include <EEPROM.h>
#include <Arduino.h>

void Storage::getP1Name(char buffer[MAX_NAME_LEN]) {
  for (int i = 0; i < MAX_NAME_LEN; i++) {
    buffer[i] = EEPROM.read(P1_NAME_ADDRESS + i);
  }
}

void Storage::getP2Name(char buffer[MAX_NAME_LEN]) {
  for (int i = 0; i < MAX_NAME_LEN; i++) {
    buffer[i] = EEPROM.read(P2_NAME_ADDRESS + i);
  }
}

uint8_t Storage::getMatrixBrightnessLv() {
  return EEPROM.read(MATRIX_BRIGHTNESS_ADDRESS);
}


uint8_t Storage::getLCDBrightnessLv() {
  return EEPROM.read(LCD_BRIGHTNESS_ADDRESS);
}

uint16_t Storage::getMaxHP() {
  uint16_t result;

  byte* p = (byte*)(void*)&result;
  *p++ = EEPROM.read(MAX_HP_ADDRESS);
  *p = EEPROM.read(MAX_HP_ADDRESS + 1);
  return result;
}

uint8_t Storage::getMaxBlocks() {
  return EEPROM.read(MAX_BLOCKS_ADDRESS);
}

uint32_t Storage::getRoundTime() {
  uint32_t result;

  byte* p = (byte*)(void*)&result;
  *p++ = EEPROM.read(ROUND_TIME_ADDRESS);
  *p++ = EEPROM.read(ROUND_TIME_ADDRESS + 1);
  *p++ = EEPROM.read(ROUND_TIME_ADDRESS + 2);
  *p = EEPROM.read(ROUND_TIME_ADDRESS + 3);
  return result;
}

void Storage::init() {
  updateP1NameChar('P', 0);
  updateP1NameChar('0', 1);
  updateP1NameChar('1', 2);
  updateP1NameChar(NULL, 3);

  updateP2NameChar('P', 0);
  updateP2NameChar('0', 1);
  updateP2NameChar('2', 2);
  updateP2NameChar(NULL, 3);

  updateMatrixBrightnessLv(DEFAULT_MATRIX_BRIGHTNESS_LV);
  updateLCDBrightnessLv(DEFAULT_LCD_BRIGHTNESS_LV);
  updateMaxHp(DEFAULT_MAX_HP);
  updateMaxBlocks(DEFAULT_MAX_BLOCKS);
  updateRoundTime(DEFAULT_ROUND_TIME);
}


void Storage::updateLCDBrightnessLv(uint8_t lv) {
  EEPROM.update(LCD_BRIGHTNESS_ADDRESS, lv);
}

void Storage::updateMatrixBrightnessLv(uint8_t lv) {
  EEPROM.update(MATRIX_BRIGHTNESS_ADDRESS, lv);
}

void Storage::updateMaxBlocks(uint8_t maxBlocks) {
  EEPROM.update(MAX_BLOCKS_ADDRESS, maxBlocks);
}

void Storage::updateMaxHp(uint16_t maxHp) {
  byte const* currentByte = (const byte*)(const void*)&maxHp;
  EEPROM.update(MAX_HP_ADDRESS, *currentByte++);
  EEPROM.update(MAX_HP_ADDRESS + 1, *currentByte++);
}

void Storage::updateP1NameChar(char newValue, uint8_t index) {
  EEPROM.update(P1_NAME_ADDRESS + index, newValue);
}

void Storage::updateP2NameChar(char newValue, uint8_t index) {
  EEPROM.update(P2_NAME_ADDRESS + index, newValue);
}

void Storage::updateRoundTime(uint32_t roundTime) {
  byte const* currentByte = (const byte*)(const void*)&roundTime;
  EEPROM.update(ROUND_TIME_ADDRESS, *currentByte++);
  EEPROM.update(ROUND_TIME_ADDRESS + 1, *currentByte++);
  EEPROM.update(ROUND_TIME_ADDRESS + 2, *currentByte++);
  EEPROM.update(ROUND_TIME_ADDRESS + 3, *currentByte++);
}