#include "interruptcontroller.h"
#include "../utils/bitmanip.h"
#include <assert.h>

namespace pgb
{

InterruptController::InterruptController(BYTE* IE, BYTE* IF)
	: m_interruptMasterFlag(false),
	m_interruptEnableRegister(IE),
	m_interruptFlagRegister(IF)
{ }

auto InterruptController::EnableInterrupts() -> void
{
	m_interruptMasterFlag = true;
}

auto InterruptController::DisableInterrupts() -> void
{
	m_interruptMasterFlag = false;
}

auto InterruptController::RequestInterrupt(InterruptType type) -> void
{
	int flagBit = GetFlagBitFromIntType(type);
	BitManip::SetBit<BYTE>(*m_interruptFlagRegister, flagBit);
}

auto InterruptController::EnableInterrupt(InterruptType type) -> void
{
	int flagBit = GetFlagBitFromIntType(type);
	BitManip::SetBit<BYTE>(*m_interruptEnableRegister, flagBit);
}

auto InterruptController::ServeInterrupt(InterruptType type) -> void
{
	int flagBit = GetFlagBitFromIntType(type);
	BitManip::UnsetBit<BYTE>(*m_interruptEnableRegister, flagBit);
	BitManip::UnsetBit<BYTE>(*m_interruptFlagRegister, flagBit);
	m_interruptMasterFlag = false;
}

auto InterruptController::Write(WORD address, BYTE data) -> void
{
#ifdef M_DEBUG
	assert(address == INTERRUPT_ENABLE_REGISTER_ADDRESS ||
		address == INTERRUPT_FLAG_REGISTER_ADDRESS);
#endif
	data &= 0b00011111;

	if (address == INTERRUPT_ENABLE_REGISTER_ADDRESS)
		*m_interruptEnableRegister = data;
	else *m_interruptFlagRegister = data;
}

auto InterruptController::Load(WORD address) -> BYTE
{
#ifdef M_DEBUG
	assert(address == INTERRUPT_ENABLE_REGISTER_ADDRESS ||
		address == INTERRUPT_FLAG_REGISTER_ADDRESS);
#endif

	if (address == INTERRUPT_ENABLE_REGISTER_ADDRESS)
		return *m_interruptEnableRegister;
	else return *m_interruptFlagRegister;
}

auto InterruptController::IsInterruptEnabled(InterruptType type) -> bool
{
	int flagBit = GetFlagBitFromIntType(type);
	return BitManip::BitTest(*m_interruptEnableRegister, flagBit);
}


auto InterruptController::GetFlagBitFromIntType(InterruptType type) -> int
{
	switch (type)
	{
	case InterruptType::VBLANK: return VBLANK_INT_BIT;
	case InterruptType::LCDSTAT: return LCDSTAT_INT_BIT;
	case InterruptType::TIMER: return TIMER_INT_BIT;
	case InterruptType::SERIAL: return SERIAL_INT_BIT;
	case InterruptType::JOYPAD: return JOYPAD_INT_BIT;
	}

#ifdef M_DEBUG
	// This point should never be hit
	assert(false);
	return VBLANK_INT_BIT;
#endif
}


}