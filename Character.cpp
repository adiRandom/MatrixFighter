#include "Character.hpp"


Character::Character()
  : _origin{ Point{ 0, 0 } }, _collider{ Collider(getBoundingBox()) } {
}

Character::Character(Point initialPosition, Orientation isFacedRight = true)
  : _origin{ initialPosition }, _collider{ Collider(getBoundingBox()) } {
  _state = isFacedRight ? Character::State::IDLE_FACE_RIGHT : Character::State::IDLE_FACE_LEFT;
}

List<Pixel> Character::getPixels(int32_t displayHeight) const {
  List<Pixel> result;
  switch (_state) {
    case State::IDLE:
      {
        if (_orientation == Orientation::RIGHT) {
          result.push(Point{ _origin.getX(), _origin.getY() - 1 }.toPixel(displayHeight));
          result.push(_origin.toPixel(displayHeight));
          result.push(Point{ _origin.getX(), _origin.getY() + 1 }.toPixel(displayHeight));
          result.push(Point{ _origin.getX() + 1, _origin.getY() }.toPixel(displayHeight));
        }
        break;
      }
    case State::PUNCHIG:
      {
        if (_orientation == Orientation::RIGHT) {
          result.push(Point{ _origin.getX(), _origin.getY() - 1 }.toPixel(displayHeight));
          result.push(_origin.toPixel(displayHeight));
          result.push(Point{ _origin.getX(), _origin.getY() + 1 }.toPixel(displayHeight));
          result.push(Point{ _origin.getX() + 1, _origin.getY() }.toPixel(displayHeight));
          result.push(Point{ _origin.getX() + 2, _origin.getY() }.toPixel(displayHeight));
        }
        break;
      }
    default:
      {
        break;
      }
  }
  return result;
}

BoundingBox Character::getBoundingBox() {
  switch (_state) {
    case Character::State::IDLE_FACE_RIGHT:
      {
        return BoundingBox{
          Point{ _origin.getX(), _origin.getY() + 1 },
          Point{ _origin.getX() + 1, _origin.getY() - 1 },
        };
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

void Character::punch() {
  switch (_state) {
    case State::PUNCHIG:
    case State::CROUCHED_PUNCHING:
    case State::JUMPING:
      {
        return;
      }
    case State::CROUCHED:
      {
        _state = State::CROUCHED_PUNCHING;
        break;
      }
    default:
      {
        _state = State::PUNCHING;
        break;
      }

      _punchingTimer = millis();
  }
}

void Character::runAnimations() {
  uint32_t now = millis();

  switch (_state) {
    case State::PUNCHIG:
      {
        if (now - _punchingTimer > PUNCH_ANIMATION_TIME) {
          _state = State::IDLE;
          break;
        }
      }
    case State::CROUCHED_PUNCHING:
      {
        if (now - _punchingTimer > PUNCH_ANIMATION_TIME) {
          _state = State::CHROUCHED;
          break;
        }
      }
    default:
      {
        break;
      }
  }
}