#include "game.h"


// The time of the game start
unsigned long int startMillis;

// Level Difficulty and Move
byte baseLevel;
const int millisForDiffIncrease = 7000;
unsigned long int lastLevelMove;
const int baseMoveMillis = 300;
int millisForLevelMove;
bool dontMoveLevel;
int customLevelMillis;
bool playerIsDead;
unsigned long int timeSpentDead;
unsigned long int deathTime;
bool hardMode;

// Matrix and Level
const byte displayHeight = 16;
const byte displayWidth = 8;
const byte levelHeight = 24;
const byte levelWidth = 8;
const byte displayOffset = levelHeight - displayHeight;
byte level[levelHeight][levelWidth];

// Obstacles
const byte noOfObstacles = 24;
const byte obstacles[noOfObstacles][6] = {    
  {1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1},
  
  {1, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 1},
  
  {1, 0, 0, 0, 0, 1},
  {0, 0, 1, 1, 0, 0},

  {0, 0, 0, 1, 1, 1},
  {0, 1, 0, 0, 1, 0},
  
  {2, 0, 0, 0, 0, 0},
  {1, 1, 1, 0, 0, 0},
  
  {1, 0, 0, 0, 1, 1},
  {1, 1, 0, 0, 0, 1},
  
  {1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1},

  {0, 1, 1, 1, 1, 0},
  {5, 0, 0, 0, 0, 0},
  
  {1, 1, 0, 0, 1, 1},
  {4, 0, 0, 0, 0, 0},  

  {3, 0, 0, 0, 0, 0},
  {6, 0, 0, 0, 0, 0},
  
  {1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1},
  
  {1, 0, 0, 1, 1, 1},
  {1, 1, 1, 0, 0, 1}

};

const byte noOfMovingObstacles = 5;
const bool movingObstacles[noOfMovingObstacles][6][6] = { // the moving objects have 6 frames
  { // 1
    {1, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0},
    {1, 1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0}
  },
  { // 2
    {1, 1, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 1, 1},
    {1, 0, 0, 0, 0, 1}
  },
  { // 3
    {1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 1},
    {0, 0, 1, 1, 1, 0},
    {0, 1, 1, 1, 0, 0}
  },
  { // 4
    {1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 1, 1},
    {1, 1, 0, 0, 1, 1},
    {1, 1, 0, 0, 1, 1}
  },
  { // 5
    {0, 0, 0, 0, 1, 1},
    {0, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 0},
    {1, 1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 1}
  }
};

unsigned long int lastMillisForMovingObstacles[noOfMovingObstacles];

const int millisForMovingObstacles[noOfMovingObstacles][6] = {
  {200, 200, 500, 200, 200, 300},
  {200, 200, 200, 200, 200, 200},
  {800, 400, 400, 800, 400, 400},
  {200, 200, 200, 200, 200, 200},
  {200, 200, 200, 200, 200, 200}
};

// represents the current configuration for each moving obstacle
byte movingObstaclesFrames[noOfMovingObstacles];

byte lastObstacleY;
byte currentObstacle;

// Player
byte playerPos[2];
unsigned long int lastPlayerShowChange;
const byte playerShowBlink = 0; // I want to blink it as fast as it can because the loop already takes a really long time (around 60 ms). I think i have to stop with the game complexity here... :(
bool showPlayer;
int currentXMove;
unsigned long int lastPlayerXMove;
const int baseMillisForXMove = 100;
int millisForXMove;

// ---------------------------------------------------------------------------------------------- functions

unsigned long int gameMillis(){
  return millis() - startMillis - timeSpentDead;
}

void displayRowCol(byte row, byte col, bool mode){
  int matrixNo = row / 8;
  lc.setLed(matrixNo, row % 8, 7 - col, mode);
}

void displayLevel(){
  for (byte row = 0; row < displayHeight; row++){
    byte matrixRow = row + displayOffset;
    for (int col = 1; col < displayWidth - 1; col++){ // we only need to display the left and right borders once, at the beginning of the game
      if (matrixRow == playerPos[0] && col == playerPos[1]){
        continue;
      }
      if (level[matrixRow][col] == 0 || level[matrixRow][col] == 1){
        displayRowCol(row, col, level[matrixRow][col]);
      }
      else {
        displayRowCol(row, col, 0);
      }
    }
  }
}

