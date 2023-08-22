#include "RP2A03.h"

namespace NESEmu {
	RP2A03::RP2A03()
		:  BusDevice("RP2A03")
	{
		addRange("CPU",0x4000, 0x4018, 0xFFFF);

		mInstructionsLookup = {
			INSTR(BRK, IMP, 7),INSTR(ORA, IZX, 6),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(ORA, ZP0, 3),INSTR(ASL, ZP0, 5),INSTR(XXX, IMP, 8),INSTR(PHP, IMP, 3),INSTR(ORA, IMM, 2),INSTR(ASL, ACC, 2),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(ORA, ABS, 4),INSTR(ASL, ABS, 6),INSTR(XXX, IMP, 8),
			INSTR(BPL, REL, 2),INSTR(ORA, IZY, 5),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(ORA, ZPX, 4),INSTR(ASL, ZPX, 6),INSTR(XXX, IMP, 8),INSTR(CLC, IMP, 2),INSTR(ORA, ABY, 4),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(ORA, ABX, 4),INSTR(ASL, ABX, 7),INSTR(XXX, IMP, 8),
			INSTR(JSR, ABS, 6),INSTR(AND, IZX, 6),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(BIT, ZP0, 3),INSTR(AND, ZP0, 3),INSTR(ROL, ZP0, 5),INSTR(XXX, IMP, 8),INSTR(PLP, IMP, 4),INSTR(AND, IMM, 2),INSTR(ROL, ACC, 2),INSTR(XXX, IMP, 8),INSTR(BIT, ABS, 4),INSTR(AND, ABS, 4),INSTR(ROL, ABS, 6),INSTR(XXX, IMP, 8),
			INSTR(BMI, REL, 2),INSTR(AND, IZY, 5),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(AND, ZPX, 4),INSTR(ROL, ZPX, 6),INSTR(XXX, IMP, 8),INSTR(SEC, IMP, 2),INSTR(AND, ABY, 4),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(AND, ABX, 4),INSTR(ROL, ABX, 7),INSTR(XXX, IMP, 8),
			INSTR(RTI, IMP, 6),INSTR(EOR, IZX, 6),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(EOR, ZP0, 3),INSTR(LSR, ZP0, 5),INSTR(XXX, IMP, 8),INSTR(PHA, IMP, 3),INSTR(EOR, IMM, 2),INSTR(LSR, ACC, 2),INSTR(XXX, IMP, 8),INSTR(JMP, ABS, 3),INSTR(EOR, ABS, 4),INSTR(LSR, ABS, 6),INSTR(XXX, IMP, 8),
			INSTR(BVC, REL, 2),INSTR(EOR, IZY, 5),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(EOR, ZPX, 4),INSTR(LSR, ZPX, 6),INSTR(XXX, IMP, 8),INSTR(CLI, IMP, 2),INSTR(EOR, ABY, 4),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(EOR, ABX, 4),INSTR(LSR, ABX, 7),INSTR(XXX, IMP, 8),
			INSTR(RTS, IMP, 5),INSTR(ADC, IZX, 6),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(ADC, ZP0, 3),INSTR(ROR, ZP0, 5),INSTR(XXX, IMP, 8),INSTR(PLA, IMP, 4),INSTR(ADC, IMM, 2),INSTR(ROR, ACC, 2),INSTR(XXX, IMP, 8),INSTR(JMP, IND, 5),INSTR(ADC, ABS, 4),INSTR(ROR, ABS, 6),INSTR(XXX, IMP, 8),
			INSTR(BVS, REL, 2),INSTR(ADC, IZY, 5),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(ADC, ZPX, 4),INSTR(ROR, ZPX, 6),INSTR(XXX, IMP, 8),INSTR(SEI, IMP, 2),INSTR(ADC, ABY, 4),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(ADC, ABX, 4),INSTR(ROR, ABX, 7),INSTR(XXX, IMP, 8),
			INSTR(XXX, IMP, 8),INSTR(STA, IZX, 6),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(STY, ZP0, 3),INSTR(STA, ZP0, 3),INSTR(STX, ZP0, 3),INSTR(XXX, IMP, 8),INSTR(DEY, IMP, 2),INSTR(XXX, IMP, 8),INSTR(TXA, IMP, 2),INSTR(XXX, IMP, 8),INSTR(STY, ABS, 4),INSTR(STA, ABS, 4),INSTR(STX, ABS, 4),INSTR(XXX, IMP, 8),
			INSTR(BCC, REL, 2),INSTR(STA, IZY, 6),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(STY, ZPX, 4),INSTR(STA, ZPX, 4),INSTR(STX, ZPY, 4),INSTR(XXX, IMP, 8),INSTR(TYA, IMP, 2),INSTR(STA, ABY, 5),INSTR(TXS, IMP, 2),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(STA, ABX, 5),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),
			INSTR(LDY, IMM, 2),INSTR(LDA, IZX, 6),INSTR(LDX, IMM, 2),INSTR(XXX, IMP, 8),INSTR(LDY, ZP0, 3),INSTR(LDA, ZP0, 3),INSTR(LDX, ZP0, 3),INSTR(XXX, IMP, 8),INSTR(TAY, IMP, 2),INSTR(LDA, IMM, 2),INSTR(TAX, IMP, 2),INSTR(XXX, IMP, 8),INSTR(LDY, ABS, 4),INSTR(LDA, ABS, 4),INSTR(LDX, ABS, 4),INSTR(XXX, IMP, 8),
			INSTR(BCS, REL, 2),INSTR(LDA, IZY, 5),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(LDY, ZPX, 4),INSTR(LDA, ZPX, 4),INSTR(LDX, ZPY, 4),INSTR(XXX, IMP, 8),INSTR(CLV, IMP, 2),INSTR(LDA, ABY, 4),INSTR(TSX, IMP, 2),INSTR(XXX, IMP, 8),INSTR(LDY, ABX, 4),INSTR(LDA, ABX, 4),INSTR(LDX, ABY, 4),INSTR(XXX, IMP, 8),
			INSTR(CPY, IMM, 2),INSTR(CMP, IZX, 6),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(CPY, ZP0, 3),INSTR(CMP, ZP0, 3),INSTR(DEC, ZP0, 5),INSTR(XXX, IMP, 8),INSTR(INY, IMP, 2),INSTR(CMP, IMM, 2),INSTR(DEX, IMP, 2),INSTR(XXX, IMP, 8),INSTR(CPY, ABS, 4),INSTR(CMP, ABS, 4),INSTR(DEC, ABS, 6),INSTR(XXX, IMP, 8),
			INSTR(BNE, REL, 2),INSTR(CMP, IZY, 5),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(CMP, ZPX, 4),INSTR(DEC, ZPX, 6),INSTR(XXX, IMP, 8),INSTR(CLD, IMP, 2),INSTR(CMP, ABY, 4),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(CMP, ABX, 4),INSTR(DEC, ABX, 7),INSTR(XXX, IMP, 8),
			INSTR(CPX, IMM, 2),INSTR(SBC, IZX, 6),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(CPX, ZP0, 3),INSTR(SBC, ZP0, 3),INSTR(INC, ZP0, 5),INSTR(XXX, IMP, 8),INSTR(INX, IMP, 2),INSTR(SBC, IMM, 2),INSTR(NOP, IMP, 2),INSTR(XXX, IMP, 8),INSTR(CPX, ABS, 4),INSTR(SBC, ABS, 4),INSTR(INC, ABS, 6),INSTR(XXX, IMP, 8),
			INSTR(BEQ, REL, 2),INSTR(SBC, IZY, 5),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(SBC, ZPX, 4),INSTR(INC, ZPX, 5),INSTR(XXX, IMP, 8),INSTR(SED, IMP, 2),INSTR(SBC, ABY, 4),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(XXX, IMP, 8),INSTR(SBC, ABX, 4),INSTR(INC, ABX, 7),INSTR(XXX, IMP, 8)
		};

