#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include "DisplayController.hpp"
#include "Character.hpp"
#include "InputController.hpp"
#include "LCDController.hpp"
#include "Utils.h"

class GameManager {
private:
  DisplayController _displayController;
  Character _player1;
  InputController _inputController;
  LCDController _lcdController;
  bool _changed = true;
  bool _isPlayingGame = false;
  Pixel _player1CharacterBuffer[CHARACTER_MODEL_BUFFER_SIZE];

  void handlePlayer1JoyInput(Direction direction);
  void handleMenuJoyInput(Direction direciton);

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