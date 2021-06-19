#pragma once

#include "typedefs.h"
#include "constants.h"
#include "Register.h"

class Genp;
class Instruction;
class ReadWriteWrapper8;

class Gb_CPU {
public:
	Gb_CPU(Genp* emulator);

	void executeNextInstruction();

	byte read8Memory(word addr);

	void write8Memory(word addr, byte val);

private:

	Genp* m_emulator;

	// registers
	Register m_regs16[genp_constant::NUM_16_BIT_REGISTERS];

	// register operations
	byte add8(byte b1, byte b2);	// TODO: REWRITE THESE TO WORK WITH NEW REG IMPL
	word add16(word w1, word w2);
	void set16(word* reg, word val);

	// stack pointer
	word m_sp;

	// program counter
	word m_pc;

	// disassembly tables to take advantage of certain patterns in CPU's instructions
	ReadWriteWrapper8* m_table_reg8[8];		// 8 bit register table
	word* m_table_spReg16[4];	// 16 bit registers that interact with stack ptr
	word* m_table_afReg16[4];	// 16 bit registers that interact with AF

	byte read8Indirect(int reg16idx);

	void decodePrefixedInstruction(Instruction& instr);

	void setCarryFlag(bool b);
};
