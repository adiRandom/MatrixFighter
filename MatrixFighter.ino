#include "DisplayController.hpp"

uint8_t const DISPLAY_DATA_PIN = 12;
uint8_t const DISPLAY_CLK_PIN = 11;
uint8_t const DISPLAY_LOAD_PIN = 10;
uint8_t const DISPLAY_SIZE = 8;
uint8_t const DISPLAY_WIDTH = 8;
uint8_t const DISPLAY_HEIGHT = 8;
uint8_t const HORIZONTAL_DISPLAY_COUNT = 1;
uint8_t const DISPLAY_COUNT = 1;

DisplayController displayController;

void setup() {
  displayController = DisplayController(
    DISPLAY_DATA_PIN,
    DISPLAY_LOAD_PIN,
    DISPLAY_CLK_PIN,
    DISPLAY_WIDTH,
    DISPLAY_HEIGHT,
    HORIZONTAL_DISPLAY_COUNT,
    DISPLAY_COUNT,
    DISPLAY_SIZE
  );

  displayController.setPixel(2, 3, true);
}

void loop() {
  displayController.draw();
}