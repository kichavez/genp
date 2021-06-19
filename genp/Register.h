#pragma once
#include "typedefs.h"
#include <type_traits>

namespace reg_const {
	inline constexpr int HI_BYTE = 1;
	inline constexpr int LO_BYTE = 0;
	inline constexpr int BYTE_SIZE = 8;
}

class Gb_CPU;

class Register {

public:

//	static_assert(std::endian::native == std::endian::little, "Machine is not little endian!");

	void set16(word value);

	word get16() const;

	void setHighByte(byte value);

	byte getHighByte();

	void setLowByte(byte value);

	byte getLowByte();

	// operator overloads

	Register& operator=(word value);

	operator word() const;

	// end operator overloads

private:
	byte m_rawBytes[2];
};
