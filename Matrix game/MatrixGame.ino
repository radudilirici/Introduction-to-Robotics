#include <LiquidCrystal.h>
#include <string.h>
#include <EEPROM.h>
#include "game.h"
#include "EEPROMAnything.h"

#define MAX_NAME_LEN 4

// LCD setup
const byte RSPin = 4;
const byte EPin = 5;
const byte D4Pin = 6;
const byte D5Pin = 7;
const byte D6Pin = 8;
const byte D7Pin = 9;
LiquidCrystal lcd(RSPin, EPin, D4Pin, D5Pin, D6Pin, D7Pin);

typedef struct player
{
  int pScore = 0;
  char pName[MAX_NAME_LEN + 1] = "UNKN";
};
player playerNo1, playerNo2, playerNo3;
bool highscoreNavStart;

// Menu setup and variables
unsigned long int lastDisplayMillis = 0;
byte displayDelay = 200;
byte startingLevelValue = 1;
unsigned long int startingTime = 0;
byte menuSelected = 1;
byte menuWasClicked = 0;

bool menuJustStarted = 1;
bool gameJustStarted = 1;
bool settingsJustStarted = 1;
bool highscoreJustStarted = 1;

bool menuChanged;
bool settingsChanged;
bool highscoreNavChanged;
bool alphabetChanged = 1;
bool changingName;
bool infoChanged = 1;

byte settingSelected = 1;
byte settingsDisplayStart = 1; // you can scroll in the settings section so I want to know what options to display
byte infoDisplayStart = 1; //same here

char currentPlayer[MAX_NAME_LEN + 1] = "AAAA";
byte selectedChar = 65;
byte charToReplace = 0;

unsigned long int lastMillisForAlphabet;
const int millisForAlphabet = 500;
bool characterShow = 1;

byte gameOverScreenNo;
bool gameOverScreenChanged;
bool gameOverSelection;

char gitLink[80] = " https://github.com/radudilirici/IntroductionToRobotics/tree/master/Matrix game";
byte gitOffset = 0;
const int millisForGitScroll = 500;
unsigned long int lastMillisForGitScroll;

byte howToSection = 1;
bool viewingHowTo;
bool howToChanged;

// ---------------------------------------- Bradut

byte bradut[16][8] = {
  {0, 2, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 2, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 2, 0},
  {0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1}
};

unsigned long int lastSnowChange;
const byte millisForSnowChange =  10;
bool snowStatus = 1;

unsigned long int lastSnowMove;
const int millisForSnowMove = 500;

void moveShowflakes(){
  if (millis() - lastSnowMove >= millisForSnowMove){
    for (byte i = 15; i > 0; i--){
      for (byte j = 0; j < 8; j++){
        if (bradut[i - 1][j] >= 2){
          byte dir = random() % 3;
          if (j + dir - 1 < 0 || j + dir - 1 > 7){
            dir = 1;
          }
          bradut[i][j + dir - 1] +=2;
          bradut[i - 1][j] -= 2;
        }
      }
    }
    for (byte j = 0; j < 8; j++){
      byte spawn = random() % 32;
      if (spawn == 0){
        bradut[0][j] = 2;
      }
    }
    for (byte j = 0; j < 8; j++){
      if (bradut[15][j] >= 2){
        bradut[15][j] -= 2;
      }
    }
    lastSnowMove = millis();
  }
}

void printBradut(){
  if (millis() - lastSnowChange >= millisForSnowChange){
    snowStatus = !snowStatus;
    lastSnowChange = millis();
  }
  for (byte i = 0; i < 16; i++){
    for (byte j = 0; j < 8; j++){
      if (bradut[i][j] >= 2){
        displayRowCol(i, j, snowStatus);
      }
      else{
        displayRowCol(i, j, bradut[i][j]);
      }
    }
  }
  moveShowflakes();
}

// --------------------------------------------- functii

void clearDisk(){
  for (int i = 0 ; i < EEPROM.length() ; i++){
    EEPROM.write(i, 0);
  }
}

player getPlayer(byte pos){
  player p;
  EEPROM_readAnything((pos - 1) * 10, p);
  return p;
}

void writePlayer(byte pos, player p){
  EEPROM_writeAnything((pos - 1) * 10, p);
}

