#pragma once

#include <typedefs.h>
#include <constants.h>

class Genp;
class Instruction;

class Gb_CPU {
public:
	Gb_CPU(Genp* emulator);

	void executeNextInstruction();

private:

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

	class Register {

	public:

		Register(HalfRegister& lowByte, HalfRegister& highByte) {
			set16(0); // initialize registers to 0
		}

		// little endian registers
		static const int LO_BYTE = 0;
		static const int HI_BYTE = 1;

		byte get8Hi() { return m_raw[HI_BYTE];}
		byte get8Lo() { return m_raw[LO_BYTE]; }
		word get16() { return ((word*)m_raw)[0]; }

		void set8Hi(byte b) { m_raw[HI_BYTE] = b; }
		void set8Lo(byte b) { m_raw[LO_BYTE] = b; }
		void set16(byte b) { (word*)m_raw = b; }

		void operator=(word w) {
			set16(w);
		}

		word operator+(Register other) {
			return get16() + other.get16();
		}

	protected:
		byte m_raw[2];

	private:
		friend Gb_CPU;

	};

	class RegisterAF : public Register {

	public:
		setZeroFlag(bool b) { m_raw[LO_BYTE] &= 0x7F; m_raw[LO_BYTE] |= ((byte)b << 7); }
		setCarryFlag(bool b) { m_raw[LO_BYTE] &= 0xF7; m_raw[LO_BYTE] |= ((byte)b << 4); }


	private:
		friend Gb_CPU;
	};

	Genp* m_emulator;

	// registers
	RegisterAF m_reg_af;
	Register m_reg_bc;
	Register m_reg_de;
	Register m_reg_hl;

	// stack pointer
	Register m_sp;

	// program counter
	Register m_pc;

	// disassembly tables to take advantage of certain patterns in CPU's instructions
	byte* m_table_reg[8];	// 8 bit register table
	

	void decodePrefixedInstruction(Instruction& instr);

};
