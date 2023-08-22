#pragma once

#include "Base/Bus.h"

#include <array>

namespace NESEmu {

	class VRAM : public BusDevice {
	public:
		VRAM();
		~VRAM();

		virtual void write(const std::string& busName, uint16_t address, uint8_t value) override;
		virtual uint8_t read(const std::string& busName, uint16_t address) override;

	private:
		std::array<uint8_t, 2048> mMemory;
	};

}