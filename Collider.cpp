#include "Collider.hpp"
#include <Arduino.h>

Collider::Collider(BoundingBox boundingBox)
  : _boundingBox{ boundingBox } {
}

Collider::Collider()
  : _boundingBox{ BoundingBox{ Point{ -1, -1 }, Point{ -1, -1 } } } {
}

void Collider::updateBoundingBox(BoundingBox boundingBox) {
  _boundingBox = boundingBox;
}

bool Collider::isColliding(Collider other) const {
  return /* left - left */ abs(other._boundingBox.topLeft.getX() - _boundingBox.topLeft.getX()) == 1 ||
         /* left - right */ abs(other._boundingBox.bottomRight.getX() - _boundingBox.topLeft.getX()) == 1 ||
         /* right - left */ abs(other._boundingBox.topLeft.getX() - _boundingBox.bottomRight.getX()) == 1 ||
         /* right - right */ abs(other._boundingBox.bottomRight.getX() - _boundingBox.bottomRight.getX()) == 1;
}

bool Collider::isRightEdgeColliding(Collider other) const {
  return abs(other._boundingBox.bottomRight.getX() - _boundingBox.bottomRight.getX()) == 1 || abs(other._boundingBox.topLeft.getX() - _boundingBox.bottomRight.getX()) == 1;
}

bool Collider::isLeftEdgeColliding(Collider other) const {
  return abs(other._boundingBox.topLeft.getX() - _boundingBox.topLeft.getX()) == 1 ||
         /* left - right */ abs(other._boundingBox.bottomRight.getX() - _boundingBox.topLeft.getX()) == 1;
}

BoundingBox Collider::getBoundingBox() const {
  return _boundingBox;
}