void displayHighScore(){
  if (highscoreNavChanged){ // display only when something changed so the screen won't flicker
    if (highscoreNavStart == 0){
      lcd.clear();
      lcd.print("1. ");
      lcd.print(playerNo1.pName);
      lcd.print("  ");
      lcd.print(playerNo1.pScore);
      lcd.setCursor(0, 1);
      lcd.print("2. ");
      lcd.print(playerNo2.pName);
      lcd.print("  ");
      lcd.print(playerNo2.pScore);
      lcd.setCursor(15, 1);
      lcd.print("v");
    }
    else{
      lcd.clear();
      lcd.print("2. ");
      lcd.print(playerNo2.pName);
      lcd.print("  ");
      lcd.print(playerNo2.pScore);
      lcd.setCursor(0, 1);
      lcd.print("3. ");
      lcd.print(playerNo3.pName);
      lcd.print("  ");
      lcd.print(playerNo3.pScore);
      lcd.setCursor(15, 0);
      lcd.print("^");
    }
    highscoreNavChanged = 0;
  }
}

void navigateHighScores(){
  if (highscoreJustStarted){
    highscoreNavChanged = 1;
    highscoreJustStarted = 0;
  }
  displayHighScore();
  if (readJoyStickY()){
    highscoreNavStart = !highscoreNavStart;
    highscoreNavChanged = 1;
  }
  if (readJoyStickButton()){
    menuWasClicked = 0;
    highscoreJustStarted = 1;
    highscoreNavStart = 0;
  }
}

bool viewHowTo(){
  if (howToChanged){
    lcd.clear();
    if (howToSection == 1){
      lcd.print("Use the joystick");
      lcd.setCursor(0, 1);
      lcd.print("to set the      ");
    }
    else if (howToSection == 2){
      lcd.print("player's side   ");
      lcd.setCursor(0, 1);
      lcd.print("direction. He   ");
    }
    else if (howToSection == 3){
      lcd.print("will move untill");
      lcd.setCursor(0, 1);
      lcd.print("he will reach   ");
    }
    else if (howToSection == 4){
      lcd.print("the wall or the ");
      lcd.setCursor(0, 1);
      lcd.print("direction is    ");
    }
    else if (howToSection == 5){
      lcd.print("changed. You    ");
      lcd.setCursor(0, 1);
      lcd.print("lose a life when");
    }
    else if (howToSection == 6){
      lcd.print("an obstacle hits");
      lcd.setCursor(0, 1);
      lcd.print("you (you can    ");
    }
    else if (howToSection == 7){
      lcd.print("touch them from ");
      lcd.setCursor(0, 1);
      lcd.print("the side).      ");
    }
    else if (howToSection == 8){
      lcd.print("Good luck!      ");
    }
    
    howToChanged = 0;
  }
  int yAxis = readJoyStickY();
  if (yAxis == -1){ // scroll down
    howToSection++;
    if (howToSection > 8){
      howToSection = 1;
    }
    howToChanged = 1;
  }
  else if (yAxis == 1){
    howToSection--;
    if (howToSection < 1){
      howToSection = 8;
    }
    howToChanged = 1;
  }
  if (readJoyStickButton()){
    return 0;
  }
  else{
    return 1;
  }
}

void displayAlphabet(){
  if (alphabetChanged){
    lcd.clear();
    for (byte i = 65; i < 81; i++){
      if (i != selectedChar){
        lcd.print(char(i));
      }
      else{
        if (characterShow){
          lcd.print(char(i));
        }
        else{
          lcd.print(' ');
        }
      }
    }
    lcd.setCursor(0, 1);
    for (byte i = 81; i < 91; i++){
      if (i != selectedChar){
        lcd.print(char(i));
      }
      else{
        if (characterShow){
          lcd.print(char(i));
        }
        else{
          lcd.print(' ');
        }
      }
    }
    if (91 != selectedChar){ // the space
      lcd.print('_');
    }
    else{
      if (characterShow){
        lcd.print('_');
      }
      else{
        lcd.print(' ');
      }
    }
    alphabetChanged = 0;
  }
}

