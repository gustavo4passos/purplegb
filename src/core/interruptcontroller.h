#pragma once

namespace pgb
{

using BYTE 		  = unsigned char;
using SIGNED_BYTE = char;
using WORD		  = unsigned short;
using SIGNED_WORD = short;

constexpr unsigned VBLANK_INT_BIT  = 0;
constexpr unsigned LCDSTAT_INT_BIT = 1;
constexpr unsigned TIMER_INT_BIT   = 2;
constexpr unsigned SERIAL_INT_BIT  = 3;
constexpr unsigned JOYPAD_INT_BIT  = 4;

constexpr unsigned INTERRUPT_ENABLE_REGISTER_ADDRESS = 0xFFFF;
constexpr unsigned INTERRUPT_FLAG_REGISTER_ADDRESS = 0xFF0F;

class PurpleGB;

enum class InterruptType
{
	VBLANK,
	LCDSTAT,
	TIMER,
	SERIAL,
	JOYPAD
};

class InterruptController
{
public:
	explicit InterruptController(BYTE* IE, BYTE* IF);

	auto EnableInterrupts() -> void;
	auto DisableInterrupts() -> void;

	auto RequestInterrupt(InterruptType type)	-> void;
	auto EnableInterrupt(InterruptType type)	-> void;
	auto ServeInterrupt(InterruptType type)		-> void;
	auto Write(WORD address, BYTE data)			-> void;
	auto Load(WORD address)						-> BYTE;
	auto IsInterruptEnabled(InterruptType type) -> bool;

	auto AreIterruptsEnabled() { return m_interruptMasterFlag; }

private:
	bool m_interruptMasterFlag;
	BYTE* m_interruptEnableRegister;
	BYTE* m_interruptFlagRegister;

	auto GetFlagBitFromIntType(InterruptType type) -> int;
};

}