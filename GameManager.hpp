#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include "DisplayController.hpp"
#include "Character.hpp"
#include "InputController.hpp"
#include "LCDController.hpp"
#include "Utils.h"
#include "Collider.hpp"
#include "DisplayConstants.h"

class GameManager {
private:
  DisplayController _displayController;
  Character _player1;
  InputController _inputController;
  LCDController _lcdController;
  bool _changed = true;
  bool _isPlayingGame = false;
  Pixel _player1CharacterBuffer[CHARACTER_MODEL_BUFFER_SIZE];
  Collider _screenWalls{ BoundingBox{
    Point{ -1, -1 },
    Point{ DISPLAY_WIDTH, DISPLAY_HEIGHT } } };

  bool handlePlayerJoyInput(Character& player, Direction direction);
  void handleMenuJoyInput(Direction direciton);
  bool handlePlayerBtnInput(Character& player, bool isPrimaryPressed, bool isSecondaryPressed);
  bool canPlayerMove(Character& player, Direction direction);
  void updateMovementRestrictions(Character& player, Direction lastDirection);

public:
  GameManager();
  GameManager(
    DisplayController _displayController,
    Character player1,
    InputController inputController,
    LCDController lcdController
  );
  GameManager& operator=(GameManager const& other);

  void getNextFrame();
  void handleInput();
  void getLCDState(char const introMessage[]);
};

#endif