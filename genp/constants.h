#pragma once

namespace genp_constant {
	inline constexpr unsigned int MEMORY_SIZE = 0x10000;
	inline constexpr int MAX_CARTRIDGE_SIZE = 0x200000;
	inline constexpr int TARGET_FRAMERATE = 60;
	inline constexpr int MAX_CYCLES_PER_SECOND = 4194304;
	inline constexpr int TARGET_CLOCK_CYCLES_PER_SECOND = MAX_CYCLES_PER_SECOND / TARGET_FRAMERATE;
	inline constexpr int PC_INIT_VALUE = 0x100; // skip bootrom
	inline constexpr int NUM_8_BIT_REGISTERS = 8;
	inline constexpr int NUM_16_BIT_REGISTERS = 4;
	inline constexpr int SP_INIT_VALUE = 0xFFFE;
}
