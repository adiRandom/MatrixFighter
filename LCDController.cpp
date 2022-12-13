#include "LCDController.hpp"
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "MemoryFree.h"
#include "string.h"

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
    _height{ 0 } {
}

LCDController::LCDController(
  uint8_t rsPin,
  uint8_t enablePin,
  uint8_t d4Pin,
  uint8_t d5Pin,
  uint8_t d6Pin,
  uint8_t d7Pin,
  uint8_t height,
  uint8_t width
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
    _height{ height } {
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
    case State::GAME_OVER:
      {
        // We already set the message when ended the game
        showGameOver("");
        break;
      }
    case State::MENU:
      {
        showMainMenu();
        break;
      }
    case State::ABOUT:
      {
        showAbout(0);
        break;
      }
    case State::GAME:
      {
        showGame();
        break;
      }
    case State::HELP:
      {
        showHelp(0);
        break;
      }
    case State::SETTINGS:
      {
        showSettingsMenu();
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

bool LCDController::isPreGame() const {
  return _state == State::PREGAME;
}

void LCDController::displaySelector() {
  if (_selectedEntry == nullptr || _lastSelectedEntry == _selectedEntry || _selectedEntry->getSelectorPos().getX() == -1) {
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
        return "HELP";
      }
    default:
      {
        return "";
      }
  }
}

char const* LCDController::getHelpMenuEntryName(uint16_t id) const {
  switch (id) {
    case HELP_MOVE_LINE_ID:
      {
        return "Move and crouch";
      }
    case HELP_JOYSTICK_LINE_ID:
      {
        return "With joystick";
      }
    case HELP_ATK_LINE_ID:
      {
        return "Hit - select btn";
      }
    case HELP_DEF_LINE_ID:
      {
        return "Block - back btn";
      }
    default:
      {
        return "";
      }
  }
}

char const* LCDController::getAboutMenuEntryName(uint16_t id) const {
  switch (id) {
    case ABOUT_GAME_NAME_ID:
      {
        return "MATRIX FIGHTER";
      }
    case ABOUT_CREATOR_NAME_ID:
      {
        return "Adrian Pascu";
      }
    case ABOUT_CREATOR_GITHUB_USERNAME_ID:
      {
        return "@adiRandom";
      }
    default:
      {
        return "";
      }
  }
}

void LCDController::initDisplay() {
  _lcd.begin(_width, _height);

  uint8_t heart[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000
  };

  uint8_t shield[8] = {
    0b00000,
    0b00000,
    0b11111,
    0b10001,
    0b10101,
    0b10001,
    0b01010,
    0b00100,
  };

  _lcd.createChar(HEART_SYMBOL, heart);
  _lcd.createChar(SHIELD_SYMBOL, shield);
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
        break;
      }
    case State::ABOUT:
      {
        moveAboutMenuSelector(direction);
        break;
      }
    case State::HELP:
      {
        moveHelpMenuSelector(direction);
        break;
      }
    case State::SETTINGS:
      {
        moveSettingsMenuSelector(direction);
        break;
      }
    default:
      {
        break;
      }
  }
}

void LCDController::showIntro(char const introMessage[INTRO_MESSAGE_SIZE]) {
  if (_introTimer == 0) {
    _lcd.setCursor(0, 0);
    _lcd.print(introMessage);
    _introTimer = millis();
  } else {
    if (millis() - _introTimer > INTRO_SHOW_TIME) {
      _state = State::MENU;
    }
  }
}

