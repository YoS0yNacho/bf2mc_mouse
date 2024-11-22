//===========================================================
// Mouse Injector for Dolphin
//==========================================================================
// Copyright (C) 2019-2020 Carnivorous
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
#include <stdint.h>
#include "../main.h"
#include "../memory.h"
#include "../mouse.h"
#include "game.h"
#include <math.h>

// #define BF2_CAM_BASE_PTR 0x782B98
#define BF2_CAM_BASE_PTR 0xF4200
#define BF2_SCOPE_BASE_PTR 0xF4204
#define BF2_RIGHTJOY_X 0xDF36E0
#define BF2_RIGHTJOY_Y 0xDF36E4
#define BF2_ONFOOT 0x1FE6A13
#define BF2_FOV 0xE03FDE

// offsets from camBase
#define BF2_CAMX 0x4
#define BF2_CAMY 0x8

#define DEAD_ZONE 0.02f

static uint8_t PS2_BF2_Status(void);
static uint8_t PS2_BF2_DetectCamBase(void);
static void PS2_BF2_Inject(void);

static const GAMEDRIVER GAMEDRIVER_INTERFACE =
{
	"Battlefield 2: Modern Combat Online",
	PS2_BF2_Status,
	PS2_BF2_Inject,
	1, // 1000 Hz tickrate
	0 // crosshair sway not supported for driver
};

const GAMEDRIVER *GAME_PS2_BATTLEFIELD2 = &GAMEDRIVER_INTERFACE;

static uint32_t camBase = 0;

//==========================================================================
// Purpose: return 1 if game is detected
//==========================================================================
static uint8_t PS2_BF2_Status(void)
{
	// SLUS_210.26
	return (PS2_MEM_ReadWord(0x001ADB50) == 0x494E0042U ); //MP.BIN.BFMC
}


static uint8_t PS2_BF2_DetectCamBase(void)
{
	uint32_t tempCamBase = PS2_MEM_ReadUInt(BF2_CAM_BASE_PTR);
	//printdebug(tempCamBase);

	// TODO: sanity check
	if (tempCamBase)
	{
		camBase = tempCamBase;
		return 1;
	}

	return 0;
}


static void SmoothMouseInput(float *x, float *y)
{
    static float lastX = 0;
    static float lastY = 0;
    const float smoothingFactor = 0.3f; // Adjust this value to find the right smoothness

    // Smooth the mouse input for better movement
    float deltaX = *x - lastX;
    float deltaY = *y - lastY;

    // Make adjustments based on the distance moved
    if (fabs(deltaX) > DEAD_ZONE || fabs(deltaY) > DEAD_ZONE)
    {
        // Apply smoothing
        *x = lastX + (deltaX * smoothingFactor);
        *y = lastY + (deltaY * smoothingFactor);
    }
    else
    {
        // Keep last position if within dead zone
        *x = lastX;
        *y = lastY;
    }

    lastX = *x;
    lastY = *y;
}

static void InjectCam(void)
{
	uint32_t scopeModeBase = PS2_MEM_ReadUInt(BF2_SCOPE_BASE_PTR);
	uint8_t scopeMode = PS2_MEM_ReadUInt8(scopeModeBase + 0x20);
		
	float looksensitivity = (float)sensitivity / 40.f;
		
	if (scopeMode == 1)
	{
		looksensitivity = looksensitivity / 2;	
	} 
	else if (scopeMode == 2)
	{
		looksensitivity = looksensitivity / 4;
	} 
		
	float scale = 250.f;
	uint8_t fov = PS2_MEM_ReadUInt8(BF2_FOV) / 86;
	//float fov = 1.f;
		
	float camX = PS2_MEM_ReadFloat(camBase + BF2_CAMX);
	camX += (float)xmouse * looksensitivity / scale * fov;
	PS2_MEM_WriteFloat(camBase + BF2_CAMX, (float)camX);
		
	float camY = PS2_MEM_ReadFloat(camBase + BF2_CAMY);
	camY -= (float)(invertpitch ? -ymouse : ymouse) * looksensitivity / scale * fov;
	PS2_MEM_WriteFloat(camBase + BF2_CAMY, (float)camY);
}

