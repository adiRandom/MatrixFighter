#include "DisplayController.hpp"

DisplayController::DisplayController(
  uint8_t dataPin,
  uint8_t loadPin,
  uint8_t clkPin,
  uint8_t width,
  uint8_t height,
  uint8_t horizontalDisplayCount,
  uint8_t displayCount,
  uint8_t displaySize
)
  : _dataPin{ dataPin },
    _loadPin{ loadPin },
    _clkPin{ clkPin },
    _width{ width },
    _height{ height },
    _horizontalDisplayCount{ horizontalDisplayCount },
    _lc{
      LedControl(dataPin, clkPin, loadPin, displayCount)
    },
    _displaySize{ displaySize },
    _displayCount{ displayCount } {

  _state = new bool*[height];
  for (int i = 0; i < height; i++) {
    _state[i] = new bool[width];
    for (int j = 0; j < width; j++) {
      _state[i][j] = false;
    }
  }

  _lc.shutdown(0, false);
  // TODO: Get from settings
  _lc.setIntensity(0, 2);
  clearDisplay();
}

DisplayController::PixelCoords DisplayController::resolvePixel(uint8_t row, uint8_t column) const {
  DisplayController::PixelCoords pixel;
  pixel.row = row % _displaySize;
  pixel.column = column % _displaySize;

  uint8_t displayX = row / _displaySize;
  uint8_t displayY = column / _displaySize;

  pixel.display = _horizontalDisplayCount * displayY + displayX;

  return pixel;
}

void DisplayController::setPixel(uint8_t row, uint8_t column, bool value) {
  _state[column][row] = value;
}

void DisplayController::setPixels(Pixel pixels[], uint32_t length) {
  for (uint32_t i = 0; i < length; i++) {
    Pixel pixel = pixels[i];
    _state[pixel.y][pixel.x] = pixel.value;
  }
}

void DisplayController::draw() {
  for (uint8_t i = 0; i < _height; i++) {
    for (uint8_t j = 0; j < _width; j++) {
      DisplayController::PixelCoords pixel = resolvePixel(i, j);
      _lc.setLed(pixel.display, pixel.row, pixel.column, _state[i][j]);
    }
  }
}

void DisplayController::clearDisplay() {
  for (uint8_t i = 0; i < _displayCount; i++) {
    _lc.clearDisplay(i);
  }
}