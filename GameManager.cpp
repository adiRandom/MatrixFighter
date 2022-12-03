#include "GameManager.hpp"
#include "List.cpp"
#include "Utils.h"
#include "Direction.hpp"

GameManager::GameManager(
  DisplayController displayController,
  Character player1,
  InputController inputController,
  LCDController lcdController
)
  : _displayController{ displayController },
    _player1{ player1 },
    _inputController{ inputController },
    _lcdController{ lcdController } {
}

GameManager::GameManager() {
}

GameManager& GameManager::operator=(GameManager const& other) {
  if (this == &other) {
    return *this;
  }

  _displayController = other._displayController;
  _player1 = other._player1;
  _inputController = other._inputController;
  _lcdController = other._lcdController;

  return *this;
}

void GameManager::getNextFrame() {
  if (!_lcdController.showGame()) {
    return;
  }
  bool player1AnimationRes = _player1.runAnimations();
  if (!_changed && !player1AnimationRes) {
    return;
  }
  List<Pixel> player1Pixels = _player1.getPixels(_displayController.getHeight());

  _displayController.setPixels(player1Pixels);
  _displayController.commitNextFrame();

  _changed = false;
}

void GameManager::handleInput() {
  Direction player1Direction = _inputController.getJoyDirection(true);
  bool isPlayer1Punching = _inputController.isPunching();

  if (player1Direction.isRight()) {
    _player1.moveRight();
    // TODO: Check orientation change
    _changed = true;
  } else if (player1Direction.isLeft()) {
    _player1.moveLeft();
    // TODO: Check orientation change
    _changed = true;
  }

  if (isPlayer1Punching) {
    _changed = _player1.punch();
  }
}

void GameManager::getLCDState(char const introMessage[]) {
  _lcdController.displayCurrentState(introMessage);
}