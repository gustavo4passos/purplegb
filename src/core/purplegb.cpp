#include "purplegb.h"

#include <fstream>
#include <iostream>
#include "../utils/logger.h"

namespace pgb
{

PurpleGB::PurpleGB()
{
	memset(&m_cartridgeROM[0], 0, 0x200000);
}

auto PurpleGB::GetError() -> const std::string
{
	if(m_errorQueue.size() == 0) return "";

	const std::string errorMessage = m_errorQueue.front();
	m_errorQueue.pop();
	return errorMessage;
}

auto PurpleGB::LoadROM(const char* filename) -> bool
{
	std::ifstream romFile;
	romFile.open(filename, std::fstream::binary);
	if(!romFile.is_open())
	{
		m_errorQueue.push( ERROR_STRING("Unable to open ROM file. File name: " + filename) );
		return false;
	}

	romFile.read((char*)&m_cartridgeROM[0], 0x200000);
	
	return true;
}

}
