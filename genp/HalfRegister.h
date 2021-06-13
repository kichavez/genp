#pragma once

#include "typedefs.h"

class HalfRegister {

public:
	HalfRegister() { m_raw = 0; }

	void set(byte value) { m_raw = value; }
	byte get() { return m_raw; }

	HalfRegister& operator=(byte val) { set(val); return *this; }
	HalfRegister& operator=(HalfRegister otherReg) { set(otherReg.get(); return *this; }

private:
	byte m_raw;
};
