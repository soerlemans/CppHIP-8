#pragma once

#include <array>

#include "utils.hpp"

namespace chip8
{
  enum RegisterName : u8 {
	V0 = 0,	V1,	V2,	V3,	V4,	V5,	V6,	V7,
	V8,	V9,	VA,	VB,	VC,	VD,	VE,	VF
  };

  class RegisterMap {
  private:
	std::array<u8, 16> m_registers;

  public:
	RegisterMap();

	std::array<u8, 16>::const_iterator begin() const;
	std::array<u8, 16>::const_iterator end() const;

	u8& operator[](const u8 t_index);
  };
}
