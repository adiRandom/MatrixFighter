#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <stdint.h>
#include "Utils.h"
#include "Collider.hpp"
#include "Arduino.h"

uint32_t const PUNCH_ANIMATION_TIME = 500;
uint8_t const CHARACTER_MODEL_BUFFER_SIZE = 6;

class Character {
public:
  enum Orientation {
    LEFT,
    RIGHT
  };

private:
  Point _origin;
  Collider _collider;

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

  BoundingBox getBoundingBox();

public:
  Character();
  Character(Point initialPosition, Orientation orientation = Orientation::RIGHT);
  void moveLeft();
  void moveRight();
  void jump();
  void crouch();
  /**
   * Returns whether the action succeded or not
   */
  bool punch();
  void block();
  // Uncrouch
  void rest();
  bool isBlocking();
  bool isPunching();
  /**
   * Return whether or not the state of the character changed and we need to redraw the frame
   */
  bool runAnimations();
  Collider getCollider() const;
  uint8_t getPixels(Pixel buffer[]) const;
};

#endif