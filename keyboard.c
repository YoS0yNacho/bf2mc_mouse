#include "keyboard.h"
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int fd;
struct input_event ie;
bool active;

// sudo evtest -> check which eventX has the keyboard
void open_keyboard() {
  fd = open("/dev/input/event2", O_RDONLY | O_NONBLOCK);
  if (fd == -1) {
    printf("Need to run as root!\n");
    exit(1);
  }
}

// https://www.kernel.org/doc/Documentation/input/input.txt
void manage_input(Game *game) {
  int bytes = read(fd, &ie, sizeof(ie));
  if (bytes > 0 && ie.type == EV_KEY && ie.value == 1) {
    if (ie.code == KEY_F1) {
      active = !active;
    }
  }
}
