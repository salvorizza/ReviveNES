#include "Palette.h"

#include <fstream>
#include <vector>

namespace NESEmu {
	namespace Utils {
		std::optional<Palette> ReadPaletteFile(const std::string& filePath)
		{
			Palette result;

			std::ifstream inFile(filePath, std::ios_base::binary);

			inFile.seekg(0, std::ios_base::end);
			size_t length = inFile.tellg();
			inFile.seekg(0, std::ios_base::beg);

			if (length != sizeof(Palette)) {
				return {};
			}

			inFile.read((char*)result.data(), result.size());

			inFile.close();

			return result;
		}
	}
}