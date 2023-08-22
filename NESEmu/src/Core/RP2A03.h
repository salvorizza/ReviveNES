#pragma once

#include <Base/Bus.h>

#include <array>

namespace NESEmu {

	#define SP_BASE 0x100
	#define NMI_VEC 0xFFFA
	#define RESET_VEC 0xFFFC
	#define IRQ_INT_VEC 0xFFFE

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


	struct RP2A03_Registers {
		uint16_t PC;
		uint8_t A;
		uint8_t X;
		uint8_t Y;
		uint8_t SP;
		uint8_t SR;
	};

	struct RP2A03_Instruction {
		std::string Mnemonic;
		operate Operation;
		addrmode AddressingMode;
		uint8_t Cycles;
	};

#define INSTR(o,a,c) {#o,&RP2A03::o,&RP2A03::a,c}

	class RP2A03 : public BusDevice {
	public:
		RP2A03();
		~RP2A03();

		void clock();
		void reset();
		void irq();
		void nmi();

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
		
	private:
		RP2A03_Registers mRegisters;
		uint8_t mCycles;
		std::vector<RP2A03_Instruction> mInstructionsLookup;
		uint16_t mAddressAbsolute;
		int8_t mAddressRelative;
		uint8_t mFetched;
		RP2A03_Instruction mCurrentInstruction;
	};

}