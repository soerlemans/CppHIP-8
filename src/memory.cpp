#include "memory.hpp"

#include <algorithm>

// Memory:
enum MemoryAdress : u16 {
  // Fontset 0x000 - 0x080
  EndFontset = 0x080,
  // Memory 0x200 - 0xFFF
  StartMemory = 0x200
};

Memory::Memory()
  :m_memory{ // fontset
	  { 0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	  } },
   m_opcode{0}, m_pc{0}, m_index_register{0}
{
  std::fill(m_memory.begin() + 5 * 16, m_memory.end(), 0);
}

Memory::Memory(const Memory& t_rhs)
  :m_memory{t_rhs.m_memory},
   m_opcode{t_rhs.m_opcode},
   m_pc{t_rhs.m_pc},
   m_index_register{t_rhs.m_index_register}
{
}

u16 Memory::get_opcode() const
{
  return m_opcode;
}

Memory& Memory::operator++()
{
  m_opcode = m_memory[m_pc] << 8 | m_memory[m_pc + 1];
  m_pc += 2;

  return *this;
}

Memory Memory::operator++(int)
{
  Memory tmp{*this};
  operator++();

  return tmp;
}

Memory& Memory::operator--()
{
  m_opcode = m_memory[m_pc] << 8 | m_memory[m_pc + 1];
  m_pc -= 2;

  return *this;
}

Memory Memory::operator--(int)
{
  Memory tmp{*this};
  operator--();

  return tmp;
}
