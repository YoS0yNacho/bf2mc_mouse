# Mouse Injector for BF2MC 

An external app that injects cursor input into game memory.

## Supported versions
SLUS_210.26 v1.00 (NTSC-U)
* NOTE: If the injector is not hooking, restart the emulator (System -> Restart) and then select the multiplayer option in the in-game menu and it should work.
* NOTE: When using vehicles/mounted gun, the injector emulates the right joystick so it may feel worse / different.
* NOTE: PCSX2 will only hook with **BIOS versions 5XXXX and up**.
* NOTE: Some aspects may break when a game is started with overclocking. Requires testing.
* PCSX2 Settings: **Disable** *'Settings->Interface->Double-Click Toggles Fullscreen'* | **Enable** *'Settings->Interface->Hide Cursor In Fullscreen'*
* Place cheat files in 'cheats/PS2' folder in the main PCSX2 directory. In PCSX2 go to 'Settings->Emulation' and tick 'Enable Cheats'.
* Place inputprofiles files in 'inputprofiles' folder in the main PCSX2 directory. In PCSX2 go to 'Settings->Controller' and load the profile.


## Supported Emulators
| Emulator/Frontend | Version | Executable name (case sensitive) |
| --- | :---: | :---: |
| PCSX2 Nightly | latest | pcsx2-qt.exe<br>pcsx2-qtx64.exe<br>pcsx2-qtx64-avx2.exe |
* NOTE: Versions given are the latest that have been tested working, may work with newer
* NOTE: PCSX2 will only hook with **BIOS versions 5XXXX and up**.

## Mappings
* 0 = Enable / Disable Mouse
* 1-5 = switch weapons
* Left click = R1 / Fire
* Right click = R3 / Scope
* Middle Mouse btn = R2 / Switch to primary weapon
* WASD = movement
* Arrows = dpad
* E = X / use button
* R = Circle / reload
* Q = Square
* F = Triangle / parachute, switch seat
* Space = L1 / jump
* CTRL = L2 / crouch
* Enter = Start
* TAB = Select

## How to Use
1. Start emulator first
2. Start MouseInjector, read initial information then press ctrl+1
3. Make sure game is running and press '0' to hook into the process
    1. If game is supported then the mouse will be captured at the position it was at when hooked
        * You will be <b><u>unable</u></b> to use the mouse elsewhere while it is hooked, press 0 to unhook
    2. Unsupported/broken games will not hook and mouse won't be captured
4. Adjust options with numbers 4-7 while in-game, ctrl+0 will lock the settings
* NOTE: The cursor still moves but gets moved back to it's initial hook position so windowed mode may not
work very well if you have also mapped the mouse buttons as you may click off the window. Fullscreen is
recommended and with dual-monitors it is recommended to put the cursor in the corner before hooking to
avoid clicking off the window.




# ManyMouse

ManyMouse is Copyright (c) 2005-2012 Ryan C. Gordon and others. https://icculus.org/manymouse/
