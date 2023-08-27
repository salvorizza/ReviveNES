#pragma once

#include <Base/Bus.h>

#include <array>

namespace NESEmu {

	constexpr uint16_t SP_BASE = 0x100;
	constexpr uint16_t NMI_VEC = 0xFFFA;
	constexpr uint16_t RESET_VEC = 0xFFFC;
	constexpr uint16_t IRQ_INT_VEC = 0xFFFE;

	constexpr uint16_t SQ1_VOL_ADDR = 0x4000;
	constexpr uint16_t SQ1_SWEEP_ADDR = 0x4001;
	constexpr uint16_t SQ1_LO_ADDR = 0x4002;
	constexpr uint16_t SQ1_HI_ADDR = 0x4003;
	constexpr uint16_t SQ2_VOL_ADDR = 0x4004;
	constexpr uint16_t SQ2_SWEEP_ADDR = 0x4005;
	constexpr uint16_t SQ2_LO_ADDR = 0x4006;
	constexpr uint16_t SQ2_HI_ADDR = 0x4007;
	constexpr uint16_t TRI_LINEAR_ADDR = 0x4008;
	constexpr uint16_t TRI_LO_ADDR = 0x400A;
	constexpr uint16_t TRI_HI_ADDR = 0x400B;
	constexpr uint16_t NOISE_VOL_ADDR = 0x400C;
	constexpr uint16_t NOISE_LO_ADDR = 0x400E;
	constexpr uint16_t NOISE_HI_ADDR = 0x400F;
	constexpr uint16_t DMC_FREQ_ADDR = 0x4010;
	constexpr uint16_t DMC_RAW_ADDR = 0x4011;
	constexpr uint16_t DMC_START_ADDR = 0x4012;
	constexpr uint16_t DMC_LEN_ADDR = 0x4013;
	constexpr uint16_t SND_CHN_ADDR = 0x4015;
	constexpr uint16_t JOY1_ADDR = 0x4016;
	constexpr uint16_t JOY2_ADDR = 0x4017;

	typedef class RP2A03;
	typedef uint8_t(RP2A03::*operate)(void);
	typedef uint8_t(RP2A03::*addrmode)(void);

	enum class RP2A03_StatusFlag : uint8_t {
		C = 1 << 0,
		Z = 1 << 1,
		I = 1 << 2,
		D = 1 << 3,
		B = 1 << 4,
		U = 1 << 5,
		V = 1 << 6,
		N = 1 << 7
	};
	
	struct PulseChannel {
		uint8_t VOL;
		uint8_t SWEEP;
		uint8_t LO;
		uint8_t HI;
	};

	struct TriangleChannel {
		uint8_t LINEAR;
		uint8_t LO;
		uint8_t HI;
	};

	struct NoiseChannel {
		uint8_t VOL;
		uint8_t LO;
		uint8_t HI;
	};

	struct DMCChannel {
		uint8_t FREQ;
		uint8_t RAW;
		uint8_t START;
		uint8_t LEN;
	};

	struct RP2A03_Registers {
		uint16_t PC;
		uint8_t A;
		uint8_t X;
		uint8_t Y;
		uint8_t SP;
		uint8_t SR;

		PulseChannel SQ1, SQ2;
		TriangleChannel TRI;
		NoiseChannel NOISE;
		DMCChannel DMC;

		uint8_t SND_CHN;
		uint8_t JOY1, JOY2;

		uint16_t LFSR;
	};

	struct RP2A03_Instruction {
		std::string Mnemonic;
		operate Operation;
		addrmode AddressingMode;
		uint8_t Cycles;

		int32_t opcode = 0x00;
		int32_t lo = -1;
		int32_t hi = -1;
		uint16_t address = 0x0000;
	};

	#define INSTR(o,a,c) {#o,&RP2A03::o,&RP2A03::a,c}


	class RP2A03 : public BusDevice {
	public:

		RP2A03();
		~RP2A03();

		void clock();
		void powerUp();
		void reset();
		void irq();
		void nmi();

		virtual void writeLine(const std::string& busName, const std::string& lineName, bool value) override;
		virtual void write(const std::string& busName,uint16_t address, uint8_t value) override;
		virtual uint8_t read(const std::string& busName, uint16_t address) override;

		uint8_t getFlag(RP2A03_StatusFlag flag);
		void setFlag(RP2A03_StatusFlag flag, bool value);

	private:
		uint8_t fetch();

		/*Load/Store Operations*/
		uint8_t LDA(); uint8_t LDX(); uint8_t LDY(); uint8_t STA(); uint8_t STX(); uint8_t STY();
		/*Register Transfers*/
		uint8_t TAX(); uint8_t TAY(); uint8_t TXA(); uint8_t TYA();
		/*Stack Operations*/
		uint8_t TSX(); uint8_t TXS(); uint8_t PHA(); uint8_t PHP(); uint8_t PLA(); uint8_t PLP();
		/*Logical*/
		uint8_t AND(); uint8_t EOR(); uint8_t ORA(); uint8_t BIT();
		/*Arithmetic*/
		uint8_t ADC(); uint8_t SBC(); uint8_t CMP(); uint8_t CPX(); uint8_t CPY();
		/*Increments & Decrements*/
		uint8_t INC(); uint8_t INX(); uint8_t INY(); uint8_t DEC(); uint8_t DEX(); uint8_t DEY();
		/*Shifts*/
		uint8_t ASL(); uint8_t LSR(); uint8_t ROL(); uint8_t ROR();
		/*Jumps & Calls*/
		uint8_t JMP(); uint8_t JSR(); uint8_t RTS();
		/*Branches*/
		uint8_t BCC(); uint8_t BCS(); uint8_t BEQ(); uint8_t BMI(); uint8_t BNE(); uint8_t BPL(); uint8_t BVC(); uint8_t BVS();
		/*Status Flag Changes*/
		uint8_t CLC(); uint8_t CLD(); uint8_t CLI(); uint8_t CLV(); uint8_t SEC(); uint8_t SED(); uint8_t SEI();
		/*System Functions*/
		uint8_t BRK(); uint8_t NOP(); uint8_t RTI();
		/*Unknown*/
		uint8_t XXX();

		/*Accumulator*/
		uint8_t ACC();

		/*Absolute*/
		uint8_t ABS(); 
		
		/*Absolute,X*/
		uint8_t ABX(); 

		/*Absolute,Y*/
		uint8_t ABY();

		/*Immediate*/
		uint8_t IMM();
		
		/*Implied*/
		uint8_t IMP();

		/*Indirect*/
		uint8_t IND(); 

		/*Indexed Indirect*/
		uint8_t IZX(); 

		/*Indirect Indexed*/
		uint8_t IZY();

		/*Relative*/
		uint8_t REL();

		/*Zero Page*/
		uint8_t ZP0();

		/*Zero Page,X*/
		uint8_t ZPX();

		/*Zero Page,Y*/
		uint8_t ZPY();
		
	public:
		RP2A03_Registers mRegisters;
		size_t mCyclesToConsume = 0;
		size_t mCyclesPassed = 0;

		std::vector<RP2A03_Instruction> mInstructionsLookup;
		uint16_t mAddressAbsolute;
		int8_t mAddressRelative;
		uint8_t mFetched;
		RP2A03_Instruction mCurrentInstruction;
		bool mNMILine = false, mNMIPending = false;
	};

}