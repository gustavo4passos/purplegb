#pragma once

#include <queue>
#include <string>

namespace pgb
{
using BYTE 		  = unsigned char;
using SIGNED_BYTE = char;
using WORD		  = unsigned short;
using SIGNED_WORD = short;

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
	std::queue<std::string> m_errorQueue;
};

}
