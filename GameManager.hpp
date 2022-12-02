#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include "DisplayController.hpp"
#include "Character.hpp"
#include "InputController.hpp"

class GameManager {
private:
  DisplayController _displayController;
  Character _player1;
  InputController _inputController;
  bool _changed = true;
public:
  GameManager();
  GameManager(DisplayController _displayController, Character player1, InputController inputController);
  GameManager& operator=(GameManager const& other);

  void getNextFrame();
  void handleInput();
};

#endif