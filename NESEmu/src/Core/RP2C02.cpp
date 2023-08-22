#include "RP2C02.h"

namespace NESEmu {



	RP2C02::RP2C02()
		: BusDevice("RP2C02")
	{
		addRange("CPU", 0x2000, 0x2008, 0x2007);
		addRange("CPU", 0x4014, 0x4015, 0xFFFF);
	}

	RP2C02::~RP2C02()
	{
	}

	void RP2C02::clock()
	{		 
		
		//VBLANK Status & PPUSTATUS Clearing
		if (mCycles == 1) {
			if (mScanline == 241) {
				mRegisters.PPUSTATUS |= 0x80;
			} else if (mScanline == -1) {
				mRegisters.PPUSTATUS &= 0x1F;
			}
		}

		//RENDERING
		if (mCycles != 0) {
			uint8_t ppuState = mCycles % 8;

			switch (ppuState) {
				case 2:
					//NT Byte
					mNTByte = readCurrentTile();
					break;

				case 4:
					//AT Byte
					mATByte = readCurrentTileAttribute();
					break;

				case 6:
					//BG lsbits
					mBGplane0 = readBGTilePlanePattern(mNTByte, 0, mScanline % 8);
					break;

				case 7:
					//BG msbit
					mBGplane1 = readBGTilePlanePattern(mNTByte, 1, mScanline % 8);
					break;

				case 0:
					//Load into shift registers
					mBGPlane0ShiftRegister &= 0x00FF;
					mBGPlane0ShiftRegister |= mBGplane0 << 8;

					mBGPlane1ShiftRegister &= 0x00FF;
					mBGPlane1ShiftRegister |= mBGplane1 << 8;

					mBGPaletteAttributeShiftRegisters[1] = mATByte;
					break;

				case 1:
					if (mCycles > 8) {
						//Shift registers reloading
						mBGPlane0ShiftRegister >>= 8;
						mBGPlane1ShiftRegister >>= 8;
						std::swap(mBGPaletteAttributeShiftRegisters[0], mBGPaletteAttributeShiftRegisters[1]);
					}
					break;
			}
		}

		//Scrolling
		if (mCycles == 256) {
			if ((mRegisters.PPUMASK & 0x1E) != 0x0) {
				if ((mCurrentVRAMAddress & 0x7000) != 0x7000) {
					mCurrentVRAMAddress += 0x1000;
				}
				else {
					mCurrentVRAMAddress &= ~0x7000;
					uint16_t y = (mCurrentVRAMAddress & 0x03E0) >> 5;
					switch (y) {
						case 29:
							y = 0;
							mCurrentVRAMAddress ^= 0x0800;
							break;

						case 31:
							y = 0;
							break;

						default:
							y++;
							break;
					}

					mCurrentVRAMAddress = (mCurrentVRAMAddress & ~0x03E0) | (y << 5);
				}
			}
		} else if (mCycles == 257) {
			mCurrentVRAMAddress &= ~0x041F;
			mCurrentVRAMAddress |= (mTemporaryVRAMAddress & 0x041F);
		} else if (mCycles >= 280 && mCycles <= 304 && mScanline == -1) {
			mCurrentVRAMAddress &= ~0x7BE0;
			mCurrentVRAMAddress |= (mTemporaryVRAMAddress & 0x7BE0);
		} else {
			if ((mRegisters.PPUMASK & 0x1E) != 0x0) {
				if (mCycles % 8 == 0) {
					if ((mCurrentVRAMAddress & 0x001F) == 31) {
						mCurrentVRAMAddress &= ~0x001F;
						mCurrentVRAMAddress ^= 0x0400;
					}
					else {
						mCurrentVRAMAddress++;
					}
				}
			}
		}


		if (mCycles == 341) {
			mCycles = 0;
			mScanline++;
			if (mScanline > 260) {
				mScanline = -1;
			}
		}
		mCycles++;
	}

