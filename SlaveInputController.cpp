#include "SlaveInputController.hpp"
#include "Direction.hpp"

SlaveInputController::SlaveInputController() {
}

SlaveInputController::SlaveInputController(InputController& inputController)
  : _inputController{
      inputController
    } {
}

SlaveInputController& SlaveInputController::operator=(
  SlaveInputController const& other
) {
  if (&other == this) {
    return *this;
  }

  _inputController = other._inputController;
  return *this;
}

uint8_t SlaveInputController::encode(InputBundle inputBundle) {
  uint8_t binaryForm = inputBundle.direction.getDirection();
  binaryForm = binaryForm << 1;
  if (inputBundle.isPrimaryBtnPressed) {
    binaryForm = binaryForm | 1;
  }

  binaryForm = binaryForm << 1;
  if (inputBundle.isSecondaryBtnPressed) {
    binaryForm = binaryForm | 1;
  }

  return binaryForm;
}

InputBundle SlaveInputController::decode(uint8_t binaryForm) {
  InputBundle result;
  uint8_t binaryFormCopy = binaryForm;

  result.isSecondaryBtnPressed = binaryFormCopy & 1;
  binaryFormCopy = binaryFormCopy >> 1;

  result.isPrimaryBtnPressed = binaryFormCopy & 1;
  binaryFormCopy = binaryFormCopy >> 1;

  result.direction = Direction(binaryFormCopy);

  return result;
}

void SlaveInputController::sendBundle() {
  Direction direction = _inputController.getJoyDirection(false);
  bool isPrimaryBtnPressed = _inputController.isPrimaryBtnPressed();
  bool isSecondaryBtnPressed = _inputController.isSecondaryBtnPressed();

  uint8_t binaryForm = encode(InputBundle{
    direction, isPrimaryBtnPressed, isSecondaryBtnPressed });
}

InputBundle SlaveInputController::getBundle() {
  if (Serial.available() > 0) {
    uint8_t input = Serial.read();
    
    return decode(input);
  }

  return InputBundle{ Direction(), false, false };
}