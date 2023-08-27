#include "Cartridge.h"


namespace NESEmu {

	void Cartridge::read_iNES(const std::shared_ptr<Cartridge>& cartridge, const DataBuffer& buffer) {
		size_t bufferPointer = 0;

		std::memcpy(&cartridge->mHeader, &buffer.Data[bufferPointer], sizeof(CartridgeHeader));
		bufferPointer += sizeof(CartridgeHeader);

		if ((cartridge->mHeader.Flags6 & 0x4) != 0) {
			size_t trainerRomSizeInBytes = BYTE(512);
			cartridge->mTrainer.resize(trainerRomSizeInBytes);
			std::memcpy(cartridge->mTrainer.data(), &buffer.Data[bufferPointer], trainerRomSizeInBytes);
			bufferPointer += trainerRomSizeInBytes;
		}

		size_t prgRomSizeInBytes = KIBI(16) * cartridge->mHeader.PRGRomSize;
		cartridge->mPRGRom.resize(prgRomSizeInBytes);
		std::memcpy(cartridge->mPRGRom.data(), &buffer.Data[bufferPointer], prgRomSizeInBytes);
		bufferPointer += prgRomSizeInBytes;

		if ((cartridge->mHeader.Flags10 & 0x10) != 0) {
			cartridge->mPRGRam.resize(KIBI(8));
		}

		if (cartridge->mHeader.Flags8 != 0) {
			cartridge->mPRGRam.resize(KIBI(8) * cartridge->mHeader.Flags8);
		}

		size_t chrRomSizeInBytes = KIBI(8) * cartridge->mHeader.CHRRomSize;
		cartridge->mCHRRom.resize(chrRomSizeInBytes);
		std::memcpy(cartridge->mCHRRom.data(), &buffer.Data[bufferPointer], chrRomSizeInBytes);
		bufferPointer += chrRomSizeInBytes;

		if ((cartridge->mHeader.Flags7 & 0x2) != 0) {
			size_t playChoiceInstRomSizeInBytes = BYTE(512);
			cartridge->mPlayChoiceInstRom.resize(playChoiceInstRomSizeInBytes);
			std::memcpy(cartridge->mPlayChoiceInstRom.data(), &buffer.Data[bufferPointer], playChoiceInstRomSizeInBytes);
			bufferPointer += playChoiceInstRomSizeInBytes;
		}

		uint8_t mapperNumber = (cartridge->mHeader.Flags7 & 0xF0) | ((cartridge->mHeader.Flags6 & 0xF0) >> 4);

	}

	void Cartridge::read_NES20(const std::shared_ptr<Cartridge>& cartridge, const DataBuffer& buffer) {
	}


	uint8_t Cartridge::read(uint16_t address)
	{
		uint8_t value = 0;

		if (address >= 0x6000 && address <= 0x7FFF) {
			value = mPRGRam[(address & 0xBFFF) - 0x8000];
		} else if (address >= 0x8000 && address <= 0xFFFF) {
			value = mPRGRom[(address & 0xBFFF) - 0x8000];
		}

		return value;
	}

	void Cartridge::write(uint16_t address, uint8_t value)
	{
	}

	std::shared_ptr<Cartridge> Cartridge::loadFromFile(const std::string& filePath)
	{
		auto cartridge = std::make_shared<Cartridge>();


		DataBuffer buffer;
		auto result = ReadFile(filePath.c_str(), buffer);

		cartridge->mHeaderVersion = NESFileVersion::iNES;
		if ((buffer.Data[7] & 0x0C) == 0x08) {
			cartridge->mHeaderVersion = NESFileVersion::NES20;
		}
		else if ((buffer.Data[7] & 0x0C) == 0x04) {
			cartridge->mHeaderVersion = NESFileVersion::ArchaiciNES;
		}
		else if ((buffer.Data[7] & 0xC) == 0x00 && buffer.Data[12] == 0x00 && buffer.Data[13] == 0x00 && buffer.Data[14] == 0x00 && buffer.Data[15] == 0x00) {
			cartridge->mHeaderVersion = NESFileVersion::iNES;
		}
		else {
			cartridge->mHeaderVersion = NESFileVersion::iNES;
		}

		switch (cartridge->mHeaderVersion) {
			case NESFileVersion::iNES: {
				read_iNES(cartridge, buffer);
				break;
			}
		}


		DeleteBuffer(buffer);

		return cartridge;
	}

}