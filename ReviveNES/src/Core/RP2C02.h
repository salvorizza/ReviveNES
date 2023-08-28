#pragma once

#include "Base/Bus.h"
#include "Utils/Palette.h"

#include <array>

namespace NESEmu {
	constexpr uint16_t PPUCTRL_ADDR = 0x2000;//0x2000
	constexpr uint16_t PPUMASK_ADDR = 0x2001;//0x2001
	constexpr uint16_t PPUSTATUS_ADDR = 0x2002;//0x2002
	constexpr uint16_t OAMADDR_ADDR = 0x2003;//0x2003
	constexpr uint16_t OAMDATA_ADDR = 0x2004;//0x2004
	constexpr uint16_t PPUSCROLL_ADDR = 0x2005;//0x2005
	constexpr uint16_t PPUADDR_ADDR = 0x2006;//0x2006
	constexpr uint16_t PPUDATA_ADDR = 0x2007;//0x2007
	constexpr uint16_t OAMDMA_ADDR = 0x4014;//0x4014

	struct RP2C02_Registers {
		uint8_t PPUCTRL;	/*VPHB SINN 	NMI enable (V), PPU master/slave (P), sprite height (H), background tile select (B), sprite tile select (S), increment mode (I), nametable select (NN)*/
		uint8_t PPUMASK;	/*BGRs bMmG 	color emphasis (BGR), sprite enable (s), background enable (b), sprite left column enable (M), background left column enable (m), greyscale (G)*/
		uint8_t PPUSTATUS;	/*VSO- ---- 	vblank (V), sprite 0 hit (S), sprite overflow (O); read resets write pair for $2005/$2006*/
		uint8_t OAMADDR;	/*aaaa aaaa 	OAM read/write address*/
		uint8_t PPUSCROLL;	/*xxxx xxxx 	fine scroll position (two writes: X scroll, Y scroll)*/
		uint8_t PPUADDR;	/*aaaa aaaa 	PPU read/write address (two writes: most significant byte, least significant byte)*/
		uint8_t PPUDATA;	/* dddd 	PPU data read/write*/
		uint8_t OAMDMA;		/*aaaa aaaa 	OAM DMA high address*/
		std::array<uint8_t, 256> OAM; 

		uint8_t NMI_occurred;
		uint8_t NMI_output;

		uint16_t v; /*Current VRAM address(15 bits)*/
		uint16_t t; /*Temporary VRAM address(15 bits); can also be thought of as the address of the top left onscreen tile.*/
		uint8_t x; /*Fine X scroll(3 bits)*/
		bool w; /*First or second write toggle(1 bit)*/
	};

	
	class RP2C02 : public BusDevice {
	public:
		RP2C02(const Palette& palette);
		~RP2C02();

		void powerUp();
		void reset();
		void clock();

		virtual void write(const std::string& busName, uint16_t address, uint8_t value) override;
		virtual uint8_t read(const std::string& busName, uint16_t address) override;

	private:
		uint8_t readBGTilePlanePattern(uint8_t tile,uint8_t bitPlane,uint8_t fineY);
		uint8_t readCurrentTile();
		uint8_t readCurrentTileAttribute();
	public:
		Palette mPalette;
		RP2C02_Registers mRegisters;
		int16_t mScanline = 0;
		uint16_t mCycles = 0;

		uint8_t mNTByte, mATByte, mBGplane0, mBGplane1;

		uint16_t mBGPlane0ShiftRegister, mBGPlane1ShiftRegister;
		std::array<uint8_t,2> mBGPaletteAttributeShiftRegisters;
		bool mEvenFrame = true;
	public:
		std::array<PaletteColor, 256 * 240> mScreen;
	};

}