#include "Gb_CPU.h"
#include "Genp.h"
#include "constants.h"
#include "typedefs.h"

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
	m_table_reg8[0] = &m_registers[B];
	m_table_reg8[1] = &m_registers[C];
	m_table_reg8[2] = &m_registers[D];
	m_table_reg8[3] = &m_registers[E];
	m_table_reg8[4] = &m_registers[H];
	m_table_reg8[5] = &m_registers[L];
	m_table_reg8[6] = &(emulator->m_memory[((word*)m_registers)[HL]]); // TODO: THIS IS INCORRECT; MOVE TO FUNC
	m_table_reg8[7] = &m_registers[A];
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

