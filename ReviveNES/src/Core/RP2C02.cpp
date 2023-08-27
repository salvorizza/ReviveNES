#include "RP2C02.h"

namespace NESEmu {

	RP2C02::RP2C02(const Palette& palette)
		:	BusDevice("RP2C02"),
			mPalette(palette)
	{
		addRange("CPU", 0x2000, 0x2008, 0x2007);
		addRange("CPU", 0x4014, 0x4015, 0xFFFF);
	}

	RP2C02::~RP2C02()
	{
	}

	void RP2C02::powerUp()
	{
		mRegisters.PPUCTRL = 0x00;
		mRegisters.PPUMASK = 0x00;
		mRegisters.PPUSTATUS &= ~0xE0;
		mRegisters.PPUSTATUS |= 0xA0;
		mRegisters.OAMADDR = 0x00;
		mRegisters.w = false;
		mRegisters.PPUSCROLL = 0x0000;
		mRegisters.PPUADDR = 0x0000;
		mRegisters.PPUDATA = 0x00;
		mEvenFrame = true;
	}

	void RP2C02::reset()
	{
		mRegisters.PPUCTRL = 0x00;
		mRegisters.PPUMASK = 0x00;
		mRegisters.w = false;
		mRegisters.PPUSCROLL = 0x0000;
		mRegisters.PPUDATA = 0x00;
		mEvenFrame = true;
	}

