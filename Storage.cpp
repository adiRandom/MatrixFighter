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
  EEPROM.update(P1_NAME_ADDRESS, 'P');
  EEPROM.update(P1_NAME_ADDRESS + 1, '1');
  EEPROM.update(P1_NAME_ADDRESS + 2, NULL);

  EEPROM.update(P2_NAME_ADDRESS, 'P');
  EEPROM.update(P2_NAME_ADDRESS + 1, '2');
  EEPROM.update(P2_NAME_ADDRESS + 2, NULL);

  EEPROM.update(MATRIX_BRIGHTNESS_ADDRESS, DEFAULT_BRIGHTNESS_LV);
  EEPROM.update(LCD_BRIGHTNESS_ADDRESS, DEFAULT_BRIGHTNESS_LV);

  byte const* currentByte = (const byte*)(const void*)&DEFAULT_MAX_HP;
  EEPROM.update(MAX_HP_ADDRESS, *currentByte++);
  EEPROM.update(MAX_HP_ADDRESS + 1, *currentByte++);


  EEPROM.update(MAX_BLOCKS_ADDRESS, DEFAULT_MAX_BLOCKS);

  currentByte = (const byte*)(const void*)&DEFAULT_ROUND_TIME;
  EEPROM.update(ROUND_TIME_ADDRESS, *currentByte++);
  EEPROM.update(ROUND_TIME_ADDRESS + 1, *currentByte++);
  EEPROM.update(ROUND_TIME_ADDRESS + 2, *currentByte++);
  EEPROM.update(ROUND_TIME_ADDRESS + 3, *currentByte++);
}