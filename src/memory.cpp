#include "memory.hpp"

#include <algorithm>
#include <cstdio>
#include <ios>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

using namespace chip8;

// Memory:
enum MemoryAdress : u16 {
  // Fontset 0x000 - 0x080
  EndFontset = 0x080,
  // Memory 0x200 - 0xFFF
  StartMemory = 0x200
};

// Private:
void Memory::load_opcode()
{
  m_opcode = m_memory[m_pc] << 8 | m_memory[m_pc + 1];
}

// Public:
Memory::Memory(const std::string& t_path)
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
  // Fill with zeros but skip the fontset at the beginning
  std::fill(m_memory.begin() + 5 * 16, m_memory.end(), 0);

  std::ifstream ifs{t_path, std::ios::binary};

  if(ifs.is_open() == false)
	{
	  throw std::runtime_error{"Rom cant be opened for reading!"};
	}
  for(int index{StartMemory}; !ifs.eof(); index++)
	ifs.read((char *)&(m_memory[index]), sizeof(char));

  load_opcode();
}

Memory::Memory(const Memory &t_rhs)
    : m_memory{t_rhs.m_memory}, m_opcode{t_rhs.m_opcode}, m_pc{t_rhs.m_pc},
      m_index_register{t_rhs.m_index_register}
{
  load_opcode();
}

void Memory::start()
{
  m_pc = MemoryAdress::StartMemory;
  load_opcode();
}

void Memory::jump(const u16 t_pc)
{
  m_pc = t_pc;
  load_opcode();
}

[[nodiscard]] u16 Memory::get_pc() const
{
  return m_pc;
}
u16 Memory::get_opcode() const
{
  return m_opcode;
}

void Memory::set_ir(const u16 t_ir)
{
  m_index_register = t_ir;
}

void Memory::inc_ir(const u16 t_inc)
{
  m_index_register += t_inc;
}

u16 Memory::get_ir() const
{
  return m_index_register;
}

std::array<u8, 4096>::const_iterator Memory::begin()
{
  return m_memory.begin();
}

std::array<u8, 4096>::const_iterator Memory::end()
{
  return m_memory.end();
}
	
u8& Memory::operator[](const size_t t_index)
{
  return m_memory[t_index];
}

Memory& Memory::operator++()
{
  if(m_pc + 2 > (int)m_memory.size())
	{
	  std::ostringstream ss;
	  ss << "Out of bounds access(upper): [" << m_pc << "] size: " << m_memory.size() << '\n';
	  
	  throw std::out_of_range{ss.str()};
	}

  m_pc += 2;
  load_opcode();

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
  if(m_pc <= 2)
	{
	  std::ostringstream ss;
	  ss << "Out of bounds access(lower): [" << m_pc << "] size: " << m_memory.size() << '\n';
	  
	  throw std::out_of_range{ss.str()};
	}

  m_pc -= 2;
  load_opcode();

  return *this;
}

Memory Memory::operator--(int)
{
  Memory tmp{*this};
  operator--();

  return tmp;
}
