#include "GameManager.hpp"
#include "Direction.hpp"
#include "MemoryFree.h"

GameManager::GameManager(
  DisplayController& displayController,
  Character& player1,
  InputController& player1InputController,
  Character& player2,
  InputController& player2InputController,
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
  Direction player1Direction = _player1InputController.getJoyDirection(!_isPlayingGame);
  bool isPlayer1PrimaryBtnPressed = _player1InputController.isPrimaryBtnPressed();
  bool isPlayer1SecondaryBtnPressed = _player1InputController.isSecondaryBtnPressed();

  Direction player2Direction = _player2InputController.getJoyDirection(false);
  bool isPlayer2PrimaryBtnPressed = _player2InputController.isPrimaryBtnPressed();
  bool isPlayer2SecondaryBtnPressed = _player2InputController.isSecondaryBtnPressed();

  if (_isPlayingGame) {

    if (!_lcdController.isGameUIInit()) {
      initGame();
      _lcdController.setGameUIInit();
    }

    bool didPlayer1Update = handlePlayerInput(_player1, player1Direction, isPlayer1PrimaryBtnPressed, isPlayer1SecondaryBtnPressed);
    bool didPlayer2Update = handlePlayerInput(_player2, player2Direction, isPlayer2PrimaryBtnPressed, isPlayer2SecondaryBtnPressed);

    _changed = didPlayer1Update || didPlayer2Update;
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
    return false;
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

    hitPlayer(player, otherPlayer);

    return punchRes;
  } else if (isSecondaryPressed && player.canBlock()) {
    bool shouldUpdateFrame = player.block();

    updatePlayerBlocks();
    return shouldUpdateFrame;
  } else if (player.isBlocking()) {
    return player.stopBlocking();
  }
}

void GameManager::hitPlayer(Character& player, Character& otherPlayer) {
  if (player.isHit(otherPlayer)) {
    otherPlayer.gotHit();
    if (otherPlayer.isDead()) {
      char nameBuffer[MAX_NAME_LEN];

      if (player.getPlayerIndex() == PLAYER1_INDEX) {
        _settingsStorage.getP1Name(nameBuffer);
      } else {
        _settingsStorage.getP2Name(nameBuffer);
      }

      gameOver(nameBuffer);
    } else {
      updatePlayerHP();
    }
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
  player.setCanGoLeft(true);
  player.setCanGoRight(true);

  if (player.getCollider().isLeftEdgeColliding(_screenWalls)) {
    player.setCanGoLeft(false);
  } else if (player.getCollider().isRightEdgeColliding(_screenWalls)) {
    player.setCanGoRight(false);
  }

  if (_player1.getCollider().isColliding(_player2.getCollider())) {
    _player1.setCanGoRight(false);
    _player2.setCanGoLeft(false);
  }
}

void GameManager::handleMenuBtnInput(bool isPrimaryPressed, bool isSecondaryPressed) {
  _isPlayingGame = _lcdController.onSelectChange(isPrimaryPressed);
  _lcdController.onBackBtnChange(isSecondaryPressed);
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


void GameManager::initGame() {
  _player1.reset(_settingsStorage.getMaxHP(), _settingsStorage.getMaxBlocks());
  _player2.reset(_settingsStorage.getMaxHP(), _settingsStorage.getMaxBlocks());
  updatePlayerHP();
  updatePlayerBlocks();


  _lastRoundTimerTick = millis();
  _changed = true;

  uint8_t matrixIntensity = _settingsStorage.getLCDBrightnessLv();
  _displayController.setIntensityLv(matrixIntensity);

  getNextFrame();
}

void GameManager::updatePlayerHP() {
  _lcdController.setPlayer1Hp(_player1.getHP());
  _lcdController.setPlayer2Hp(_player2.getHP());
}

void GameManager::updatePlayerBlocks() {
  _lcdController.setPlayer1Blocks(_player1.getBlockCount());
  _lcdController.setPlayer2Blocks(_player2.getBlockCount());
}

void GameManager::gameOver(char const name[MAX_NAME_LEN]) {
  if (strcmp("", name) == 0) {
    _lcdController.gameOver("DRAW");
  } else {
    char msg[GAME_OVER_MSG_LENGTH];
    uint8_t index = 0;
    while (name[index] != NULL) {
      msg[index] = name[index];
      index++;
    }
    msg[index++] = ' ';
    msg[index++] = 'W';
    msg[index++] = 'O';
    msg[index++] = 'N';
    msg[index++] = NULL;
    _lcdController.gameOver(msg);
  }

  _isPlayingGame = false;
  _displayController.clear();

  uint16_t maxHp = _settingsStorage.getMaxHP();
  uint8_t maxBlockCount = _settingsStorage.getMaxBlocks();
  _player1.reset(maxHp, maxBlockCount);
  _player2.reset(maxHp, maxBlockCount);
}

void GameManager::runRoundTimer() {
  if (!_isPlayingGame) {
    return;
  }

  uint32_t now = millis();
  if (now - _lastRoundTimerTick > 1000) {

    bool isTimeUp = !_lcdController.tickRoundTimer();

    if (isTimeUp) {
      gameOver("");
      return;
    }
    _lastRoundTimerTick = now;
  }
}

void GameManager::runBlockRechargeTimer() {
  if (!_isPlayingGame) {
    return;
  }

  uint32_t now = millis();

  if (_player1.shouldRunBlockRechargeTimer() && _player1BlockRechargeTimer == 0) {
    _player1BlockRechargeTimer = now;
  }

  if (_player2.shouldRunBlockRechargeTimer() && _player2BlockRechargeTimer == 0) {
    _player2BlockRechargeTimer = now;
  }

  if (now - _player1BlockRechargeTimer > BLOCK_RECHARGE_TIME) {
    _player1BlockRechargeTimer = 0;
    _player1.rechargeBlock();
    updatePlayerBlocks();
  }

  if (now - _player2BlockRechargeTimer > BLOCK_RECHARGE_TIME) {
    _player2BlockRechargeTimer = 0;
    _player2.rechargeBlock();
    updatePlayerBlocks();
  }
}