#pragma once

#include "Base/Bus.h"
#include "Utils/Cartridge.h"

namespace NESEmu {

	class CartridgeSlot : public BusDevice {
	public:
		CartridgeSlot();
		~CartridgeSlot() = default;

		virtual void write(const std::string& busName, uint16_t address, uint8_t value) override;
		virtual uint8_t read(const std::string& busName, uint16_t address) override;

		void insert(const std::shared_ptr<Cartridge>& cartridge);

	private:
		std::shared_ptr<Cartridge> mCartridge;
	};

}