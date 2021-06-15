#include "Register.h"
#include "typedefs.h"

void Register::set16(word value) {
	m_rawBytes[reg_const::LO_BYTE] = value & 0xFF;
	m_rawBytes[reg_const::HI_BYTE] = value >> reg_const::BYTE_SIZE;
}

word Register::get16() const {
	word retVal = m_rawBytes[reg_const::HI_BYTE];
	retVal <<= reg_const::BYTE_SIZE;
	retVal |= m_rawBytes[reg_const::LO_BYTE];
	return retVal;
}

Register& Register::operator=(word value) {
	set16(value);

	return *this;
}

Register::operator word() const {
	get16();
}
