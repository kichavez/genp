#pragma once

class RegisterAF : public Register {

public:
	void setZeroFlag(bool b) { m_raw[LO_BYTE] &= 0x7F; m_raw[LO_BYTE] |= ((byte)b << 7); }
	void setCarryFlag(bool b) { m_raw[LO_BYTE] &= 0xF7; m_raw[LO_BYTE] |= ((byte)b << 4); }

};
