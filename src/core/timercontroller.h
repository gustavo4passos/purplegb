#pragma once
#include "purplegb.h"
#include <map>

namespace pgb
{

constexpr unsigned DIV_REGISTER_ADDRESS  = 0xFF04;
constexpr unsigned TIMA_REGISTER_ADDRESS = 0xFF05;
constexpr unsigned TMA_REGISTER_ADDRESS  = 0xFF06;
constexpr unsigned TAC_REGISTER_ADDRESS  = 0xFF07;

constexpr unsigned F4096_CYCLES_PER_TICK   = CPU_CLOCK_SPEED / 4096;
constexpr unsigned F262144_CYCLES_PER_TICK = CPU_CLOCK_SPEED / 262144;
constexpr unsigned F65536_CYCLES_PER_TICK  = CPU_CLOCK_SPEED / 65536;
constexpr unsigned F16384_CYCLES_PER_TICK  = CPU_CLOCK_SPEED / 16384;

constexpr unsigned DIV_UPDATE_RATE = 16384;
constexpr unsigned DIV_CYCLES_PER_UPDATE = CPU_CLOCK_SPEED / DIV_UPDATE_RATE;

class InterruptController;

class TimerController
{

public:
	TimerController(BYTE* DIV, BYTE* TIMA, BYTE* TMA, BYTE* TAC);

	auto UpdateTimers(unsigned cyclesElapsed, InterruptController* ic) -> void;
	auto WriteByte(WORD address, BYTE data) -> void;

private:
	BYTE* m_DIVRegister;
	BYTE* m_TIMARegister;
	BYTE* m_TMARegister;
	BYTE* m_TACRegister;

	int m_divTicksElapsedCounter;

	unsigned m_timaTicksElapsedCounter;
	unsigned m_timaCurrentCyclesPerUpdate;

	enum TimerControlFreqs
	{
		F4096	= 0x00,
		F262144 = 0x01,
		F65536	= 0X02,
		F16384	= 0x03
	};

	TimerControlFreqs m_TACFreq;
	std::map<TimerControlFreqs, unsigned> m_timaFreqToCyclesMap;
	bool m_timerRunning;


	auto UpdateDiv(unsigned cyclesElapsed) -> void;
	auto UpdateTima(unsigned cyclesElapsed, InterruptController* ic) -> void;
	auto CreateTIMAFreqToCyclesMap() -> void;
};
}