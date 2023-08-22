#pragma once

#include <cstdint>
#include <array>
#include <string>
#include <optional>

namespace NESEmu {
	struct PaletteColor {
		uint8_t R;
		uint8_t G;
		uint8_t B;
	};

	typedef std::array<PaletteColor, 64> Palette;

	namespace Utils {
		std::optional<Palette> ReadPaletteFile(const std::string& filePath);
	}
}