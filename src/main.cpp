#include <iostream>
#include "core/purplegb.h"

int main(int argc, char* args[])
{
	pgb::PurpleGB* gb = new pgb::PurpleGB();

	if (!gb->LoadROM("roms/tetris.gb"))
	{
		std::cout << gb->GetError() << std::endl;
	}
	else
	{
		std::cout << "ROM sucessfully loaded." << std::endl;
		std::cout << "MBC type: " << gb->CartridgeMBCType() << std::endl;
		gb->Run();
	}

	return 0;
}