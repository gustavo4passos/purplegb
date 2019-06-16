#include "purplegb.h"

#include <fstream>
#include <assert.h>
#include "../utils/logger.h"

namespace pgb
{

PurpleGB::PurpleGB()
	: m_mbcType(MBCType::NONE),
	m_externalRAMEnabled(false),
	m_ROMBankingEnabled(false),
	m_currentROMBank(0x1000)
{
	memset(&m_cartridgeROM[0], 0, sizeof(m_cartridgeROM));
	memset(&m_internalROM[0], 0, sizeof(m_internalROM));
	memset(&m_externalRAM[0], 0, sizeof(m_externalRAM));
}

auto PurpleGB::GetError() -> const std::string
{
	if (m_errorQueue.size() == 0) return "";

	const std::string errorMessage = m_errorQueue.front();
	m_errorQueue.pop();
	return errorMessage;
}

auto PurpleGB::Load(WORD address) -> BYTE
{
	if (address >= 0x4000 && address <= 0x7FFF)
	{
		// ROM bank address space
		if (m_mbcType == MBCType::NONE) return m_cartridgeROM[address];
		else return m_cartridgeROM[m_currentROMBank * 0x4000 + address - 0x4000];

	}
	else if (address >= EXRAM_START_ADDRESS && address <= 0xBFFF)
	{
		// External RAM address space
		if (m_mbcType == MBCType::NONE)
		{
			LOG_WARNING("Type-0 cartridge is trying to load from external RAM.");
		}
		else
		{
			WORD offset = address - EXRAM_START_ADDRESS;
			return m_externalRAM[EXRAM_START_ADDRESS + m_currentRAMBank * ERAM_BANK_SIZE + offset];
		}

	}

	return m_internalROM[address];
}

auto PurpleGB::WriteByte(WORD address, BYTE data) -> void
{
	// Write requests to this are of the address space
	// is intercepted by the MBC controller, and are 
	// used to control RAM banking
	if (address <= 0x7FFF)
	{
		MBCIntercept(address, data);
	}

}

auto PurpleGB::MBCIntercept(WORD address, BYTE data) -> void
{
	if (m_mbcType == MBCType::MBC1)
	{
		MBC1Intercept(address, data);
	}
	else if (m_mbcType == MBCType::MBC2)
	{
		MBC2Intercept(address, data);
	}
	else if (m_mbcType == MBCType::MBC3)
	{

	}
	else return;
}

auto PurpleGB::MBC1Intercept(WORD address, BYTE data) -> void
{
	// RAM enable/ disable
	if (address <= 0x1FFF)
	{
		if ((data & 0b1111) == 0x0A)
		{
			m_externalRAMEnabled = true;
		}
		else m_externalRAMEnabled = false;
	}
	// ROM bank number
	else if (address >= 0x2000 && address <= 0x3FFF)
	{
		BYTE bankNumber = data & 0b11111;
		// 0 defaults to 1
		bankNumber = bankNumber == 0 ? 1 : bankNumber;
		m_currentROMBank = (m_currentROMBank & 0b11100000) | bankNumber;
	}
	else if (address >= 0x4000 && address <= 0x5FFF)
	{
		if (m_mbc1BankMode == MBC1BankMode::RAM_MODE)
		{
			m_currentRAMBank = data & 0b11;
		}
		else
		{
			m_currentROMBank = (m_currentROMBank & 0b11111) | (data << 5);
		}
	}
	// ROM/RAM mode select
	if (address >= 0x6000 && address <= 0x7FFF)
	{
		if ((data & 0b1) == 0x00)
		{
			m_mbc1BankMode = MBC1BankMode::ROM_MODE;

		}
		else
		{
			m_mbc1BankMode = MBC1BankMode::RAM_MODE;
		}
	}
}

auto PurpleGB::MBC2Intercept(WORD address, BYTE data) -> void
{
	// RAM Enable/Disable
	if (address <= 0x1FFF)
	{
		WORD toggle = (address & 0xA0) >> 8;
		if (toggle == 0b0)
		{
			m_externalRAMEnabled = !m_externalRAMEnabled;
		}
	}
	// ROM Bank selection
	else if (address >= 0x2000 && address <= 0x3FFF)
	{
		WORD enableSelection = (address & 0xA0) >> 8;
		if (enableSelection == 0b1)
		{
			m_currentROMBank = data & 0x15;
		}
	}
}

auto PurpleGB::GetMBCTypeFromCartridge() -> MBCType
{
	// Cartridge type info is on address 0x0147 in the cartridge
	switch (m_cartridgeROM[0x147])
	{
	case 0x00:
		return MBCType::NONE;

	case 0x01:
	case 0x02:
	case 0x03:
		return MBCType::MBC1;

	case 0x05:
	case 0x06:
		return MBCType::MBC2;

	case 0x0F:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
		return MBCType::MBC3;

	default:
		return MBCType::UNKNOWN;
	}
}

auto PurpleGB::LoadROM(const char* filename) -> bool
{
	std::ifstream romFile;
	romFile.open(filename, std::fstream::binary);
	if (!romFile.is_open())
	{
		m_errorQueue.push(ERROR_STRING("Unable to open ROM file. File name: " + filename));
		return false;
	}

	romFile.read((char*)& m_cartridgeROM[0], 0x200000);

	m_mbcType = GetMBCTypeFromCartridge();
	return true;
}

}
