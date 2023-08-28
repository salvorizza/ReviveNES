#include "RP2A03.h"

namespace NESEmu {
	static std::string disHex(uint32_t n, uint8_t d) {
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	}

	RP2A03::RP2A03()
		:  BusDevice("RP2A03")
	{
		addRange("CPU",0x4000, 0x4020, 0xFFFF);

		mInstructionsLookup = {
				   /*X0*/			  /*X1*/			 /*X2*/				/*X3*/			   /*X4*/			  /*X5*/			 /*X6*/				/*X7*/			   /*X8*/			  /*X9*/			 /*XA*/				/*XB*/			   /*XC*/             /*XD*/			 /*XE*/				/*XF*/
			/*0X*/ INSTR(BRK, IMP, 7),INSTR(ORA, IZX, 6),INSTR(XXX, IMP, 8),INSTR(SLO, IZX, 8),INSTR(IGN, IMM, 3),INSTR(ORA, ZP0, 3),INSTR(ASL, ZP0, 5),INSTR(SLO, ZP0, 5),INSTR(PHP, IMP, 3),INSTR(ORA, IMM, 2),INSTR(ASL, ACC, 2),INSTR(ANC, IMM, 2),INSTR(IGN, ABS, 4),INSTR(ORA, ABS, 4),INSTR(ASL, ABS, 6),INSTR(SLO, ABS, 6),
			/*1X*/ INSTR(BPL, REL, 2),INSTR(ORA, IZY, 5),INSTR(XXX, IMP, 8),INSTR(SLO, IZY, 8),INSTR(IGN, ZPX, 4),INSTR(ORA, ZPX, 4),INSTR(ASL, ZPX, 6),INSTR(SLO, ZPX, 6),INSTR(CLC, IMP, 2),INSTR(ORA, ABY, 4),INSTR(NOP, IMP, 2),INSTR(SLO, ABY, 7),INSTR(IGN, ABX, 4),INSTR(ORA, ABX, 4),INSTR(ASL, ABX, 7),INSTR(SLO, ABX, 7),
			/*2X*/ INSTR(JSR, ABS, 6),INSTR(AND, IZX, 6),INSTR(XXX, IMP, 8),INSTR(RLA, IZX, 8),INSTR(BIT, ZP0, 3),INSTR(AND, ZP0, 3),INSTR(ROL, ZP0, 5),INSTR(RLA, ZP0, 5),INSTR(PLP, IMP, 4),INSTR(AND, IMM, 2),INSTR(ROL, ACC, 2),INSTR(XXX, IMP, 8),INSTR(BIT, ABS, 4),INSTR(AND, ABS, 4),INSTR(ROL, ABS, 6),INSTR(RLA, ABS, 6),
			/*3X*/ INSTR(BMI, REL, 2),INSTR(AND, IZY, 5),INSTR(XXX, IMP, 8),INSTR(RLA, IZY, 8),INSTR(IGN, ZPX, 4),INSTR(AND, ZPX, 4),INSTR(ROL, ZPX, 6),INSTR(RLA, ZPX, 6),INSTR(SEC, IMP, 2),INSTR(AND, ABY, 4),INSTR(NOP, IMP, 2),INSTR(RLA, ABY, 7),INSTR(IGN, ABX, 4),INSTR(AND, ABX, 4),INSTR(ROL, ABX, 7),INSTR(RLA, ABX, 7),
			/*4X*/ INSTR(RTI, IMP, 6),INSTR(EOR, IZX, 6),INSTR(XXX, IMP, 8),INSTR(SRE, IZX, 8),INSTR(IGN, IMM, 3),INSTR(EOR, ZP0, 3),INSTR(LSR, ZP0, 5),INSTR(SRE, ZP0, 5),INSTR(PHA, IMP, 3),INSTR(EOR, IMM, 2),INSTR(LSR, ACC, 2),INSTR(ALR, IMM, 2),INSTR(JMP, ABS, 3),INSTR(EOR, ABS, 4),INSTR(LSR, ABS, 6),INSTR(SRE, ABS, 6),
			/*5X*/ INSTR(BVC, REL, 2),INSTR(EOR, IZY, 5),INSTR(XXX, IMP, 8),INSTR(SRE, IZY, 8),INSTR(IGN, ZPX, 4),INSTR(EOR, ZPX, 4),INSTR(LSR, ZPX, 6),INSTR(SRE, ZPX, 6),INSTR(CLI, IMP, 2),INSTR(EOR, ABY, 4),INSTR(NOP, IMP, 2),INSTR(SRE, ABY, 7),INSTR(IGN, ABX, 4),INSTR(EOR, ABX, 4),INSTR(LSR, ABX, 7),INSTR(SRE, ABX, 7),
			/*6X*/ INSTR(RTS, IMP, 6),INSTR(ADC, IZX, 6),INSTR(XXX, IMP, 8),INSTR(RRA, IZX, 8),INSTR(IGN, IMM, 3),INSTR(ADC, ZP0, 3),INSTR(ROR, ZP0, 5),INSTR(RRA, ZP0, 5),INSTR(PLA, IMP, 4),INSTR(ADC, IMM, 2),INSTR(ROR, ACC, 2),INSTR(ARR, IMM, 2),INSTR(JMP, IND, 5),INSTR(ADC, ABS, 4),INSTR(ROR, ABS, 6),INSTR(RRA, ABS, 6),
			/*7X*/ INSTR(BVS, REL, 2),INSTR(ADC, IZY, 5),INSTR(XXX, IMP, 8),INSTR(RRA, IZY, 8),INSTR(IGN, ZPX, 4),INSTR(ADC, ZPX, 4),INSTR(ROR, ZPX, 6),INSTR(RRA, ZPX, 6),INSTR(SEI, IMP, 2),INSTR(ADC, ABY, 4),INSTR(NOP, IMP, 2),INSTR(RRA, ABY, 7),INSTR(IGN, ABX, 4),INSTR(ADC, ABX, 4),INSTR(ROR, ABX, 7),INSTR(RRA, ABX, 7),
			/*8X*/ INSTR(SKB, IMM, 2),INSTR(STA, IZX, 6),INSTR(SKB, IMM, 2),INSTR(SAX, IZX, 6),INSTR(STY, ZP0, 3),INSTR(STA, ZP0, 3),INSTR(STX, ZP0, 3),INSTR(SAX, ZP0, 3),INSTR(DEY, IMP, 2),INSTR(SKB, IMM, 2),INSTR(TXA, IMP, 2),INSTR(XXX, IMP, 8),INSTR(STY, ABS, 4),INSTR(STA, ABS, 4),INSTR(STX, ABS, 4),INSTR(SAX, ABS, 4),
			/*9X*/ INSTR(BCC, REL, 2),INSTR(STA, IZY, 6),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(STY, ZPX, 4),INSTR(STA, ZPX, 4),INSTR(STX, ZPY, 4),INSTR(SAX, ZPY, 4),INSTR(TYA, IMP, 2),INSTR(STA, ABY, 5),INSTR(TXS, IMP, 2),INSTR(XXX, IMP, 8),INSTR(NOP, ABX, 4),INSTR(STA, ABX, 5),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),
			/*AX*/ INSTR(LDY, IMM, 2),INSTR(LDA, IZX, 6),INSTR(LDX, IMM, 2),INSTR(LAX, IZX, 6),INSTR(LDY, ZP0, 3),INSTR(LDA, ZP0, 3),INSTR(LDX, ZP0, 3),INSTR(LAX, ZP0, 3),INSTR(TAY, IMP, 2),INSTR(LDA, IMM, 2),INSTR(TAX, IMP, 2),INSTR(XXX, IMP, 8),INSTR(LDY, ABS, 4),INSTR(LDA, ABS, 4),INSTR(LDX, ABS, 4),INSTR(LAX, ABS, 4),
			/*BX*/ INSTR(BCS, REL, 2),INSTR(LDA, IZY, 5),INSTR(XXX, IMP, 8),INSTR(LAX, IZY, 5),INSTR(LDY, ZPX, 4),INSTR(LDA, ZPX, 4),INSTR(LDX, ZPY, 4),INSTR(LAX, ZPY, 4),INSTR(CLV, IMP, 2),INSTR(LDA, ABY, 4),INSTR(TSX, IMP, 2),INSTR(XXX, IMP, 8),INSTR(LDY, ABX, 4),INSTR(LDA, ABX, 4),INSTR(LDX, ABY, 4),INSTR(LAX, ABY, 4),
			/*CX*/ INSTR(CPY, IMM, 2),INSTR(CMP, IZX, 6),INSTR(SKB, IMM, 2),INSTR(DCP, IZX, 8),INSTR(CPY, ZP0, 3),INSTR(CMP, ZP0, 3),INSTR(DEC, ZP0, 5),INSTR(DCP, ZP0, 5),INSTR(INY, IMP, 2),INSTR(CMP, IMM, 2),INSTR(DEX, IMP, 2),INSTR(AXS, IMM, 2),INSTR(CPY, ABS, 4),INSTR(CMP, ABS, 4),INSTR(DEC, ABS, 6),INSTR(DCP, ABS, 6),
			/*DX*/ INSTR(BNE, REL, 2),INSTR(CMP, IZY, 5),INSTR(XXX, IMP, 8),INSTR(DCP, IZY, 8),INSTR(IGN, ZPX, 4),INSTR(CMP, ZPX, 4),INSTR(DEC, ZPX, 6),INSTR(DCP, ZPX, 6),INSTR(CLD, IMP, 2),INSTR(CMP, ABY, 4),INSTR(NOP, IMP, 2),INSTR(DCP, ABY, 7),INSTR(IGN, ABX, 4),INSTR(CMP, ABX, 4),INSTR(DEC, ABX, 7),INSTR(DCP, ABX, 7),
			/*EX*/ INSTR(CPX, IMM, 2),INSTR(SBC, IZX, 6),INSTR(SKB, IMM, 2),INSTR(ISC, IZX, 8),INSTR(CPX, ZP0, 3),INSTR(SBC, ZP0, 3),INSTR(INC, ZP0, 5),INSTR(ISC, ZP0, 5),INSTR(INX, IMP, 2),INSTR(SBC, IMM, 2),INSTR(NOP, IMP, 2),INSTR(SBC, IMM, 2),INSTR(CPX, ABS, 4),INSTR(SBC, ABS, 4),INSTR(INC, ABS, 6),INSTR(ISC, ABS, 6),
			/*FX*/ INSTR(BEQ, REL, 2),INSTR(SBC, IZY, 5),INSTR(XXX, IMP, 8),INSTR(ISC, IZY, 8),INSTR(IGN, ZPX, 4),INSTR(SBC, ZPX, 4),INSTR(INC, ZPX, 6),INSTR(ISC, ZPX, 6),INSTR(SED, IMP, 2),INSTR(SBC, ABY, 4),INSTR(NOP, IMP, 2),INSTR(ISC, ABY, 7),INSTR(IGN, ABX, 4),INSTR(SBC, ABX, 4),INSTR(INC, ABX, 7),INSTR(ISC, ABX, 7)
		};

