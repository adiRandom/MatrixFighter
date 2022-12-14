#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include "Direction.hpp"

struct Pixel {
  uint8_t x;
  uint8_t y;
  bool value;
};

class Point {
private:
  int16_t _x;
  int16_t _y;

public:
  Point(int16_t x, int16_t y);
  int16_t getX() const;
  int16_t getY() const;
  void updateX(int16_t delta);
  void updateY(int16_t delta);
  Pixel toPixel() const;
};

struct BoundingBox {
  Point topLeft;
  Point bottomRight;
};

struct InputBundle {
  Direction direction;
  bool isPrimaryBtnPressed;
  bool isSecondaryBtnPressed;
};

struct BlinkState {
  bool shouldBlink = false;
  bool blinkState;
  uint8_t blinkCount;
  uint32_t blinkTimer;
};

#endif