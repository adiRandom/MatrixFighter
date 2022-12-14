#ifndef LCD_MANAGER_HPP
#define LCD_MANAGER_HPP

#include <LiquidCrystal.h>
#include "Utils.h"
#include "MenuEntry.hpp"
#include "Direction.hpp"
#include <Arduino.h>
#include "Constants.h"
#include "Storage.hpp"

uint32_t const INTRO_SHOW_TIME = 3 * 1000;
uint16_t const DEFAULT_CONTRAST = 600;

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

uint8_t const SETTINGS_P1_NAME_ID = 0;
uint8_t const SETTINGS_P2_NAME_ID = 1;
uint8_t const SETTINGS_LCD_BRIGHTNESS = 2;
uint8_t const SETTINGS_MATRIX_BRIGHTNESS = 3;
uint8_t const SETTINGS_MAX_HP_ID = 4;
uint8_t const SETTINGS_MAX_BLOCKS_ID = 5;
uint8_t const SETTINGS_ROUND_TIME = 6;
uint8_t const SETTINGS_RESET = 7;
uint8_t const SETTINGS_MENU_SIZE = 8;

uint8_t const LCD_MIN_BRIGHT = 1;
uint8_t const LCD_MAX_BRIGHT = 10;
uint8_t const LCD_BRIGHT_FACTOR = 25;

uint8_t const MATRIX_MIN_BRIGHT = 1;
uint8_t const MATRIX_MAX_BRIGHT = 5;


uint8_t const MIN_HP = 5;
uint8_t const MAX_HP = 100;

uint8_t const MIN_BLOCKS = 3;
uint8_t const MAX_BLOCKS = 100;

uint8_t const MIN_ROUND_TIME = 30;
// 10 min
uint8_t const MAX_ROUND_TIME = 60 * 10;

uint8_t const HEART_SYMBOL = 0;
uint8_t const SHIELD_SYMBOL = 1;

uint8_t const SETTINGS_NAME_LINE = 0;
uint8_t const SETTINGS_VALUE_LINE = 1;

uint8_t const FIRST_ALLOWED_LETTER_ASCII = 65;
uint8_t const LAST_ALLOWED_LETTER_ASCII = 90;
uint8_t const FIRST_ALLOWED_DIGIT_ASCII = 48;
uint8_t const LAST_ALLOWED_DIGIT_ASCII = 57;



uint8_t const MAIN_MENU_ID = 0;
uint8_t const SETTINGS_MENU_ID = 1;
uint8_t const ABOUT_MENU_ID = 2;
uint8_t const HELP_MENU_ID = 3;

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
  // We add one so we actually start with the intended time
  // since a second will tick when we start
  int16_t _roundTimer = DEFAULT_ROUND_TIME + 1;
  uint8_t _brightnessPin;

  MenuEntry _selectedEntry;
  MenuEntry _lastSelectedEntry;
  bool _isSettingEditMode = false;
  // The letter we are editing in player name
  uint8_t _selectedLetterIndex = 0;

  bool _ignoreSelectBtn = false;
  bool _ignoreBackBtn = false;

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

  Storage _settingsStorage;

  void displaySelector();
  char const* getMainMenuEntryName(uint16_t id) const;
  char const* getAboutMenuEntryName(uint16_t id) const;
  char const* getHelpMenuEntryName(uint16_t id) const;
  char const* getSettingsMenuEntryName(uint16_t id) const;

  void initDisplay();

  void showGame();
  void showAbout(uint16_t topEntryIntex);
  void showHelp(uint8_t topEntryIndex);
  void showIntro(char const introMessage[INTRO_MESSAGE_SIZE]);
  void showMainMenu();
  void moveMainMenuSelector(Direction direction);
  void moveAboutMenuSelector(Direction direction);
  void moveHelpMenuSelector(Direction direction);
  void moveSettingsMenuSelector(Direction direction);
  void selectInMainMenu();
  void selectInSettings();
  void moveSettingsMenuSelectorEditMode(Direction directon);
  void showGameOver(char const message[GAME_OVER_MSG_LENGTH]);
  void showSettingsMenu();
  void updateBrightness(uint8_t lv);
  void updatePlayerNameLetter(
    char* currentName,
    Direction direcion,
    uint8_t playerIndex
  );
  void LCDController::onBackInSettings();
  void getMenuEntries(uint8_t menuId, MenuEntry buffer[]);

public:

  LCDController();
  LCDController(
    uint8_t rsPin,
    uint8_t enablePin,
    uint8_t d4Pin,
    uint8_t d5Pin,
    uint8_t d6Pin,
    uint8_t d7Pin,
    uint8_t brightnessPin,
    uint8_t height,
    uint8_t width
  );
  LCDController(LCDController const& other);
  LCDController& operator=(LCDController const& other);

  void displayCurrentState(char const introMessage[INTRO_MESSAGE_SIZE]);

  void moveSelector(Direction direction);

  // Returns true if we need to start the game
  bool onSelectChange(bool isPressed);
  void onBackBtnChange(bool isPressed);
  void startGame();
  bool isPreGame() const;

  void setPlayer1Hp(uint16_t hp);
  void setPlayer2Hp(uint16_t hp);

  void setPlayer1Blocks(uint8_t blocks);
  void setPlayer2Blocks(uint8_t blocks);

  void setGameUIInit();
  bool isGameUIInit();
  void gameOver(char const msg[GAME_OVER_MSG_LENGTH]);
  /**
   * return false when time is up
   */
  bool tickRoundTimer();
};

#endif