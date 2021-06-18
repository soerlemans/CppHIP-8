#pragma once

#include <array>

#include "utils.hpp"

class Stack {
private:
  std::array<u8, 16> m_stack;
  u8 m_sp;
	
public:
  Stack();
	
  void push();
  void pop();

};

