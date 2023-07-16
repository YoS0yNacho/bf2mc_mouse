#include "game.h"
#include "utils.h"
#include "keyboard.h"
#include <stdio.h>
#include "mouse.h"


bool mouseON;

int main() {
  printf("waiting for emulator...\n");
  Game game;
  open_game(&game);
  printf("pid: %d\n", game.pid);
  printf("pcsx2-qt module: %lx\n", game.module);
  MOUSE_Init();
  open_keyboard();
  GetEmuOffset(game);

  while (true) {
    manage_input(&game);
    mouseON = active;
    if (mouseON == true){
      MOUSE_Lock();
      MOUSE_Update();
      BF2MC_Inject(game);
      sleep_ms(1);
    }

  }

  printf("end\n");
  return 0;
}
