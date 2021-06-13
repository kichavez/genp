#pragma once

#include "typedefs.h"
#include "constants.h"
#include "Register.h"
#include "HalfRegister.h"
#include "RegisterAF.h"

class Genp;
class Instruction;

class Gb_CPU {
public:
	Gb_CPU(Genp* emulator);

	void executeNextInstruction();

private:

	Genp* m_emulator;

	// registers
	RegisterAF m_reg_af;
	Register m_reg_bc;
	Register m_reg_de;
	Register m_reg_hl;

	// stack pointer
	Register m_sp;

	// program counter
	Register m_pc;

	// disassembly tables to take advantage of certain patterns in CPU's instructions
	byte* m_table_reg[8];	// 8 bit register table
	

	void decodePrefixedInstruction(Instruction& instr);

};
