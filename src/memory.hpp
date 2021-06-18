#pragma once

#include <array>

#include "utils.hpp"

enum Opcode : u16 {
  CLS = 0x0000,
  RET = 0x00EE,
  JMP = 0x1000
};

class Memory {
private:
  std::array<u8, 4096> m_memory;
  u16 m_opcode;
  u16 m_pc;
  u16 m_index_register;

public:
  Memory();
  Memory(const Memory& t_rhs);

  u16 get_opcode() const;

  Memory& operator++();
  Memory operator++(int);

  Memory& operator--();
  Memory operator--(int);
};

