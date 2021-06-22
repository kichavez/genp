#include "Gb_CPU.h"
#include "Genp.h"
#include "constants.h"
#include "typedefs.h"
#include "ReadWriteWrapper8.h"
#include <stdexcept>

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

	const enum class ALUOperations {
		ADD_A,
		ADC_A,
		SUB,
		SBC_A,
		AND,
		XOR,
		OR,
		CP
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
	m_sp = genp_constant::SP_INIT_VALUE;

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

	m_table_spReg16[0] = &m_regs16[static_cast<int>(Reg16::BC)];
	m_table_spReg16[1] = &m_regs16[static_cast<int>(Reg16::DE)];
	m_table_spReg16[2] = &m_regs16[static_cast<int>(Reg16::HL)];
	m_table_spReg16[3] = &m_sp;

	m_table_afReg16[0] = &m_regs16[static_cast<int>(Reg16::BC)];
	m_table_afReg16[1] = &m_regs16[static_cast<int>(Reg16::DE)];
	m_table_afReg16[2] = &m_regs16[static_cast<int>(Reg16::HL)];
	m_table_afReg16[3] = &m_regs16[static_cast<int>(Reg16::AF)];
}

void Gb_CPU::executeNextInstruction() {
	Instruction instr(fetchNextInstruction());

	switch (instr.getHighOctal()) {

	// prefix opcodes
	case 0xCB:
	case 0xDD:
	case 0xED:
	case 0xFD:

		/* Handle prefixed stuff */
		
		break;

	case 0x00:
		doX0(instr);
		break;

	case 0x01:
		doX1(instr);
		break;

	case 0x02:
		doX2(instr);
		break;

	case 0x03:
		doX3(instr);
		break;

	default:
		// we should never reach this
		throw std::runtime_error("High octal value outside of range 0-3 in Gb_CPU::executeNextInstruction()! Something has gone very wrong if you can see this!");
		break;
	}
}

byte Gb_CPU::read8Memory(word addr) {
	return m_emulator->m_memory[addr];
}

void Gb_CPU::write8Memory(word addr, byte val) {
	m_emulator->m_memory[addr] = val;
}

Gb_CPU::~Gb_CPU() {
#ifndef FAST_EXIT
	for (int i = 0; i < 8; i++) {
		delete m_table_reg8[i];
	}
#endif
}

byte Gb_CPU::add8(byte b1, byte b2) {
	word result = static_cast<word>(b1) + b2;
	setCarryFlag(result >> 4);
	return result;
}

word Gb_CPU::sub16(word w1, word w2) {

	setCarryFlag(w1 < w2);

	if (getCarryFlag())
		return w2 - w1;
	else return w1 - w2;
}

word Gb_CPU::add16(word w1, word w2) {
	double_word result = static_cast<double_word>(w1) + w2;
	setCarryFlag(result >> 8);
	return result;
}

void Gb_CPU::doAluOp(int operation, word arg1, word arg2 = 0) {

	ReadWriteWrapper8* regAPtr = m_table_reg8[static_cast<int>(Reg8_Table_Index::A)];

	switch (operation) {
	
	case static_cast<int>(ALUOperations::ADD_A):
		regAPtr->set(add8(regAPtr->get(), arg1));
		break;

	case static_cast<int>(ALUOperations::ADC_A):
		add8(regAPtr->get(), add8(arg1, getCarryFlag()));
		break;

	case static_cast<int>(ALUOperations::SUB):
		//regAPtr->set()
		break;

	case static_cast<int>(ALUOperations::SBC_A):
		// TODO: impl
		break;

	case static_cast<int>(ALUOperations::AND):
		// TODO: impl
		break;

	case static_cast<int>(ALUOperations::XOR):
		// TODO: impl
		break;

	case static_cast<int>(ALUOperations::OR):
		// TODO: impl
		break;

	case static_cast<int>(ALUOperations::CP):
		// TODO: impl
		break;
	
	default:
		break;
	}
}

byte Gb_CPU::read8Indirect(int reg16idx) {
	return m_emulator->m_memory[m_regs16[reg16idx].get16()];
}

byte Gb_CPU::fetchNextInstruction() {
	byte result = m_emulator->m_memory[m_pc];
	m_pc++;
	return result;
}

int Gb_CPU::doX1(Instruction& i) {

	int indirectHLIdx = static_cast<int>(Reg8_Table_Index::HL);
	short y = i.getMidOctal();
	short z = i.getLowOctal();

	if (y == indirectHLIdx && z == indirectHLIdx) {
		// TODO: raise exception (HALT)
		return -1;
	}

	m_table_reg8[y]->set(m_table_reg8[z]->get());

	// Register-to-register loads take 4 cycles, indirect load takes 8 cycles
	if (y == indirectHLIdx || z == indirectHLIdx) {
		return 8;
	}
	else return 4;
}
