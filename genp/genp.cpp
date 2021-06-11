#include "genp.h"
#include "genp.h"
#include "SDL.h"
#include "constants.h"
#include "typedefs.h"
#include <string>
#include <stdexcept>
#include "Gb_CPU.h";

Genp::Genp() {
	m_targetFramerate = TARGET_FRAMERATE;
	m_cpu = new Gb_CPU(this);
}

Genp::~Genp() {

	// It's faster to just let the OS reclaim the memory
#ifndef FAST_EXIT
	delete m_cpu;
#endif

}

void Genp::loadRom(byte rom_buffer[], int rom_size) {
	if (rom_size > MAX_CARTRIDGE_SIZE) {
		throw std::runtime_error("ROM size exceeds maximum! (MAX = " + std::to_string(MAX_CARTRIDGE_SIZE) + " bytes)");
	}

	for (int i = 0; i < rom_size; i++) {
		m_cartridgeMem[i] = rom_buffer[i];
	}
}

void Genp::run() {
	m_running = true;

	while (m_running) {
		// draw frames at the target framerate
		// TODO: Implement this
	}
}

void Genp::processNextFrame() {
	int cycles{ 0 };

	while (cycles < MAX_CYCLES_PER_SECOND) {
		cycles += execNextOpcode();
	}
}
