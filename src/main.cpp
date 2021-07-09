#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>

#include <array>
#include <cstdint>
#include <exception>
#include <iostream>
#include <random>
#include <sstream>

#include "emulator.hpp"
#include "memory.hpp"
#include "registermap.hpp"
#include "stack.hpp"
#include "display.hpp"
#include "utils.hpp"

int main(int argc, char *argv[])
{
  // Define variables
  //chip8::Memory memory{"../roms/INVADERS"};
  // chip8::Memory memory{"../roms/MAZE"};
  // chip8::Memory memory{"../roms/PONG"};
  chip8::Memory memory{"../roms/PUZZLE"};
  memory.start();
  
  chip8::RegisterMap rm;
  chip8::Stack stack;
  chip8::Display display;

  chip8::Emulator emulator(&memory, &rm, &stack, &display);

  try{
	while(true)
	  {
		emulator.cycle();

		display.print();

		if(chip8::g_delay_timer)
		  chip8::g_delay_timer--;

		if(chip8::g_sound_timer)
		  chip8::g_sound_timer--;

		// Delay to not run on full cpu (Hardlocked at 60FPS)
		sf::sleep(sf::milliseconds(16));
	  }
  }catch(std::exception& e)
	{
	  std::cout << e.what() << std::endl;
	}

  return 0;
}
