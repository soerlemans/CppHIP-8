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

  class Emulator
  {
  private:
	Memory* m_memory;
	RegisterMap* m_rm;
	Stack* m_stack;
	Display* m_display;

	u16 m_keys;
	bool m_jumped;

	u8 m_delay_timer;
	u8 m_sound_timer;
	
	std::mt19937 m_gen32;
	
  public:
	Emulator(Memory* t_memory, RegisterMap* t_rm, Stack* t_stack, Display* t_display);

	void timers();
	
	void register_operations();
	void cycle();
  };
}
