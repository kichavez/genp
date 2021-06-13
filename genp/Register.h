#pragma once

#include "typedefs.h"
#include "HalfRegister.h"

class Register {

public:

	Register(HalfRegister& lowByte, HalfRegister& highByte) {
		set16(0); // initialize registers to 0
	}

	// little endian registers
	static const int LO_BYTE = 0;
	static const int HI_BYTE = 1;

	byte get8Hi() { return m_raw[HI_BYTE]; }
	byte get8Lo() { return m_raw[LO_BYTE]; }
	word get16() { return ((word*)m_raw)[0]; }

	void set8Hi(byte b) { m_raw[HI_BYTE] = b; }
	void set8Lo(byte b) { m_raw[LO_BYTE] = b; }
	void set16(byte b) { (word*)m_raw = b; }

	void operator=(word w) {
		set16(w);
	}

	word operator+(Register other);

protected:
	byte m_raw[2];

private:
	friend Gb_CPU;

};
