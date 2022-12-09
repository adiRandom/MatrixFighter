#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include "DisplayController.hpp"
#include "Character.hpp"
#include "InputController.hpp"
#include "LCDController.hpp"
#include "Utils.h"
#include "Collider.hpp"
#include "DisplayConstants.h"
#include "SlaveInputController.hpp"

class GameManager {
private:
  DisplayController _displayController;
  LCDController _lcdController;

  Character _player1;
  InputController _player1InputController;

  Character _player2;
  SlaveInputController _player2InputController;

  bool _changed = true;
  bool _isPlayingGame = false;

  Pixel _playerCharacterBuffer[CHARACTER_MODEL_BUFFER_SIZE];

  Collider _screenWalls{ BoundingBox{
    Point{ -1, -1 },
    Point{ DISPLAY_WIDTH, DISPLAY_HEIGHT } } };

  bool handlePlayerJoyInput(Character& player, Direction direction);
  void handleMenuJoyInput(Direction direciton);
  void handleMenuBtnInput(bool isPrimaryPressed, bool isSecondaryPressed);
  bool handlePlayerBtnInput(Character& player, bool isPrimaryPressed, bool isSecondaryPressed);
  bool canPlayerMove(Character& player, Direction direction);
  void updateMovementRestrictions(Character& player, Direction lastDirection);

  /**
   * Return if frame should be updated
   */
  bool handlePlayerInput(Character& player, Direction direciton, bool isPrimaryPressed, bool isSecondaryPressed);

public:
  GameManager();
  GameManager(
    DisplayController& displayController,
    Character& player1,
    InputController& player1InputController,
    Character& player2,
    SlaveInputController& player2InputController,
    LCDController& lcdController
  );
  GameManager& operator=(GameManager const& other);

  void getNextFrame();
  void handleInput();
  void getLCDState(char const introMessage[]);
};

#endif