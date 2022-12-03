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
    _introMessage{ "" },
    _lcd{ LiquidCrystal(_rsPin, _enablePin, _d4Pin, _d5Pin, _d6Pin, _d7Pin) },
    _introTimer{ 0 },
    _width{ 0 },
    _height{ 0 },
    _contrastPin{ 0 } {
}


LCDController::LCDController(
  uint8_t rsPin,
  uint8_t enablePin,
  uint8_t d4Pin,
  uint8_t d5Pin,
  uint8_t d6Pin,
  uint8_t d7Pin,
  char const* introMessage,
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
    _introMessage{ introMessage },
    _lcd{ LiquidCrystal(_rsPin, _enablePin, _d4Pin, _d5Pin, _d6Pin, _d7Pin) },
    _introTimer{ 0 },
    _width{ width },
    _height{ height },
    _contrastPin{ contrastPin } {

  _lcd.begin(_width, _height);
  analogWrite(_contrastPin, DEFAULT_CONTRAST);
}

LCDController::LCDController(LCDController const& other)
  : _rsPin{ other._rsPin },
    _enablePin{ other._enablePin },
    _d4Pin{ other._d4Pin },
    _d5Pin{ other._d5Pin },
    _d6Pin{ other._d6Pin },
    _d7Pin{ other._d7Pin },
    _introMessage{ other._introMessage },
    _lcd{ LiquidCrystal(_rsPin, _enablePin, _d4Pin, _d5Pin, _d6Pin, _d7Pin) },
    _introTimer{ 0 },
    _width{ other._width },
    _height{ other._height },
    _contrastPin{ other._contrastPin } {

  Serial.print(other._rsPin);
  _lcd.begin(_width, _height);
  analogWrite(_contrastPin, DEFAULT_CONTRAST);
}

void LCDController::displayCurrentState() {
  switch (_state) {
    case State::INTRO:
      {
        if (_introTimer == 0) {
          Serial.println(_rsPin);
          _lcd.print("HEllo");
          _introTimer = millis();
        } else {
          if (millis() - _introTimer > INTRO_SHOW_TIME) {
            _state = State::MENU;
          }
        }
        break;
      }
    case State::MENU:
      {
        _lcd.print(":D");
        break;
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