		mRegisters.SR = 0x34;
		mRegisters.A = mRegisters.X = mRegisters.Y = 0;
		mRegisters.SP = 0xFD;
	}

	RP2A03::~RP2A03()
	{}

	void RP2A03::clock()
	{
		if (mCycles == 0) {
			uint8_t opcode = getBus("CPU")->read(mRegisters.PC);
			mRegisters.PC++;

			mCurrentInstruction = mInstructionsLookup[opcode];

			uint8_t additionalAddressingModeCycles = (this->*mCurrentInstruction.AddressingMode)();
			uint8_t additionalOperationCycles = (this->*mCurrentInstruction.Operation)();

			mCycles += additionalOperationCycles & additionalAddressingModeCycles;
		}

		mCycles--;
	}

	void RP2A03::reset()
	{
		mRegisters.PC = (((uint16_t)getBus("CPU")->read(RESET_VEC)) << 8) | getBus("CPU")->read(RESET_VEC + 1);

		mRegisters.SR = 0x00 | (uint8_t)RP2A03_StatusFlag::U;
		mRegisters.A = mRegisters.X = mRegisters.Y = 0;
		mRegisters.SP = 0xFD;

		mAddressRelative = 0x0000;
		mAddressAbsolute = 0x0000;
		mFetched = 0;

		mCycles = 8;
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

			mRegisters.PC = (((uint16_t)getBus("CPU")->read(IRQ_INT_VEC)) << 8) | getBus("CPU")->read(IRQ_INT_VEC + 1);
		
			mCycles = 7;
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

		mRegisters.PC = (((uint16_t)getBus("CPU")->read(NMI_VEC)) << 8) | getBus("CPU")->read(NMI_VEC + 1);

		mCycles = 8;
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
		getBus("CPU")->write(SP_BASE + mRegisters.SP, mRegisters.SR);
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

		setFlag(RP2A03_StatusFlag::Z, mRegisters.A & val);
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

		setFlag(RP2A03_StatusFlag::C, (s & 0xFF00) == 0);
		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::V, ((a ^ s) & (m ^ s) & 0x80) > 0);
		setFlag(RP2A03_StatusFlag::N, s & 0x80);

		return 1;
	}

	uint8_t RP2A03::SBC()
	{
		uint8_t c = getFlag(RP2A03_StatusFlag::C);
		uint8_t m = fetch();
		uint8_t a = mRegisters.A;
		uint16_t s = a - m - (1 - c);

		mRegisters.A = s & 0xFF;

		setFlag(RP2A03_StatusFlag::C, (s & 0xFF00) > 0);
		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::V, ((a ^ s) & (m ^ s) & 0x80) > 0);
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
		uint8_t a = mRegisters.A;

		mRegisters.A <<= 1;

		setFlag(RP2A03_StatusFlag::C, a & 0x80);
		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.A & 0x80);

		return 0;
	}

	uint8_t RP2A03::LSR()
	{
		uint8_t a = mRegisters.A;

		mRegisters.A >>= 1;

		setFlag(RP2A03_StatusFlag::C, a & 0x1);
		setFlag(RP2A03_StatusFlag::Z, mRegisters.A == 0);
		setFlag(RP2A03_StatusFlag::N, mRegisters.A & 0x80);

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
		mRegisters.PC = (((uint16_t)getBus("CPU")->read(SP_BASE + mRegisters.SP + 1)) << 8) | getBus("CPU")->read(SP_BASE + mRegisters.SP + 2);
		mRegisters.SP += 2;

		mRegisters.PC++;

		return 0;
	}

	uint8_t RP2A03::BCC()
	{
		if (getFlag(RP2A03_StatusFlag::C) == 0) {
			mCycles++;

			uint16_t tempPC = mRegisters.PC;
			mRegisters.PC += mAddressRelative;

			if ((tempPC & 0xFF00) != (mRegisters.PC & 0xFF00))
				mCycles++;
		}

		return 0;
	}

	uint8_t RP2A03::BCS()
	{
		if (getFlag(RP2A03_StatusFlag::C) == 1) {
			mCycles++;

			uint16_t tempPC = mRegisters.PC;
			mRegisters.PC += mAddressRelative;

			if ((tempPC & 0xFF00) != (mRegisters.PC & 0xFF00))
				mCycles++;
		}

		return 0;
	}

	uint8_t RP2A03::BEQ()
	{
		if (getFlag(RP2A03_StatusFlag::Z) == 1) {
			mCycles++;

			uint16_t tempPC = mRegisters.PC;
			mRegisters.PC += mAddressRelative;

			if ((tempPC & 0xFF00) != (mRegisters.PC & 0xFF00))
				mCycles++;
		}

		return 0;
	}

	uint8_t RP2A03::BMI()
	{
		if (getFlag(RP2A03_StatusFlag::N) == 1) {
			mCycles++;

			uint16_t tempPC = mRegisters.PC;
			mRegisters.PC += mAddressRelative;

			if ((tempPC & 0xFF00) != (mRegisters.PC & 0xFF00))
				mCycles++;
		}

		return 0;
	}

	uint8_t RP2A03::BNE()
	{
		if (getFlag(RP2A03_StatusFlag::Z) == 0) {
			mCycles++;

			uint16_t tempPC = mRegisters.PC;
			mRegisters.PC += mAddressRelative;

			if ((tempPC & 0xFF00) != (mRegisters.PC & 0xFF00))
				mCycles++;
		}

		return 0;
	}

	uint8_t RP2A03::BPL()
	{
		if (getFlag(RP2A03_StatusFlag::N) == 0) {
			mCycles++;

			uint16_t tempPC = mRegisters.PC;
			mRegisters.PC += mAddressRelative;

			if ((tempPC & 0xFF00) != (mRegisters.PC & 0xFF00))
				mCycles++;
		}

		return 0;
	}

	uint8_t RP2A03::BVC()
	{
		if (getFlag(RP2A03_StatusFlag::V) == 0) {
			mCycles++;

			uint16_t tempPC = mRegisters.PC;
			mRegisters.PC += mAddressRelative;

			if ((tempPC & 0xFF00) != (mRegisters.PC & 0xFF00))
				mCycles++;
		}

		return 0;
	}

	uint8_t RP2A03::BVS()
	{
		if (getFlag(RP2A03_StatusFlag::V) == 1) {
			mCycles++;

			uint16_t tempPC = mRegisters.PC;
			mRegisters.PC += mAddressRelative;

			if ((tempPC & 0xFF00) != (mRegisters.PC & 0xFF00))
				mCycles++;
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
		return 0;
	}

	uint8_t RP2A03::RTI()
	{
		mRegisters.SR = getBus("CPU")->read(SP_BASE + mRegisters.SP + 1);
		mRegisters.PC = (((uint16_t)getBus("CPU")->read(SP_BASE + mRegisters.SP + 2)) << 8) | getBus("CPU")->read(SP_BASE + mRegisters.SP + 3);
		mRegisters.SP += 3;

		mRegisters.PC++;

		return 0;
	}

	uint8_t RP2A03::XXX()
	{
		return 0;
	}

	uint8_t RP2A03::ACC()
	{
		mFetched = mRegisters.A;
		return 0;
	}

	uint8_t RP2A03::ABS()
	{
		mAddressAbsolute = (uint16_t)getBus("CPU")->read(mRegisters.PC) | ((uint16_t)getBus("CPU")->read(mRegisters.PC + 1) << 8);
		mRegisters.PC += 2;

		return 0;
	}

	uint8_t RP2A03::ABX()
	{
		uint16_t address = (uint16_t)getBus("CPU")->read(mRegisters.PC) | ((uint16_t)getBus("CPU")->read(mRegisters.PC + 1) << 8);
		mRegisters.PC += 2;

		mAddressAbsolute = address + mRegisters.X;

		return (address & 0xFF00) != (mAddressAbsolute & 0xFF00);
	}

	uint8_t RP2A03::ABY()
	{
		uint16_t address = (uint16_t)getBus("CPU")->read(mRegisters.PC) | ((uint16_t)getBus("CPU")->read(mRegisters.PC + 1) << 8);
		mRegisters.PC += 2;

		mAddressAbsolute = address + mRegisters.Y;

		return (address & 0xFF00) != (mAddressAbsolute & 0xFF00);
	}

	uint8_t RP2A03::IMM()
	{
		mAddressAbsolute = mRegisters.PC;
		mRegisters.PC++;

		return 0;
	}

	uint8_t RP2A03::IMP()
	{
		return 0;
	}

	uint8_t RP2A03::IND()
	{
		uint16_t address = (uint16_t)getBus("CPU")->read(mRegisters.PC) | ((uint16_t)getBus("CPU")->read(mRegisters.PC + 1) << 8);
		mRegisters.PC += 2;

		mAddressAbsolute = (uint16_t)getBus("CPU")->read(address) | ((uint16_t)getBus("CPU")->read(address + 1) << 8);

		return 0;
	}

	uint8_t RP2A03::IZX()
	{
		uint16_t arg = getBus("CPU")->read(mRegisters.PC);
		mRegisters.PC++;

		mAddressAbsolute = (uint16_t)getBus("CPU")->read((arg + mRegisters.X) & 0xFF) | ((uint16_t)getBus("CPU")->read((arg + mRegisters.X + 1) & 0xFF) << 8);

		return 0;
	}
	uint8_t RP2A03::IZY()
	{
		uint16_t arg = getBus("CPU")->read(mRegisters.PC);
		mRegisters.PC++;

		uint16_t address = (uint16_t)getBus("CPU")->read(arg) | ((uint16_t)getBus("CPU")->read((arg + 1) & 0xFF) << 8);
		mAddressAbsolute = address + mRegisters.Y;

		return (address & 0xFF00) != (mAddressAbsolute & 0xFF00);
	}
	uint8_t RP2A03::REL()
	{
		mAddressRelative = getBus("CPU")->read(mRegisters.PC);
		mRegisters.PC++;

		return 0;
	}
	uint8_t RP2A03::ZP0()
	{
		mAddressAbsolute = getBus("CPU")->read(mRegisters.PC);
		mRegisters.PC++;

		return 0;
	}
	uint8_t RP2A03::ZPX()
	{
		uint16_t address = getBus("CPU")->read(mRegisters.PC);
		mRegisters.PC++;

		mAddressAbsolute = (address + mRegisters.X) & 0xFF;

		return ((address + mRegisters.X) & 0xFF00) > 0;
	}
	uint8_t RP2A03::ZPY()
	{
		uint16_t address = getBus("CPU")->read(mRegisters.PC);
		mRegisters.PC++;

		mAddressAbsolute = (address + mRegisters.Y) & 0xFF;

		return ((address + mRegisters.Y) & 0xFF00) > 0;
	}
}