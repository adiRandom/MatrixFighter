#include "GameManager.hpp"
#include "Direction.hpp"
#include "MemoryFree.h"

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

  uint8_t bufferLength = _player1.getPixels(_player1CharacterBuffer);

  _displayController.setPixels(_player1CharacterBuffer, bufferLength);
  _displayController.commitNextFrame();

  _changed = false;
}

void GameManager::handleInput() {
  Direction player1Direction = _inputController.getJoyDirection(true);
  bool isPlayer1PrimaryBtnPressed = _inputController.isPrimaryBtnPressed();
  bool isPlayer1SecondaryBtnPressed = _inputController.isSecondaryBtnPressed();

  if (_isPlayingGame) {
    bool hasMoved = handlePlayerJoyInput(_player1, player1Direction);
    bool hasDoneAction = handlePlayerBtnInput(_player1, isPlayer1PrimaryBtnPressed, isPlayer1SecondaryBtnPressed);

    // Serial.println(hasDoneAction);

    _changed = hasMoved || hasDoneAction;
  } else {
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


bool GameManager::handlePlayerJoyInput(Character& player, Direction direction) {
  if (direction.isRight()) {
    player.moveRight();
  } else if (direction.isLeft()) {
    player.moveLeft();
  } else if (direction.isDown()) {
    player.crouch();
  } else if (direction.isUp()) {
    // Do nothing
  } else {
    return player.uncrouch();
  }

  return true;
}

bool GameManager::handlePlayerBtnInput(Character& player, bool isPrimaryPressed, bool isSecondaryPressed) {

  if (isPrimaryPressed) {
    return player.punch();
  } else if (isSecondaryPressed) {
    return player.block();
  } else if (player.isBlocking()) {
    return player.stopBlocking();
  }
}

void GameManager::handleMenuJoyInput(Direction direction) {
  _lcdController.moveSelector(direction);
}