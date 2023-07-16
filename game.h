#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <sys/types.h>


typedef struct {
  pid_t pid;
  unsigned long module;
} Game;

void open_game(Game *game);
void GetEmuOffset(Game game);
void BF2MC_Inject(Game game);
//void writeTest(Game game, int value);

#endif
