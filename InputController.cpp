#include "InputController.hpp"
#include <Arduino.h>

InputController::InputController(
  uint8_t joyXAxisPin,
  uint8_t joyYAxisPin,
  bool switchedAxes = false,
  bool invertedXAxis = false,
  bool invertedYAxis = false
)
  : _joyXAxisPin{ joyXAxisPin },
    _joyYAxisPin{ joyYAxisPin },
    _switchedAxes{ switchedAxes },
    _invertedXAxis{ invertedXAxis },
    _invertedYAxis{ invertedYAxis } {
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

Direction InputController::getJoyDirection() const {
  int8_t xAxisVal = getJoyValueOnAxis(_joyXAxisPin);
  int8_t yAxisVal = getJoyValueOnAxis(_joyYAxisPin);

  Direction direction = Direction(xAxisVal, yAxisVal);

  return getDirectionWithInversions(direction);
}

Direction InputController::getDirectionWithInversions(Direction direction) const {
  Direction finalDirection = direction;

  if(_switchedAxes){
    finalDirection.switchAxes();
  }

  if(_invertedXAxis){
    finalDirection.invertXAxis();
  }

  if(_invertedYAxis){
    finalDirection.invertYAxis();
  }

  return finalDirection;
}