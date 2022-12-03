#ifndef LCD_MANAGER_HPP
#define LCD_MANAGER_HPP

#include <LiquidCrystal.h>

uint32_t const INTRO_SHOW_TIME = 3 * 1000;
uint16_t const DEFAULT_CONTRAST = 400;

class LCDController {
private:

  uint8_t _rsPin;
  uint8_t _enablePin;
  uint8_t _d4Pin;
  uint8_t _d5Pin;
  uint8_t _d6Pin;
  uint8_t _d7Pin;
  LiquidCrystal _lcd;
  char const* _introMessage;
  uint32_t _introTimer;

  uint8_t _height;
  uint8_t _width;
  uint8_t _contrastPin;

  enum State {
    INTRO,
    MENU,
    SETTINGS,
    GAME
  };
  State _state = State::INTRO;

public:

  LCDController();
  LCDController(
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
  );
  LCDController(LCDController const& other);

  void displayCurrentState();
  void selectSettings();
  void closeSubmenu();
  void startGame();
  bool showGame();
  void showAbout();
  void showTutorial();
};

#endif