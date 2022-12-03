#ifndef LCD_MANAGER_HPP
#define LCD_MANAGER_HPP

#include <LiquidCrystal.h>
#include "Utils.h"
#include "MenuEntry.hpp"
#include "List.cpp"

uint32_t const INTRO_SHOW_TIME = 3 * 1000;
uint16_t const DEFAULT_CONTRAST = 400;
uint8_t const INTRO_MESSAGE_SIZE = 16;
uint8_t const MAIN_MENU_SIZE = 4;

class LCDController {
private:

  uint8_t _rsPin;
  uint8_t _enablePin;
  uint8_t _d4Pin;
  uint8_t _d5Pin;
  uint8_t _d6Pin;
  uint8_t _d7Pin;
  LiquidCrystal _lcd;
  uint32_t _introTimer;
  uint8_t _height;
  uint8_t _width;
  uint8_t _contrastPin;

  List<MenuEntry> _mainMenuEntries;

  MenuEntry* _selectedEntry = nullptr;

  enum State {
    INTRO,
    MENU,
    SETTINGS,
    GAME
  };
  State _state = State::INTRO;

  void displaySelector();
  void initMenues();

public:

  LCDController();
  LCDController(
    uint8_t rsPin,
    uint8_t enablePin,
    uint8_t d4Pin,
    uint8_t d5Pin,
    uint8_t d6Pin,
    uint8_t d7Pin,
    uint8_t height,
    uint8_t width,
    uint8_t contrastPin
  );
  LCDController(LCDController const& other);
  LCDController& operator=(LCDController const& other);

  void displayCurrentState(char const introMessage[INTRO_MESSAGE_SIZE]);
  void selectSettings();
  void closeSubmenu();
  void startGame();
  bool showGame();
  void showAbout();
  void showTutorial();
};

#endif