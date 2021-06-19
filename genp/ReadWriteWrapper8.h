#pragma once

#include "typedefs.h"

// A class to make 8-bit reads and writes easier,
// whether reading to half of a register or
// indirectly to memory
class ReadWriteWrapper8 {
public:
	virtual byte get() = 0;
	virtual void set(byte) = 0;

};