static void InjectRightStick(void)
{
    float scale = 250.f;
    float looksensitivity = (float)sensitivity / 40.f;
    looksensitivity = looksensitivity * 500;

    float rx = (float)xmouse * looksensitivity / scale;
    float ry = (float)(!invertpitch ? -ymouse : ymouse) * looksensitivity / scale;

    // Implement dead zone
    if (fabs(rx) < DEAD_ZONE) rx = 0;
    if (fabs(ry) < DEAD_ZONE) ry = 0;

    // Smooth the mouse input
    SmoothMouseInput(&rx, &ry);

    // Clamp the values to ensure they stay within the -1 to 1 range
    if (rx > 1.0f) rx = 1.0f;
    if (rx < -1.0f) rx = -1.0f;
    if (ry > 1.0f) ry = 1.0f;
    if (ry < -1.0f) ry = -1.0f;

    PS2_MEM_WriteFloat(BF2_RIGHTJOY_X, rx);
    PS2_MEM_WriteFloat(BF2_RIGHTJOY_Y, ry);
}


static void PS2_BF2_Inject(void)
{
	//uint32_t currentWepBase = PS2_MEM_ReadUInt(BF2_CURRENTWEP_PTR + 0x34);
	if (K_1)
	{
	 	// rx 0 ry 0 primary weapon
	    Sleep(20);
	    PS2_MEM_WriteFloat(BF2_RIGHTJOY_X, 0);
		PS2_MEM_WriteFloat(BF2_RIGHTJOY_Y, 0);
		return;
	}
	if (K_2)
	{
	 	// rx 0 ry -1 pistol
	    Sleep(20);
	    PS2_MEM_WriteFloat(BF2_RIGHTJOY_X, 0);
		PS2_MEM_WriteFloat(BF2_RIGHTJOY_Y, -1);
		return;
	}
	if (K_3)
	{
	 	// rx 0 ry 1 accesory 1 / knife
	    Sleep(20);
	    PS2_MEM_WriteFloat(BF2_RIGHTJOY_X, 0);
		PS2_MEM_WriteFloat(BF2_RIGHTJOY_Y, 1);
		return;
	}
	if (K_4)
	{
	    // rx -1 ry 0 accesory 2 / bazooka
	    Sleep(20);
	    PS2_MEM_WriteFloat(BF2_RIGHTJOY_X, -1);
		PS2_MEM_WriteFloat(BF2_RIGHTJOY_Y, 0);
		return;
	}
	if (K_5)
	{
	    // rx 1 ry 0 accesory 3 
	    Sleep(20);
		PS2_MEM_WriteFloat(BF2_RIGHTJOY_Y, 0);
	    PS2_MEM_WriteFloat(BF2_RIGHTJOY_X, 1);
		return;
	}
	
	//if(xmouse == 0 && ymouse == 0) // if mouse is idle
	//	return;
	
	if (!PS2_BF2_DetectCamBase())
		return;

	if (!K_1 && !K_2 && !K_3 && !K_4 && !K_5 )
	{
		PS2_MEM_WriteFloat(BF2_RIGHTJOY_X, 0);
		PS2_MEM_WriteFloat(BF2_RIGHTJOY_Y, 0);
	}

	uint32_t scopeModeBase = PS2_MEM_ReadUInt(BF2_SCOPE_BASE_PTR);
	uint8_t scopeMode = PS2_MEM_ReadUInt8(scopeModeBase + 0x20);

    if (PS2_MEM_ReadUInt8(BF2_FOV) < 127 || scopeMode > 0 )
	{ 
		InjectCam();
	}
	else 
	{
		InjectRightStick();
	}


}	


