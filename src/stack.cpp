#include "stack.hpp"
#include <stdexcept>

using namespace chip8;

// Stack
Stack::Stack()
  :m_sp{0}
{
  m_stack.fill(0);
}

void Stack::push(const u16 t_addr)
{
  if(m_sp >= m_stack.size() - 1)
	throw std::logic_error{"Stack overflow"};

  m_stack[m_sp++] = t_addr;
}    

u16 Stack::pop()
{
  if(m_sp <= 0)
	throw std::logic_error{"Stack underflow"};

  return m_stack[--m_sp];
}  
