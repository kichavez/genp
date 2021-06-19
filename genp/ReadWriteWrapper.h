#pragma once

#include "typedefs.h"

class ReadWriteWrapper {
public:
	virtual word get() = 0;
	virtual void set(word) = 0;
};