char selectFromAlphabet(){
  if (millis() - lastMillisForAlphabet >= millisForAlphabet){ // when you select a letter for your name you enter in this section where the whole alphabet is showed and you can choose your letter
    characterShow = !characterShow;
    alphabetChanged = 1;
    lastMillisForAlphabet = millis();
  }
  displayAlphabet();
  
  int xAxis = readJoyStickX();
  if (xAxis == 1){
    selectedChar++;
    if (selectedChar > 91){
      selectedChar = 65;
    }
    alphabetChanged = 1;
  }
  else if (xAxis == -1){
    selectedChar--;
    if (selectedChar < 65){
      selectedChar = 91;
    }
    alphabetChanged = 1;
  }

  int yAxis = readJoyStickY();
  if (yAxis == -1){
    if (selectedChar + 16 <= 91){
      selectedChar += 16;
      alphabetChanged = 1;
    }
  }
  else if (yAxis == 1){
    if (selectedChar - 16 >= 65){
      selectedChar -= 16;
      alphabetChanged = 1;
    }
  }

  if (readJoyStickButton()){
    if (selectedChar == 91){
      selectedChar = 65;
      return ' ';
    }
    int aux = selectedChar;
    selectedChar = 65;
    return char(aux);
  }
  
  return '.';
}

void displaySettings(){
  if (settingSelected == 2){
    if (millis() - lastMillisForAlphabet >= millisForAlphabet){
      characterShow = !characterShow;
      settingsChanged = 1;
      lastMillisForAlphabet = millis();
    }
  }
  if (settingsChanged > 0){
    
    if (settingsDisplayStart == 1){
      lcd.clear();
      if (settingSelected == 1){
        lcd.print(">");
      }
      else{
        lcd.print(" ");
      }
      lcd.print("Level: < ");
      lcd.print(startingLevelValue);
      lcd.print(" >");
      
      lcd.setCursor(0, 1);
      if (settingSelected == 2){
        lcd.print(">Name: ");
        for (byte i = 0; i < MAX_NAME_LEN; i++){
          if (i != charToReplace){
            lcd.print(currentPlayer[i]);
          }
          else{
            if (characterShow){
              if (currentPlayer[i] == ' '){
                lcd.print('_');
              }
              else{
                lcd.print(currentPlayer[i]);
              }
            }
            else{
              lcd.print(' ');
            }
          }
        }
      }
      else{
        lcd.print(" Name: ");
        lcd.print(currentPlayer);
      } 
      
      lcd.setCursor(15, 1);
      lcd.print("v");
    }
    else if (settingsDisplayStart == 2){ // hard coded "scrolling". not very elegant, but works
      lcd.clear();
      if (settingSelected == 3){/*
        lcd.print(">Name: ");
        for (byte i = 0; i < MAX_NAME_LEN; i++){
          if (i != charToReplace){
            lcd.print(currentPlayer[i]);
          }
          else{
            if (characterShow){
              if (currentPlayer[i] == ' '){
                lcd.print('_');
              }
              else{
                lcd.print(currentPlayer[i]);
              }
            }
            else{
              lcd.print(' ');
            }
          }
        }*/
        lcd.print(">");
      }
      else{/*
        lcd.print(" Name: ");
        lcd.print(currentPlayer);*/
        lcd.print(" ");
      }
      lcd.print("How to play?");
      lcd.setCursor(0, 1);
      if (settingSelected == 4){
        lcd.print(">");
      }
      else{
        lcd.print(" ");
      }
      lcd.print("Back");
  
      lcd.setCursor(15, 0);
      lcd.print("^");
    }
    else{
      lcd.clear();
      lcd.print("PANICA");
    }
    
    settingsChanged = 0;
  }
}

