#ifndef BLUETOOTH_H_INCLUDED
#define BLUETOOTH_H_INCLUDED

#include <SoftwareSerial.h>

SoftwareSerial BTSerial(A3, A2); // RX | TX

void BTSetup();
int BTRead();

#endif // BLUETOOTH_H_INCLUDED
