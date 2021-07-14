#pragma once

#include <array>
#include <exception>

#include "utils.hpp"

namespace chip8
{
  enum Opcode : u16 {
	SYS = 0x0000,
	CLS = 0x00E0,
	RET = 0x00EE,

	JMP  = 0x1000,
	CALL = 0x2000,
	SE   = 0x3000,
	SNE  = 0x4000,
	SE_R = 0x5000,
	LD   = 0x6000,
	ADD  = 0x7000,

	// Register operations:
	LD_R   = 0x8000,
	OR_R   = 0x8001,
	AND_R  = 0x8002,
	XOR_R  = 0x8003,
	ADD_R  = 0x8004,
	SUB_R  = 0x8005,
	SHR_R  = 0x8006,
	SUBN_R = 0x8007,
	SHL_R  = 0x800E,

	SNE_R  = 0x9000,
	LD_I   = 0xA000,
	JMP_V0 = 0xB000,
	RND    = 0xC000,
	DRW    = 0xD000,

	SKP  = 0xE09E,
	SKNP = 0xE0A1,

	LD_DT_R = 0xF007,
	LD_K_R = 0xF00A,

	LD_R_DT = 0xF015,
	LD_R_ST = 0xF018,

	ADD_I   = 0xF01E,
	LD_FONT = 0xF029,
	LD_BCD  = 0xF033,

	LD_R_M = 0xF055,
	LD_M_R = 0xF065
  };

  class Memory {
  private:
	std::array<u8, 4096> m_memory;
	u16 m_opcode;
	u16 m_pc;
	u16 m_index_register;

	void load_opcode();

  public:
	Memory(const std::string& t_path);
	Memory(const Memory& t_rhs);

	void start();

	void jump(const u16 t_pc);
	[[nodiscard]] u16 get_pc() const;
	[[nodiscard]] u16 get_opcode() const;

	void set_ir(const u16 t_ir);
	void inc_ir(const u16 t_inc);
	[[nodiscard]] u16 get_ir() const;

	std::array<u8, 4096>::const_iterator begin();
	std::array<u8, 4096>::const_iterator end();
	
	template<typename T>
	void set(T t_begin, T t_end)
	{
	  const auto iter{m_memory.begin() + m_index_register};
	  std::copy(t_begin, t_end, iter);
	}

	template<typename T>
	void get_nth(const u8 t_nth, T t_iter) const
	{
	  const auto begin{m_memory.begin() + m_index_register};
	  std::copy(begin, begin + t_nth, t_iter);
	}

	[[nodiscard]] u8& operator[](const size_t t_index);

	Memory& operator++();
	Memory operator++(int);

	Memory& operator--();
	Memory operator--(int);
  };
}
