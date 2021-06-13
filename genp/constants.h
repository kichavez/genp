#pragma once

namespace genp_constant {
	inline constexpr int MAX_CARTRIDGE_SIZE = 0x200000;
	inline constexpr int TARGET_FRAMERATE = 60;
	inline constexpr int MAX_CYCLES_PER_SECOND = 4194304;
	inline constexpr int TARGET_CLOCK_CYCLES_PER_SECOND = MAX_CYCLES_PER_SECOND / TARGET_FRAMERATE;
	inline constexpr int PC_INIT_VALUE = 0x100; // skip bootrom
	inline constexpr int NUM_REGISTERS = 8;
}
