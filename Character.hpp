#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <stdint.h>
#include "Utils.h"
#include "Collider.hpp"
#include "Arduino.h"
#include "Constants.h"

uint32_t const PUNCH_ANIMATION_TIME = 500;
uint8_t const CHARACTER_MODEL_BUFFER_SIZE = 7;
uint32_t const DEFAULT_MOVE_THROTTLE_TIME = 100;

uint8_t const LEFT_PLAYER_X = 0;
uint8_t const RIGHT_PLAYER_X = DISPLAY_WIDTH - 1;
uint8_t const CHARACTER_INITIAL_Y = 1;
uint8_t const BLINKS_ON_HIT = 2;
uint32_t const BLINK_TIME = 200;
uint32_t const BLOCK_RECHARGE_TIME = 4000;

class Character {
public:
  enum Orientation {
    LEFT,
    RIGHT
  };

private:
  Point _origin;
  Collider _collider;
  bool _canGoLeft = false;
  bool _canGoRight = false;
  uint16_t _hp = 0;

  // When the user punches and hits, set this to true
  // so the punch won't deal more than 1 dmg while the animation is on
  bool _didPunchHit = false;

  enum State {
    IDLE,
    PUNCHIG,
    CROUCHED,
    CROUCHED_PUNCHING,
    BLOCKING,
    CROUCHED_BLOCKING
  };

  State _state = State::IDLE;
  Orientation _orientation;

  uint32_t _punchingTimer = millis();
  uint32_t _lastMoveTime = 0;
  BlinkState _blinkState;
  uint8_t _blockCount = 0;
  uint8_t _maxBlockCount = 0;

  BoundingBox getBoundingBox();
  void refreshBoundingBox();
  bool tookHitAnimation();
  void resetBlinkState();

public:
  Character();
  Character(
    Point initialPosition,
    Orientation orientation,
    uint16_t maxHp,
    uint8_t maxBlocks
  );
  void moveLeft();
  void moveRight();
  void crouch();
  /**
   * Returns whether the action succeded or not
   */
  bool punch();
  bool block();
  bool stopBlocking();
  bool uncrouch();
  bool isBlocking();
  bool isPunching();
  bool isCrouching();
  /**
   * Return whether or not the state of the character changed and we need to redraw the frame
   */
  bool runAnimations();
  Collider getCollider() const;
  /**
   * Returns the number of pixels put in the buffer
   */
  uint8_t getPixels(Pixel buffer[]) const;


  bool canGoLeft() const;
  bool canGoRight() const;

  void setCanGoLeft(bool canGoLeft);
  void setCanGoRight(bool canGoRight);
  void gotHit();
  uint16_t getHP();
  // Check it this character is hitting the other
  bool isHit(Character &other);
  bool canMove() const;
  void resetMoveTimer();
  bool isDead() const;
  bool canBlock() const;
  bool shouldRunBlockRechargeTimer() const;

  // Is player 1 or 2
  uint8_t getPlayerIndex() const;
  void reset(uint16_t maxHp, uint8_t maxBlockCount);
  uint8_t getBlockCount() const;

  void rechargeBlock();
};

#endif