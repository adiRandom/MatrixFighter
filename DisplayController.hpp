#ifndef DISPLAY_CONTROLLER_HPP
#define DISPLAY_CONTROLLER_HPP

#include "LedControl.h"
#include <stdint.h>
#include "Utils.h"
#include "Constants.h"

class DisplayController {
private:
  uint8_t _dataPin;
  uint8_t _loadPin;
  uint8_t _clkPin;
  bool _frameBuffer[DISPLAY_HEIGHT][DISPLAY_WIDTH];
  bool _shouldRedraw = true;

  LedControl _lc;

  struct PixelCoords {
    uint8_t row;
    uint8_t column;
    uint8_t display;
  };

  PixelCoords resolvePixel(uint8_t row, uint8_t column) const;
  void initDisplay();
  void emptyNextFrame();

public:
  DisplayController();
  DisplayController(
    uint8_t dataPin,
    uint8_t loadPin,
    uint8_t clkPin
  );
  DisplayController(DisplayController const& other);

  DisplayController& operator=(DisplayController const& other);

  void draw();
  void setPixel(Pixel pixel);
  void setPixels(Pixel pixels[], uint32_t length);
  void commitNextFrame();
  void initStateAndNextFrame(DisplayController* initValue);
  void clear();
};

#endif