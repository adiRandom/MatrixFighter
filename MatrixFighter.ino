#include "DisplayController.hpp"
#include "Character.hpp"
#include "Utils.h"
#include "InputController.hpp"
#include "GameManager.hpp"
#include "LCDController.hpp"
#include "DisplayConstants.h"
#include "SlaveInputController.hpp"
// #include <Wire.h>

uint8_t const DISPLAY_DATA_PIN = 12;
uint8_t const DISPLAY_CLK_PIN = 11;
uint8_t const DISPLAY_LOAD_PIN = 10;
uint8_t const HORIZONTAL_DISPLAY_COUNT = 1;
uint8_t const JOYSTICK_X_PIN = A0;
uint8_t const JOYSTICK_Y_PIN = A1;
uint8_t const PRIMARY_BTN_PIN = 13;
uint8_t const SECONDARY_BTN_PIN = A2;
uint8_t const LCD_RS_PIN = 9;
uint8_t const LCD_ENABLE_PIN = 8;
uint8_t const LCD_D4_PIN = 7;
uint8_t const LCD_D5_PIN = 6;
uint8_t const LCD_D6_PIN = 5;
uint8_t const LCD_D7_PIN = 4;
uint8_t const LCD_WIDTH = 16;
uint8_t const LCD_HEIGHT = 2;
uint8_t const LCD_CONTRAST_PIN = 3;
uint8_t const SLAVE_PIN = 2;
char const* GREETING = "HELLO BRAWLER!";

bool isSlave = false;

DisplayController displayController;
InputController player1InputController(
  JOYSTICK_X_PIN,
  JOYSTICK_Y_PIN,
  PRIMARY_BTN_PIN,
  SECONDARY_BTN_PIN,
  true,
  false,
  true
);
Character player1;

InputController player2InputController(
  JOYSTICK_X_PIN,
  JOYSTICK_Y_PIN,
  PRIMARY_BTN_PIN,
  SECONDARY_BTN_PIN,
  false,
  true,
  true
);
Character player2;

SlaveInputController player2SlaveInputController(player2InputController);

GameManager gameManager;
LCDController lcdController(
  LCD_RS_PIN,
  LCD_ENABLE_PIN,
  LCD_D4_PIN,
  LCD_D5_PIN,
  LCD_D6_PIN,
  LCD_D7_PIN,
  LCD_HEIGHT,
  LCD_WIDTH,
  LCD_CONTRAST_PIN
);


void setup() {
  Serial.begin(9600);

  pinMode(SLAVE_PIN, INPUT);

  displayController = DisplayController(
    DISPLAY_DATA_PIN,
    DISPLAY_LOAD_PIN,
    DISPLAY_CLK_PIN
  );

  player1 = Character(Point{ 0, 1 }, Character::Orientation::RIGHT);
  player2 = Character(Point{ DISPLAY_WIDTH - 1, 1 }, Character::Orientation::LEFT);

  gameManager = GameManager(displayController, player1, player1InputController, player2, player2SlaveInputController, lcdController);

  isSlave = digitalRead(SLAVE_PIN) == HIGH;

  // if (isSlave) {
  //   Wire.begin();
  // } else {
  //   Wire.begin(MAIN_ARDUINO_ADDRESS);
  //   Wire.onReceive(onI2CBytesReceived);
  // }
}

// void onI2CBytesReceived(int bytes) {
//   uint8_t input = Wire.read();
//   Serial.println(input);
//   player2SlaveInputController.onByteReceived(input);
// }

void loop() {
  Serial.println(isSlave);
  if (isSlave) {
    player2SlaveInputController.sendBundle();
  } else {
    gameManager.handleInput();
    gameManager.getNextFrame();
    gameManager.getLCDState(GREETING);
  }
}