	void RP2C02::write(const std::string& busName, uint16_t address, uint8_t value)
	{
		if (busName == "CPU") {
			switch (address) {
				case PPUCTRL_ADDR: {
					mTemporaryVRAMAddress &= ~0x73FF;
					mTemporaryVRAMAddress |= (value & 0x3) << 10;

					mRegisters.PPUCTRL = value;
					break;
				}
				case PPUMASK_ADDR: {
					mRegisters.PPUMASK = value;
					break;
				}
				case OAMADDR_ADDR: {
					mRegisters.OAMADDR = value;
					break;
				}
				case OAMDATA_ADDR: {
					mRegisters.OAM[mRegisters.OAMADDR++] = value;
					break;
				}
				case PPUSCROLL_ADDR: {
					if (mWriteToggle) {
						mTemporaryVRAMAddress &= ~0x0C1F;
						mTemporaryVRAMAddress |= ((value & 0x7) << 12) | ((value & 0xF8) << 5);
					} else {
						mTemporaryVRAMAddress &= ~0x7FE0;
						mTemporaryVRAMAddress |= value >> 3;

						mFineXScroll = value & 0x7;
					}
					mWriteToggle = !mWriteToggle;
					break;
				}
				case PPUADDR_ADDR: {
					if (mWriteToggle) {
						mTemporaryVRAMAddress &= ~0xFF00;
						mTemporaryVRAMAddress |= value;

						mCurrentVRAMAddress = mTemporaryVRAMAddress;
					} else {
						mTemporaryVRAMAddress &= ~0x40FF;
						mTemporaryVRAMAddress |= (value & 0x3F) << 8;

						mTemporaryVRAMAddress &= 0x3FFF;
					}
					mWriteToggle = !mWriteToggle;
					break;
				}
				case PPUDATA_ADDR: {
					getBus("PPU")->write(mCurrentVRAMAddress & 0x3FFF, value);
					mCurrentVRAMAddress += (mRegisters.PPUCTRL & 0x2) ? 32 : 1;
					break;
				}
				case OAMDMA_ADDR: {
					for (size_t i = 0; i < 0x100; i++) {
						mRegisters.OAM[mRegisters.OAMADDR + i] = getBus("CPU")->read(i);
					}
					break;
				}
			}
		}
	}

	uint8_t RP2C02::read(const std::string& busName, uint16_t address)
	{
		if (busName == "CPU") {
			switch (address) {
				case PPUSTATUS_ADDR: {
					mRegisters.PPUSTATUS &= 0x7F;
					mWriteToggle = false;
					return mRegisters.PPUSTATUS;
				}
				case OAMDATA_ADDR: {
					return mRegisters.OAM[mRegisters.OAMADDR];
				}
				case PPUDATA_ADDR: {
					uint8_t value = getBus("PPU")->read(mCurrentVRAMAddress & 0x3FFF);
					mCurrentVRAMAddress += (mRegisters.PPUCTRL & 0x2) ? 32 : 1;
					return value;
				}
			}
		}

		return 0;
	}

	uint8_t RP2C02::readBGTilePlanePattern(uint8_t tile, uint8_t bitPlane, uint8_t fineY) {
		uint8_t h = (mRegisters.PPUCTRL & 0x8) != 0x0;
		uint16_t address = h << 12 | tile << 4 | bitPlane << 3 | fineY;
		return getBus("PPU")->read(address);
	}

	uint8_t RP2C02::readCurrentTile() {
		uint16_t address = 0x2000 | (mCurrentVRAMAddress & 0x0FFF);
		return getBus("PPU")->read(address);
	}

	uint8_t RP2C02::readCurrentTileAttribute() {
		uint16_t address = 0x23C0 | (mCurrentVRAMAddress & 0x0C00) | ((mCurrentVRAMAddress >> 4) & 0x38) | ((mCurrentVRAMAddress >> 2) & 0x07);
		return getBus("PPU")->read(address);
	}

}