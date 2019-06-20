#include "timercontroller.h"
#include "interruptcontroller.h"
#include <assert.h>

namespace pgb
{
TimerController::TimerController(BYTE* DIV, BYTE* TIMA, BYTE* TMA, BYTE* TAC)
	: m_DIVRegister(DIV),
	m_TIMARegister(TIMA),
	m_TMARegister(TMA),
	m_TACRegister(TAC),
	m_divTicksElapsedCounter(0),
	m_timaTicksElapsedCounter(0),
	m_timaCurrentCyclesPerUpdate(F4096_CYCLES_PER_TICK),
	m_TACFreq(F4096),
	m_timerRunning(false)
{
	// Zero memory from all registers
	*m_DIVRegister = *m_TIMARegister = *m_TMARegister = *m_TACRegister = 0;
		
	CreateTIMAFreqToCyclesMap();
}

auto TimerController::UpdateTimers(unsigned cyclesElapsed, InterruptController* ic) -> void
{
	UpdateDiv(cyclesElapsed);
	UpdateTima(cyclesElapsed, ic);
}

auto TimerController::WriteByte(WORD address, BYTE data) -> void
{
	// DIV is reset whenever it receives a write request
	if (address == DIV_REGISTER_ADDRESS)
	{
		(*m_DIVRegister) = 0;
	}
	else if (address == TMA_REGISTER_ADDRESS)
	{
		(*m_TMARegister) = data;
	}
	else if (address == TAC_REGISTER_ADDRESS)
	{
		// Data two lower bits represent the timer control update frequency
		BYTE freqData = data & 0b11;
		m_TACFreq = (TimerControlFreqs)freqData;

		m_timerRunning = (data & 0x4) == 1 ? true : false;

		(*m_TACRegister) &= 0x7;
		(*m_TACRegister) |= (data & 0x7);
	}
#ifdef M_DEBUG
	else
	{
		// Only the registers above are valid
		assert(false);
	}
#endif
}

auto TimerController::UpdateDiv(unsigned cyclesElapsed) -> void
{
	m_divTicksElapsedCounter += cyclesElapsed;

	if (m_divTicksElapsedCounter >= DIV_CYCLES_PER_UPDATE)
	{
		m_divTicksElapsedCounter -= DIV_CYCLES_PER_UPDATE;

		// DIV register is about to overflow
		if ((*m_DIVRegister) == 255)
		{
			(*m_DIVRegister) = (*m_TMARegister);
		}
		else
		{
			(*m_DIVRegister)++;
		}
	}
}

auto TimerController::UpdateTima(unsigned cyclesElapsed, InterruptController* ic) -> void
{
	m_timaTicksElapsedCounter += cyclesElapsed;

	if (m_timaTicksElapsedCounter >= m_timaCurrentCyclesPerUpdate)
	{
		m_timaTicksElapsedCounter -= m_timaCurrentCyclesPerUpdate;


		// TIMA register is about to overflow
		if ((*m_TIMARegister) == 255)
		{
			m_timaCurrentCyclesPerUpdate = m_timaFreqToCyclesMap[m_TACFreq];
			ic->RequestInterrupt(InterruptType::TIMER);
		}
		(*m_TIMARegister)++;
	}
}

auto TimerController::CreateTIMAFreqToCyclesMap() -> void
{
	m_timaFreqToCyclesMap[F4096  ] = F4096_CYCLES_PER_TICK;
	m_timaFreqToCyclesMap[F262144] = F262144_CYCLES_PER_TICK;
	m_timaFreqToCyclesMap[F65536 ] = F65536_CYCLES_PER_TICK;
	m_timaFreqToCyclesMap[F16384 ] = F16384_CYCLES_PER_TICK;
}
}
