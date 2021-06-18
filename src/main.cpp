#include <iostream>

#include "chip8.hpp"

void cycle()
{

}

int main(int argc, char *argv[])
{
  // Do shit
  std::cout << "Hello world!\n";

  while(true)
	{

	  chip8::g_memory.get_opcode();
	}

  return 0;
}
