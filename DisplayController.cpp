#include "DisplayController.hpp"

DisplayController::DisplayController(
  uint8_t dataPin,
  uint8_t loadPin,
  uint8_t clkPin
)
  : _dataPin{ dataPin },
    _loadPin{ loadPin },
    _clkPin{ clkPin },
    _lc{
      LedControl(dataPin, clkPin, loadPin, DISPLAY_COUNT)
    } {

  initStateAndNextFrame(nullptr);
  initDisplay();
}

DisplayController::DisplayController()
  : _dataPin{ 0 },
    _loadPin{ 0 },
    _clkPin{ 0 },
    _lc{
      LedControl(_dataPin, _clkPin, _loadPin, DISPLAY_COUNT)
    },
    _frameBuffer{ nullptr } {
}

DisplayController::PixelCoords DisplayController::resolvePixel(uint8_t row, uint8_t column) const {
  DisplayController::PixelCoords pixel;
  pixel.row = row % DISPLAY_SIZE;
  pixel.column = column % DISPLAY_SIZE;

  uint8_t displayX = row / DISPLAY_SIZE;
  uint8_t displayY = column / DISPLAY_SIZE;

  pixel.display = displayY + displayX;

  return pixel;
}

void DisplayController::setPixel(Pixel pixel) {
  _frameBuffer[pixel.y][pixel.x] = pixel.value;
  _shouldRedraw = true;
}

void DisplayController::setPixels(Pixel pixels[], uint32_t length) {
  for (uint32_t i = 0; i < length; i++) {
    Pixel pixel = pixels[i];
    _frameBuffer[pixel.y][pixel.x] = pixel.value;
  }
  _shouldRedraw = true;
}

void DisplayController::draw() {
  if (!_shouldRedraw) {
    return;
  }

  for (uint8_t i = 0; i < DISPLAY_HEIGHT; i++) {
    for (uint8_t j = 0; j < DISPLAY_WIDTH; j++) {
      DisplayController::PixelCoords pixel = resolvePixel(i, j);
      _lc.setLed(pixel.display, pixel.row, pixel.column, _frameBuffer[i][j]);
    }
  }

  _shouldRedraw = false;
}

void DisplayController::initDisplay() {
  uint8_t intensity = _settingsStorage.getMatrixBrightnessLv();
  setIntensityLv(intensity);

  for (uint8_t i = 0; i < DISPLAY_COUNT; i++) {
    _lc.shutdown(i, false);
    _lc.clearDisplay(i);
  }
}

void DisplayController::setIntensityLv(uint8_t intensityLv) {
  for (uint8_t i = 0; i < DISPLAY_COUNT; i++) {
    _lc.setIntensity(i, intensityLv * MATRIX_BRIGHT_FACTOR);
  }
}

DisplayController& DisplayController::operator=(DisplayController const& other) {
  if (&other == this) {
    return *this;
  }

  _dataPin = other._dataPin;
  _loadPin = other._loadPin;
  _clkPin = other._clkPin;
  _lc = LedControl(_dataPin, _clkPin, _loadPin, DISPLAY_COUNT);
  _shouldRedraw = other._shouldRedraw;

  initStateAndNextFrame(&other);
  initDisplay();
  return *this;
}

DisplayController::DisplayController(DisplayController const& other)
  : _dataPin{ other._dataPin },
    _loadPin{ other._loadPin },
    _clkPin{ other._clkPin },
    _lc{
      LedControl(other._dataPin, other._clkPin, other._loadPin, DISPLAY_COUNT)
    },
    _shouldRedraw{ other._shouldRedraw } {
  initStateAndNextFrame(&other);
  initDisplay();
}

void DisplayController::initStateAndNextFrame(DisplayController* initValue = nullptr) {
  for (int i = 0; i < DISPLAY_HEIGHT; i++) {
    for (int j = 0; j < DISPLAY_WIDTH; j++) {
      _frameBuffer[i][j] = initValue != nullptr ? initValue->_frameBuffer[i][j] : false;
    }
  }
}

void DisplayController::emptyNextFrame() {
  for (int i = 0; i < DISPLAY_HEIGHT; i++) {
    for (int j = 0; j < DISPLAY_WIDTH; j++) {
      _frameBuffer[i][j] = false;
    }
  }
}

void DisplayController::commitNextFrame() {
  draw();
  emptyNextFrame();
}

void DisplayController::clear() {
  emptyNextFrame();
  _shouldRedraw = true; 
  commitNextFrame();
}
