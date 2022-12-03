#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include "DisplayController.hpp"
#include "Character.hpp"
#include "InputController.hpp"
#include "LCDController.hpp"

class GameManager {
private:
  DisplayController _displayController;
  Character _player1;
  InputController _inputController;
  LCDController _lcdController;
  bool _changed = true;
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
  void getLCDState();
};

#endif