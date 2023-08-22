#include "VRAM.h"

namespace NESEmu {



	VRAM::VRAM()
		:	BusDevice("WRAM"),
			mMemory()
	{
		addRange("PPU", 0x2000, 0x3000, 0x27FF); // Nametable Vertical mirroring
	}

	VRAM::~VRAM()
	{
	}

	void VRAM::write(const std::string& busName, uint16_t address, uint8_t value)
	{
		mMemory[address - 0x2000] = value;
	}

	uint8_t VRAM::read(const std::string& busName, uint16_t address)
	{
		return mMemory[address - 0x2000];
	}

}