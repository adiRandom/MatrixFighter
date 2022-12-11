#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <stdint.h>
#include "Utils.h"
#include "Collider.hpp"
#include "Arduino.h"

uint32_t const PUNCH_ANIMATION_TIME = 500;
uint8_t const CHARACTER_MODEL_BUFFER_SIZE = 6;
uint16_t const DEFAULT_MAX_HP = 10;
uint32_t const DEFAULT_MOVE_THROTTLE_TIME = 100;

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
  uint16_t _hp = DEFAULT_MAX_HP;

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

  BoundingBox getBoundingBox();
  void refreshBoundingBox();

public:
  Character();
  Character(Point initialPosition, Orientation orientation);
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

  // Is player 1 or 2
  uint8_t getPlayerIndex() const;
};

#endif