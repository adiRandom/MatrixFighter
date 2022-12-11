#include "SlaveInputController.hpp"
#include "Direction.hpp"
#include <Wire.h>

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

  result.direction = Direction(binaryForm);

  return result;
}

void SlaveInputController::sendBundle() {
  Direction direction = _inputController.getJoyDirection(false);
  bool isPrimaryBtnPressed = _inputController.isPrimaryBtnPressed();
  bool isSecondaryBtnPressed = _inputController.isSecondaryBtnPressed();

  uint8_t binaryForm = encode(InputBundle{
    direction, isPrimaryBtnPressed, isSecondaryBtnPressed });

  Serial.println("Before");
  Serial.println(binaryForm);
  Wire.beginTransmission(MAIN_ARDUINO_ADDRESS);
  // Serial.println("After");

  Wire.write(binaryForm);  // sends x
  Wire.endTransmission();
  delay(500);
}

InputBundle SlaveInputController::getBundle() {
  return _lastBundle;
}

void SlaveInputController::onByteReceived(uint8_t byte) {
  _lastBundle = decode(byte);
}