void displayPlayer(){
  if (gameMillis() - lastPlayerShowChange >= playerShowBlink){ // blink
    showPlayer = !showPlayer;
    lastPlayerShowChange = gameMillis();
  }
  displayRowCol(playerPos[0] - displayOffset, playerPos[1], showPlayer);
}

bool readJoyStickButton(){
  bool buttonVal = digitalRead(joystickButtonPin);
  if (buttonVal == 0 && lastButtonVal == 1){ // it is inversed because of INPUT_PULLUP (here it is pressed)
    lastButtonVal = 0;
    return 1;
  }
  if (buttonVal == 1 && lastButtonVal == 0){ // not pressed
    lastButtonVal = 1;
  }
  return 0;
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
  if (xAxis <= highAxis - 200 && xAxis >= lowAxis + 200){
    xAxisReset = 1;
  }
  return 0;
}

int readJoyStickY(){
  int yAxis = analogRead(yPin);
  if (yAxis > highAxis && yAxisReset){
    yAxisReset = 0;
    return -1; // Y axis is reversed for some reason
  }
  else if (yAxis < lowAxis && yAxisReset){
    yAxisReset = 0;
    return 1;
  }
  if (yAxis <= highAxis - 200 && yAxis >= lowAxis + 200){
    yAxisReset = 1;
  }
  return 0;
}

void movePlayer(int xDir){
  bool shouldMove = 0;
  if (playerIsDead){
    shouldMove = 1;
  }
  else if (gameMillis() - lastPlayerXMove > millisForXMove){
    shouldMove = 1;
  }
  if (shouldMove){
    if (level[playerPos[0]][playerPos[1] + xDir] == 0){ // note that there is no collision when moving sideways, the player dies only when something smashes his face
      playerPos[1] += xDir;
      lastPlayerXMove = gameMillis();
    }
  }
}

void updateXDirAndMovePlayer(){ // check if the moving direction has changed and then move
  int xAxis = readJoyStickX();
  if (playerIsDead){
    currentXMove = 0;
    movePlayer(xAxis);
    return;
  }
  if (xAxis != 0){
    currentXMove = xAxis;
  }
  movePlayer(currentXMove);
}

int playerGotSmashed(){
  if (playerIsDead){
    return 0; // you can't die while being dead...
  }
  if (level[playerPos[0]][playerPos[1]] >= 1){
    return 1;
  }
  return 0;
}

int enoughSpaceToStart(){ // after the player dies we want to stop only if there is enough free space ahead
  for (byte row = playerPos[0] - 3; row <= playerPos[0]; row++){ // 2 free lines
    for (byte col = max(1, playerPos[0] - 1); col < min(levelWidth - 1, playerPos[0] + 2); col++){ // only checks nearby blocks
      if (level[row][col] == 1){
        return 0;
      }
    }
  }
  return 1;
}

void slowAndStop(){ // used when the player dies
  if (customLevelMillis <= 700){
    customLevelMillis += 100;
  }
  if (enoughSpaceToStart()){
    dontMoveLevel = 1;
  }
}

void moveLevel(){
  if (dontMoveLevel){
    return 0;
  }
  bool shouldMove = 0;
  if (playerIsDead){
    if (gameMillis() - lastLevelMove >= customLevelMillis){
      shouldMove = 1;
      slowAndStop();
    }
  }
  else if (gameMillis() - lastLevelMove >= millisForLevelMove){
    shouldMove = 1;
  }
  if (shouldMove){
    for (byte i = levelHeight - 1; i > 0; i--){ // the top of the matrix is always clean
      for (byte j = 0; j < levelWidth - 1; j++){
        level[i][j] = level[i - 1][j];
      }
    }
    lastObstacleY++;
    lastLevelMove = gameMillis();
    currentScore += diffLevel + hardMode; // increasing the score each time the level moves
  }
}

