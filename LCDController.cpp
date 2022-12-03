#include "LCDController.hpp"
#include <Arduino.h>
#include <LiquidCrystal.h>

LCDController::LCDController()
  : _rsPin{ 0 },
    _enablePin{ 0 },
    _d4Pin{ 0 },
    _d5Pin{ 0 },
    _d6Pin{ 0 },
    _d7Pin{ 0 },
    _lcd{ LiquidCrystal(_rsPin, _enablePin, _d4Pin, _d5Pin, _d6Pin, _d7Pin) },
    _introTimer{ 0 },
    _width{ 0 },
    _height{ 0 },
    _contrastPin{ 0 } {
  initMenues();
}

LCDController::LCDController(
  uint8_t rsPin,
  uint8_t enablePin,
  uint8_t d4Pin,
  uint8_t d5Pin,
  uint8_t d6Pin,
  uint8_t d7Pin,
  uint8_t height,
  uint8_t width,
  uint8_t contrastPin
)
  : _rsPin{ rsPin },
    _enablePin{ enablePin },
    _d4Pin{ d4Pin },
    _d5Pin{ d5Pin },
    _d6Pin{ d6Pin },
    _d7Pin{ d7Pin },
    _lcd{ LiquidCrystal(_rsPin, _enablePin, _d4Pin, _d5Pin, _d6Pin, _d7Pin) },
    _introTimer{ 0 },
    _width{ width },
    _height{ height },
    _contrastPin{ contrastPin } {

  _lcd.begin(_width, _height);
  analogWrite(_contrastPin, DEFAULT_CONTRAST);

  initMenues();
}

LCDController::LCDController(LCDController const& other)
  : _rsPin{ other._rsPin },
    _enablePin{ other._enablePin },
    _d4Pin{ other._d4Pin },
    _d5Pin{ other._d5Pin },
    _d6Pin{ other._d6Pin },
    _d7Pin{ other._d7Pin },
    _lcd{ LiquidCrystal(_rsPin, _enablePin, _d4Pin, _d5Pin, _d6Pin, _d7Pin) },
    _introTimer{ 0 },
    _width{ other._width },
    _height{ other._height },
    _contrastPin{ other._contrastPin },
    _selectedEntry{ other._selectedEntry } {

  _lcd.begin(_width, _height);
  analogWrite(_contrastPin, DEFAULT_CONTRAST);

  initMenues();
}

LCDController& LCDController::operator=(LCDController const& other) {
  if (&other == this) {
    return *this;
  }
  _rsPin = other._rsPin;
  _enablePin = other._enablePin;
  _d4Pin = other._d4Pin;
  _d5Pin = other._d5Pin;
  _d6Pin = other._d6Pin;
  _d7Pin = other._d7Pin;
  _lcd = LiquidCrystal(_rsPin, _enablePin, _d4Pin, _d5Pin, _d6Pin, _d7Pin);
  _introTimer = 0;
  _width = other._width;
  _height = other._height;
  _contrastPin = other._contrastPin;
  _selectedEntry = other._selectedEntry;

  initMenues();

  return *this;
}

void LCDController::displayCurrentState(char const introMessage[INTRO_MESSAGE_SIZE]) {
  switch (_state) {
    case State::INTRO:
      {
        if (_introTimer == 0) {
          Serial.print(introMessage);
          _lcd.print(introMessage);
          _introTimer = millis();
        } else {
          if (millis() - _introTimer > INTRO_SHOW_TIME) {
            // _state = State::MENU;
            // _selectedEntry = &_mainMenuEntries[0];
          }
        }
        break;
      }
    case State::MENU:
      {
        // displaySelector();

        // for (int i = 0; i < MAIN_MENU_SIZE; i++) {
        //   MenuEntry entry = _mainMenuEntries[i];
        //   _lcd.setCursor(entry.getSelectorPos().getX() + 1, entry.getSelectorPos().getY());
        //   _lcd.print(entry.getName());
        // }

        // break;
      }
    default:
      {
        break;
      }
  }
}

bool LCDController::showGame() {
  return _state == State::GAME;
}

void LCDController::displaySelector() {
  if (_selectedEntry == nullptr) {
    return;
  }

  _lcd.setCursor(
    _selectedEntry->getSelectorPos().getX(),
    _selectedEntry->getSelectorPos().getY()
  );
  _lcd.print(">");
}

void LCDController::initMenues() {
  // TODO: Extract consts
  _mainMenuEntries.push(MenuEntry("Play", Point{ 0, 0 }));
  _mainMenuEntries.push(MenuEntry("Settings", Point{ 6, 0 }));
  // _mainMenuEntries.push(MenuEntry("About", Point{ 0, 1 }));
  // _mainMenuEntries.push(MenuEntry("Help", Point{ 7, 1 }));
}