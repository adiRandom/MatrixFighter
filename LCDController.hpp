#ifndef LCD_MANAGER_HPP
#define LCD_MANAGER_HPP

#include <LiquidCrystal.h>
#include "Utils.h"
#include "MenuEntry.hpp"
#include "Direction.hpp"
#include <Arduino.h>
#include "DisplayConstants.h"

uint32_t const INTRO_SHOW_TIME = 3 * 1000;
uint16_t const DEFAULT_CONTRAST = 600;

uint16_t const DEFAULT_ROUND_TIME = 10;
uint32_t const GAME_OVER_TIME = 3 * 1000;


uint8_t const PLAYER_1_HP_POS = 0;
uint8_t const PLAYER_1_HEART_POS = 3;
uint8_t const PLAYER_1_BLOCKS_POS = 4;
uint8_t const PLAYER_1_SHIELD_POS = 6;
uint8_t const PLAYER_1_NAME_POS = 0;
uint8_t const PLAYER_2_HP_POS = 9;
uint8_t const PLAYER_2_HEART_POS = 12;
uint8_t const PLAYER_2_BLOCKS_POS = 13;
uint8_t const PLAYER_2_SHIELD_POS = 15;
uint8_t const PLAYER_2_NAME_POS = 13;
uint8_t const TIME_POS = 6;
uint8_t const INFO_LINE = 0;
uint8_t const STATS_LINE = 1;

uint8_t const GAME_OVER_MSG_LENGTH = 8;
uint8_t const GAME_OVER_MSG_POS_X = 4;
uint8_t const GAME_OVER_MSG_POS_Y = 0;

uint8_t const INTRO_MESSAGE_SIZE = 16;
uint8_t const MAIN_MENU_SIZE = 4;
uint8_t const HELP_MENU_SIZE = 4;
uint8_t const ABOUT_MENU_SIZE = 3;


uint8_t const MAIN_MENU_PLAY_ID = 0;
uint8_t const MAIN_MENU_SETTINGS_ID = 1;
uint8_t const MAIN_MENU_ABOUT_ID = 2;
uint8_t const MAIN_MENU_HELP_ID = 3;

uint8_t const ABOUT_GAME_NAME_ID = 0;
uint8_t const ABOUT_CREATOR_NAME_ID = 1;
uint8_t const ABOUT_CREATOR_GITHUB_USERNAME_ID = 2;

uint8_t const HELP_MOVE_LINE_ID = 0;
uint8_t const HELP_JOYSTICK_LINE_ID = 1;
uint8_t const HELP_ATK_LINE_ID = 2;
uint8_t const HELP_DEF_LINE_ID = 3;

uint8_t const HEART_SYMBOL = 0;
uint8_t const SHIELD_SYMBOL = 1;


class LCDController {
private:

  uint8_t _rsPin;
  uint8_t _enablePin;
  uint8_t _d4Pin;
  uint8_t _d5Pin;
  uint8_t _d6Pin;
  uint8_t _d7Pin;
  LiquidCrystal _lcd;
  uint32_t _introTimer = 0;
  uint8_t _height;
  uint8_t _width;
  uint8_t _contrastPin;
  // We add one so we actually start with the intended time
  // since a second will tick when we start
  int16_t _roundTimer = DEFAULT_ROUND_TIME + 1;

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

  MenuEntry _helpMenuEntries[HELP_MENU_SIZE] = {
    MenuEntry(HELP_MOVE_LINE_ID, Point{ -1, -1 }),
    MenuEntry(HELP_JOYSTICK_LINE_ID, Point{ -1, -1 }),
    MenuEntry(HELP_ATK_LINE_ID, Point{ -1, -1 }),
    MenuEntry(HELP_DEF_LINE_ID, Point{ -1, -1 })
  };

  MenuEntry* _selectedEntry = nullptr;
  MenuEntry* _lastSelectedEntry = nullptr;

  enum State {
    INTRO,
    MENU,
    SETTINGS,
    // Intermediary state before select button is released
    PREGAME,
    GAME,
    ABOUT,
    HELP,
    HIGHSCORE,
    GAME_OVER
  };

  State _state = State::INTRO;
  State _lastState = State::INTRO;
  bool _isGameUIInit = false;
  uint32_t _gameOverTimer = 0;

  void displaySelector();
  char const* getMainMenuEntryName(uint16_t id) const;
  char const* getAboutMenuEntryName(uint16_t id) const;
  char const* getHelpMenuEntryName(uint16_t id) const;
  void initDisplay();

  void showGame();
  void showAbout(uint16_t topEntryIntex);
  void showHelp(uint8_t topEntryIndex);
  void showIntro(char const introMessage[INTRO_MESSAGE_SIZE]);
  void showMainMenu();
  void moveMainMenuSelector(Direction direction);
  void moveAboutMenuSelector(Direction direction);
  void moveHelpMenuSelector(Direction direction);
  void selectInMainMenu();
  void showGameOver(char const message[GAME_OVER_MSG_LENGTH]);

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
  void startGame();
  bool isPreGame() const;

  void setPlayer1Hp(uint16_t hp);
  void setPlayer2Hp(uint16_t hp);

  void setPlayer1Blocks(uint16_t blocks);
  void setPlayer2Blocks(uint16_t blocks);

  void setGameUIInit();
  bool isGameUIInit();
  void gameOver(char const msg[GAME_OVER_MSG_LENGTH]);
  /**
   * return false when time is up
   */
  bool tickRoundTimer();
};


#endif