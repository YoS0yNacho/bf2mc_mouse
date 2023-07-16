//==========================================================================
// Mouse Injector for Dolphin
//==========================================================================
// Copyright (C) 2019 Carnivorous
// All rights reserved.
//
// Mouse Injector is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, visit http://www.gnu.org/licenses/gpl-2.0.html
//==========================================================================
#include <stdio.h>
#include <stdint.h>
#include "mouse.h"
#include "./manymouse/manymouse.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int32_t xmouse, ymouse; // holds mouse input data (used for gamedrivers)

int root_x, root_y, win_x, win_y; // center screen X and Y var for mouse
static ManyMouseEvent event; // hold current mouse event
static unsigned char lockmouseclock = 0; // limit SetCursorPos execution

uint8_t MOUSE_Init(void);
void MOUSE_Quit(void);
void MOUSE_Update(void);

//==========================================================================
// Purpose: initialize manymouse and returns detected devices (0 = not found)
//==========================================================================
uint8_t MOUSE_Init(void)
{
	return ManyMouse_Init();
}
//==========================================================================
// Purpose: safely quit manymouse
//==========================================================================
void MOUSE_Quit(void)
{
	ManyMouse_Quit();
}
//==========================================================================
// Purpose: update cursor lock position
//==========================================================================
void MOUSE_Lock(void)
{
	Display *display = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(display);
    unsigned int mask;
	XQueryPointer(display, root, &root, &root, &root_x, &root_y, &win_x, &win_y, &mask);
    XCloseDisplay(display);
}
//==========================================================================
// Purpose: update xmouse/ymouse with mouse input
// Changed Globals: xmouse, ymouse, event
//==========================================================================
void MOUSE_Update(void)
{
	Display* display = XOpenDisplay(NULL);

    // Get the default screen
    int screen = DefaultScreen(display);
  
    // Get the screen dimensions
    Window root = RootWindow(display, screen);
    int width = XWidthOfScreen(XScreenOfDisplay(display, screen));
    int height = XHeightOfScreen(XScreenOfDisplay(display, screen));

    // Calculate the center coordinates
    int x = width / 2;
    int y = height / 2;

	if(lockmouseclock % 25 == 0) // don't execute every tick
		XWarpPointer(display, None, root, 0, 0, 0, 0, x, y); // set mouse position back to center of screen
    XCloseDisplay(display);
	xmouse = ymouse = 0; // reset mouse input
	while(ManyMouse_PollEvent(&event))
	{
		if(event.type == MANYMOUSE_EVENT_RELMOTION)
		{
			if(event.item == 0)
				xmouse += event.value;
			else
				ymouse += event.value;
		}
	}
    //printf("x: %d\n", xmouse);
    //printf("y: %d\n", ymouse);
	lockmouseclock--;
}
