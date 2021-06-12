#include "Gb_CPU.h"
#include "Genp.h"
#include "constants.h"
#include "typedefs.h"

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

	const getFullInstruction() { return m_rawInstruction; }

private:
	byte m_rawInstruction;

};

Gb_CPU::Gb_CPU(Genp* emulator) {
	m_emulator = emulator;
	m_pc = PC_INIT_VALUE;
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
