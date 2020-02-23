#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>

// Display wiring: VCC - 3.3V, GND - ground, CS - 6, RESET - 12, A0 - 5, SDA - 11, SCK - 13, LED - 3.3V with 100 ohm resistance
#define __DC 6
#define __CS 7
#define __RST 12
TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC, __RST);

const byte displaySize = 128;
const byte charW = 10, charH = 14, charS = 2;  // width, height, space

void displaySetup();

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

#endif // DISPLAY_H_INCLUDED
