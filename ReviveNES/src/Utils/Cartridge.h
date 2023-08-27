#pragma once

#include "UI/Utils.h"

#include <cstdint>
#include <vector>
#include <array>
#include <memory>
#include <string>

namespace NESEmu {

	struct CartridgeHeader {
		std::array<char, 4> Constant; /*Constant $4E $45 $53 $1A (ASCII "NES" followed by MS-DOS end-of-file)*/
		uint8_t PRGRomSize;/*Size of PRG ROM in 16 KB units*/
		uint8_t CHRRomSize;/*Size of CHR ROM in 8 KB units (value 0 means the board uses CHR RAM)*/
		uint8_t Flags6;/*Mapper, mirroring, battery, trainer*/
		uint8_t Flags7;/*Mapper, VS/Playchoice, NES 2.0*/
		uint8_t Flags8;/*PRG-RAM size (rarely used extension)*/
		uint8_t Flags9;/*TV system (rarely used extension)*/
		uint8_t Flags10;/*TV system, PRG-RAM presence (unofficial, rarely used extension)*/
		std::array<uint8_t, 5> Unused;/*Unused padding (should be filled with zero, but some rippers put their name across bytes 7-15)*/
	};

	enum class NESFileVersion {
		ArchaiciNES,
		iNES,
		NES20
	};

	class Cartridge {
	public:
		Cartridge() = default;
		~Cartridge() = default;

		uint8_t read(uint16_t address);
		void write(uint16_t address, uint8_t value);

		static std::shared_ptr<Cartridge> loadFromFile(const std::string& filePath);

	private:
		static void read_iNES(const std::shared_ptr<Cartridge>& cartridge, const DataBuffer& buffer);
		static void read_NES20(const std::shared_ptr<Cartridge>& cartridge, const DataBuffer& buffer);

	private:
		CartridgeHeader mHeader;
		NESFileVersion mHeaderVersion;

		std::vector<uint8_t> mTrainer;
		std::vector<uint8_t> mPRGRom;
		std::vector<uint8_t> mPRGRam;
		std::vector<uint8_t> mCHRRom;
		std::vector<uint8_t> mPlayChoiceInstRom;
		std::vector<uint8_t> mPlayChoicePRom;
	};

}