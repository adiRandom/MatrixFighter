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

  initStateAndNextFrame(nullptr);
  initDisplay();
}

DisplayController::~DisplayController() {
  cleanup();
}

DisplayController::DisplayController()
  : _dataPin{ 0 },
    _loadPin{ 0 },
    _clkPin{ 0 },
    _width{ 0 },
    _height{ 0 },
    _horizontalDisplayCount{ 0 },
    _lc{
      LedControl(_dataPin, _clkPin, _loadPin, _displayCount)
    },
    _displaySize{ 0 },
    _displayCount{ 0 },
    _state{ nullptr },
    _nextFrame{nullptr} {
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

void DisplayController::setPixel(Pixel pixel) {
  _nextFrame[pixel.y][pixel.x] = pixel.value;
}

void DisplayController::setPixels(Pixel pixels[], uint32_t length) {
  for (uint32_t i = 0; i < length; i++) {
    Pixel pixel = pixels[i];
    _nextFrame[pixel.y][pixel.x] = pixel.value;
  }
}

void DisplayController::setPixels(List<Pixel> pixels) {
  List<Pixel>::Node* node = pixels.getHead();
  while (node != nullptr) {
    setPixel(node->value);
    node = node->next;
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

void DisplayController::initDisplay() {
  for (uint8_t i = 0; i < _displayCount; i++) {
    _lc.shutdown(i, false);
    // TODO: Get from settings
    _lc.setIntensity(i, 2);
    _lc.clearDisplay(i);
  }
}

DisplayController& DisplayController::operator=(DisplayController const& other) {
  if (&other == this) {
    return *this;
  }

  _dataPin = other._dataPin;
  _loadPin = other._loadPin;
  _clkPin = other._clkPin;
  _width = other._width;
  _height = other._height;
  _horizontalDisplayCount = other._horizontalDisplayCount;
  _displaySize = other._displaySize;
  _displayCount = other._displayCount;
  _lc = LedControl(_dataPin, _clkPin, _loadPin, _displayCount);

  cleanup();
  initStateAndNextFrame(&other);
  initDisplay();
  return *this;
}

DisplayController::DisplayController(DisplayController const& other)
  : _dataPin{ other._dataPin },
    _loadPin{ other._loadPin },
    _clkPin{ other._clkPin },
    _width{ other._width },
    _height{ other._height },
    _horizontalDisplayCount{ other._horizontalDisplayCount },
    _lc{
      LedControl(other._dataPin, other._clkPin, other._loadPin, other._displayCount)
    },
    _displaySize{ other._displaySize },
    _displayCount{ other._displayCount } {

  initStateAndNextFrame(&other);
  initDisplay();
}

void DisplayController::cleanup() {

  if (_state == nullptr || _nextFrame == nullptr) {
    return;
  }

  for (int i = 0; i < _width; i++) {
    delete[] _state[i];
    delete[] _nextFrame[i];
  }

  delete[] _state;
  delete[] _nextFrame;

  _state = nullptr;
  _nextFrame = nullptr;
}

void DisplayController::initStateAndNextFrame(DisplayController* initValue = nullptr) {
  _state = new bool*[_height];
  for (int i = 0; i < _height; i++) {
    _state[i] = new bool[_width];
    for (int j = 0; j < _width; j++) {
      _state[i][j] = initValue != nullptr ? initValue->_state[i][j] : false;
    }
  }

  _nextFrame = new bool*[_height];
  for (int i = 0; i < _height; i++) {
    _nextFrame[i] = new bool[_width];
    for (int j = 0; j < _width; j++) {
      _nextFrame[i][j] = initValue != nullptr ? initValue->_nextFrame[i][j] : false;
    }
  }
}

void DisplayController::emptyNextFrame() {
  for (int i = 0; i < _height; i++) {
    for (int j = 0; j < _width; j++) {
      _nextFrame[i][j] = false;
    }
  }
}

void DisplayController::commitNextFrame() {
  for (int i = 0; i < _height; i++) {
    for (int j = 0; j < _width; j++) {
      _state[i][j] = _nextFrame[i][j];
    }
  }
  
  draw();
  emptyNextFrame();
}

uint8_t DisplayController::getHeight() const {
  return _height;
}