void changeSettings(){
  if (settingsJustStarted){ // like a setup
    settingsChanged = 1;
    settingsJustStarted = 0;
  }

  if (changingName){
    char aux = selectFromAlphabet();
    if (aux == '.'){
      return;
    }
    currentPlayer[charToReplace] = aux;
    changingName = 0;
  }
  else if (viewingHowTo){
    if (viewHowTo()){ // if still viewing
      return;
    }
    else{
      viewingHowTo = 0;
      howToSection = 1;
      settingsChanged = 1;
    }
  }
  
  displaySettings();
  if (settingSelected == 1){ // changing the starting difficulty
    int xAxis = readJoyStickX();
    if (xAxis == -1){
      startingLevelValue--;
      if (startingLevelValue < 1){
        startingLevelValue = maxLevel;
      }
      settingsChanged = 1; // display just when something is changed
    }
    else if (xAxis == 1){
      startingLevelValue++;
      if (startingLevelValue > maxLevel){
        startingLevelValue = 1;
      }
      settingsChanged = 1;
    }
  }
  else if (settingSelected == 2){ // navingating through the name letters
    int xAxis = readJoyStickX();
    if (xAxis == 1){
      if (charToReplace + 1 < 4){
        charToReplace++;
        settingsChanged = 1;
      }
    }
    else if (xAxis == -1){
      if (charToReplace - 1 >= 0){
        charToReplace--;
        settingsChanged = 1;
      }
    }
  }
  int yAxis = readJoyStickY();
  if (yAxis == -1){
    settingSelected++;
    if (settingSelected > 4){
      settingSelected = 1;
    }
    settingsDisplayStart = (settingSelected + 1) / 2;/*
    if (settingSelected == 3){
      settingsDisplayStart = 2;
    }
    if (settingSelected == 1){
      settingsDisplayStart = 1;
    }*/
    settingsChanged = 1;
  }
  else if (yAxis == 1){
    settingSelected--;
    if (settingSelected < 1){
      settingSelected = 4;
    }
    settingsDisplayStart = (settingSelected + 1) / 2;/*
    if (settingSelected == 3){
      settingsDisplayStart = 2;
    }
    if (settingSelected == 1){
      settingsDisplayStart = 1;
    }*/
    settingsChanged = 1;
  }

  if (settingSelected == 2){
    if (readJoyStickButton()){
      changingName = 1;
      char aux = currentPlayer[charToReplace];
      if (aux == ' '){
        selectedChar = 91;
      }
      else {
        selectedChar = (byte) aux;
      }
    }
  }
  else if(settingSelected == 3){
    if (readJoyStickButton()){
      viewingHowTo = 1;
      howToChanged = 1;
    }
  }
  else if (settingSelected == 4 || settingSelected == 1){
    if (readJoyStickButton()){
      menuWasClicked = 0;
      settingsJustStarted = 1;
      settingSelected = 1;
      settingsDisplayStart = 1;
      charToReplace = 0;
    }
  }
}

void displayInfo(){
  if (infoChanged){
    if (infoDisplayStart == 1){
      lcd.clear();
      lcd.print("Grapp Remastered");
      lcd.setCursor(0, 1);
      lcd.print("Radu Dilirici  v");
    }
    else if (infoDisplayStart == 2){
      lcd.clear();
      lcd.print("@UnibucRobotics^");
      lcd.setCursor(0, 1);
      for (byte i = 0; i < 16; i++){
        if (gitLink[(i + gitOffset) % 80] == '\0'){
          lcd.print(' ');
        }
        else{
          lcd.print(gitLink[(i + gitOffset) % 80]);
        }
      }
    }
    infoChanged = 0;
  }
  if (infoDisplayStart == 2){  // scroll only when the git text is visible
    if (millis() - lastMillisForGitScroll >= millisForGitScroll){ // scroll the text
    gitOffset = (gitOffset + 1) % 80;
    infoChanged = 1;
    lastMillisForGitScroll = millis();
    }
  }
  int yAxis = readJoyStickY();
  if (yAxis == 1){
    infoDisplayStart--;
    if (infoDisplayStart < 1){
      infoDisplayStart = 2;
    }
    infoChanged = 1;
  }
  else if (yAxis == -1){
    infoDisplayStart++;
    if (infoDisplayStart > 2){
      infoDisplayStart = 1;
    }
    infoChanged = 1;
  }
  if (readJoyStickButton()){
    menuWasClicked = 0;
    infoChanged = 1;
    infoDisplayStart = 1;
    gitOffset = 0;
  }
}