void LCDController::showGameOver(char const msg[GAME_OVER_MSG_LENGTH]) {
  if (_gameOverTimer == 0) {
    _lcd.clear();
    _lcd.setCursor(GAME_OVER_MSG_POS_X, GAME_OVER_MSG_POS_Y);
    _lcd.print(msg);
    _gameOverTimer = millis();
  } else {
    if (millis() - _gameOverTimer > GAME_OVER_TIME) {
      _state = State::MENU;
      _selectedEntry = &_mainMenuEntries[MAIN_MENU_PLAY_ID];

      // Reset state from previous run
      _lastSelectedEntry = nullptr;
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
  _selectedEntry = &_mainMenuEntries[MAIN_MENU_PLAY_ID];

  // Reset state from previous run
  _lastSelectedEntry = nullptr;
}

void LCDController::moveMainMenuSelector(Direction direction) {
  if (_selectedEntry == nullptr) {
    return;
  }

  switch (_selectedEntry->getId()) {
    case MAIN_MENU_PLAY_ID:
      {
        if (direction.isRight()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_SETTINGS_ID];
        } else if (direction.isDown()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_ABOUT_ID];
        }
        break;
      }
    case MAIN_MENU_SETTINGS_ID:
      {
        if (direction.isLeft()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_PLAY_ID];
        } else if (direction.isDown()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_HELP_ID];
        }
        break;
      }
    case MAIN_MENU_ABOUT_ID:
      {
        if (direction.isRight()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_HELP_ID];
        } else if (direction.isUp()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_PLAY_ID];
        }
        break;
      }
    case MAIN_MENU_HELP_ID:
      {
        if (direction.isLeft()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_ABOUT_ID];
        } else if (direction.isUp()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_SETTINGS_ID];
        }
        break;
      }
    default:
      {
        break;
      }
  }
}

void LCDController::select() {
  switch (_state) {
    case State::MENU:
      {
        selectInMainMenu();
        break;
      }
    default:
      {
        return;
      }
  }
}

void LCDController::selectInMainMenu() {
  if (_selectedEntry == nullptr) {
    return;
  }

  switch (_selectedEntry->getId()) {
    case MAIN_MENU_ABOUT_ID:
      {
        _state = State::ABOUT;
        break;
      }
    case MAIN_MENU_HELP_ID:
      {
        _state = State::HELP;
        break;
      }
    case MAIN_MENU_PLAY_ID:
      {
        // Set the game state to initial values so we can reset the game
        _roundTimer = DEFAULT_ROUND_TIME + 1;
        _isGameUIInit = false;
        _gameOverTimer = 0;
        _lastSelectedEntry = nullptr;

        _state = State::PREGAME;
        // Set this so we won't update the screen before game starts
        _lastState = State::PREGAME;
        break;
      }
    case MAIN_MENU_SETTINGS_ID:
      {
        _selectedEntry = &_settingsMenuEntries[0];
        _state = State::SETTINGS;
      }
    default:
      {
        return;
      }
  }
}

void LCDController::moveAboutMenuSelector(Direction direction) {
  if (_selectedEntry == nullptr) {
    return;
  }

  switch (_selectedEntry->getId()) {
    case ABOUT_GAME_NAME_ID:
      {
        if (direction.isDown()) {
          showAbout(ABOUT_CREATOR_NAME_ID);
          _selectedEntry = &_aboutEntries[ABOUT_CREATOR_NAME_ID];
        }
        break;
      }
    case ABOUT_CREATOR_NAME_ID:
      {
        if (direction.isDown()) {
          _selectedEntry = &_aboutEntries[ABOUT_CREATOR_GITHUB_USERNAME_ID];
        } else if (direction.isUp()) {
          showAbout(ABOUT_GAME_NAME_ID);
          _selectedEntry = &_aboutEntries[ABOUT_GAME_NAME_ID];
        }
        break;
      }
    case ABOUT_CREATOR_GITHUB_USERNAME_ID:
      {
        if (direction.isUp()) {
          _selectedEntry = &_aboutEntries[ABOUT_CREATOR_NAME_ID];
        }
        break;
      }
    default:
      {
        return "";
      }
  }
}

