#ifndef LCD_MANAGER_HPP
#define LCD_MANAGER_HPP

#include <LiquidCrystal.h>
#include "Utils.h"
#include "MenuEntry.hpp"
#include "Direction.hpp"

uint32_t const INTRO_SHOW_TIME = 3 * 1000;
uint16_t const DEFAULT_CONTRAST = 600;
uint8_t const INTRO_MESSAGE_SIZE = 16;
uint8_t const MAIN_MENU_SIZE = 4;
uint8_t const ABOUT_MENU_SIZE = 3;


uint16_t const MAIN_MENU_PLAY_ID = 0;
uint16_t const MAIN_MENU_SETTINGS_ID = 1;
uint16_t const MAIN_MENU_ABOUT_ID = 2;
uint16_t const MAIN_MENU_HELP_ID = 3;

// char const MAIN_MENU_PLAY_NAME[5] = "PLAY";
// char const MAIN_MENU_SETTINGS_NAME[9] = "SETTINGS";
// char const MAIN_MENU_ABOUT_NAME[2] = "A";
// char const MAIN_MENU_HELP_NAME[5] = "HELP";

uint16_t const ABOUT_GAME_NAME_ID = 0;
uint16_t const ABOUT_CREATOR_NAME_ID = 1;
uint16_t const ABOUT_CREATOR_GITHUB_USERNAME_ID = 2;

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

  MenuEntry _mainMenuEntries[MAIN_MENU_SIZE] = {
    MenuEntry(MAIN_MENU_PLAY_ID, Point{ 0, 0 }),
    MenuEntry(MAIN_MENU_SETTINGS_ID, Point{ 7, 0 }),
    MenuEntry(MAIN_MENU_ABOUT_ID, Point{ 0, 1 }),
    MenuEntry(MAIN_MENU_HELP_ID, Point{ 7, 1 })
  };

  MenuEntry _aboutEntries[ABOUT_MENU_SIZE] = {
    MenuEntry(ABOUT_GAME_NAME_ID, Point{ 0, 0 }),
    MenuEntry(ABOUT_CREATOR_NAME_ID, Point{ 0, 0 }),
    MenuEntry(ABOUT_CREATOR_GITHUB_USERNAME_ID, Point{ 0, 1 })
  };

  MenuEntry* _selectedEntry = nullptr;
  MenuEntry* _lastSelectedEntry = nullptr;

  enum State {
    INTRO,
    MENU,
    SETTINGS,
    GAME,
    ABOUT,
    HELP,
    HIGHSCORE
  };
  State _state = State::INTRO;
  State _lastState = State::INTRO;

  void displaySelector();
  char const* getMainMenuEntryName(uint16_t id) const;
  char const* getAboutMenuEntryName(uint16_t id) const;
  void initDisplay();
  void startGame();
  void showGame();
  void showAbout(uint16_t topEntryIntex);
  void showTutorial();
  void showIntro(char const introMessage[INTRO_MESSAGE_SIZE]);
  void showMainMenu();
  void moveMainMenuSelector(Direction direction);
  void moveAboutMenuSelector(Direction direction);
  void selectInMainMenu();

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

  void moveSelector(Direction direction);
  void select();
  void back();

  bool isShowingGame() const;
};

#endif