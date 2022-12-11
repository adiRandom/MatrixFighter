#ifndef SLAVE_INPUT_CONTROLLER_HPP
#define SLAVE_INPUT_CONTROLLER_HPP

#include "Utils.h"
#include "InputController.hpp"

uint8_t const MAIN_ARDUINO_ADDRESS = 1;

class SlaveInputController {

private:
  InputBundle _lastBundle = InputBundle{ Direction(), false, false };

  InputController _inputController;
  uint8_t encode(InputBundle inputBundle);
  InputBundle decode(uint8_t binaryForm);

public:
  SlaveInputController(InputController& inputController);
  SlaveInputController();
  SlaveInputController& operator=(SlaveInputController const& other);
  void sendBundle();
  InputBundle getBundle();
  void onByteReceived(uint8_t byte);
};

#endif