void LCDController::moveHelpMenuSelector(Direction direction) {
  if (_selectedEntry == nullptr) {
    return;
  }

  switch (_selectedEntry->getId()) {
    case HELP_MOVE_LINE_ID:
      {
        if (direction.isDown()) {
          showHelp(HELP_JOYSTICK_LINE_ID);
          _selectedEntry = &_helpMenuEntries[HELP_JOYSTICK_LINE_ID];
        }
        break;
      }
    case HELP_JOYSTICK_LINE_ID:
      {
        if (direction.isDown()) {
          showHelp(HELP_ATK_LINE_ID);
          _selectedEntry = &_helpMenuEntries[HELP_ATK_LINE_ID];
        } else if (direction.isUp()) {
          showAbout(HELP_MOVE_LINE_ID);
          _selectedEntry = &_helpMenuEntries[HELP_MOVE_LINE_ID];
        }
        break;
      }
    case HELP_ATK_LINE_ID:
      {
        if (direction.isDown()) {
          showHelp(HELP_DEF_LINE_ID);
          _selectedEntry = &_helpMenuEntries[HELP_DEF_LINE_ID];
        } else if (direction.isUp()) {
          showAbout(HELP_JOYSTICK_LINE_ID);
          _selectedEntry = &_helpMenuEntries[HELP_JOYSTICK_LINE_ID];
        }
        break;
      }
    case HELP_DEF_LINE_ID:
      {
        if (direction.isUp()) {
          showHelp(HELP_ATK_LINE_ID);
          _selectedEntry = &_helpMenuEntries[HELP_ATK_LINE_ID];
        }
        break;
      }
    default:
      {
        return "";
      }
  }
}

void LCDController::showAbout(uint16_t topEntryIndex) {
  _lcd.clear();

  for (int i = 0; i < 2; i++) {
    MenuEntry entry = _aboutEntries[topEntryIndex + i];
    _lcd.setCursor(
      entry.getSelectorPos().getX() + 1,
      i
    );
    _lcd.print(getAboutMenuEntryName(entry.getId()));
  }
  _lastState = State::ABOUT;
  _selectedEntry = &_aboutEntries[ABOUT_GAME_NAME_ID];
}

void LCDController::showGame() {
  _lcd.clear();

  // Player 1

  _lcd.setCursor(PLAYER_1_HP_POS, INFO_LINE);
  _lcd.print("P1");

  _lcd.setCursor(PLAYER_1_HEART_POS, STATS_LINE);
  _lcd.write((byte)HEART_SYMBOL);

  _lcd.setCursor(PLAYER_1_SHIELD_POS, STATS_LINE);
  _lcd.write((byte)SHIELD_SYMBOL);


  // Player 2
  _lcd.setCursor(PLAYER_2_NAME_POS, INFO_LINE);
  _lcd.print("P2");

  _lcd.setCursor(PLAYER_2_HEART_POS, STATS_LINE);
  _lcd.write((byte)HEART_SYMBOL);

  _lcd.setCursor(PLAYER_2_SHIELD_POS, STATS_LINE);
  _lcd.write((byte)SHIELD_SYMBOL);


  // Time
  _lcd.setCursor(TIME_POS, INFO_LINE);
  _lcd.print(DEFAULT_ROUND_TIME);

  _lastState = State::GAME;
}

void LCDController::startGame() {
  _state = State::GAME;
}

void LCDController::back() {
  switch (_state) {
    case State::ABOUT:
      {
        _state = State::MENU;
        break;
      }
    case State::HELP:
      {
        _state = State::MENU;
        _lcd.noAutoscroll();
        break;
      }
    case State::SETTINGS:
      {
        _state = State::MENU;
        _selectedEntry = &_mainMenuEntries[0];
        break;
      }
    default:
      {
        break;
      }
  }
}

void LCDController::showHelp(uint8_t topEntryIndex) {
  _lcd.clear();

  for (int i = 0; i < 2; i++) {
    MenuEntry entry = _helpMenuEntries[topEntryIndex + i];
    _lcd.setCursor(
      entry.getSelectorPos().getX() + 1,
      i
    );
    _lcd.print(getHelpMenuEntryName(entry.getId()));
  }
  _lastState = State::HELP;
  _selectedEntry = &_helpMenuEntries[HELP_MOVE_LINE_ID];
}

void LCDController::setPlayer1Hp(uint16_t hp) {
  _lcd.setCursor(PLAYER_1_HP_POS, STATS_LINE);
  _lcd.print("   ");
  _lcd.setCursor(PLAYER_1_HP_POS, STATS_LINE);
  _lcd.print(hp);
}

void LCDController::setPlayer2Hp(uint16_t hp) {
  _lcd.setCursor(PLAYER_2_HP_POS, STATS_LINE);
  _lcd.print("   ");
  _lcd.setCursor(PLAYER_2_HP_POS, STATS_LINE);
  _lcd.print(hp);
}


