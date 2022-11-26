#ifndef INPUT_CONTROLLER_HPP
#define INPUT_CONTROLLER_HPP

#include <stdint.h>
#include "Direction.hpp"

// The thresholds of the joystick from which we consider the input as being on the positive semi-axis or on the negative
uint16_t const POSITIVE_THRESHOLD = 700;
uint16_t const NEGATIVE_THRESHOLD = 300;
uint8_t const INPUT_DEBOUNCE_TIME = 50;

class InputController {
private:
  uint8_t const _joyXAxisPin;
  uint8_t const _joyYAxisPin;
  bool const _switchedAxes;
  bool const _invertedXAxis;
  bool const _invertedYAxis;

  // @Returns 1 for the positive semi-axis,
  // -1 for the negative one,
  // 0 if neutral
  int8_t getJoyValueOnAxis(uint8_t pin) const;
  /**
   * Handle all the inverted axes and get the correct direction
   */
  Direction getDirectionWithInversions(Direction direction) const;

public:
  InputController(
    uint8_t joyXAxisPin,
    uint8_t joyYAxisPin,
    bool switchedAxis,
    bool invertedXAxis,
    bool invertedYAxis
  );

  Direction getJoyDirection() const;
};

#endif