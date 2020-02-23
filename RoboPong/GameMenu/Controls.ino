#include "Controls.h"

void controlsSetup() {
  pinMode(xPin1, INPUT);
  pinMode(xPin2, INPUT);
  pinMode(bPin1, INPUT_PULLUP);
}

int readX1Reset() {  // joystick must reset position before reading another direction
  int xAxis = analogRead(xPin1);
  if (xAxis > highAxis && xAxisReset1) {
    xAxisReset1 = 0;
    return 1;
  }
  else if (xAxis < lowAxis && xAxisReset1) {
    xAxisReset1 = 0;
    return -1;
  }
  if (xAxis <= highAxis - 200 && xAxis >= lowAxis + 200) {
    xAxisReset1 = 1;
  }
  return 0;
}

int readX1Value() {  // just read if it is in the middle, right or left
  int xAxis = analogRead(xPin1);
  if (xAxis > highAxis) {
    return 1;
  }
  else if (xAxis < lowAxis) {
    return -1;
  }
  return 0;
}

int readX2Reset() {
  int xAxis = analogRead(xPin2);
  if (xAxis > highAxis && xAxisReset2) {
    xAxisReset2 = 0;
    return 1;
  }
  else if (xAxis < lowAxis && xAxisReset2) {
    xAxisReset2 = 0;
    return -1;
  }
  if (xAxis <= highAxis - 200 && xAxis >= lowAxis + 200) {
    xAxisReset2 = 1;
  }
  return 0;
}

int readX2Value() {
  int xAxis = analogRead(xPin2);
  if (xAxis > highAxis) {
    return 1;
  }
  else if (xAxis < lowAxis) {
    return -1;
  }
  return 0;
}

bool readB1Reset() {
  int bVal = digitalRead(bPin1);
  if (bVal == LOW && bReset1) {  // pressed
    bReset1 = false;
    return true;
  }
  if (bVal == HIGH) {  // not pressed
    bReset1 = true;
  }
  return false;
}
