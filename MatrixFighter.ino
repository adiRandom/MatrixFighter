#include "DisplayController.hpp"
#include "Character.hpp"
#include "Utils.h"
#include "InputController.hpp"
#include "GameManager.hpp"
#include "LCDController.hpp"
#include "Constants.h"
#include "Storage.hpp"


uint8_t const DISPLAY_DATA_PIN = 12;
uint8_t const DISPLAY_CLK_PIN = 11;
uint8_t const DISPLAY_LOAD_PIN = 10;
uint8_t const HORIZONTAL_DISPLAY_COUNT = 1;
uint8_t const P1_JOYSTICK_X_PIN = A0;
uint8_t const P1_JOYSTICK_Y_PIN = A1;
uint8_t const P1_PRIMARY_BTN_PIN = 13;
uint8_t const P1_SECONDARY_BTN_PIN = A2;
uint8_t const LCD_RS_PIN = 9;
uint8_t const LCD_ENABLE_PIN = 8;
uint8_t const LCD_D4_PIN = 7;
uint8_t const LCD_D5_PIN = 6;
uint8_t const LCD_D6_PIN = 5;
uint8_t const LCD_D7_PIN = 4;
uint8_t const LCD_WIDTH = 16;
uint8_t const LCD_HEIGHT = 2;
uint8_t const P2_JOYSTICK_X_PIN = A3;
uint8_t const P2_JOYSTICK_Y_PIN = A4;
uint8_t const P2_PRIMARY_BTN_PIN = 2;
uint8_t const P2_SECONDARY_BTN_PIN = A5;

uint8_t const LCD_BRIGHT_PIN = 3;

char const* GREETING = "HELLO BRAWLER!";

bool isSlave = false;

DisplayController displayController;
InputController player1InputController(
  P1_JOYSTICK_X_PIN,
  P1_JOYSTICK_Y_PIN,
  P1_PRIMARY_BTN_PIN,
  P1_SECONDARY_BTN_PIN,
  true,
  false,
  true
);
Character player1;

InputController player2InputController(
  P2_JOYSTICK_X_PIN,
  P2_JOYSTICK_Y_PIN,
  P2_PRIMARY_BTN_PIN,
  P2_SECONDARY_BTN_PIN,
  true,
  false,
  true
);
Character player2;

GameManager gameManager;
LCDController lcdController(
  LCD_RS_PIN,
  LCD_ENABLE_PIN,
  LCD_D4_PIN,
  LCD_D5_PIN,
  LCD_D6_PIN,
  LCD_D7_PIN,
  LCD_BRIGHT_PIN,
  LCD_HEIGHT,
  LCD_WIDTH
);


void setup() {
  pinMode(LCD_BRIGHT_PIN, OUTPUT);
  Serial.begin(9600);

  Storage storage;
  analogWrite(LCD_BRIGHT_PIN, LCD_BRIGHT_FACTOR * storage.getLCDBrightnessLv());

  displayController = DisplayController(
    DISPLAY_DATA_PIN,
    DISPLAY_LOAD_PIN,
    DISPLAY_CLK_PIN
  );

  player1 = Character(Point{ LEFT_PLAYER_X, CHARACTER_INITIAL_Y }, Character::Orientation::RIGHT, storage.getMaxHP());
  player2 = Character(Point{ RIGHT_PLAYER_X, CHARACTER_INITIAL_Y }, Character::Orientation::LEFT, storage.getMaxHP());

  gameManager = GameManager(displayController, player1, player1InputController, player2, player2InputController, lcdController);
}

void loop() {
  gameManager.handleInput();
  gameManager.getNextFrame();
  gameManager.getLCDState(GREETING);
  gameManager.runRoundTimer();
}