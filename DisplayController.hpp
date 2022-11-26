#ifndef DISPLAY_CONTROLLER_HPP
#define DISPLAY_CONTROLLER_HPP

#include "LedControl.h"
#include <stdint.h>
#include "Uitls.h"

class DisplayController {
private:
  uint8_t const _dataPin;
  uint8_t const _loadPin;
  uint8_t const _clkPin;
  uint8_t const _width;
  uint8_t const _height;
  uint8_t const _displaySize;
  uint8_t const _horizontalDisplayCount;
  uint8_t const _displayCount;
  bool** _state;

  LedControl const _lc;

  struct PixelCoords {
    uint8_t row;
    uint8_t column;
    uint8_t display;
  };

  PixelCoords resolvePixel(uint8_t row, uint8_t column) const;
  void clearDisplay();


public:
  DisplayController();
  DisplayController(
    uint8_t dataPin,
    uint8_t loadPin,
    uint8_t clkPin,
    uint8_t width,
    uint8_t height,
    uint8_t horizontalDisplayCount,
    uint8_t displayCount,
    uint8_t displaySize
  );

  ~DisplayController();

  void draw();
  void setPixel(uint8_t row, uint8_t column, bool value);
  void setPixels(Pixel pixels[], uint32_t length);
};

#endif