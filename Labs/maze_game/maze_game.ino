#include "LedControl.h"

LedControl lc = LedControl(13, 12, 11, 1); //DIN, CLK, LOAD, No of drivers

const int xPin = A0;
const int yPin = A1;
const int highAxis = 800;
const int lowAxis = 200;
int xAxisReset = 1;
int yAxisReset = 1;

int level[8][8] = {
  {2, 1, 0, 0, 0, 0, 0, 0},
  {0, 1, 2, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 1, 1, 1, 0},
  {0, 0, 0, 0, 1, 0, 1, 0},
  {0, 0, 1, 1, 1, 0, 1, 0},
  {0, 0, 1, 0, 0, 0, 1, 0},
  {0, 0, 1, 1, 0, 0, 1, 0},
  {2, 0, 0, 0, 0, 0, 1, 0}
};

int playerPos[2] = {7, 4};
unsigned long int lastPlayerShowChange = 0;
const int playerShowBlink = 500;
int showPlayer = 1;

const int noOfBombs = 3;
unsigned long int lastBombShowChange = 0;
const int bombShowBlink = 100;
int showBombs = 1;

void gameOver(){
  for (int row = 0; row < 8; row++){
    for (int col = 0; col < 8; col++){
      lc.setLed(0, 7 - col, row, true); // drive no, col, row, true - turns on the led
    }
  }
  while (1){
  }
}

int readJoyStickX(){
  int xAxis = analogRead(xPin);
  if (xAxis > highAxis && xAxisReset){
    xAxisReset = 0;
    return 1;
  }
  else if (xAxis < lowAxis && xAxisReset){
    xAxisReset = 0;
    return -1;
  }
  if (xAxis <= highAxis && xAxis >= lowAxis){
    xAxisReset = 1;
  }
  return 0;
}

int readJoyStickY(){
  int yAxis = analogRead(yPin);
  if (yAxis > highAxis && yAxisReset){
    yAxisReset = 0;
    return 1;
  }
  else if (yAxis < lowAxis && yAxisReset){
    yAxisReset = 0;
    return -1;
  }
  if (yAxis <= highAxis && yAxis >= lowAxis){
    yAxisReset = 1;
  }
  return 0;
}

void movePlayer(){
  int xDir = readJoyStickX();
  int yDir = readJoyStickY();
  if (level[playerPos[0]][playerPos[1] + xDir] == 2){
    gameOver(); 
  }
  else if (level[playerPos[0]][playerPos[1] + xDir] == 0 && playerPos[1] + xDir >= 0 && playerPos[1] + xDir <= 7){
    playerPos[1] += xDir;
  }
  
  if (level[playerPos[0] + yDir][playerPos[1]] == 2){
    gameOver(); 
  }
  else if (level[playerPos[0] + yDir][playerPos[1]] == 0 && playerPos[0] + yDir >= 0 && playerPos[0] + yDir <= 7){
    playerPos[0] += yDir;
  }
}

void displayLevel(){
  for (int row = 0; row < 8; row++){
    for (int col = 0; col < 8; col++){
      if (level[row][col] == 0 || level[row][col] == 1){
        lc.setLed(0, 7 - col, row, level[row][col]); // drive no, col, row, true - turns on the led
      }
      else if (level[row][col] == 2){
        lc.setLed(0, 7 - col, row, showBombs);
      }
      else {
        lc.setLed(0, 7 - col, row, 0);
      }
    }
  }
}

void displayPlayer(){
  if (millis() - lastPlayerShowChange >= playerShowBlink){
    showPlayer = !showPlayer;
    lastPlayerShowChange = millis();
  }
  lc.setLed(0, 7 - playerPos[1], playerPos[0], showPlayer);
}

void changeBombsDisplayState(){
  if (millis() - lastBombShowChange >= bombShowBlink){
    showBombs = !showBombs;
    lastBombShowChange = millis();
  }
}

void clearMatrix(){
  for (int row = 0; row < 8; row++){
    for (int col = 0; col < 8; col++){
      lc.setLed(0, 7 - col, row, false);
    }
  }
}

void setup(){
  lc.shutdown(0, false);
  lc.setIntensity(0, 2); // 0 15
  lc.clearDisplay(0);
//  Serial.begin(9600);

//  displayLevel();
//  displayLevel();
//  displayLevel();
}

void loop(){
//  Serial.println(level[0][1]);
//  displayLevel();
//  delay(3000);
//  changeBombsDisplayState();
//  displayLevel();
//  clearMatrix();
//  displayLevel();
//  delay(1000);
//  displayPlayer();
//  movePlayer();



  for (int row = 0; row < 8; row++){
    for (int col = 0; col < 8; col++){
      lc.setLed(0, 7 - col, row, true);
      delay(50);
    }
  }
  for (int row = 0; row < 8; row++){
    for (int col = 0; col < 8; col++){
      lc.setLed(0, 7 - col, row, false);
      delay(50);
    }
  }
  
}
