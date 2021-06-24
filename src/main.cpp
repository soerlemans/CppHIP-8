#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <exception>
#include <iostream>
#include <sstream>

#include "chip8.hpp"

void cycle()
{
  // const u16 opcode{(u16)(chip8::g_memory.get_opcode() & 0xF000)};
  const u8 opcode{(u8)(chip8::g_memory.get_opcode() & 0xF000)};
  if(opcode)
	std::cout << "opcode: " << opcode << std::endl;
  // switch(opcode)
	// {
	// default:
	//   std::cerr << "unrecognised opcode: " << opcode << std::endl;
	// }
}

int main(int argc, char *argv[])
{
  try{
	// Do shit
	while(true)
	{
	  cycle();

	  chip8::g_memory++;
	  
	  // Delay to not run on full cpu
	  sf::sleep(sf::milliseconds(1));
	}
  }catch(std::exception e)
	{
	  std::cout << e.what() << std::endl;
	}

  return 0;
}
