#include "Gb_CPU.h"
#include "Genp.h"
#include "constants.h"
#include "typedefs.h"

namespace {
	inline constexpr int HI_BYTE = 0;
	inline constexpr int LO_BYTE = 1;
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

Gb_CPU::Gb_CPU(Genp* emulator):
m_reg8_a { m_reg16_af.raw[HI_BYTE] },
m_reg8_f { m_reg16_af.raw[LO_BYTE] },
m_reg8_b { m_reg16_bc.raw[HI_BYTE] },
m_reg8_c { m_reg16_bc.raw[LO_BYTE] },
m_reg8_d { m_reg16_de.raw[HI_BYTE] },
m_reg8_e { m_reg16_de.raw[LO_BYTE] },
m_reg8_h { m_reg16_hl.raw[HI_BYTE] },
m_reg8_l { m_reg16_hl.raw[LO_BYTE] }
{
	m_emulator = emulator;
	m_pc = genp_constant::PC_INIT_VALUE;

	// initialize disassembly tables
	m_table_reg8[0] = &m_reg8_b;
	m_table_reg8[1] = &m_reg8_c;
	m_table_reg8[2] = &m_reg8_d;
	m_table_reg8[3] = &m_reg8_e;
	m_table_reg8[4] = &m_reg8_h;
	m_table_reg8[5] = &m_reg8_l;
	m_table_reg8[6] = nullptr;		// special case to be handled; treat HL as ptr to 8-bit value
	m_table_reg8[7] = &m_reg8_a;
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
	setCarryFlag((result & 0xFF) > 0);
	return result;
}

word Gb_CPU::add16(word w1, word w2) {
	double_word result = (double_word)w1 + w2;
}

void Gb_CPU::set16(Register& reg, word val) {
	*((word*)reg.raw) = val;
}