	void RP2C02::clock()
	{		 
		if (mCycles == 0 && mScanline >= 0 && mScanline <= 239) {
			//IDLE CYCLE
			mCycles++;
			return;
		}

		//VBLANK Status & PPUSTATUS Clearing
		if (mCycles == 1) {
			if (mScanline == 241) {
				mRegisters.PPUSTATUS |= 0x80;
				mRegisters.NMI_occurred = 1;

				if (mRegisters.NMI_occurred && mRegisters.NMI_output) {
					//Pull NMI low requesting an NMI
					getBus("CPU")->writeLine("NMI", true);
					getBus("CPU")->writeLine("NMI", false);
				}

			} else if (mScanline == -1) {
				mRegisters.PPUSTATUS &= 0x1F;
				mRegisters.NMI_occurred = 0;
			}
		}

		if (mScanline <= 239) {
			if (mCycles >= 257 && mCycles <= 320) {
				mRegisters.OAMADDR = 0x00;
			}
		}

		//RENDERING
		if (mScanline >= -1 && mScanline <= 239 && mCycles >= 1 && mCycles <= 256) {
			if (mCycles <= 256) {
				uint8_t ppuState = mCycles % 8;
				uint8_t fineY = (mRegisters.v >> 12) & 0x3;

				switch (ppuState) {
				case 2:
					//NT Byte
					mNTByte = readCurrentTile();
					break;

				case 4:
					//AT Byte
					mATByte = readCurrentTileAttribute();

					if (((mRegisters.v >> 5) & 0x1F) & 0x02) mATByte >>= 4;
					if ((mRegisters.v & 0x1F) & 0x02) mATByte >>= 2;
					mATByte &= 0x03;
					break;

				case 6:
					//BG lsbits
					mBGplane0 = readBGTilePlanePattern(mNTByte, 0, fineY);
					break;

				case 7:
					//BG msbit
					mBGplane1 = readBGTilePlanePattern(mNTByte, 1, fineY);
					break;
				}
			} else if (mCycles <= 320) {
				//The tile data for the sprites on the next scanline are fetched here.
			} else if (mCycles <= 336) {
				//This is where the first two tiles for the next scanline are fetched, and loaded into the shift registers
				uint8_t ppuState = mCycles % 8;
				uint8_t fineY = (mRegisters.v >> 12) & 0x3;

				switch (ppuState) {
				case 2:
					//NT Byte
					mNTByte = readCurrentTile();
					break;

				case 4:
					//AT Byte
					mATByte = readCurrentTileAttribute();

					if (((mRegisters.v >> 5) & 0x1F) & 0x02) mATByte >>= 4;
					if ((mRegisters.v & 0x1F) & 0x02) mATByte >>= 2;
					mATByte &= 0x03;
					break;

				case 6:
					//BG lsbits
					mBGplane0 = readBGTilePlanePattern(mNTByte, 0, fineY);
					break;

				case 0:
					//BG msbit
					mBGplane1 = readBGTilePlanePattern(mNTByte, 1, fineY);

					mBGPlane0ShiftRegister <<= 8;
					mBGPlane0ShiftRegister |= mBGplane0;

					mBGPlane1ShiftRegister <<= 8;
					mBGPlane1ShiftRegister |= mBGplane1;

					mBGPaletteAttributeShiftRegisters[0] = mATByte;

					std::swap(mBGPaletteAttributeShiftRegisters[0], mBGPaletteAttributeShiftRegisters[1]);
					break;
				}
			} else if (mCycles <= 340) {
				//Two bytes are fetched, but the purpose for this is unknown
				uint8_t ppuState = mCycles % 2;
				if (ppuState == 0) {
					mNTByte = readCurrentTile();
				}
			}

			if (mCycles >= 9 && mCycles <= 257) {
				//Shift registers reloading
				uint8_t ppuState = mCycles % 8;
				if (ppuState == 1) {
					mBGPlane0ShiftRegister &= 0xFF00;
					mBGPlane0ShiftRegister |= mBGplane0;

					mBGPlane1ShiftRegister &= 0xFF00;
					mBGPlane1ShiftRegister |= mBGplane1;

					std::swap(mBGPaletteAttributeShiftRegisters[0], mBGPaletteAttributeShiftRegisters[1]);

					mBGPaletteAttributeShiftRegisters[1] = mATByte;
				}
			}

			
			if (mScanline >= 0 && mCycles <= 256) {
				uint8_t paletteColorIndex = (mBGPlane0ShiftRegister & (1 << (16 - mRegisters.x)) ? 0b01 : 0b00) | (mBGPlane1ShiftRegister & (1 << (16 - mRegisters.x)) ? 0b10 : 0b00);
				uint8_t paletteIndex = getBus("PPU")->read(0x3F00 + (mBGPaletteAttributeShiftRegisters[0] * 4) + 1 + paletteColorIndex);
				PaletteColor pixelColor = mPalette[paletteIndex];

				mScreen[mScanline * 256 + (mCycles - 1)] = pixelColor;

				mBGPlane0ShiftRegister <<= 1;
				mBGPlane1ShiftRegister <<= 1;
			}
		}

		//Scrolling
		if (mCycles == 256) {
			if ((mRegisters.PPUMASK & 0x18) != 0x0) {
				if ((mRegisters.v & 0x7000) != 0x7000) {
					mRegisters.v += 0x1000;
				}
				else {
					mRegisters.v &= ~0x7000;
					uint16_t y = (mRegisters.v & 0x03E0) >> 5;
					switch (y) {
						case 29:
							y = 0;
							mRegisters.v ^= 0x0800;
							break;

						case 31:
							y = 0;
							break;

						default:
							y++;
							break;
					}

					mRegisters.v = (mRegisters.v & ~0x03E0) | (y << 5);
				}
			}
		} else if (mCycles == 257) {
			mRegisters.v &= ~0x041F;
			mRegisters.v |= (mRegisters.t & 0x041F);
		} else if (mCycles >= 280 && mCycles <= 304 && mScanline == -1) {
			mRegisters.v &= ~0x7BE0;
			mRegisters.v |= (mRegisters.t & 0x7BE0);
		} else {
			if ((mRegisters.PPUMASK & 0x18) != 0x0) {
				if (mCycles % 8 == 0) {
					if ((mRegisters.v & 0x001F) == 31) {
						mRegisters.v &= ~0x001F;
						mRegisters.v ^= 0x0400;
					}
					else {
						mRegisters.v++;
					}
				}
			}
		}

		if (mCycles == 340 || (!mEvenFrame && mScanline == -1 && mCycles == 339)) {
			mCycles = 0;
			mScanline++;
			if (mScanline > 260) {
				mEvenFrame = !mEvenFrame;
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
					mRegisters.t &= ~0x0C00;
					mRegisters.t |= (value & 0x3) << 10;

					uint8_t oldNMI_output = (mRegisters.PPUCTRL & 0x80) != 0x00 ? 1 : 0;
					uint8_t newNMI_output = (value & 0x80) != 0x00 ? 1 : 0;
					if (mRegisters.NMI_occurred && oldNMI_output == 0 && newNMI_output == 1) {
						//Pull NMI low requesting an NMI
						getBus("CPU")->writeLine("NMI", true);
						getBus("CPU")->writeLine("NMI", false);
					}
					mRegisters.NMI_output = newNMI_output;

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
					if (mRegisters.w) {
						mRegisters.t &= ~0x73E0;
						mRegisters.t |= ((value & 0x7) << 12) | ((value & 0xF8) << 5);
					} else {
						mRegisters.t &= ~0x001F;
						mRegisters.t |= value >> 3;
						mRegisters.x = value & 0x7;
					}
					mRegisters.w = !mRegisters.w;
					break;
				}
				case PPUADDR_ADDR: {
					if (mRegisters.w) {
						mRegisters.t &= ~0x00FF;
						mRegisters.t |= value;

						mRegisters.v = mRegisters.t;
					} else {
						mRegisters.t &= ~0x3F00;
						mRegisters.t |= (value & 0x3F) << 8;

						mRegisters.t &= 0x3FFF;
					}
					mRegisters.w = !mRegisters.w;
					break;
				}
				case PPUDATA_ADDR: {
					getBus("PPU")->write(mRegisters.v & 0x3FFF, value);
					mRegisters.v += (mRegisters.PPUCTRL & 0x2) ? 32 : 1;
					break;
				}
				case OAMDMA_ADDR: {
					getBus("CPU")->writeLine("DMA", true);
					for (uint16_t i = 0; i < 0x100; i++) {
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
					uint8_t value = mRegisters.PPUSTATUS & 0x3F;
					value |= mRegisters.NMI_occurred << 7;

					mRegisters.NMI_occurred = false;
					mRegisters.w = false;

					return value;
				}
				case OAMDATA_ADDR: {
					return mRegisters.OAM[mRegisters.OAMADDR];
				}
				case PPUDATA_ADDR: {
					uint8_t value = getBus("PPU")->read(mRegisters.v & 0x3FFF);
					mRegisters.v += (mRegisters.PPUCTRL & 0x2) ? 32 : 1;
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
		uint16_t address = 0x2000 | (mRegisters.v & 0x0FFF);
		return getBus("PPU")->read(address);
	}

	uint8_t RP2C02::readCurrentTileAttribute() {
		uint16_t address = 0x23C0 | (mRegisters.v & 0x0C00) | ((mRegisters.v >> 4) & 0x38) | ((mRegisters.v >> 2) & 0x07);
		return getBus("PPU")->read(address);
	}

}