#ifndef SERVOCONTROLS_H_INCLUDED
#define SERVOCONTROLS_H_INCLUDED

#include <Servo.h>
#include <Wire.h>

#define SLAVE_ADDR 9

const int jointDownPin = 9, jointUpPin = 10;
Servo jointDown, jointUp;  // servo for right-left, lower joint and upper joint

unsigned long int fingerTimer, dancePhase = 1;

void sendDirToSlave(int dir);
void resetJoints();
void fingerDance();
void servoSetup();

#endif // SERVOCONTROLS_H_INCLUDED
