#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "game.h"
extern bool active;

void open_keyboard();

void manage_input(Game *game);

#endif
