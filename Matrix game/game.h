#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "LedControl.h"
LedControl lc = LedControl(12, 11, 10, 2); //DIN, CLK, LOAD, No of drivers

// Player status
bool gameOver;
byte diffLevel;
int currentScore;
byte currentLives;

// Level
const byte maxLevel = 12;
void displayRowCol(byte row, byte col, bool mode);

// Joystick
const int xPin = A0;
const int yPin = A1;
const int joystickButtonPin = A2;
const int highAxis = 800;
const int lowAxis = 200;
bool xAxisReset;
bool yAxisReset;
bool lastButtonVal;

// Joystick
bool readJoyStickButton();// the readJoyStick functions will be used in the menu too
int readJoyStickX();
int readJoyStickY();

// Running the game
void gameSetup(byte startingLevel);
void iterateGame();

#endif // GAME_H_INCLUDED
