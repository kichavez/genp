#pragma once

#include "typedefs.h"
#include "constants.h"

class Genp;
class Instruction;

class Gb_CPU {
public:
	Gb_CPU(Genp* emulator);

	void executeNextInstruction();

private:

	Genp* m_emulator;

	// registers
	byte m_registers[genp_constant::NUM_8_BIT_REGISTERS];

	// register operations
	byte add8(byte b1, byte b2);
	word add16(word w1, word w2);
	void set16(word* reg, word val);

	// stack pointer
	word m_sp;

	// program counter
	word m_pc;

	// disassembly tables to take advantage of certain patterns in CPU's instructions
	byte* m_table_reg8[8];		// 8 bit register table
	word* m_table_spReg16[4];	// 16 bit registers that interact with stack ptr
	word* m_table_afReg16[4];	// 16 bit registers that interact with AF

	byte read8Indirect(int register16Idx);

	void decodePrefixedInstruction(Instruction& instr);

	void setCarryFlag(bool b);
};
