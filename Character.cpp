#include "Character.hpp"
#include "Constants.h"


Character::Character()
  : _origin{ Point{ 0, 0 } },
    _orientation{ Orientation::RIGHT },
    _collider{ Collider(getBoundingBox()) },
    _state{ State::IDLE },
    _canGoLeft{ false },
    _canGoRight{ true },
    _hp{ 0 } {
}

Character::Character(Point initialPosition, Orientation orientation, uint16_t maxHp, uint8_t maxBlocks)
  : _origin{ initialPosition },
    _orientation{ orientation },
    _state{ State::IDLE },
    _canGoLeft{ orientation == Orientation::LEFT },
    _canGoRight{ orientation == Orientation::RIGHT },
    _hp{ maxHp },
    _blockCount{ maxBlocks },
    _maxBlockCount{ maxBlocks } {

  _collider = Collider{ getBoundingBox() };
}

uint8_t Character::getPixels(Pixel buffer[]) const {
  uint8_t index = 0;
  uint8_t armOffset = _orientation == Orientation::RIGHT ? 1 : -1;

  if (_blinkState.shouldBlink && _blinkState.blinkState == false) {
    return 0;
  }

  switch (_state) {
    case State::IDLE:
      {
        buffer[index++] = Point{ _origin.getX(), _origin.getY() - 1 }.toPixel();
        buffer[index++] = _origin.toPixel();
        buffer[index++] = Point{ _origin.getX(), _origin.getY() + 1 }.toPixel();
        buffer[index++] = Point{ _origin.getX() + armOffset, _origin.getY() }.toPixel();
        break;
      }
    case State::PUNCHIG:
      {
        buffer[index++] = Point{ _origin.getX(), _origin.getY() - 1 }.toPixel();
        buffer[index++] = _origin.toPixel();
        buffer[index++] = Point{ _origin.getX(), _origin.getY() + 1 }.toPixel();
        buffer[index++] = Point{ _origin.getX() + armOffset, _origin.getY() }.toPixel();
        buffer[index++] = Point{ _origin.getX() + 2 * armOffset, _origin.getY() }.toPixel();
        break;
      }
    case State::CROUCHED:
      {
        buffer[index++] = _origin.toPixel();
        buffer[index++] = Point{ _origin.getX(), _origin.getY() + 1 }.toPixel();
        buffer[index++] = Point{ _origin.getX() + armOffset, _origin.getY() }.toPixel();
        break;
      }
    case State::CROUCHED_PUNCHING:
      {
        buffer[index++] = _origin.toPixel();
        buffer[index++] = Point{ _origin.getX(), _origin.getY() + 1 }.toPixel();
        buffer[index++] = Point{ _origin.getX() + armOffset, _origin.getY() }.toPixel();
        buffer[index++] = Point{ _origin.getX() + 2 * armOffset, _origin.getY() }.toPixel();
        break;
      }
    case State::BLOCKING:
      {
        buffer[index++] = Point{ _origin.getX(), _origin.getY() - 1 }.toPixel();
        buffer[index++] = _origin.toPixel();
        buffer[index++] = Point{ _origin.getX(), _origin.getY() + 1 }.toPixel();
        buffer[index++] = Point{ _origin.getX() + armOffset, _origin.getY() }.toPixel();
        buffer[index++] = Point{ _origin.getX() + armOffset, _origin.getY() + 1 }.toPixel();
        break;
      }

    case State::CROUCHED_BLOCKING:
      {
        buffer[index++] = _origin.toPixel();
        buffer[index++] = Point{ _origin.getX(), _origin.getY() + 1 }.toPixel();
        buffer[index++] = Point{ _origin.getX() + armOffset, _origin.getY() }.toPixel();
        buffer[index++] = Point{ _origin.getX() + armOffset, _origin.getY() + 1 }.toPixel();
        break;
      }
    default:
      {
        break;
      }
  }

  return index;
}

