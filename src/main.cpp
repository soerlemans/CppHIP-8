#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>

#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <cstdint>
#include <exception>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>

#include "emulator.hpp"
#include "memory.hpp"
#include "registermap.hpp"
#include "stack.hpp"
#include "display.hpp"
#include "timer.hpp"
#include "utils.hpp"

int main(int argc, char *argv[])
{
  if(argc < 2)
	{
	  std::cerr << "No rom path was given!\n";
	  return 1;
	}

  std::string rom_path{argv[1]};
  
  // Define variables
  try{
	chip8::Memory memory{rom_path};

	memory.start();
	
	chip8::RegisterMap rm;
	chip8::Stack stack;
	chip8::Display display;
	
	chip8::Emulator emulator(&memory, &rm, &stack, &display);
	
	std::chrono::milliseconds ms{16};
	Timer timer{ms};
	
	for(bool quit{false}; !quit;)
	  {
		emulator.timers();
		emulator.cycle();
		display.print();
		
		// Locks the execution to exactly 60 Hz
		while(!timer.check())
		  ;
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		  quit = true;
	  }
  }catch(const std::runtime_error& e)
	{
	  std::cerr << "RT ERROR: " << e.what() << std::endl;
	  return 2;
	}
  catch(const std::exception& e)
	{
	  std::cerr << "EXCEPT: " << e.what() << std::endl;
	  return 3;
	}

  return 0;
}
