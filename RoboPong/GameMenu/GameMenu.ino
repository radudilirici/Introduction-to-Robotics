#include "Display.h"
#include "Controls.h"
#include "Pong.h"
#include "Bluetooth.h"
#include "ServoControls.h"

byte menuOption, settingsOption;
const byte NoOfMenuOptions = 2, NoOfSettingsOptions = 2;
byte yOffset = 30; // to display the text a little lower from the top of the screen

bool menuChanged;     // for a faster and no flickering display, the funtions only replace the pixels that change
bool oponentChanged;  // in the display functions you will see some (kind of) hardcoded display clears
                      // I used the fact that on text scale 2 one character has 10 pixels width, 14 pixels height and there are 2 pixels between characters

void displayInitialMenu() {  // the first time the menu has to be displayed. after this, only the parts of the screen that change will be modified
  tft.clearScreen();
  tft.setTextColor(CYAN);
  tft.setTextSize(2);
  tft.setCursor(0, yOffset);

  if (menuOption == 0) {
    tft.print(" >");
  }
  else {
    tft.print("  ");
  }
  tft.println(" Play");

  if (menuOption == 1) {
    tft.print(" >");
  }
  else {
    tft.print("  ");
  }
  tft.println(" Options");
}

void displayMenu() {  // efficient menu display, without flickering. for this to work, the menu has to be displayed normally before
  if (!menuChanged) {
    return;
  }

  tft.fillRect(charW + charS, 0, charW, displaySize, BLACK);  // one character at size 2 has 10 pixels. this line clears the > sign because printing a space over it won't delete it
  tft.setTextColor(CYAN);
  tft.setTextSize(2);
  tft.setCursor(0, yOffset);

  for (int i = 0; i < NoOfMenuOptions; i++) {
    if (menuOption == i) {
      tft.println(" >");
    }
    else {
      tft.println("");
    }
  }

  menuChanged = false;
}

void navigateMenu() {
  // check if the option changes
  int xDir = readX1Reset();
  if (xDir == 1) {
    menuOption++;
    if (menuOption == NoOfMenuOptions) {
      menuOption = 0;
    }
    menuChanged = true;
  }
  else if (xDir == -1) {
    if (menuOption == 0) {
      menuOption = NoOfMenuOptions - 1;
    }
    else {
      menuOption--;
    }
    menuChanged = true;
  }

  // check if one option is pressed
  bool bVal = readB1Reset();
  if (bVal) {
    inSubMenu = true;
    justEnteredMenu = true;
  }
}

void displayInitialSettings() {
  tft.clearScreen();
  tft.setTextColor(CYAN);
  tft.setTextSize(2);
  tft.setCursor(0, yOffset);

  if (settingsOption == 0) {
    tft.print(" >");
  }
  else {
    tft.print("  ");
  }
  tft.print(" vs ");
  if (whosPlaying == 0) {  // Robot hand
    tft.println("AI");
  }
  else {  // Bluetooth
    tft.println("BT");
  }

  if (settingsOption == 1) {
    tft.print(" >");
  }
  else {
    tft.print("  ");
  }
  tft.println(" Back");
}

void displaySettings() {
  if (menuChanged) {
    tft.fillRect(charW + charS, 0, charW, displaySize, BLACK);
    tft.setTextColor(CYAN);
    tft.setTextSize(2);
    tft.setCursor(0, yOffset);
  
    for (int i = 0; i < NoOfSettingsOptions; i++) {
      if (settingsOption == i) {
        tft.println(" >");
      }
      else {
        tft.println("");
      }
    }
  } 

  if (oponentChanged) {
    tft.fillRect(charW * 6 + charS * 6, yOffset, charW * 2 + charS, charH, BLACK);  // clear the "AI" or "BT" text from the screen
    tft.setTextColor(CYAN);
    tft.setTextSize(2);
    tft.setCursor(charW * 6 + charS * 6, yOffset);
    if (whosPlaying == 0) {
      tft.println("AI");
    }
    else {  // Bluetooth
      tft.println("BT");
    }
  }

  menuChanged = false;
  oponentChanged = false;
}

void navigateSettings() {
  int xDir = readX1Reset();
  if (xDir == 1) {
    settingsOption++;
    if (settingsOption == NoOfSettingsOptions) {
      settingsOption = 0;
    }
    menuChanged = true;
  }
  else if (xDir == -1) {
    if (settingsOption == 0) {
      settingsOption = NoOfSettingsOptions - 1;
    }
    else {
      settingsOption--;
    }
    menuChanged = true;
  }

  // check if one option is pressed
  bool bVal = readB1Reset();
  if (bVal) {
    switch (settingsOption) {
      case 0:
        whosPlaying = !whosPlaying;
        oponentChanged = true;
        break;
      
      case 1:
        inSubMenu = false;
        justEnteredMenu = true;
        settingsOption = 0;
        break;

      default:
        break;
    }
  }
}

void setup() {
  displaySetup();
  controlsSetup();
  BTSetup();
  servoSetup();
  
  menuOption = 0;
  justEnteredMenu = true;
//  Serial.begin(9600);
}

void loop() {
  switch (inSubMenu) {
    
    case false:  // just navigating the main menu
      if (justEnteredMenu) {
        displayInitialMenu();
        justEnteredMenu = false;
      }
      else {
        displayMenu();        
      }
      navigateMenu();
      break;

    case true:  // entered in something
      switch (menuOption) {
        
        case 0:  // in game
          if (justEnteredMenu) {
            restartPong();
            justEnteredMenu = false;
          }
          else {
            iteratePong();
          }
          break;

        case 1:  // in settings
          if (justEnteredMenu) {
            displayInitialSettings();
            justEnteredMenu = false;
          }
          else {
            displaySettings();
          }
          navigateSettings();
          break;
        
        default:
          break;
     }
  }
}
