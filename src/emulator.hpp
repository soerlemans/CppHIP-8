#pragma once

#include <random>

#include "utils.hpp"

// Forward declare necessary classes
namespace chip8
{
  class Memory;
  class RegisterMap;
  class Stack;
  class Display;

  extern u8 g_delay_timer;
  extern u8 g_sound_timer;

  class Emulator
  {
  private:
	std::mt19937 m_gen32;
	
	Memory* m_memory;
	RegisterMap* m_rm;
	Stack* m_stack;
	Display* m_display;

	u16 m_keys;
	bool m_jumped;
	
  public:
	Emulator(Memory* t_memory, RegisterMap* t_rm, Stack* t_stack, Display* t_display);

	void next_instruction();
	void register_operations();
	void cycle();
  };
}
