#include "Gb_CPU.h"
#include "Genp.h"
#include "constants.h"
#include "typedefs.h"
#include "ReadWriteWrapper.h"

namespace {
	inline constexpr int HI_BYTE = 0;
	inline constexpr int LO_BYTE = 1;

	const enum REG16 {
		AF,
		BC,
		DE,
		HL
	};

	const enum REG8 {
		A,
		F,
		B,
		C,
		D,
		E,
		H,
		L
	};

}

class Register8Wrapper : public ReadWriteWrapper {
public:
	word get() { return (m_regPtr->*m_get8)(); }
	void set(word input) { return (m_regPtr->*m_set8)(input); }

	Register8Wrapper(Register* fullRegPtr, byte(Register::* getter) (), void(Register::* setter) (byte)) {
		m_set8 = setter;
		m_get8 = getter;
		m_regPtr = fullRegPtr;
	}

private:
	void(Register::* m_set8) (byte);
	byte(Register::* m_get8) ();
	Register* m_regPtr;
};

class IndirectReadWriteWrapper : public ReadWriteWrapper {
public:
	word get();
	void set(word input);

	IndirectReadWriteWrapper(Register* reg16) {
		m_reg = reg16;
	}

private:
	Register* m_reg;
};

class Instruction {
	
public:
	Instruction(byte instruction){
		m_rawInstruction = instruction;
	}

	const byte getHighOctal() { return m_rawInstruction >> 6; }
	const byte getMidOctal() { return m_rawInstruction >> 3 && 0xFF; }
	const byte getLowOctal() { return m_rawInstruction & 0xFF; }

	// get second octal right shifted 1 bit
	const byte getMidRshifted() { return m_rawInstruction >> 4 & 0xFF; }

	// get modulo 2 of second octal (aka LSB of 2nd oct)
	const byte getMidMod2() { return m_rawInstruction >> 3 & 0x01; }

	const byte getFullInstruction() { return m_rawInstruction; }

private:
	byte m_rawInstruction;

};

Gb_CPU::Gb_CPU(Genp* emulator) {
	m_emulator = emulator;
	m_pc = genp_constant::PC_INIT_VALUE;

	// initialize disassembly tables
	// TODO: init these; make sure (HL) is an indirect reference
	for (int i = 0; i < genp_constant::NUM_16_BIT_REGISTERS; i++) {
		int upperHalfRegIdx = i * 2;
		int lowerHalfRegIdx = upperHalfRegIdx + 1;

		
	}
}

void Gb_CPU::executeNextInstruction() {
	Instruction instr(m_emulator->m_cartridgeMem[m_pc]);

	switch (instr.getHighOctal()) {

	// prefix opcodes
	case 0xCB:
	case 0xDD:
	case 0xED:
	case 0xFD:

		/* Handle prefixed stuff */
		
		break;



	default:
		break;
	}
}

byte Gb_CPU::add8(byte b1, byte b2) {
	word result = (word)b1 + b2;
	setCarryFlag(result >> 4);
	return result;
}

word Gb_CPU::add16(word w1, word w2) {
	double_word result = (double_word)w1 + w2;
	setCarryFlag(result >> 8);
	return result;
}

void Gb_CPU::set16(word* reg, word val) {
	*reg = val;
}

byte Gb_CPU::read8Indirect(int reg16idx) {
	return m_emulator->m_memory[m_regs16[reg16idx].get16()];
}

byte Gb_CPU::read8Memory(word address) {
	return m_emulator->m_memory[address];
}

