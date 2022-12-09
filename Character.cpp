#include "Character.hpp"
#include "DisplayConstants.h"


Character::Character()
  : _origin{ Point{ 0, 0 } },
    _collider{ Collider(getBoundingBox()) },
    _orientation{ Orientation::RIGHT },
    _state{ State::IDLE } {
}

Character::Character(Point initialPosition, Orientation orientation = Orientation::RIGHT)
  : _origin{ initialPosition },
    _collider{ Collider(getBoundingBox()) },
    _orientation{ orientation },
    _state{ State::IDLE } {
}

uint8_t Character::getPixels(Pixel buffer[]) const {
  uint8_t index = 0;
  switch (_state) {
    case State::IDLE:
      {
        if (_orientation == Orientation::RIGHT) {
          buffer[index++] = Point{ _origin.getX(), _origin.getY() - 1 }.toPixel();
          buffer[index++] = _origin.toPixel();
          buffer[index++] = Point{ _origin.getX(), _origin.getY() + 1 }.toPixel();
          buffer[index++] = Point{ _origin.getX() + 1, _origin.getY() }.toPixel();
        }
        break;
      }
    case State::PUNCHIG:
      {
        if (_orientation == Orientation::RIGHT) {
          buffer[index++] = Point{ _origin.getX(), _origin.getY() - 1 }.toPixel();
          buffer[index++] = _origin.toPixel();
          buffer[index++] = Point{ _origin.getX(), _origin.getY() + 1 }.toPixel();
          buffer[index++] = Point{ _origin.getX() + 1, _origin.getY() }.toPixel();
          buffer[index++] = Point{ _origin.getX() + 2, _origin.getY() }.toPixel();
        }
        break;
      }
    case State::CROUCHED:
      {
        if (_orientation == Orientation::RIGHT) {
          buffer[index++] = _origin.toPixel();
          buffer[index++] = Point{ _origin.getX(), _origin.getY() + 1 }.toPixel();
          buffer[index++] = Point{ _origin.getX() + 1, _origin.getY() }.toPixel();
        }
        break;
      }
    case State::CROUCHED_PUNCHING:
      {
        if (_orientation == Orientation::RIGHT) {
          buffer[index++] = _origin.toPixel();
          buffer[index++] = Point{ _origin.getX(), _origin.getY() + 1 }.toPixel();
          buffer[index++] = Point{ _origin.getX() + 1, _origin.getY() }.toPixel();
          buffer[index++] = Point{ _origin.getX() + 2, _origin.getY() }.toPixel();
        }
        break;
      }
    default:
      {
        break;
      }
  }
}

BoundingBox Character::getBoundingBox() {
  switch (_state) {
    case Character::State::IDLE:
      {
        if (_orientation == Orientation::RIGHT) {
          return BoundingBox{
            Point{ _origin.getX(), _origin.getY() + 1 },
            Point{ _origin.getX() + 1, _origin.getY() - 1 },
          };
        } else {
          return BoundingBox{
            Point{ _origin.getX() - 1, _origin.getY() + 1 },
            Point{ _origin.getX(), _origin.getY() - 1 },
          };
        }
      }
    case State::CROUCHED:
      {
        if (_orientation == Orientation::RIGHT) {
          return BoundingBox{
            Point{ _origin.getX(), _origin.getY() + 1 },
            Point{ _origin.getX() + 1, _origin.getY() },
          };
        }
      }
    case State::PUNCHIG:
      {
        if (_orientation == Orientation::RIGHT) {
          return BoundingBox{
            Point{ _origin.getX(), _origin.getY() + 1 },
            Point{ _origin.getX() + 2, _origin.getY() - 1 },
          };
        }
      }
    case State::CROUCHED_PUNCHING:
      {
        if (_orientation == Orientation::RIGHT) {
          return BoundingBox{
            Point{ _origin.getX(), _origin.getY() + 1 },
            Point{ _origin.getX() + 2, _origin.getY() },
          };
        }
      }
    default:
      {
        return BoundingBox{
          Point{ 0, 0 },
          Point{ 0, 0 }
        };
      }
  }
}

void Character::moveRight() {
  // TODO: Handle colission
  _origin.updateX(1);
}

void Character::moveLeft() {
  // TODO: Handle colission
  _origin.updateX(-1);
}

bool Character::punch() {
  switch (_state) {
    case State::PUNCHIG:
    case State::CROUCHED_PUNCHING:
      {
        return false;
      }
    case State::CROUCHED:
      {
        _state = State::CROUCHED_PUNCHING;
        break;
      }
    default:
      {
        _state = State::PUNCHIG;
        break;
      }
  }
  _punchingTimer = millis();
  return true;
}

void Character::crouch() {
  // switch (_state) {
  //   case State::CROUCHED:
  //   case State::CROUCHED_BLOCKING:
  //   case State::CROUCHED_PUNCHING:
  //     {
  //       return;
  //     }
  //   case State::PUNCHIG:
  //   case State::IDLE:
  //     {
  //       _state = State::CROUCHED;
  //       break;
  //     }
  //   case State::BLOCKING:
  //     {
  //       _state = State::CROUCHED_BLOCKING;
  //       break;
  //     }
  //   default:
  //     {
  //       return;
  //     }
  // }

  // _origin.updateY(-1);
}

void Character::rest() {
  // Serial.println(_state);
  // switch (_state) {
  //   case State::BLOCKING:
  //   case State::IDLE:
  //     {

  //       return;
  //     }
  //   case State::PUNCHIG:
  //   case State::CROUCHED:
  //   case State::CROUCHED_PUNCHING:
  //     {
  //       _state = State::IDLE;
  //       break;
  //     }
  //   case State::CROUCHED_BLOCKING:
  //     {
  //       _state = State::BLOCKING;
  //       break;
  //     }
  //   default:
  //     {
  //       return;
  //     }
  // }
  // _origin.updateY(1);
}

bool Character::runAnimations() {
  uint32_t now = millis();

  switch (_state) {
    case State::PUNCHIG:
      {
        if (now - _punchingTimer > PUNCH_ANIMATION_TIME) {
          _state = State::IDLE;
          return true;
        }
      }
    case State::CROUCHED_PUNCHING:
      {
        if (now - _punchingTimer > PUNCH_ANIMATION_TIME) {
          _state = State::CROUCHED;
          return true;
        }
      }
    default:
      {
        return false;
      }
  }
}