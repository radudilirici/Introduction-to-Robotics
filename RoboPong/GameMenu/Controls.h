#ifndef CONTROLS_H_INCLUDED
#define CONTROLS_H_INCLUDED

const byte xPin1 = A0, xPin2 = A1, bPin1 = 8;  //bPin is for the button
const int highAxis = 800;
const int lowAxis = 200;
bool xAxisReset1, xAxisReset2, bReset1;

int readX1Reset();
int readX2Reset();
int readX1Value();
int readX2Value();
bool readB1Reset();
void controlsSetup();

#endif // CONTROLS_H_INCLUDED
