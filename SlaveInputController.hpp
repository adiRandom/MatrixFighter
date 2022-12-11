#ifndef SLAVE_INPUT_CONTROLLER_HPP
#define SLAVE_INPUT_CONTROLLER_HPP

#include "Utils.h"
#include "InputController.hpp"

class SlaveInputController {

private:
  InputController _inputController;
  uint8_t encode(InputBundle inputBundle);
  InputBundle decode(uint8_t binaryForm);

public:
  SlaveInputController(InputController& inputController);
  SlaveInputController();
  SlaveInputController& operator=(SlaveInputController const& other);
  void sendBundle();
  InputBundle getBundle();
};

#endif