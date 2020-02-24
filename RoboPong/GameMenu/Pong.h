#ifndef PONG_H_INCLUDED
#define PONG_H_INCLUDED

bool whosPlaying = 0;  // 0 = MrRoboto, 1 = Bluetooth
byte winScore = 11;

bool inSubMenu, justEnteredMenu;  // these two are mainly used in the Game Menu, but I need them in Pong for Game Over so the game can return to menu

void restartPong();
void iteratePong();

#endif // PONG_H_INCLUDED
