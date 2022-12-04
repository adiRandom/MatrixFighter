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
  if (!isPlayingGame()) {
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
  // Rename this to primaryButton
  bool isPlayer1Punching = _inputController.isPunching();

  if (isPlayingGame()) {
    handlePlayer1JoyInput(player1Direction);
  } else {
    handleMenuJoyInput(player1Direction);
  }


  if (isPlayer1Punching) {
    _changed = _player1.punch();
  }
}

void GameManager::getLCDState(char const introMessage[]) {
  _lcdController.displayCurrentState(introMessage);
}


void GameManager::handlePlayer1JoyInput(Direction direction) {
  if (direction.isRight()) {
    _player1.moveRight();
    // TODO: Check orientation change
    _changed = true;
  } else if (direction.isLeft()) {
    _player1.moveLeft();
    // TODO: Check orientation change
    _changed = true;
  }
}

void GameManager::handleMenuJoyInput(Direction direction) {
  _lcdController.moveSelector(direction);
}

bool GameManager::isPlayingGame() {
  return _lcdController.isShowingGame();
}