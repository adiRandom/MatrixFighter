#ifndef DISPLAY_CONTROLLER_HPP
#define DISPLAY_CONTROLLER_HPP

#include "LedControl.h"
#include <stdint.h>
#include "Utils.h"

class DisplayController {
private:
  uint8_t _dataPin;
  uint8_t _loadPin;
  uint8_t _clkPin;
  uint8_t _width;
  uint8_t _height;
  uint8_t _displaySize;
  uint8_t _horizontalDisplayCount;
  uint8_t _displayCount;
  bool** _state;
  bool** _nextFrame;

  LedControl _lc;

  struct PixelCoords {
    uint8_t row;
    uint8_t column;
    uint8_t display;
  };

  PixelCoords resolvePixel(uint8_t row, uint8_t column) const;
  void initDisplay();
  void cleanup();
  void emptyNextFrame();


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

  DisplayController& operator=(DisplayController const& other);

  void draw();
  void setPixel(uint8_t row, uint8_t column, bool value);
  void setPixels(Pixel pixels[], uint32_t length);
  void commitNextFrame();
  void initStateAndNextFrame(DisplayController* initValue);
};

#endif