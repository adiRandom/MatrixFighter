#include "Character.hpp"


Character::Character()
  : _origin{ Point{ 0, 0 } }, _collider{ Collider(getBoundingBox()) } {
}

Character::Character(Point initialPosition, bool isFacedRight = true)
  : _origin{ initialPosition }, _collider{ Collider(getBoundingBox()) } {
  _state = isFacedRight ? Character::State::IDLE_FACE_RIGHT : Character::State::IDLE_FACE_LEFT;
}

List<Pixel> Character::getPixels(int32_t displayHeight) const {
  List<Pixel> result;
  switch (_state) {
    case Character::State::IDLE_FACE_RIGHT:
      {
        result.push(Point{ _origin.getX(), _origin.getY() - 1 }.toPixel(displayHeight));
        result.push(_origin.toPixel(displayHeight));
        result.push(Point{ _origin.getX(), _origin.getY() + 1 }.toPixel(displayHeight));
        result.push(Point{ _origin.getX() + 1, _origin.getY() }.toPixel(displayHeight));
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

void Character::MoveRight() {
  // TODO: Handle colission
  _origin.updateX(1);
}