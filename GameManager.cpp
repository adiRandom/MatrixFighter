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
  if (!_isPlayingGame) {
    return;
  }
  bool player1AnimationRes = _player1.runAnimations();
  if (!_changed && !player1AnimationRes) {
    return;
  }
  List<Pixel> player1Pixels = _player1.getPixels(DISPLAY_HEIGHT);

  _displayController.setPixels(player1Pixels);
  _displayController.commitNextFrame();

  _changed = false;
}

void GameManager::handleInput() {
  Direction player1Direction = _inputController.getJoyDirection(true);
    Serial.println(player1Direction.getDirection());
  bool isPlayer1PrimaryBtnPressed = _inputController.isPrimaryBtnPressed();
  bool isPlayer1SecondaryBtnPressed = _inputController.isSecondaryBtnPressed();

  if (_isPlayingGame) {
      handlePlayer1JoyInput(player1Direction);

      if (isPlayer1PrimaryBtnPressed) {
        _changed = _player1.punch();
      }
    }
  else {
    handleMenuJoyInput(player1Direction);

    if (isPlayer1PrimaryBtnPressed) {
      _lcdController.select();
    } else if (_lcdController.isPreGame()) {
      // We are releasing the button after selecting PLAY
      _lcdController.startGame();
      _isPlayingGame = true;
    }

    if (isPlayer1SecondaryBtnPressed) {
      _lcdController.back();
    }
  }
}

void GameManager::getLCDState(char const introMessage[]) {
  _lcdController.displayCurrentState(introMessage);
}


void GameManager::handlePlayer1JoyInput(Direction direction) {
  if (direction.isRight()) {
    _player1.moveRight();
  } else if (direction.isLeft()) {
    _player1.moveLeft();
  } else if (direction.isDown()) {
    _player1.crouch();
  } else if(direction.isUp()){
    // Do nothing
  } 
  else {
    _player1.rest();
  }

  _changed = true;
}

void GameManager::handleMenuJoyInput(Direction direction) {
  _lcdController.moveSelector(direction);
}