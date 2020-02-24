#include "ServoControls.h"

#include <Wire.h>

void sendDirToSlave(int dir) {
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(dir + 1);
  Wire.endTransmission();
}

void resetJoints() {
  sendDirToSlave(0);
  jointUp.write(180);  // 180 is completely relaxed, 90 is still relaxed, 30 is tight
  jointDown.write(0);  // 0 is completely relaxed, 130 is tight
  delay(500);  // sorry Andrei
  jointDown.write(130);
  delay(100);
  jointUp.write(30);
  delay(500);
}

void sadFinger() {
  sendDirToSlave(0);
  jointUp.write(180);
  jointDown.write(0);
  delay(500);
  for (int i = 0; i < 30; i++) {
    if (i < 20) {
      jointDown.write(i * 6);
    }
    if (i >= 15) {
      jointUp.write(180 - (i - 10) * 7);
    }
    delay(50);
  }
}

void fingerDance() {
  if (dancePhase == 1 and millis() - fingerTimer >= 500) {
    sendDirToSlave(0);
    jointDown.write(0);
    fingerTimer = millis();
    dancePhase++;
  }
  else if (dancePhase == 2 and millis() - fingerTimer >= 500) {
    sendDirToSlave(1);
    jointDown.write(130);
    fingerTimer = millis();
    dancePhase++;
  }
  else if (dancePhase == 3 and millis() - fingerTimer >= 500) {
    sendDirToSlave(0);
    jointDown.write(0);
    fingerTimer = millis();
    dancePhase++;
  }
  else if (dancePhase == 4 and millis() - fingerTimer >= 500) {
    sendDirToSlave(-1);
    jointDown.write(130);
    fingerTimer = millis();
    dancePhase = 1;
  }
}

void servoSetup() {
  Wire.begin();
  
  jointUp.attach(jointUpPin);
  jointDown.attach(jointDownPin);
}