void displayMenu(){
  if (menuChanged){
    lcd.clear();
    lcd.setCursor(0, 0);
    if (menuSelected == 1){
      lcd.print(">");
    }
    else {
      lcd.print(" ");
    }
    lcd.print("Start ");
    if (menuSelected == 2){
      lcd.print(">");
    }
    else {
      lcd.print(" ");
    }
    lcd.print("Settings");
    
    lcd.setCursor(0, 1);
    if (menuSelected == 3){
      lcd.print(">");
    }
    else {
      lcd.print(" ");
    }
    lcd.print("Score ");
    if (menuSelected == 4){
      lcd.print(">");
    }
    else {
      lcd.print(" ");
    }
    lcd.print("About");

    menuChanged = 0;
  }
}

void menuSelect(){
  if (menuJustStarted){
    menuChanged = 1;
    menuJustStarted = 0;
  }
  displayMenu();
  int xAxis = readJoyStickX();
  if (xAxis == 1){
    menuSelected++;
    if (menuSelected > 4){
      menuSelected = 1;
    }
    menuChanged = 1;
  }
  else if (xAxis == -1){
    menuSelected--;
    if (menuSelected < 1){
      menuSelected = 4;
    }
    menuChanged = 1;
  }
  int yAxis = readJoyStickY();
  if (yAxis != 0){
    if (menuSelected == 1 || menuSelected == 2){
      menuSelected += 2;
    }
    else{
      menuSelected -= 2;
    }
    menuChanged = 1;
  }
}

void displayGameStatus(byte currentLives, byte levelValue, int currentScore){
  if (millis() - lastDisplayMillis > displayDelay){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Lives:");
    lcd.print(currentLives);
    lcd.print(" Level:");
    lcd.print(levelValue);
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.print(currentScore);
    lastDisplayMillis = millis();
  }
}

/* 0 - update highscore
 * 1 - new highscore => alert the player
 * 2 - 2nd place
 * 3 - 3rd place
 * 4 - no highscore
 * 5 - name, score; play again?, menu (select)
 */
