#include "Gb_CPU.h"
#include "Genp.h"
#include "constants.h"
#include "typedefs.h"
#include "ReadWriteWrapper8.h"

namespace {
	inline constexpr int HI_BYTE = 0;
	inline constexpr int LO_BYTE = 1;

	const enum class Reg16 {
		AF,
		BC,
		DE,
		HL
	};

	const enum class Reg8 {
		A,
		F,
		B,
		C,
		D,
		E,
		H,
		L
	};

	const enum class Reg8_Table_Index {
		B,
		C,
		D,
		E,
		H,
		L,
		HL,
		A
	};
}

class Register8Wrapper : public ReadWriteWrapper8 {
public:
	byte get() { return (m_regPtr->*m_get8)(); }
	void set(byte input) { return (m_regPtr->*m_set8)(input); }

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

class IndirectReadWriteWrapper : public ReadWriteWrapper8 {
public:
	byte get() { m_cpuPtr->read8Memory(m_reg->get16()); }
	void set(byte input) { m_cpuPtr->write8Memory(m_reg->get16(), input); }

	IndirectReadWriteWrapper(Gb_CPU* cpuPtr, Register* reg16) {
		m_reg = reg16;
		m_cpuPtr = cpuPtr;
	}

private:
	Register* m_reg;
	Gb_CPU* m_cpuPtr;
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

	// start at register BC and handle AF and (HL) separately
	for (int i = 1; i < genp_constant::NUM_16_BIT_REGISTERS - 1; i++) {

		int upperHalfRegIdx = (i - 1) * 2;
		int lowerHalfRegIdx = upperHalfRegIdx + 1;

		m_table_reg8[upperHalfRegIdx] = new Register8Wrapper(&m_regs16[i], &Register::getHighByte, &Register::setHighByte);
		m_table_reg8[lowerHalfRegIdx] = new Register8Wrapper(&m_regs16[i], &Register::getLowByte, &Register::setLowByte);
	}

	m_table_reg8[static_cast<int>(Reg8_Table_Index::HL)] = new IndirectReadWriteWrapper(this, &m_regs16[static_cast<int>(Reg16::HL)]);
	m_table_reg8[static_cast<int>(Reg8_Table_Index::A)] = new Register8Wrapper(&m_regs16[static_cast<int>(Reg16::AF)], &Register::getHighByte, &Register::setHighByte);
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

byte Gb_CPU::read8Memory(word addr) {
	return m_emulator->m_memory[addr];
}

void Gb_CPU::write8Memory(word addr, byte val) {
	m_emulator->m_memory[addr] = val;
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
