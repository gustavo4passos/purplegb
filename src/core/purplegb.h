#pragma once

#include <queue>
#include <string>

namespace pgb
{
using BYTE 		  = unsigned char;
using SIGNED_BYTE = char;
using WORD		  = unsigned short;
using SIGNED_WORD = short;

#define EXRAM_START_ADDRESS 0xA000
#define ERAM_BANK_SIZE 0x2000
#define FIRST_ROM_BANK 0x4000

enum class MBCType
{
	NONE,
	MBC1,
	MBC2,
	MBC3,
	UNKNOWN
};

enum class MBC1BankMode
{
	ROM_MODE,
	RAM_MODE
};

/* 	WARNING: This class may be to big to be
	initialized on the stack. Always initi-
	alize it on the heap					*/

class PurpleGB
{
public: 
	PurpleGB();
	auto LoadROM(const char* filename) -> bool;
	auto GetError() -> const std::string;

private:

	BYTE m_cartridgeROM[0x200000];
	BYTE m_internalROM[0x10000];
	BYTE m_externalRAM[0x8000];

	std::queue<std::string> m_errorQueue;

	MBCType m_mbcType;
	bool m_externalRAMEnabled;
	bool m_ROMBankingEnabled;

	MBC1BankMode m_mbc1BankMode;
	BYTE m_currentROMBank;
	BYTE m_currentRAMBank;

	auto Load(WORD address) -> BYTE;
	auto WriteByte(WORD address, BYTE data)		-> void;
	auto MBCIntercept(WORD address, BYTE data)  -> void;

	auto MBC1Intercept(WORD address, BYTE data) -> void;
	auto MBC2Intercept(WORD address, BYTE data) -> void;

	auto GetMBCTypeFromCartridge() -> MBCType;

};

}