void LCDController::setPlayer1Blocks(uint16_t blocks) {
  _lcd.setCursor(PLAYER_1_BLOCKS_POS, STATS_LINE);
  _lcd.print("   ");
  _lcd.setCursor(PLAYER_1_BLOCKS_POS, STATS_LINE);
  _lcd.print(blocks);
}

void LCDController::setPlayer2Blocks(uint16_t blocks) {
  _lcd.setCursor(PLAYER_2_BLOCKS_POS, STATS_LINE);
  _lcd.print("   ");
  _lcd.setCursor(PLAYER_2_BLOCKS_POS, STATS_LINE);
  _lcd.print(blocks);
}


bool LCDController::tickRoundTimer() {
  _roundTimer--;

  if (_roundTimer < 0) {
    return false;
  }

  // Clear the previous time first
  _lcd.setCursor(TIME_POS, INFO_LINE);
  _lcd.print("   ");
  _lcd.setCursor(TIME_POS, INFO_LINE);
  _lcd.print(_roundTimer);
  return true;
}

void LCDController::setGameUIInit() {
  _isGameUIInit = true;
}

bool LCDController::isGameUIInit() {
  return _isGameUIInit;
}

void LCDController::gameOver(char const msg[GAME_OVER_MSG_LENGTH]) {
  showGameOver(msg);
  _state = State::GAME_OVER;
}

char const* LCDController::getSettingsMenuEntryName(uint16_t id) const {
  switch (id) {
    case SETTINGS_P1_NAME_ID:
      {
        return "PLAYER 1 NAME";
      }
    case SETTINGS_P2_NAME_ID:
      {
        return "PLAYER 2 NAME";
      }
    case SETTINGS_LCD_BRIGHTNESS:
      {
        return "LCD BRIGHTNESS";
      }
    case SETTINGS_MATRIX_BRIGHTNESS:
      {
        return "MATRIX BRIGHT";
      }
    case SETTINGS_MAX_HP_ID:
      {
        return "MAX HP";
      }
    case SETTINGS_MAX_BLOCKS_ID:
      {
        return "MAX BLOCKS";
      }
    case SETTINGS_ROUND_TIME:
      {
        return "ROUND TIME";
      }
    default:
      {
        return "";
      }
  }
}

void LCDController::showSettingsMenu() {
  _lcd.clear();
  _lcd.setCursor(0, 0);
  _lcd.print(getSettingsMenuEntryName(_selectedEntry->getId()));
  _lcd.setCursor(0, 1);

  switch (_selectedEntry->getId()) {
    case SETTINGS_P1_NAME_ID:
      {
        _lcd.print(_settingsStorage.getP1Name());
        break;
      }
    case SETTINGS_P2_NAME_ID:
      {
        _lcd.print(_settingsStorage.getP2Name());
        break;
      }
    case SETTINGS_LCD_BRIGHTNESS:
      {
        _lcd.print(_settingsStorage.getLCDBrightnessLv());
        break;
      }
    case SETTINGS_MATRIX_BRIGHTNESS:
      {
        _lcd.print(_settingsStorage.getMatrixBrightnessLv());
        break;
      }
    case SETTINGS_MAX_HP_ID:
      {
        _lcd.print(_settingsStorage.getMaxHP());
        break;
      }
    case SETTINGS_MAX_BLOCKS_ID:
      {
        _lcd.print(_settingsStorage.getMaxBlocks());
        break;
      }
    case SETTINGS_ROUND_TIME:
      {
        _lcd.print(_settingsStorage.getRoundTime());
        break;
      }
    default:
      {
        break;
      }
  }

  _lastState = State::SETTINGS;
}


void LCDController::moveSettingsMenuSelector(Direction direction) {

  if (direction.isNeutral()) {
    return;
  } else if (direction.isUp() && _selectedEntry->getId() > 0) {
    _selectedEntry = &_settingsMenuEntries[_selectedEntry->getId() - 1];
  } else if (direction.isDown() && _selectedEntry->getId() < SETTINGS_MENU_SIZE - 1) {
    _selectedEntry = &_settingsMenuEntries[_selectedEntry->getId() + 1];
  }

  showSettingsMenu();
}