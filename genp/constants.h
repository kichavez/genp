#pragma once

const int MAX_CARTRIDGE_SIZE = 0x200000;
const int TARGET_FRAMERATE = 60;
const int MAX_CYCLES_PER_SECOND = 4194304;
const int TARGET_CLOCK_CYCLES_PER_SECOND = MAX_CYCLES_PER_SECOND / TARGET_FRAMERATE;
const int PC_INIT_VALUE = 0x100; // skip bootrom
const int NUM_REGISTERS = 8;
