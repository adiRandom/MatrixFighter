#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include <stdint.h>
#include "Utils.h"

class Collider {
private:
  BoundingBox _boundingBox;
public:
  Collider();
  Collider(BoundingBox boundingBox);
  bool isColliding(Collider other) const;
  void updateBoundingBox(BoundingBox boundingBox);

  bool isRightEdgeColliding(Collider other) const;
  bool isLeftEdgeColliding(Collider other) const;

  BoundingBox getBoundingBox() const;
};

#endif