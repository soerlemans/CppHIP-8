#pragma once

#include <array>

#include "utils.hpp"

namespace chip8
{
class Stack {
private:
  std::array<u16, 16> m_stack;
  u8 m_sp;
	
public:
  Stack();
	
  void push(const u16 t_addr);
  u16 pop();
};

}
