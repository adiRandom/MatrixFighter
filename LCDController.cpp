#include "LCDController.hpp"
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "MemoryFree.h"

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
  initDisplay();
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

  initDisplay();
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

  initDisplay();

  return *this;
}

void LCDController::displayCurrentState(char const introMessage[INTRO_MESSAGE_SIZE]) {
  displaySelector();

  if (_state == _lastState && _state != State::INTRO) {
    return;
  }

  switch (_state) {
    case State::INTRO:
      {
        showIntro(introMessage);
        break;
      }
    case State::MENU:
      {
        showMainMenu();
        break;
      }
    default:
      {
        break;
      }
  }

  // The screen was cleared due to the change of state
  // Redisplay the selector
  displaySelector();
}

bool LCDController::isShowingGame() const {
  return _state == State::GAME;
}

void LCDController::displaySelector() {
  if (_selectedEntry == nullptr || _lastSelectedEntry == _selectedEntry) {
    return;
  }

  _lcd.setCursor(
    _lastSelectedEntry->getSelectorPos().getX(),
    _lastSelectedEntry->getSelectorPos().getY()
  );

  _lcd.print(" ");

  _lcd.setCursor(
    _selectedEntry->getSelectorPos().getX(),
    _selectedEntry->getSelectorPos().getY()
  );
  _lcd.print(">");

  _lastSelectedEntry = _selectedEntry;
}

char const* LCDController::getMainMenuEntryName(uint16_t id) const {
  // Serial.println(freeMemory());
  switch (id) {
    case MAIN_MENU_PLAY_ID:
      {
        return "PLAY";
      }
    case MAIN_MENU_SETTINGS_ID:
      {
        return "SETTINGS";
      }
    case MAIN_MENU_ABOUT_ID:
      {
        return "ABOUT";
      }
    case MAIN_MENU_HELP_ID:
      {
        // TODO: Fix this
        // return "HELP";
      }
    default:
      {
        return "";
      }
  }
}

void LCDController::initDisplay() {
  pinMode(_contrastPin, OUTPUT);
  _lcd.begin(_width, _height);
  analogWrite(_contrastPin, DEFAULT_CONTRAST);
}

void LCDController::moveSelector(Direction direction) {
  switch (_state) {
    case State::INTRO:
      {
        return;
      }
    case State::MENU:
      {
        moveMainMenuSelector(direction);
      }
    default:
      {
        break;
      }
  }
}

void LCDController::showIntro(char const introMessage[INTRO_MESSAGE_SIZE]) {
  if (_introTimer == 0) {
    _lcd.print(introMessage);
    _introTimer = millis();
  } else {
    if (millis() - _introTimer > INTRO_SHOW_TIME) {
      _state = State::MENU;
    }
  }
}

void LCDController::showMainMenu() {
  _lcd.clear();

  for (int i = 0; i < MAIN_MENU_SIZE; i++) {
    MenuEntry entry = _mainMenuEntries[i];
    _lcd.setCursor(entry.getSelectorPos().getX() + 1, entry.getSelectorPos().getY());
    _lcd.print(getMainMenuEntryName(entry.getId()));
  }
  _lastState = State::MENU;
  _selectedEntry = &_mainMenuEntries[MAIN_MENU_PLAY_INDEX];
}

void LCDController::moveMainMenuSelector(Direction direction) {
  if (_selectedEntry == nullptr) {
    return;
  }

  switch (_selectedEntry->getId()) {
    case MAIN_MENU_PLAY_ID:
      {
        if (direction.isRight()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_SETTINGS_INDEX];
        } else if (direction.isDown()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_ABOUT_INDEX];
        }
        break;
      }
    case MAIN_MENU_SETTINGS_ID:
      {
        if (direction.isLeft()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_PLAY_INDEX];
        } else if (direction.isDown()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_HELP_INDEX];
        }
        break;
      }
    case MAIN_MENU_ABOUT_ID:
      {
        if (direction.isRight()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_HELP_INDEX];
        } else if (direction.isUp()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_PLAY_INDEX];
        }
        break;
      }
    case MAIN_MENU_HELP_ID:
      {
        if (direction.isLeft()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_ABOUT_INDEX];
        } else if (direction.isUp()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_SETTINGS_INDEX];
        }
        break;
      }
    default:
      {
        break;
      }
  }
}