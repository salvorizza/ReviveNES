#include "CartridgeSlot.h"

namespace NESEmu {
	CartridgeSlot::CartridgeSlot()
		: BusDevice("CartridgeSlot")
	{
		addRange("CPU", 0x4020, 0x10000, 0xFFFF);
	}

	void CartridgeSlot::write(const std::string& busName, uint16_t address, uint8_t value)
	{
		mCartridge->write(address, value);
	}

	uint8_t CartridgeSlot::read(const std::string& busName, uint16_t address)
	{
		return mCartridge->read(address);
	}

	void CartridgeSlot::insert(const std::shared_ptr<Cartridge>& cartridge)
	{
		mCartridge = cartridge;
	}
}