void playGame(){
  if (gameJustStarted){ // Game Setup
    lcd.clear();
    lcd.print("<-----   Ready");
    delay(1000); // same here... I also save some memory because I don't use another unsigned long int for the time (The Arduino IDE is already complaining about low memory available...)
    lcd.clear();
    lcd.print("<-----   Set");
    delay(1000);
    lcd.clear();
    lcd.print("<-----   Go!");
    delay(500);
    gameSetup(startingLevelValue);
    gameJustStarted = 0;
  }
  // if the player lost:
  if (gameOver){
    // don't display anything, just update highscore and redirect to the right screen
    if (gameOverScreenNo == 0){
      
      if (currentScore > playerNo1.pScore){
        for (byte i = 0; i < MAX_NAME_LEN; i++){
          playerNo3.pName[i] = playerNo2.pName[i];
        }
        playerNo3.pScore = playerNo2.pScore;
        for (byte i = 0; i < MAX_NAME_LEN; i++){
          playerNo2.pName[i] = playerNo1.pName[i];
        }
        playerNo2.pScore = playerNo1.pScore;
        for (byte i = 0; i < MAX_NAME_LEN; i++){
          playerNo1.pName[i] = currentPlayer[i];
        }
        playerNo1.pScore = currentScore;
        gameOverScreenNo = 1;
        writePlayer(1, playerNo1);
        writePlayer(2, playerNo2);
        writePlayer(3, playerNo3);
      }
      else if (currentScore > playerNo2.pScore){
        for (byte i = 0; i < MAX_NAME_LEN; i++){
          playerNo3.pName[i] = playerNo2.pName[i];
        }
        playerNo3.pScore = playerNo2.pScore;
        for (byte i = 0; i < MAX_NAME_LEN; i++){
          playerNo2.pName[i] = currentPlayer[i];
        }
        playerNo2.pScore = currentScore;
        gameOverScreenNo = 2;
        writePlayer(2, playerNo2);
        writePlayer(3, playerNo3);
      }
      else if (currentScore > playerNo3.pScore){
        for (byte i = 0; i < MAX_NAME_LEN; i++){
          playerNo3.pName[i] = currentPlayer[i];
        }
        playerNo3.pScore = currentScore;
        gameOverScreenNo = 3;
        writePlayer(3, playerNo3);
      }
      else {
        gameOverScreenNo = 4;
      }
      gameOverScreenChanged = 1;
    }
    // display congrats
    if (gameOverScreenNo == 1){
      if (gameOverScreenChanged){
        lcd.clear();
        lcd.print("Congrats ");
        lcd.print(currentPlayer);
        lcd.setCursor(0, 1);
        lcd.print("New highscore!");
        gameOverScreenChanged = 0;
      }
    }
    else if (gameOverScreenNo == 2){
      if (gameOverScreenChanged){
        lcd.clear();
        lcd.print("So close! You're");
        lcd.setCursor(0, 1);
        lcd.print("in second place.");
        gameOverScreenChanged = 0;
      }
    }
    else if (gameOverScreenNo == 3){
      if (gameOverScreenChanged){
        lcd.clear();
        lcd.print("Not bad at all!");
        lcd.setCursor(0, 1);
        lcd.print("You're 3rd place");
        gameOverScreenChanged = 0;
      }
    }
    else if (gameOverScreenNo == 4){
      if (gameOverScreenChanged){
        lcd.clear();
        lcd.print("Nice one! Better");
        lcd.setCursor(0, 1);
        lcd.print("luck next time.");
        gameOverScreenChanged = 0;
      }
    }
    
    if (gameOverScreenNo >= 1 && gameOverScreenNo <= 4){
      if (readJoyStickButton()){
        gameOverScreenNo = 5;
        gameOverScreenChanged = 1;
      }
    }
    
    // play again? + info
    else if (gameOverScreenNo == 5){
      if (gameOverScreenChanged){
        lcd.clear();
        lcd.print(currentPlayer);
        lcd.print(": ");
        lcd.print(currentScore);
        lcd.setCursor(0, 1);
        if (gameOverSelection == 0){
          lcd.print(">");
        }
        else {
          lcd.print(" ");
        }
        lcd.print("Again? ");
        if (gameOverSelection == 1){
          lcd.print(">");
        }
        else {
          lcd.print(" ");
        }
        lcd.print("Menu");
        gameOverScreenChanged = 0;
      }
      if (readJoyStickX()){
        gameOverSelection = !gameOverSelection;
        gameOverScreenChanged = 1;
      }
      if (readJoyStickButton()){
        gameOver = 0;
        gameJustStarted = 1;
        gameOverScreenNo = 0;
        if (gameOverSelection == 1){ // play again
          menuWasClicked = 0;
        }
        gameOverSelection = 0;
      }
    }
    displayLevel();
    displayPlayer();
    return;
  }

  // when playing, only these two functions will be called
  iterateGame();
  displayGameStatus(currentLives, diffLevel, currentScore);
}

void displayHello(){
  byte loadingSymbol[8] =
  {
  0b00000,
  0b00000,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b00000
  };
  lcd.createChar(0, loadingSymbol);
  lcd.clear();
  lcd.print("Hi! Powering up..");
  lcd.setCursor(0, 1);
  for (byte i = 0; i < 16; i++){
    lcd.write(byte(0));
    delay(100); // I don't want to check for this function every loop to avoid delay() because I'm already at the limits of the Arduino CPU
  }
}

void setup(){
  lcd.begin(16, 2);
  pinMode(joystickButtonPin, INPUT_PULLUP);
  
   for (byte i = 0; i < lc.getDeviceCount(); i++){
    lc.shutdown(i, false);
    lc.setIntensity(i, 10); // 0 15
    lc.clearDisplay(i);
  }  
  
  randomSeed(analogRead(2)); // pin no 2 is NOT used so it will generate a pretty random seed
//  clearDisk();
  playerNo1 = getPlayer(1);
  playerNo2 = getPlayer(2);
  playerNo3 = getPlayer(3);

  displayHello();
}

void loop(){
  if (!menuWasClicked){
    printBradut();
    if (readJoyStickButton()){
      menuWasClicked = 1;
      menuJustStarted = 1;
    }
    else {
      menuSelect();
    }
  }
  else {
    if (menuSelected == 1){
      playGame();    
    }
    else if (menuSelected == 2){
      changeSettings();
      printBradut();
    }
    else if (menuSelected == 3){
      navigateHighScores();
      printBradut();
    }
    else if (menuSelected == 4){
      displayInfo();
      printBradut();
    }
  }
}
