#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include "DisplayController.hpp"
#include "Character.hpp"
#include "InputController.hpp"
#include "LCDController.hpp"
#include "Utils.h"
#include "Collider.hpp"
#include "Constants.h"


class GameManager {
private:
  DisplayController _displayController;
  LCDController _lcdController;

  Character _player1;
  InputController _player1InputController;

  bool _changed = true;
  bool _isPlayingGame = false;

  uint32_t _player1BlockRechargeTimer = 0;
  uint32_t _player2BlockRechargeTimer = 0;


  Pixel _playerCharacterBuffer[CHARACTER_MODEL_BUFFER_SIZE];

  Collider _screenWalls{ BoundingBox{
    Point{ -1, -1 },
    Point{ DISPLAY_WIDTH, DISPLAY_HEIGHT } } };

  Character _player2;
  InputController _player2InputController;

  int32_t _lastRoundTimerTick = 0;

  bool handlePlayerJoyInput(Character& player, Direction direction);
  void handleMenuJoyInput(Direction direciton);
  void handleMenuBtnInput(bool isPrimaryPressed, bool isSecondaryPressed);
  bool handlePlayerBtnInput(Character& player, bool isPrimaryPressed, bool isSecondaryPressed);
  bool canPlayerMove(Character& player, Direction direction);
  void updateMovementRestrictions(Character& player);

  /**
   * Return if frame should be updated
   */
  bool handlePlayerInput(Character& player, Direction direciton, bool isPrimaryPressed, bool isSecondaryPressed);

  void initGameUi();
  void updatePlayerHP();
  void updatePlayerBlocks();
  void hitPlayer(Character& player, Character& otherPlayer);
  void gameOver(char const name[MAX_NAME_LEN]);

  Storage _settingsStorage;
public:
  GameManager();
  GameManager(
    DisplayController& displayController,
    Character& player1,
    InputController& player1InputController,
    Character& player2,
    InputController& player2InputController,
    LCDController& lcdController
  );
  GameManager& operator=(GameManager const& other);

  void getNextFrame();
  void handleInput();
  void getLCDState(char const introMessage[]);
  void runRoundTimer();
  void runBlockRechargeTimer();
};

#endif