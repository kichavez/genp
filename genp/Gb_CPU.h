#pragma once

#include "typedefs.h"
#include "constants.h"

class Genp;
class Instruction;

struct Register {
	byte raw[2];
};

class Gb_CPU {
public:
	Gb_CPU(Genp* emulator);

	void executeNextInstruction();

private:

	Genp* m_emulator;

	// registers
	Register m_reg16_af;
	Register m_reg16_bc;
	Register m_reg16_de;
	Register m_reg16_hl;

	// references to register halves to make 8-bit operations easier to write
	byte& m_reg8_a;
	byte& m_reg8_f;
	byte& m_reg8_b;
	byte& m_reg8_c;
	byte& m_reg8_d;
	byte& m_reg8_e;
	byte& m_reg8_h;
	byte& m_reg8_l;
	// TODO: cull unused register references later

	// register operations
	byte add8(byte b1, byte b2);
	word add16(word w1, word w2);
	void set16(Register& reg, word val);

	// stack pointer
	word m_sp;

	// program counter
	word m_pc;

	// disassembly tables to take advantage of certain patterns in CPU's instructions
	byte* m_table_reg8[8];		// 8 bit register table
	word* m_table_spReg16[4];	// 16 bit registers that interact with stack ptr
	word* m_table_afReg16[4];	// 16 bit registers that interact with AF
	

	void decodePrefixedInstruction(Instruction& instr);

	void setCarryFlag(bool b);
};