		mRegisters.SR = 0x34;
		mRegisters.A = mRegisters.X = mRegisters.Y = 0;
		mRegisters.SP = 0xFD;
	}

	RP2A03::~RP2A03()
	{}

	void RP2A03::clock(RP2C02& ppu)
	{

		if (mCyclesToConsume == 0) {
			if (mNMIPending) {
				nmi();
			} else {
				uint8_t opcode = getBus("CPU")->read(mRegisters.PC);
				mCurrentInstruction = mInstructionsLookup[opcode];
				mCurrentInstruction.opcode = opcode;
				mCurrentInstruction.address = mRegisters.PC;
				mRegisters.PC++;

				mCyclesToConsume = mCurrentInstruction.Cycles;

				uint8_t additionalAddressingModeCycles = (this->*mCurrentInstruction.AddressingMode)();

#if 1

				if (mCurrentInstruction.Operation == &RP2A03::STA ||
					mCurrentInstruction.Operation == &RP2A03::BIT) {

					mCurrentInstruction.Mnemonic += " = " + disHex(mRegisters.A, 2);
				}
				else if (mCurrentInstruction.Operation == &RP2A03::STX) {
					mCurrentInstruction.Mnemonic += " = " + disHex(mRegisters.X, 2);
				}
				else if (mCurrentInstruction.Operation == &RP2A03::STY) {
					mCurrentInstruction.Mnemonic += " = " + disHex(mRegisters.Y, 2);
				}

				printf("%04X  %02X ", mCurrentInstruction.address, mCurrentInstruction.opcode);
				if (mCurrentInstruction.lo != -1) {
					printf("%02X ", mCurrentInstruction.lo);
				}
				else {
					printf("   ");
				}
				if (mCurrentInstruction.hi != -1) {
					printf("%02X ", mCurrentInstruction.hi);
				}
				else {
					printf("   ");
				}
				printf(" %-31s ", mCurrentInstruction.Mnemonic.c_str());

				printf("A:%02X X:%02X Y:%02X P:%02X SP:%02X PPU:%3d,%3d", mRegisters.A, mRegisters.X, mRegisters.Y, mRegisters.SR, mRegisters.SP, ppu.mScanline, ppu.mCycles);
				printf(" CYC:%zd", mCyclesPassed);

				printf("\n");
#endif

				
				uint8_t additionalOperationCycles = (this->*mCurrentInstruction.Operation)();

				mCyclesToConsume += (additionalOperationCycles & additionalAddressingModeCycles);
			}
		}

		mCyclesToConsume--;
		mCyclesPassed++;
	}

	void RP2A03::powerUp()
	{
		mRegisters.SR = (uint8_t)RP2A03_StatusFlag::U | (uint8_t)RP2A03_StatusFlag::I;
		mRegisters.A = mRegisters.X = mRegisters.Y = 0;
		mRegisters.SP = 0xFD;
		mRegisters.JOY2 = 0x00;
		mRegisters.SND_CHN = 0x00;

		mRegisters.SQ1.VOL = 0x00;
		mRegisters.SQ1.SWEEP = 0x00;
		mRegisters.SQ1.LO = 0x00;
		mRegisters.SQ1.HI = 0x00;

		mRegisters.SQ2.VOL = 0x00;
		mRegisters.SQ2.SWEEP = 0x00;
		mRegisters.SQ2.LO = 0x00;
		mRegisters.SQ2.HI = 0x00;

		mRegisters.TRI.LINEAR = 0x00;
		mRegisters.TRI.LO = 0x00;
		mRegisters.TRI.HI = 0x00;

		mRegisters.NOISE.VOL = 0x00;
		mRegisters.NOISE.LO = 0x00;
		mRegisters.NOISE.HI = 0x00;

		mRegisters.DMC.FREQ = 0x00;
		mRegisters.DMC.RAW = 0x00;
		mRegisters.DMC.START = 0x00;
		mRegisters.DMC.LEN = 0x00;

		mRegisters.LFSR = 0x0000;
	}

	void RP2A03::reset()
	{
		mRegisters.PC = (((uint16_t)getBus("CPU")->read(RESET_VEC + 1)) << 8) | getBus("CPU")->read(RESET_VEC);

		mAddressRelative = 0x0000;
		mAddressAbsolute = 0x0000;
		mFetched = 0;

		mCyclesToConsume = 7;

		mRegisters.SP = 0xFD;
		mRegisters.SR |= (uint8_t)RP2A03_StatusFlag::I | (uint8_t)RP2A03_StatusFlag::U;
		mRegisters.SND_CHN = 0x00;
		//TODO: APU triangle phase is reset to 0 (i.e. outputs a value of 15, the first step of its waveform)
		//TODO: APU DPCM output ANDed with 1 (upper 6 bits cleared)

	}

	void RP2A03::irq()
	{
		if (getFlag(RP2A03_StatusFlag::I) == 0) {
			getBus("CPU")->write(SP_BASE + mRegisters.SP, mRegisters.PC >> 8);
			getBus("CPU")->write(SP_BASE + mRegisters.SP - 1, mRegisters.PC & 0xFF);

			setFlag(RP2A03_StatusFlag::B, 0);
			setFlag(RP2A03_StatusFlag::U, 1);
			setFlag(RP2A03_StatusFlag::I, 1);
			getBus("CPU")->write(SP_BASE + mRegisters.SP - 2, mRegisters.SR);
			mRegisters.SP -= 3;

			mRegisters.PC = (((uint16_t)getBus("CPU")->read(IRQ_INT_VEC + 1)) << 8) | getBus("CPU")->read(IRQ_INT_VEC);
		
			mCyclesToConsume = 7;
		}
	}

	void RP2A03::nmi()
	{
		getBus("CPU")->write(SP_BASE + mRegisters.SP, mRegisters.PC >> 8);
		getBus("CPU")->write(SP_BASE + mRegisters.SP - 1, mRegisters.PC & 0xFF);

		setFlag(RP2A03_StatusFlag::B, 0);
		setFlag(RP2A03_StatusFlag::U, 1);
		setFlag(RP2A03_StatusFlag::I, 1);
		getBus("CPU")->write(SP_BASE + mRegisters.SP - 2, mRegisters.SR);
		mRegisters.SP -= 3;

		mRegisters.PC = (((uint16_t)getBus("CPU")->read(NMI_VEC + 1)) << 8) | getBus("CPU")->read(NMI_VEC);

		mCyclesToConsume = 8;

		mNMIPending = false;
	}

	void RP2A03::writeLine(const std::string& busName, const std::string& lineName, bool value)
	{
		if (lineName == "NMI") {
			if (mNMILine == true && value == false) {
				mNMIPending = true;
			}
			mNMILine = value;
		} else if (lineName == "DMA") {
			if (value) {
				mCyclesToConsume += 513; //TODO: there is a +1 don't know why
			}
		}
	}

	void RP2A03::write(const std::string& busName,uint16_t address, uint8_t value)
	{
	}

	uint8_t RP2A03::read(const std::string& busName,uint16_t address)
	{
		return 0;
	}

	uint8_t RP2A03::getFlag(RP2A03_StatusFlag flag)
	{
		return (mRegisters.SR & (uint8_t)flag) > 0 ? 1 : 0;
	}

	void RP2A03::setFlag(RP2A03_StatusFlag flag, bool value)
	{
		mRegisters.SR &= ~((uint8_t)flag);

		if (value)
			mRegisters.SR |= (uint8_t)flag;
	}

	uint8_t RP2A03::fetch()
	{
		if (mCurrentInstruction.AddressingMode != &RP2A03::IMP && mCurrentInstruction.AddressingMode != &RP2A03::ACC)
			mFetched = getBus("CPU")->read(mAddressAbsolute);
		return mFetched;
	}

	uint8_t RP2A03::LDA()
	{
		mRegisters.A = fetch();

		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.A & 0x80);

		return 1;
	}

	uint8_t RP2A03::LDX()
	{
		mRegisters.X = fetch();

		setFlag(RP2A03_StatusFlag::Z, mRegisters.X == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.X & 0x80);

		return 1;
	}

	uint8_t RP2A03::LDY()
	{
		mRegisters.Y = fetch();

		setFlag(RP2A03_StatusFlag::Z, mRegisters.Y == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.Y & 0x80);

		return 1;
	}

	uint8_t RP2A03::STA()
	{
		getBus("CPU")->write(mAddressAbsolute, mRegisters.A);

		return 0;
	}

	uint8_t RP2A03::STX()
	{
		getBus("CPU")->write(mAddressAbsolute, mRegisters.X);

		return 0;
	}

	uint8_t RP2A03::STY()
	{
		getBus("CPU")->write(mAddressAbsolute, mRegisters.Y);

		return 0;
	}

	uint8_t RP2A03::TAX()
	{
		mRegisters.X = mRegisters.A;

		setFlag(RP2A03_StatusFlag::Z, mRegisters.X == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.X & 0x80);

		return 0;
	}

	uint8_t RP2A03::TAY()
	{
		mRegisters.Y = mRegisters.A;

		setFlag(RP2A03_StatusFlag::Z, mRegisters.Y == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.Y & 0x80);

		return 0;
	}

	uint8_t RP2A03::TXA()
	{
		mRegisters.A = mRegisters.X;

		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.A & 0x80);

		return 0;
	}

	uint8_t RP2A03::TYA()
	{
		mRegisters.A = mRegisters.Y;

		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.A & 0x80);

		return 0;
	}

	uint8_t RP2A03::TSX()
	{
		mRegisters.X = mRegisters.SP;

		setFlag(RP2A03_StatusFlag::Z, mRegisters.X == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.X & 0x80);

		return 0;
	}

	uint8_t RP2A03::TXS()
	{
		mRegisters.SP = mRegisters.X;

		return 0;
	}

	uint8_t RP2A03::PHA()
	{
		getBus("CPU")->write(SP_BASE + mRegisters.SP, mRegisters.A);
		mRegisters.SP--;

		return 0;
	}

	uint8_t RP2A03::PHP()
	{
		getBus("CPU")->write(SP_BASE + mRegisters.SP, mRegisters.SR | (uint8_t)RP2A03_StatusFlag::B);
		mRegisters.SP--;

		return 0;
	}

	uint8_t RP2A03::PLA()
	{
		mRegisters.A = getBus("CPU")->read(SP_BASE + mRegisters.SP + 1);
		mRegisters.SP++;

		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.A & 0x80);

		return 0;
	}

	uint8_t RP2A03::PLP()
	{
		mRegisters.SR = getBus("CPU")->read(SP_BASE + mRegisters.SP + 1);
		mRegisters.SP++;

		setFlag(RP2A03_StatusFlag::B, 0);
		setFlag(RP2A03_StatusFlag::U, 1);

		return 0;
	}

	uint8_t RP2A03::AND()
	{
		uint8_t val = fetch();
		mRegisters.A &= val;

		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.A & 0x80);

		return 1;
	}

	uint8_t RP2A03::EOR()
	{
		uint8_t val = fetch();
		mRegisters.A ^= val;

		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.A & 0x80);

		return 1;
	}

	uint8_t RP2A03::ORA()
	{
		uint8_t val = fetch();
		mRegisters.A |= val;

		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.A & 0x80);

		return 1;
	}

	uint8_t RP2A03::BIT()
	{
		uint8_t val = fetch();

		setFlag(RP2A03_StatusFlag::Z, (mRegisters.A & val) == 0);
		setFlag(RP2A03_StatusFlag::V, val & 0x40);
		setFlag(RP2A03_StatusFlag::N, val & 0x80);

		return 0;
	}

	uint8_t RP2A03::ADC()
	{
		uint8_t c = getFlag(RP2A03_StatusFlag::C);
		uint8_t m = fetch();
		uint8_t a = mRegisters.A;
		uint16_t s = a + m + c;

		mRegisters.A = s & 0xFF;

		setFlag(RP2A03_StatusFlag::C, (s & 0xFF00) != 0);
		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::V, ((a ^ s) & (m ^ s) & 0x80) != 0);
		setFlag(RP2A03_StatusFlag::N, s & 0x80);

		return 1;
	}

	uint8_t RP2A03::SBC()
	{
		uint8_t c = getFlag(RP2A03_StatusFlag::C);
		uint8_t m = fetch() ^ 0xFF;
		uint8_t a = mRegisters.A;

		uint16_t s = a + m + c;

		mRegisters.A = s & 0xFF;

		setFlag(RP2A03_StatusFlag::C, (s & 0xFF00) != 0);
		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::V, ((a ^ s) & (m ^ s) & 0x80) != 0);
		setFlag(RP2A03_StatusFlag::N, s & 0x80);

		return 1;
	}

	uint8_t RP2A03::CMP()
	{
		uint8_t m = fetch();
		uint8_t s = mRegisters.A - m;

		setFlag(RP2A03_StatusFlag::C, mRegisters.A >= m);
		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == m);
		setFlag(RP2A03_StatusFlag::N, s & 0x80);

		return 1;
	}

	uint8_t RP2A03::CPX()
	{
		uint8_t m = fetch();
		uint8_t s = mRegisters.X - m;

		setFlag(RP2A03_StatusFlag::C, mRegisters.X >= m);
		setFlag(RP2A03_StatusFlag::Z, mRegisters.X == m);
		setFlag(RP2A03_StatusFlag::N, s & 0x80);

		return 0;
	}

	uint8_t RP2A03::CPY()
	{
		uint8_t m = fetch();
		uint8_t s = mRegisters.Y - m;

		setFlag(RP2A03_StatusFlag::C, mRegisters.Y >= m);
		setFlag(RP2A03_StatusFlag::Z, mRegisters.Y == m);
		setFlag(RP2A03_StatusFlag::N, s & 0x80);

		return 0;
	}

	uint8_t RP2A03::INC()
	{
		uint8_t m = fetch();
		m++;

		setFlag(RP2A03_StatusFlag::Z, m == 0);
		setFlag(RP2A03_StatusFlag::N, m & 0x80);

		getBus("CPU")->write(mAddressAbsolute, m);

		return 0;
	}

	uint8_t RP2A03::INX()
	{
		mRegisters.X++;

		setFlag(RP2A03_StatusFlag::Z, mRegisters.X == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.X & 0x80);

		return 0;
	}

	uint8_t RP2A03::INY()
	{
		mRegisters.Y++;

		setFlag(RP2A03_StatusFlag::Z, mRegisters.Y == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.Y & 0x80);

		return 0;
	}

	uint8_t RP2A03::DEC()
	{
		uint8_t m = fetch();
		m--;

		setFlag(RP2A03_StatusFlag::Z, m == 0);
		setFlag(RP2A03_StatusFlag::N, m & 0x80);

		getBus("CPU")->write(mAddressAbsolute, m);

		return 0;
	}

	uint8_t RP2A03::DEX()
	{
		mRegisters.X--;

		setFlag(RP2A03_StatusFlag::Z, mRegisters.X == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.X & 0x80);

		return 0;
	}

	uint8_t RP2A03::DEY()
	{
		mRegisters.Y--;

		setFlag(RP2A03_StatusFlag::Z, mRegisters.Y == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.Y & 0x80);

		return 0;
	}

	uint8_t RP2A03::ASL()
	{
		uint8_t temp = fetch();

		uint8_t result = temp << 1;

		if (mCurrentInstruction.AddressingMode == &RP2A03::ACC)
			mRegisters.A = result;
		else
			getBus("CPU")->write(mAddressAbsolute, result);

		setFlag(RP2A03_StatusFlag::C, temp & 0x80);
		setFlag(RP2A03_StatusFlag::Z, result == 0);
		setFlag(RP2A03_StatusFlag::N, result & 0x80);

		return 0;
	}

	uint8_t RP2A03::LSR()
	{
		uint8_t temp = fetch();

		uint8_t result = temp >> 1;

		if (mCurrentInstruction.AddressingMode == &RP2A03::ACC)
			mRegisters.A = result;
		else
			getBus("CPU")->write(mAddressAbsolute, result);

		setFlag(RP2A03_StatusFlag::C, temp & 0x1);
		setFlag(RP2A03_StatusFlag::Z, result == 0);
		setFlag(RP2A03_StatusFlag::N, result & 0x80);

		return 0;
	}

	uint8_t RP2A03::ROL()
	{
		uint8_t temp = fetch();

		uint8_t result = (temp << 1) | getFlag(RP2A03_StatusFlag::C);

		if (mCurrentInstruction.AddressingMode == &RP2A03::ACC)
			mRegisters.A = result;
		else
			getBus("CPU")->write(mAddressAbsolute, result);

		setFlag(RP2A03_StatusFlag::C, temp & 0x80);
		setFlag(RP2A03_StatusFlag::Z, result == 0);
		setFlag(RP2A03_StatusFlag::N, result & 0x80);

		return 0;
	}

	uint8_t RP2A03::ROR()
	{
		uint8_t temp = fetch();

		uint8_t result = (temp >> 1) | (getFlag(RP2A03_StatusFlag::C) << 7);

		if (mCurrentInstruction.AddressingMode == &RP2A03::ACC)
			mRegisters.A = result;
		else
			getBus("CPU")->write(mAddressAbsolute, result);

		setFlag(RP2A03_StatusFlag::C, temp & 0x1);
		setFlag(RP2A03_StatusFlag::Z, result == 0);
		setFlag(RP2A03_StatusFlag::N, result & 0x80);

		return 0;
	}

	uint8_t RP2A03::JMP()
	{
		mRegisters.PC = mAddressAbsolute;

		return 0;
	}

	uint8_t RP2A03::JSR()
	{
		mRegisters.PC--;

		getBus("CPU")->write(SP_BASE + mRegisters.SP, mRegisters.PC >> 8);
		getBus("CPU")->write(SP_BASE + mRegisters.SP - 1, mRegisters.PC & 0xFF);
		mRegisters.SP -= 2;

		mRegisters.PC = mAddressAbsolute;

		return 0;
	}

	uint8_t RP2A03::RTS()
	{
		uint8_t lo = getBus("CPU")->read(SP_BASE + mRegisters.SP + 1);
		uint8_t hi = getBus("CPU")->read(SP_BASE + mRegisters.SP + 2);
		mRegisters.SP += 2;

		mRegisters.PC = (hi << 8) | lo;

		mRegisters.PC++;

		return 0;
	}

	uint8_t RP2A03::BCC()
	{
		if (getFlag(RP2A03_StatusFlag::C) == 0) {
			mCyclesToConsume++;

			uint16_t tempPC = mRegisters.PC;
			mRegisters.PC += mAddressRelative;

			if ((tempPC & 0xFF00) != (mRegisters.PC & 0xFF00))
				mCyclesToConsume++;
		}

		return 0;
	}

	uint8_t RP2A03::BCS()
	{
		if (getFlag(RP2A03_StatusFlag::C) == 1) {
			mCyclesToConsume++;

			uint16_t tempPC = mRegisters.PC;
			mRegisters.PC += mAddressRelative;

			if ((tempPC & 0xFF00) != (mRegisters.PC & 0xFF00))
				mCyclesToConsume++;			
		}

		return 0;
	}

	uint8_t RP2A03::BEQ()
	{
		if (getFlag(RP2A03_StatusFlag::Z) == 1) {
			mCyclesToConsume++;

			uint16_t tempPC = mRegisters.PC;
			mRegisters.PC += mAddressRelative;

			if ((tempPC & 0xFF00) != (mRegisters.PC & 0xFF00))
				mCyclesToConsume++;
		}

		return 0;
	}

	uint8_t RP2A03::BMI()
	{
		if (getFlag(RP2A03_StatusFlag::N) == 1) {
			mCyclesToConsume++;

			uint16_t tempPC = mRegisters.PC;
			mRegisters.PC += mAddressRelative;

			if ((tempPC & 0xFF00) != (mRegisters.PC & 0xFF00))
				mCyclesToConsume++;
		}

		return 0;
	}

	uint8_t RP2A03::BNE()
	{
		if (getFlag(RP2A03_StatusFlag::Z) == 0) {
			mCyclesToConsume++;

			uint16_t tempPC = mRegisters.PC;
			mRegisters.PC += mAddressRelative;

			if ((tempPC & 0xFF00) != (mRegisters.PC & 0xFF00))
				mCyclesToConsume++;
		}

		return 0;
	}

	uint8_t RP2A03::BPL()
	{
		if (getFlag(RP2A03_StatusFlag::N) == 0) {
			mCyclesToConsume++;

			uint16_t tempPC = mRegisters.PC;
			mRegisters.PC += mAddressRelative;

			if ((tempPC & 0xFF00) != (mRegisters.PC & 0xFF00))
				mCyclesToConsume++;
		}

		return 0;
	}

	uint8_t RP2A03::BVC()
	{
		if (getFlag(RP2A03_StatusFlag::V) == 0) {
			mCyclesToConsume++;

			uint16_t tempPC = mRegisters.PC;
			mRegisters.PC += mAddressRelative;

			if ((tempPC & 0xFF00) != (mRegisters.PC & 0xFF00))
				mCyclesToConsume++;
		}

		return 0;
	}

	uint8_t RP2A03::BVS()
	{
		if (getFlag(RP2A03_StatusFlag::V) == 1) {
			mCyclesToConsume++;

			uint16_t tempPC = mRegisters.PC;
			mRegisters.PC += mAddressRelative;

			if ((tempPC & 0xFF00) != (mRegisters.PC & 0xFF00))
				mCyclesToConsume++;
		}

		return 0;
	}

	uint8_t RP2A03::CLC()
	{
		setFlag(RP2A03_StatusFlag::C, 0);

		return 0;
	}

	uint8_t RP2A03::CLD()
	{
		setFlag(RP2A03_StatusFlag::D, 0);

		return 0;
	}
	uint8_t RP2A03::CLI()
	{
		setFlag(RP2A03_StatusFlag::I, 0);

		return 0;
	}
	uint8_t RP2A03::CLV()
	{
		setFlag(RP2A03_StatusFlag::V, 0);

		return 0;
	}
	uint8_t RP2A03::SEC()
	{
		setFlag(RP2A03_StatusFlag::C, 1);

		return 0;
	}
	uint8_t RP2A03::SED()
	{
		setFlag(RP2A03_StatusFlag::D, 1);

		return 0;
	}
	uint8_t RP2A03::SEI()
	{
		setFlag(RP2A03_StatusFlag::I, 1);

		return 0;
	}
	uint8_t RP2A03::BRK()
	{
		mRegisters.PC++;

		getBus("CPU")->write(SP_BASE + mRegisters.SP, mRegisters.PC >> 8);
		getBus("CPU")->write(SP_BASE + mRegisters.SP - 1, mRegisters.PC & 0xFF);

		setFlag(RP2A03_StatusFlag::B, 1);
		getBus("CPU")->write(SP_BASE + mRegisters.SP - 2, mRegisters.SR);
		mRegisters.SP -= 3;

		setFlag(RP2A03_StatusFlag::B, 0);
		setFlag(RP2A03_StatusFlag::I, 1);

		mRegisters.PC = (((uint16_t)getBus("CPU")->read(IRQ_INT_VEC)) << 8) | getBus("CPU")->read(IRQ_INT_VEC + 1);


		return 0;
	}

	uint8_t RP2A03::NOP()
	{
		return 1;
	}

	uint8_t RP2A03::RTI()
	{
		mRegisters.SR = getBus("CPU")->read(SP_BASE + mRegisters.SP + 1);
		uint8_t lo = getBus("CPU")->read(SP_BASE + mRegisters.SP + 2);
		uint8_t hi = getBus("CPU")->read(SP_BASE + mRegisters.SP + 3);
		mRegisters.PC = (hi << 8) | lo;
		mRegisters.SP += 3;

		setFlag(RP2A03_StatusFlag::U, 1);
		setFlag(RP2A03_StatusFlag::B, 0);

		return 0;
	}

	uint8_t RP2A03::XXX()
	{
		return 0;
	}

	uint8_t RP2A03::LAX()
	{
		mRegisters.A = fetch();
		mRegisters.X = fetch();

		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.A & 0x80);

		return 1;
	}

	uint8_t RP2A03::SAX()
	{
		getBus("CPU")->write(mAddressAbsolute, mRegisters.A & mRegisters.X);

		return 0;
	}

	uint8_t RP2A03::ALR()
	{
		uint8_t val = fetch();

		mRegisters.A &= val;

		uint8_t temp = mRegisters.A;

		uint8_t result = temp >> 1;

		mRegisters.A = result;

		setFlag(RP2A03_StatusFlag::C, temp & 0x1);
		setFlag(RP2A03_StatusFlag::Z, result == 0);
		setFlag(RP2A03_StatusFlag::N, result & 0x80);

		return 0;
	}

	uint8_t RP2A03::ANC()
	{
		uint8_t val = fetch();
		mRegisters.A &= val;

		setFlag(RP2A03_StatusFlag::C, mRegisters.A & 0x80);
		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.A & 0x80);

		return 0;
	}

	uint8_t RP2A03::ARR()
	{
		uint8_t val = fetch();
		mRegisters.A &= val;

		uint8_t temp = mRegisters.A;

		uint8_t result = (temp >> 1) | (getFlag(RP2A03_StatusFlag::C) << 7);

		mRegisters.A = result;

		setFlag(RP2A03_StatusFlag::C, temp & 0x1);
		setFlag(RP2A03_StatusFlag::Z, result == 0);
		setFlag(RP2A03_StatusFlag::N, result & 0x80);

		return 0;
	}

	uint8_t RP2A03::AXS()
	{
		uint8_t result = mRegisters.A & mRegisters.X;

		getBus("CPU")->write(mAddressAbsolute, result);

		return 0;
	}

	uint8_t RP2A03::DCP()
	{
		uint8_t temp = fetch();

		uint8_t result = temp - 1;

		getBus("CPU")->write(mAddressAbsolute, result);

		uint8_t s = mRegisters.A - result;

		setFlag(RP2A03_StatusFlag::C, mRegisters.A >= result);
		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == result);
		setFlag(RP2A03_StatusFlag::N, s & 0x80);

		return 0;
	}

	uint8_t RP2A03::ISC()
	{
		uint8_t temp = fetch();

		uint8_t result = temp + 1;

		getBus("CPU")->write(mAddressAbsolute, result);

		uint8_t c = getFlag(RP2A03_StatusFlag::C);
		uint8_t m = result ^ 0xFF;
		uint8_t a = mRegisters.A;

		uint16_t s = a + m + c;

		mRegisters.A = s & 0xFF;

		setFlag(RP2A03_StatusFlag::C, (s & 0xFF00) != 0);
		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::V, ((a ^ s) & (m ^ s) & 0x80) != 0);
		setFlag(RP2A03_StatusFlag::N, s & 0x80);

		return 0;
	}

	uint8_t RP2A03::RLA()
	{
		uint8_t temp = fetch();

		uint8_t result = (temp << 1) | getFlag(RP2A03_StatusFlag::C);

		getBus("CPU")->write(mAddressAbsolute, result);

		mRegisters.A &= result;

		setFlag(RP2A03_StatusFlag::C, temp & 0x80);
		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.A & 0x80);

		return 0;
	}

	uint8_t RP2A03::RRA()
	{
		uint8_t temp = fetch();

		uint8_t result = (temp >> 1) | (getFlag(RP2A03_StatusFlag::C) << 7);

		getBus("CPU")->write(mAddressAbsolute, result);

		uint8_t c = temp & 0x1;
		uint8_t m = result;
		uint8_t a = mRegisters.A;
		uint16_t s = a + m + c;

		mRegisters.A = s & 0xFF;

		setFlag(RP2A03_StatusFlag::C, (s & 0xFF00) != 0);
		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::V, ((a ^ s) & (m ^ s) & 0x80) != 0);
		setFlag(RP2A03_StatusFlag::N, s & 0x80);

		return 0;
	}

	uint8_t RP2A03::SLO()
	{
		uint8_t temp = fetch();

		uint8_t result = temp << 1;

		getBus("CPU")->write(mAddressAbsolute, result);

		mRegisters.A |= result;

		setFlag(RP2A03_StatusFlag::C, temp & 0x80);
		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.A & 0x80);


		return 0;
	}

	uint8_t RP2A03::SRE()
	{
		uint8_t temp = fetch();

		uint8_t result = temp >> 1;

		getBus("CPU")->write(mAddressAbsolute, result);

		mRegisters.A ^= result;

		setFlag(RP2A03_StatusFlag::C, temp & 0x1);
		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.A & 0x80);

		return 0;
	}

	uint8_t RP2A03::SKB()
	{
		uint8_t b = fetch();

		return 0;
	}

	uint8_t RP2A03::IGN()
	{
		uint8_t b = fetch();

		if (mCurrentInstruction.AddressingMode == &RP2A03::ZPX) {
			b = getBus("CPU")->read(mCurrentInstruction.lo);
		}
		else if (mCurrentInstruction.AddressingMode == &RP2A03::ABX) {
			uint16_t address = (mCurrentInstruction.hi << 8) | mCurrentInstruction.lo;
			b = getBus("CPU")->read(address + mRegisters.X - 0x100);
		}

		return 1;
	}

	uint8_t RP2A03::ACC()
	{
		mFetched = mRegisters.A;

		mCurrentInstruction.Mnemonic += " A";

		return 0;
	}

	uint8_t RP2A03::ABS()
	{
		uint8_t lo = getBus("CPU")->read(mRegisters.PC);
		uint8_t hi = getBus("CPU")->read(mRegisters.PC + 1);

		mCurrentInstruction.lo = lo;
		mCurrentInstruction.hi = hi;

		mAddressAbsolute = (hi << 8) | lo;
		mRegisters.PC += 2;

		mCurrentInstruction.Mnemonic += " $" + disHex(mAddressAbsolute, 4);

		return 0;
	}

	uint8_t RP2A03::ABX()
	{
		uint8_t lo = getBus("CPU")->read(mRegisters.PC);
		uint8_t hi = getBus("CPU")->read(mRegisters.PC + 1);

		mCurrentInstruction.lo = lo;
		mCurrentInstruction.hi = hi;

		uint16_t address = (hi << 8) | lo;
		mRegisters.PC += 2;

		mCurrentInstruction.Mnemonic += " $" + disHex(address, 4) + ",X";

		mAddressAbsolute = address + mRegisters.X;

		return (address & 0xFF00) != (mAddressAbsolute & 0xFF00);
	}

	uint8_t RP2A03::ABY()
	{
		uint8_t lo = getBus("CPU")->read(mRegisters.PC);
		uint8_t hi = getBus("CPU")->read(mRegisters.PC + 1);

		mCurrentInstruction.lo = lo;
		mCurrentInstruction.hi = hi;

		uint16_t address = (hi << 8) | lo;
		mRegisters.PC += 2;

		mCurrentInstruction.Mnemonic += " $" + disHex(address, 4) + ",Y";

		mAddressAbsolute = address + mRegisters.Y;

		return (address & 0xFF00) != (mAddressAbsolute & 0xFF00);
	}

	uint8_t RP2A03::IMM()
	{
		mCurrentInstruction.lo = mRegisters.PC;

		mAddressAbsolute = mRegisters.PC;

		uint8_t imm = fetch();

		mCurrentInstruction.Mnemonic += " #$" + disHex(imm,2);

		mCurrentInstruction.lo = getBus("CPU")->read(mAddressAbsolute);

		mRegisters.PC++;

		return 0;
	}

	uint8_t RP2A03::IMP()
	{
		return 0;
	}

	uint8_t RP2A03::IND()
	{
		uint8_t ptrLO = getBus("CPU")->read(mRegisters.PC);
		uint8_t ptrHI = getBus("CPU")->read(mRegisters.PC + 1);

		mCurrentInstruction.lo = ptrLO;
		mCurrentInstruction.hi = ptrHI;

		uint16_t address = (ptrHI << 8) | ptrLO;
		mRegisters.PC += 2;

		mCurrentInstruction.Mnemonic += " ($" + disHex(address, 4) + ")";

		if (ptrLO == 0xFF) {
			uint8_t lo = getBus("CPU")->read(address);
			uint8_t hi = getBus("CPU")->read(address & 0xFF00);

			mAddressAbsolute = (hi << 8) | lo;
		} else {
			uint8_t lo = getBus("CPU")->read(address);
			uint8_t hi = getBus("CPU")->read(address + 1);

			mAddressAbsolute = (hi << 8) | lo;
		}

		return 0;
	}

	uint8_t RP2A03::IZX()
	{
		uint8_t lo = getBus("CPU")->read(mRegisters.PC);
		mRegisters.PC++;

		mCurrentInstruction.lo = lo;

		mCurrentInstruction.Mnemonic += " ($" + disHex(lo, 2) + ",X)";

		mAddressAbsolute = (uint16_t)getBus("CPU")->read((lo + mRegisters.X) & 0xFF) | ((uint16_t)getBus("CPU")->read((lo + mRegisters.X + 1) & 0xFF) << 8);

		return 0;
	}
	uint8_t RP2A03::IZY()
	{
		uint8_t t = getBus("CPU")->read(mRegisters.PC);
		mRegisters.PC++;

		mCurrentInstruction.lo = t;

		mCurrentInstruction.Mnemonic += " ($" + disHex(t, 2) + "),Y";

		uint8_t hi = getBus("CPU")->read((t + 1) & 0xFF);
		uint8_t lo = getBus("CPU")->read(t);

		mAddressAbsolute = (hi << 8) | lo;
		mAddressAbsolute += mRegisters.Y;

		return (mAddressAbsolute & 0xFF00) != (hi << 8);
	}
	uint8_t RP2A03::REL()
	{
		uint8_t lo = getBus("CPU")->read(mRegisters.PC);

		mCurrentInstruction.lo = lo;

		mAddressRelative = lo;
		mRegisters.PC++;

		mCurrentInstruction.Mnemonic += " $" + disHex(mRegisters.PC + mAddressRelative, 4);

		return 0;
	}
	uint8_t RP2A03::ZP0()
	{
		uint8_t lo = getBus("CPU")->read(mRegisters.PC);

		mCurrentInstruction.lo = lo;

		mCurrentInstruction.Mnemonic += " $" + disHex(lo, 2);

		mAddressAbsolute = lo;
		mRegisters.PC++;

		return 0;
	}
	uint8_t RP2A03::ZPX()
	{
		uint8_t lo = getBus("CPU")->read(mRegisters.PC);

		mCurrentInstruction.lo = lo;

		mCurrentInstruction.Mnemonic += " $" + disHex(lo, 2) + ",X";

		uint16_t address = lo;
		mRegisters.PC++;

		mAddressAbsolute = (address + mRegisters.X) & 0xFF;

		return 0;
	}
	uint8_t RP2A03::ZPY()
	{
		uint8_t lo = getBus("CPU")->read(mRegisters.PC);

		mCurrentInstruction.lo = lo;

		mCurrentInstruction.Mnemonic += " $" + disHex(lo, 2) + ",Y";

		uint16_t address = lo;
		mRegisters.PC++;

		mAddressAbsolute = (address + mRegisters.Y) & 0xFF;

		return 0;
	}
}