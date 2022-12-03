#include "DisplayController.hpp"
#include "Character.hpp"
#include "Utils.h"
#include "InputController.hpp"
#include "GameManager.hpp"

uint8_t const DISPLAY_DATA_PIN = 12;
uint8_t const DISPLAY_CLK_PIN = 11;
uint8_t const DISPLAY_LOAD_PIN = 10;
uint8_t const DISPLAY_SIZE = 8;
uint8_t const DISPLAY_WIDTH = 8;
uint8_t const DISPLAY_HEIGHT = 8;
uint8_t const HORIZONTAL_DISPLAY_COUNT = 1;
uint8_t const DISPLAY_COUNT = 1;
uint8_t const PLAYER1_JOYSTICK_X_PIN = A0;
uint8_t const PLAYER1_JOYSTICK_Y_PIN = A1;
uint8_t const PLAYER1_PUNCH_PIN = 2;

DisplayController displayController;
InputController inputController(
  PLAYER1_JOYSTICK_X_PIN,
  PLAYER1_JOYSTICK_Y_PIN,
  PLAYER1_PUNCH_PIN,
  true,
  false,
  false
);
Character player1(Point{ 0, 1 });
GameManager gameManager;


void setup() {
  Serial.begin(9600);

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


  gameManager = GameManager(displayController, player1, inputController);
}

// void drawCharacter(Pixel pixel) {
//   displayController.setPixel(pixel.x, pixel.y, pixel.value);
//   Serial.println(pixel.y);
// }

void loop() {
  gameManager.handleInput();
  gameManager.getNextFrame();
}