BoundingBox Character::getBoundingBox() {
  switch (_state) {
    case Character::State::IDLE:
    case State::BLOCKING:
      // case State::PUNCHIG:
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
    case State::CROUCHED_BLOCKING:
      // case State::CROUCHED_PUNCHING:
      {
        if (_orientation == Orientation::RIGHT) {
          return BoundingBox{
            Point{ _origin.getX(), _origin.getY() + 1 },
            Point{ _origin.getX() + 1, _origin.getY() },
          };
        } else {
          return BoundingBox{
            Point{ _origin.getX() - 1, _origin.getY() + 1 },
            Point{ _origin.getX(), _origin.getY() },
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
        } else {
          return BoundingBox{
            Point{ _origin.getX() - 2, _origin.getY() + 1 },
            Point{ _origin.getX(), _origin.getY() - 1 },
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
        } else {
          return BoundingBox{
            Point{ _origin.getX() - 2, _origin.getY() + 1 },
            Point{ _origin.getX(), _origin.getY() },
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
  _origin.updateX(1);
  refreshBoundingBox();
}

void Character::moveLeft() {
  _origin.updateX(-1);
  refreshBoundingBox();
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
  refreshBoundingBox();
  return true;
}

void Character::crouch() {
  switch (_state) {
    case State::CROUCHED:
    case State::CROUCHED_BLOCKING:
    case State::CROUCHED_PUNCHING:
      {
        return;
      }
    case State::PUNCHIG:
    case State::IDLE:
      {
        _state = State::CROUCHED;
        break;
      }
    case State::BLOCKING:
      {
        _state = State::CROUCHED_BLOCKING;
        break;
      }
    default:
      {
        return;
      }
  }

  _origin.updateY(-1);
  refreshBoundingBox();
}

bool Character::uncrouch() {
  switch (_state) {
    case State::PUNCHIG:
    case State::BLOCKING:
    case State::IDLE:
      {
        return false;
      }
    case State::CROUCHED:
    case State::CROUCHED_PUNCHING:
      {
        _state = State::IDLE;
        break;
      }
    case State::CROUCHED_BLOCKING:
      {
        _state = State::BLOCKING;
        break;
      }
    default:
      {
        return false;
      }
  }
  _origin.updateY(1);
  refreshBoundingBox();
  return true;
}

bool Character::tookHitAnimation() {
  uint32_t now = millis();
  if (now - _blinkState.blinkTimer > BLINK_TIME) {
    _blinkState.blinkTimer = now;
    _blinkState.blinkState = !_blinkState.blinkState;

    if (_blinkState.blinkState == true) {
      _blinkState.blinkCount++;
    }

    if (_blinkState.blinkCount == BLINKS_ON_HIT) {
      _blinkState.shouldBlink = false;
    }


    return true;
  }

  return false;
}

bool Character::runAnimations() {
  uint32_t now = millis();

  switch (_state) {
    case State::PUNCHIG:
      {
        if (now - _punchingTimer > PUNCH_ANIMATION_TIME) {
          _state = State::IDLE;
          _didPunchHit = false;
          refreshBoundingBox();
          return true;
        }
      }
    case State::CROUCHED_PUNCHING:
      {
        if (now - _punchingTimer > PUNCH_ANIMATION_TIME) {
          _state = State::CROUCHED;
          refreshBoundingBox();
          return true;
        }
      }
    default:
      {
        break;
      }
  }

  if (_blinkState.shouldBlink) {
    return tookHitAnimation();
  } else {
    return false;
  }
}

bool Character::block() {

  switch (_state) {
    case State::BLOCKING:
    case State::CROUCHED_BLOCKING:
      { 
        _blockCount--;
        return false;
      }
    case State::CROUCHED:
    case State::CROUCHED_PUNCHING:
      {
        _state = State::CROUCHED_BLOCKING;
        return true;
      }
    case State::IDLE:
    case State::PUNCHIG:
      {
        _state = State::BLOCKING;
        return true;
      }
    default:
      {
        return false;
      }
  }

  refreshBoundingBox();
}

bool Character::stopBlocking() {
  switch (_state) {
    case State::BLOCKING:
      {
        _state = State::IDLE;
        return true;
      }
    case State::CROUCHED_BLOCKING:
      {
        _state = State::CROUCHED;
        return true;
      }
    default:
      {
        return false;
      }
  }

  refreshBoundingBox();
}

bool Character::isBlocking() {
  return _state == State::BLOCKING || _state == State::CROUCHED_BLOCKING;
}

bool Character::canGoLeft() const {
  return _canGoLeft;
}

bool Character::canGoRight() const {
  return _canGoRight;
}

void Character::setCanGoLeft(bool canGoLeft) {
  _canGoLeft = canGoLeft;
}

void Character::setCanGoRight(bool canGoRight) {
  _canGoRight = canGoRight;
}

Collider Character::getCollider() const {
  return _collider;
}

void Character::refreshBoundingBox() {
  _collider.updateBoundingBox(getBoundingBox());
}

bool Character::isCrouching() {
  return _state == State::CROUCHED || _state == State::CROUCHED_BLOCKING || _state == State::CROUCHED_PUNCHING;
}

uint16_t Character::getHP() {
  return _hp;
}

void Character::gotHit() {
  _hp--;
  resetBlinkState();
}
bool Character::isHit(Character &other) {
  if (!isPunching() || _didPunchHit) {
    // Can't hit if not punching
    // Or if the punch already hit and this is the rest of the punch animation
    return false;
  } else if (!other.getCollider().isColliding(_collider)) {
    // No collision
    return false;
  } else if (other.isCrouching() && !isCrouching()) {
    // Pucnhing above the head
    return false;
  } else if (other.isBlocking()) {
    if (other.isCrouching()) {
      // No point to hit the other player if he's bloking and crouching
      return false;
    } else if (!isCrouching()) {
      // Punching right into the shield
      return false;
    }
  }
  _didPunchHit = true;

  return true;
}

bool Character::isPunching() {
  return _state == State::PUNCHIG || _state == State::CROUCHED_PUNCHING;
}

bool Character::canMove() const {
  return millis() - _lastMoveTime >= DEFAULT_MOVE_THROTTLE_TIME;
}

void Character::resetMoveTimer() {
  _lastMoveTime = millis();
}

bool Character::isDead() const {
  return _hp <= 0;
}

uint8_t Character::getPlayerIndex() const {
  return _orientation == Orientation::RIGHT ? PLAYER1_INDEX : PLAYER2_INDEX;
}

void Character::reset(uint16_t maxHp) {
  _origin = _orientation == Orientation::LEFT ? Point{ RIGHT_PLAYER_X, CHARACTER_INITIAL_Y } : Point{ LEFT_PLAYER_X, CHARACTER_INITIAL_Y };

  _state = State::IDLE;
  _hp = maxHp;
  _blockCount = _maxBlockCount;
  _canGoLeft = _orientation == Orientation::LEFT;
  _canGoRight = _orientation == Orientation::RIGHT;

  refreshBoundingBox();

  _punchingTimer = millis();
  _lastMoveTime = 0;
  _didPunchHit = false;
}

void Character::resetBlinkState() {
  _blinkState.blinkState = true;
  _blinkState.blinkCount = 0;
  _blinkState.shouldBlink = true;
  _blinkState.blinkTimer = 0;
}

bool Character::canBlock() const {
  return _blockCount > 0;
}

bool Character::shouldRunBlockRechargeTimer() const {
  return _blockCount < _maxBlockCount;
}

uint8_t Character::getBlockCount() const {
  return _blockCount;
}

void Character::rechargeBlock() {
  if (_blockCount < _maxBlockCount) {
    _blockCount++;
  }
}