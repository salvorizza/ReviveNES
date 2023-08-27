#include "WRAM.h"

namespace NESEmu {



	WRAM::WRAM()
		: BusDevice("WRAM"),
			mMemory()
	{
		addRange("CPU", 0x0000, 0x2000, 0x07FF);

		std::memset(mMemory.data(), 0xAA, 0x800);
	}

	WRAM::~WRAM()
	{
	}

	void WRAM::write(const std::string& busName, uint16_t address, uint8_t value)
	{
		mMemory[address] = value;
	}

	uint8_t WRAM::read(const std::string& busName, uint16_t address)
	{
		return mMemory[address];
	}

}