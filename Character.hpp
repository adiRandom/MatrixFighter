#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <stdint.h>
#include "Utils.h"
#include "List.cpp"
#include "Collider.hpp"

class Character {
private:
  Point _origin;
  Collider _collider;

  enum State {
    IDLE_FACE_RIGHT,
    IDLE_FACE_LEFT,
    PUNCHIG_LEFT,
    PUNCHING_RIGHT,
    CROUCHED,
    JUMPING
  };

  State _state;

  BoundingBox getBoundingBox();

public:
  Character();
  Character(Point initialPosition, bool isFacedRight = true);
  void MoveLeft();
  void MoveRight();
  void Jump();
  void Crouch();
  void Punch();
  void Rest();
  Collider getCollider() const;
  List<Pixel> getPixels(int32_t displayHeight) const;
};

#endif