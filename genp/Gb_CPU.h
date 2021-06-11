#pragma once

#include <typedefs.h>
#include <constants.h>

class Genp;

class Gb_CPU {
public:
	Gb_CPU(Genp* emulator);

	void executeNextInstruction();

private:
	Genp* m_emulator;

	// registers
	word m_reg[NUM_REGISTERS];

	// stack pointer
	word m_sp;

	// program counter
	word m_pc;

};
