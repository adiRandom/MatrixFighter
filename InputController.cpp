#include "InputController.hpp"
#include <Arduino.h>

InputController::InputController()
  : _joyXAxisPin{ 0 },
    _joyYAxisPin{ 0 },
    _switchedAxes{ false },
    _invertedXAxis{ false },
    _invertedYAxis{ false },
    _throttleTime{ DEFAULT_THROTTLE_TIME } {
}

InputController::InputController(
  uint8_t joyXAxisPin,
  uint8_t joyYAxisPin,
  bool switchedAxes = false,
  bool invertedXAxis = false,
  bool invertedYAxis = false,
  uint32_t throttleTime = DEFAULT_THROTTLE_TIME
)
  : _joyXAxisPin{ joyXAxisPin },
    _joyYAxisPin{ joyYAxisPin },
    _switchedAxes{ switchedAxes },
    _invertedXAxis{ invertedXAxis },
    _invertedYAxis{ invertedYAxis },
    _throttleTime{ throttleTime } {
}

int8_t InputController::getJoyValueOnAxis(uint8_t pin) const {
  uint16_t value = analogRead(pin);

  if (value >= POSITIVE_THRESHOLD) {
    return 1;
  } else if (value <= NEGATIVE_THRESHOLD) {
    return -1;
  } else {
    return 0;
  }
}

Direction InputController::getJoyDirection(bool throttled = false) {
  if (throttled) {
    uint32_t now = millis();

    if (now - _throttleTimer <= _throttleTime) {
      return Direction(0, 0);
    } else {
      _throttleTimer = now;
    }
  }
  int8_t xAxisVal = getJoyValueOnAxis(_joyXAxisPin);
  int8_t yAxisVal = getJoyValueOnAxis(_joyYAxisPin);
  Direction direction = Direction(xAxisVal, yAxisVal);
  return getDirectionWithInversions(direction);
}

Direction InputController::getDirectionWithInversions(Direction direction) const {
  Direction finalDirection = direction;

  if (_switchedAxes) {
    finalDirection.switchAxes();
  }

  if (_invertedXAxis) {
    finalDirection.invertXAxis();
  }

  if (_invertedYAxis) {
    finalDirection.invertYAxis();
  }

  return finalDirection;
}


InputController& InputController::operator=(InputController const& other) {

  if (this == &other) {
    return *this;
  }

  _joyXAxisPin = other._joyXAxisPin;
  _joyYAxisPin = other._joyYAxisPin;
  _switchedAxes = other._switchedAxes;
  _invertedXAxis = other._invertedXAxis;
  _invertedYAxis = other._invertedYAxis;

  return *this;
}

void InputController::setThrotthleTime(uint32_t throttleTime) {
  _throttleTime = throttleTime;
}