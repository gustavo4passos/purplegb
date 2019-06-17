#pragma once

struct InterruptFlags
{
	bool vblank;
	bool lcdStat;
	bool timer;
	bool serial;
	bool joypad;
};