void moveMovingObstacles(){
  for (byte currentMovingObstacle = 0; currentMovingObstacle < noOfMovingObstacles; currentMovingObstacle++){ // all the moving obstacles of type n-th have the same configuration
    if (millis() - lastMillisForMovingObstacles[currentMovingObstacle] >= millisForMovingObstacles[currentMovingObstacle][movingObstaclesFrames[currentMovingObstacle]]){ // if enough time has passed since the last object move
      movingObstaclesFrames[currentMovingObstacle] += 1;
      if (movingObstaclesFrames[currentMovingObstacle] >= 6){
        movingObstaclesFrames[currentMovingObstacle] = 0;
      }
      lastMillisForMovingObstacles[currentMovingObstacle] = millis();
    }
  }
  for (byte row = 0; row < levelHeight; row++){
    if (level[row][0] >=2){ // moving obstacle
      byte currentMovingObstacle = level[row][0] - 2;
      for (byte col = 1; col < 7; col++){
        level[row][col] = movingObstacles[currentMovingObstacle][movingObstaclesFrames[currentMovingObstacle]][col - 1];
      }
    }
  }
}

byte chooseObstacle(){
   return random(min(8,max(0, 2 * diffLevel - maxLevel)), min(noOfObstacles, 2 * diffLevel));
}

void spawnObstacle(){
  byte obstaclePause = min(currentObstacle / 4 + 3, 6) - hardMode;
  int lineOfObstacle = lastObstacleY - obstaclePause;
  if (lineOfObstacle > 0){ // we want to keep the first line clean so we copy only 0s when we move the level down
    if (obstacles[currentObstacle][0] >= 2){ // moving obstacle
      byte currentMovingObstacle = obstacles[currentObstacle][0] - 2;
      for (byte j = 1; j < displayWidth - 1; j++){
        level[lineOfObstacle][j] = movingObstacles[currentMovingObstacle][movingObstaclesFrames[currentMovingObstacle]][j - 1];
      }
      level[lineOfObstacle][0] = obstacles[currentObstacle][0];
    }
    else {
      for (byte j = 1; j < displayWidth - 1; j++){
        level[lineOfObstacle][j] = obstacles[currentObstacle][j - 1];
      }
    }
    lastObstacleY = lineOfObstacle;
    currentObstacle = chooseObstacle();
  }
}

void updateDifficulty() {
  if (playerIsDead){
    return;
  }
  byte aux = gameMillis() / millisForDiffIncrease + baseLevel;
  if (aux > maxLevel){
    return;
  }
  diffLevel = aux;
  if (diffLevel == maxLevel){
    aux = 12;
    hardMode = 1;
  }
  millisForLevelMove = baseMoveMillis - aux * 20;
  millisForXMove = baseMillisForXMove - aux;
}

void updatePlayerStatus(){
  // updates the dead / alive status and changes values accordingly
  if (playerGotSmashed()){
    deathTime = millis();
    playerIsDead = 1;
    customLevelMillis = 500;
    currentLives--;
    if (currentLives == 0){
      gameOver = 1;
    }
  }
  if (dontMoveLevel){
    if (readJoyStickButton()){ // revive
      playerIsDead = 0;
      dontMoveLevel = 0;
      timeSpentDead += millis() - deathTime;
    }
  }
}

void gameSetup(byte startingLevel){
  randomSeed(analogRead(2)); // pin no 2 is NOT used
  for (byte i = 0; i < levelHeight;  i++){
    level[i][0] = level[i][levelWidth - 1] = 1;
    displayRowCol(i, 0, 1);
    displayRowCol(i, levelWidth - 1, 1);
    for (byte j = 1; j < levelWidth - 1; j++){
      level[i][j] = 0;
    }
  }
  
  diffLevel = baseLevel = startingLevel;
  hardMode = 0;
  millisForLevelMove = baseMoveMillis;
  dontMoveLevel = 0;
  customLevelMillis = 0;
  playerIsDead = 0;
  timeSpentDead = 0;
  deathTime = 0;
  
  showPlayer = 1;
  currentXMove = -1;
  millisForXMove = baseMillisForXMove;
  currentLives = 3;
  gameOver = 0;
  playerPos[0] = levelHeight - 2;
  playerPos[1] = 1;
  currentScore = 0;
  
  lastObstacleY = 15;
  currentObstacle = chooseObstacle();

  xAxisReset = 1;
  lastButtonVal = 1;
    
  startMillis = millis();
}

void iterateGame(){
  displayLevel();
  displayPlayer();
  moveLevel();
  moveMovingObstacles();
  updateXDirAndMovePlayer();
  spawnObstacle();
  updateDifficulty();
  updatePlayerStatus();
}
