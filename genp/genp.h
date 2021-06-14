// genp.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include "typedefs.h"
#include "constants.h"

class Gb_CPU;

class Genp {

public:

	Genp();

	~Genp();

	void setFlags(uint16_t flags);

	void loadRom(byte rom_buffer[], int rom_size);

	bool loadRom(byte rom_buffer[], int rom_size, uint64_t flags);

	void run();

private:

	friend class Gb_CPU;

	byte m_cartridgeMem[genp_constant::MAX_CARTRIDGE_SIZE];
	byte m_memory[genp_constant::MEMORY_SIZE];
	uint64_t m_flags;
	int m_targetFramerate;
	bool m_running;
	Gb_CPU* m_cpu;

	// perform all updates needed for next frame to render
	void processNextFrame();

	void updateTimers();
	void updateGraphics();
	void proccessInterrupts();
	void renderScreen();

	// Description: Fetches next opcode from memory and emulates it
	// Returns: number of cycles used by opcode
	int execNextOpcode();
};
