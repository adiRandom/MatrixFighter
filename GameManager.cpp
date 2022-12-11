#include "GameManager.hpp"
#include "Direction.hpp"
#include "MemoryFree.h"

GameManager::GameManager(
  DisplayController& displayController,
  Character& player1,
  InputController& player1InputController,
  Character& player2,
  SlaveInputController& player2InputController,
  LCDController& lcdController
)
  : _displayController{ displayController },
    _player1{ player1 },
    _player1InputController{ player1InputController },
    _player2{ player2 },
    _player2InputController{ player2InputController },
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
  _player1InputController = other._player1InputController;
  _player2 = other._player2;
  _player2InputController = other._player2InputController;
  _lcdController = other._lcdController;

  return *this;
}

void GameManager::getNextFrame() {
  if (!_isPlayingGame) {
    return;
  }
  bool player1AnimationRes = _player1.runAnimations();
  bool player2AnimationRes = _player2.runAnimations();
  if (!_changed && !player1AnimationRes && !player2AnimationRes) {
    return;
  }

  uint8_t bufferLength = _player1.getPixels(_playerCharacterBuffer);
  _displayController.setPixels(_playerCharacterBuffer, bufferLength);

  bufferLength = _player2.getPixels(_playerCharacterBuffer);
  _displayController.setPixels(_playerCharacterBuffer, bufferLength);

  _displayController.commitNextFrame();
  _changed = false;
}

void GameManager::handleInput() {
  Direction player1Direction = _player1InputController.getJoyDirection(false);
  bool isPlayer1PrimaryBtnPressed = _player1InputController.isPrimaryBtnPressed();
  bool isPlayer1SecondaryBtnPressed = _player1InputController.isSecondaryBtnPressed();

  InputBundle player2InputBundle = _player2InputController.getBundle();

  Direction player2Direction = player2InputBundle.direction;
  bool isPlayer2PrimaryBtnPressed = player2InputBundle.isPrimaryBtnPressed;
  bool isPlayer2SecondaryBtnPressed = player2InputBundle.isSecondaryBtnPressed;

  if (_isPlayingGame) {
    bool _didPlayer1Update = handlePlayerInput(_player1, player1Direction, isPlayer1PrimaryBtnPressed, isPlayer1SecondaryBtnPressed);
    bool _didPlayer2Update = handlePlayerInput(_player2, player2Direction, isPlayer2PrimaryBtnPressed, isPlayer2SecondaryBtnPressed);

    _changed = _didPlayer1Update || _didPlayer2Update;
  } else {
    handleMenuJoyInput(player1Direction);
    handleMenuBtnInput(isPlayer1PrimaryBtnPressed, isPlayer1SecondaryBtnPressed);
  }
}

void GameManager::getLCDState(char const introMessage[]) {
  _lcdController.displayCurrentState(introMessage);
}


bool GameManager::handlePlayerJoyInput(Character& player, Direction direction) {
  if (!canPlayerMove(player, direction)) {
    return;
  }

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
    bool punchRes = player.punch();
    Character& otherPlayer = &player == &_player1 ? _player2 : _player1;
    if (player.isHit(otherPlayer)) {
      otherPlayer.gotHit();
    }
    return punchRes;
  } else if (isSecondaryPressed) {
    return player.block();
  } else if (player.isBlocking()) {
    return player.stopBlocking();
  }
}

void GameManager::handleMenuJoyInput(Direction direction) {
  _lcdController.moveSelector(direction);
}

bool GameManager::canPlayerMove(Character& player, Direction direction) {
  bool isMovingToBlockedLeft = direction.isLeft() && !player.canGoLeft();
  bool isMovingToBlockedRight = direction.isRight() && !player.canGoRight();
  bool isAllowedByThrottle = player.canMove();

  return isAllowedByThrottle && !isMovingToBlockedLeft && !isMovingToBlockedRight;
}

void GameManager::updateMovementRestrictions(Character& player) {
  if (player.getCollider().isLeftEdgeColliding(_screenWalls)) {
    player.setCanGoLeft(false);
  } else if (player.getCollider().isRightEdgeColliding(_screenWalls)) {
    player.setCanGoRight(false);
  } else {
    player.setCanGoLeft(true);
    player.setCanGoRight(true);
  }
}

void GameManager::handleMenuBtnInput(bool isPrimaryPressed, bool isSecondaryPressed) {
  if (isPrimaryPressed) {
    _lcdController.select();
  } else if (_lcdController.isPreGame()) {
    // We are releasing the button after selecting PLAY
    _lcdController.startGame();
    _isPlayingGame = true;
  }

  if (isSecondaryPressed) {
    _lcdController.back();
  }
}

bool GameManager::handlePlayerInput(Character& player, Direction direciton, bool isPrimaryPressed, bool isSecondaryPressed) {

  bool hasMoved = handlePlayerJoyInput(player, direciton);
  bool hasDoneAction = handlePlayerBtnInput(player, isPrimaryPressed, isSecondaryPressed);

  if (hasMoved) {
    player.resetMoveTimer();
  }

  updateMovementRestrictions(player);

  return hasMoved || hasDoneAction;
}