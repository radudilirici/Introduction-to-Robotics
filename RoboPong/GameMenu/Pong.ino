#include "Display.h"
#include "Controls.h"
#include "ServoControls.h"
#include "Pong.h"
#include "Bluetooth.h"

typedef struct ball_t {
  int xDir, yDir, color;
  byte x, y, bsize;
};
ball_t ball;

byte bar1Start, bar2Start;
byte bar1Length, bar2Length;  // the bars are stored as segments [barStart, barStart + barLength)
const byte barWidth = 2;
const byte startY = 20;  // the start position for the bars

int bar1Color, bar2Color;
// the probability to change the bar color when the balls hits it (which affects the bar)
byte redPorb = 20, greenProb = 20, blueProb = 20;  // small bar, big bar, slow bar
const byte whiteLength = 15;  // baseLength
const byte redLength = 10;
const byte greenLength = 20;
byte bar1MoveNo = 1, bar2MoveNo = 1, barMoves = 3;  // to simulate the slow bar, the bar will move n - 1 out of n times when it's told to (in this case 2 moves and one stop)

byte p1Score, p2Score;

byte bar1Offset = 2;  // x offset. the display was initially shifted so I had to change the display size in the library settings
byte deadOffset = 1;

//unsigned long lastBallUpdate;
//int ballUpdateMillis = 1;

void displayBall() {  // optimized ball display. instead of clearing the old ball from the screen and then display the new ball,
  // this function only clears the pixels from behind the ball and displays the new ones
  if (ball.xDir == 1) {
    tft.drawFastVLine(ball.x - ball.xDir, ball.y - ball.yDir, ball.bsize, BLACK);
    tft.drawFastVLine(ball.x + ball.bsize - 1,  ball.y, ball.bsize, ball.color);
  }
  else if (ball.xDir == -1) {
    tft.drawFastVLine(ball.x + ball.bsize,  ball.y - ball.yDir, ball.bsize, BLACK);
    tft.drawFastVLine(ball.x, ball.y, ball.bsize, ball.color);
  }

  if (ball.yDir == 1) {
    tft.drawFastHLine(ball.x - ball.xDir, ball.y - ball.yDir, ball.bsize, BLACK);
    tft.drawFastHLine(ball.x,  ball.y + ball.bsize - 1, ball.bsize, ball.color);

  }
  else if (ball.yDir == -1) {
    tft.drawFastHLine(ball.x - ball.xDir,  ball.y + ball.bsize, ball.bsize, BLACK);
    tft.drawFastHLine(ball.x, ball.y, ball.bsize, ball.color);
  }
}

void displayBars() {
  // bar1
  tft.fillRect(bar1Offset, 0, barWidth, bar1Start, BLACK);
  tft.fillRect(bar1Offset, bar1Start, barWidth, bar1Length, bar1Color);
  tft.fillRect(bar1Offset, bar1Start + bar1Length, barWidth, displaySize, BLACK);

  // bar2
  tft.fillRect(displaySize - barWidth, 0, barWidth, bar2Start, BLACK);
  tft.fillRect(displaySize - barWidth, bar2Start, barWidth, bar2Length, bar2Color);
  tft.fillRect(displaySize - barWidth, bar2Start + bar2Length, barWidth, displaySize, BLACK);
}

void displayScore() {
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  byte digitsOffset = 1;
  byte c1 = p1Score / 10;
  while (c1) {
    c1 /= 10;
    digitsOffset++;
  }
  tft.setCursor(displaySize / 2 - (digitsOffset * 10 + 5), 10);
  tft.print(p1Score);
  tft.print("|");
  tft.print(p2Score);
}

void displayPong() {  // efficient display
  displayScore();
  displayBall();
  displayBars();
}

void gameOver() {
  tft.clearScreen();
  tft.setCursor(22, 56);
  tft.setTextColor(CYAN);
  tft.setTextSize(2);
  if (p1Score == winScore) {
    tft.println("P1 wins");
    sadFinger();
    while (!readB1Reset());
  }
  else {
    tft.println("P2 wins");
    dancePhase = 1;
    jointUp.write(180);
    while (!readB1Reset()) {
      fingerDance();
    }
  }
  inSubMenu = false;
  justEnteredMenu = true;
}

void giveRandomToYDir() {  // give random y direction, but not the same as the current one
  if (ball.yDir == 0) {
    ball.yDir = 1 - random(0, 2) * 2;  // sus sau jos
  }
  else if (ball.yDir == -1) {
    ball.yDir = random(-1, 1);
  }
  else {
    ball.yDir = random(0, 2);
  }
}

