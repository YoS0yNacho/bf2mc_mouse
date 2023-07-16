#include <stdint.h>
#include "game.h"
#include "mouse.h"
#include "utils.h"
#include <stdio.h>

#define PS2_RAM 0x1AE1000

uint64_t emuoffset = 0;
uint32_t cambase = 0;
uint32_t hpPtr = 0;

#define TAU 6.2831853f // 0x40C90FDB
float sensitivity = 0.2;


void open_game(Game *game) {
  while ((game->pid = find_pid("pcsx2-qt")) == 0) {
    sleep_ms(1000);
  }
  while ((game->module = module_addr(game->pid, "pcsx2-qt")) == 0) {
    sleep_ms(1000);
  }
}

void GetEmuOffset(Game game){
  read_addr(game.pid, game.module + PS2_RAM, &emuoffset, sizeof(emuoffset));
  printf("ramadress: %ld\n", emuoffset);
}


/* add support for vehicle cameras, weapon sway */
void BF2MC_Inject(Game game){
  if(xmouse == 0 && ymouse == 0) // if mouse is idle
		return;
  float sensi = sensitivity;
  float camx, camy;
  int scopeMode;
//get pointers
  read_addr(game.pid, emuoffset + 0xf4200, &cambase, sizeof(cambase));
// read values
  read_addr(game.pid, emuoffset + cambase + 0x4, &camx, sizeof(camx));
  read_addr(game.pid, emuoffset + cambase + 0x8, &camy, sizeof(camy));
  read_addr(game.pid, emuoffset + 0x000F4204, &scopeMode, sizeof(scopeMode));
  //
  //printf("Pitch: %f\n", camx);
  //printf("Yaw: %f\n", camy);
  if( scopeMode == 1 ){
    sensi = (sensi / 4); 
  } else if( scopeMode == 2 ){
    sensi = (sensi / 8); 
  }

  const float looksensitivity = (float)sensi / 40.f;
  const float fov = 0.8f;
  camx /= 360.f;
	camx += (float)xmouse / 10.f * looksensitivity / (360.f / TAU) / (1.2f / fov); // normal calculation method for X
	camx *= 360.f;
  camy /= 360.f;
	camy -= (float)ymouse / 10.f * looksensitivity / (360.f / TAU) / (1.2f / fov); // normal calculation method for X
	camy *= 360.f;
  write_addr(game.pid, emuoffset + cambase + 0x4, &camx, sizeof(camx));
  write_addr(game.pid, emuoffset + cambase + 0x8, &camy, sizeof(camy));
}

