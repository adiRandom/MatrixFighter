#include "Collider.hpp"
#include <Arduino.h>

Collider::Collider(BoundingBox boundingBox)
  : _boundingBox{ boundingBox } {}

void Collider::updateBoundingBox(BoundingBox boundingBox) {
  _boundingBox = boundingBox;
}

bool Collider::isColliding(Collider other) const {

  // Serial.println(other._boundingBox.topLeft.getX());
  // Serial.println(other._boundingBox.topLeft.getY());
  // Serial.println(other._boundingBox.bottomRight.getX());
  // Serial.println(other._boundingBox.bottomRight.getY());

  Serial.println(abs(other._boundingBox.topLeft.getX() - _boundingBox.topLeft.getX()) == 1);
  Serial.println(abs(other._boundingBox.bottomRight.getX() - _boundingBox.topLeft.getX()) == 1);
  Serial.println(abs(other._boundingBox.topLeft.getX() - _boundingBox.bottomRight.getX()) == 1);
  Serial.println(abs(other._boundingBox.bottomRight.getX() - _boundingBox.bottomRight.getX()) == 1);

  Serial.println(_boundingBox.bottomRight.getX());


  return /* left - left */ abs(other._boundingBox.topLeft.getX() - _boundingBox.topLeft.getX()) == 1 ||
         /* left - right */ abs(other._boundingBox.bottomRight.getX() - _boundingBox.topLeft.getX()) == 1 ||
         /* right - left */ abs(other._boundingBox.topLeft.getX() - _boundingBox.bottomRight.getX()) == 1 ||
         /* right - right */ abs(other._boundingBox.bottomRight.getX() - _boundingBox.bottomRight.getX()) == 1;
}