void updateBallPosition() {  // the ball moves every iteration. it is slow enough because there are 128 pixels, and the ball moves 1 pixel at a time
  
  if (ball.x == barWidth + bar1Offset) {  // the ball is on the player1's side
    if (ball.y > bar1Start - ball.bsize and ball.y < bar1Start + bar1Length + ball.bsize - 1) {
      ball.xDir = 1;
      giveRandomToYDir();
      if (bar1Color != WHITE) {
        bar1Color = WHITE;
        bar1Length = whiteLength;
      }
      else {
        int r = random(0, 100);
        if (r < redPorb) {
          bar1Color = RED;
          bar1Length = redLength;
        }
        else if (r < redPorb + greenProb) {
          bar1Color = GREEN;
          bar1Length = greenLength;
        }
        else if (r < redPorb + greenProb + blueProb) {
          bar1Color = BLUE;
        }
      }
    }
    else {
      p2Score++;
      if (p2Score == winScore) {
        gameOver();
      }
      else {
        startAgain();
      }
    }
  }
  
  if (ball.x == displaySize - barWidth - ball.bsize) {  // player 2's side
    if (ball.y > bar2Start - ball.bsize and ball.y < bar2Start + bar2Length + ball.bsize - 1) {
      ball.xDir = -1;
      giveRandomToYDir();
      if (bar2Color != WHITE) {
        bar2Color = WHITE;
        bar2Length = whiteLength;
      }
      else {
        int r = random(0, 100);
        if (r < redPorb) {
          bar2Color = RED;
          bar2Length = redLength;
        }
        else if (r < redPorb + greenProb) {
          bar2Color = GREEN;
          bar2Length = greenLength;
        }
        else if (r < redPorb + greenProb + blueProb) {
          bar2Color = BLUE;
        }
      }
    }
    else {
      p1Score++;
      if (p1Score == winScore) {
        gameOver();
      }
      else {
        startAgain();
      }
    }
  }
  
  if (ball.y == 0) {
    ball.yDir = 1;
  }
  if (ball.y == displaySize - ball.bsize) {
    ball.yDir = -1;
  }

  ball.x += ball.xDir;
  ball.y += ball.yDir;
}

void updateBar1() {
  int xDir = readX1Value();

  if (bar1Color == BLUE) {
    if (bar1MoveNo == barMoves) {
      xDir = 0;
      bar1MoveNo = 1;
    }
    else {
      bar1MoveNo++;
    }
  }

  if (xDir == 0) {
    return;
  }
  if ((bar1Start + xDir >= 0 or xDir == 1) and (bar1Start + xDir + bar1Length <= displaySize or xDir == -1)) {  // move only if the bar is not going off screen
    bar1Start += xDir;                                                                                          // the 'or' is for the case when the bar resizes and you are already off screen
  }
}

void updateBar2() {
  int xDir = readX2Value();
  if (bar2Color == BLUE) {
    if (bar2MoveNo == barMoves) {
      xDir = 0;
      bar2MoveNo = 1;
    }
    else {
      bar2MoveNo++;
    }
  }

  if (xDir == 0) {
    return;
  }
  if ((bar2Start + xDir >= 0 or xDir == 1) and (bar2Start + xDir + bar2Length <= displaySize or xDir == -1)) {
    bar2Start += xDir;
  }
}

bool predictedY = false;
byte newY;

void predictY() {
  if (predictedY) {
    return;
  }
  if (ball.yDir == 0) {
    newY = ball.y;
  }
  else if (ball.yDir == 1) {
    byte xDist = displaySize - ball.bsize - 2 * barWidth - bar1Offset - deadOffset;
    newY = ball.y + xDist;
    if (newY > displaySize - ball.bsize) {
      newY = (displaySize - ball.bsize) - newY % (displaySize - ball.bsize);
    }
  }
  else {
    byte xDist = displaySize - ball.bsize - 2 * barWidth - bar1Offset - deadOffset;
    int aux = ball.y - xDist;
    if (aux >= 0) {
      newY = aux;
    }
    else {
      newY = -aux;
    }
  }
  predictedY = true;
}

int AIDir = 0; // the direction of the AI bar

void updateServoAI() {
  if (ball.xDir == -1 and predictedY) {
    predictedY = false;
  }
  if (ball.xDir == 1) {  // if the ball is headin towards player 2, calculate where the ball will land
    predictY();
  }

  if (ball.xDir == -1) {  // if the ball goes towards player 1, go to the middle of the screen
    if (bar2Start < displaySize / 2 - bar2Length) {
      AIDir = 1;
    }
    else if (bar2Start > displaySize / 2) {
      AIDir = -1;
    }
    else {
      AIDir = 0;
    }
  }

  if (ball.xDir == 1) {  // go to the predicted ball position
    if ((bar2Start + bar2Length - 1)  + (ball.bsize - 1) < newY) {  // (the last positoin that the bar reaches) + (the size of the ball - 1) because the bar only needs to touch the ball with one pixel
      AIDir = 1;                                                  // also, the servo will most likely move more than it's supposed to. newY referes to the lower margin of the ball
    }
    else if (bar2Start > newY) {
      AIDir = -1;
    }
    else {
      AIDir = 0;
    }
  }
  sendDirToSlave(AIDir);
}

int BTDir = 0;

void updateServoBT() {
  char BTR = BTRead();
  //  Serial.println(BTdir);
  switch (BTR) {
    case 'R':
      BTDir = -1;
      break;

    case 'L':
      BTDir = 1;
      break;

    case '0':  // with the current bluetooth controller, the '0' character is sent when an arrow is released
      BTDir = 0;
      break;

    case 'o':  // 'o' is sent when a X/O/etc. button is released
      BTDir = 0;
      break;

    default:
      break;
  }
  sendDirToSlave(BTDir);
}

void startAgain() {
  tft.clearScreen();

  ball.x = displaySize / 2;
  ball.y = startY + 5;
  ball.xDir = -1;
  ball.yDir = 0;
  ball.bsize = 5;
  ball.color = YELLOW;
  tft.fillRect(ball.x, ball.y, ball.bsize, ball.bsize, ball.color);

  bar1Length = bar2Length = whiteLength;
  bar1Start = bar2Start = startY;
  bar1Color = bar2Color = WHITE;
}

void restartPong() {
  randomSeed(analogRead(A5));
  p1Score = p2Score = 0;
  startAgain();
  resetJoints();
}

void iteratePong() {
  displayPong();
  updateBar1();
  if (whosPlaying == 0) {
    updateServoAI();
  }
  else {
    updateServoBT();
  }
  updateBar2();
  updateBallPosition();
}
