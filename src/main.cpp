#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <exception>
#include <iostream>
#include <sstream>

#include "chip8.hpp"

void cycle(chip8::Memory& t_memory, chip8::RegisterMap& t_register_map, chip8::Display& t_display)
{
  using namespace chip8;

  // const u16 opcode{(u16)(chip8::g_memory.get_opcode() & 0xF000)};
  const u16 opcode{t_memory.get_opcode()};

  // TODO: Every command its own function
  switch((u8)opcode & 0xF000)
	{
	case 0x0:
	  switch (opcode)
		{
		case Opcode::CLS:
		  std::cout << "Clear that motherufcing screen \n";
		  t_display.clear();
		  break;

		case Opcode::RET:
		  std::cout << "Return from function\n";
		  break;

		default:
		  // Runs assembly on host machine (deprecated, functionality)
		  std::cout << "SYS: " << (opcode & 0x0FFF) << '\n';
		  break;
		}
	  break;

	case Opcode::JMP: {
	  std::cout << "JMP: " << (opcode & 0x0FFF) << '\n';
	  const u16 addr = opcode & 0x0FFF;
	  t_memory.jump(addr);
	  break;
	}

	case Opcode::CALL:
	  // PC = 0nnn, pop current location to stack
	  break;

	case 0x3:
	  break;

	case 0x4:
	  break;

	case Opcode::LD: {
	  const u8 index = opcode & 0x0F00;
	  const u8 byte = opcode & 0x00FF;
	  t_register_map[index] = byte;
	  std::cout << "LD: " << index << ", " << t_register_map[index] << std::endl;
	  break;
	}

	case Opcode::ADD: {
	  const u8 index = opcode & 0x0F00;
	  const u8 byte = opcode & 0x00FF;
	  t_register_map[index] += byte;
	  break;
	}

	case 0x8000:
	  switch (opcode) {
	  case Opcode::LD_R:
		break;

	  default:
		break;
	  }
	  break;

	default:
	  std::cerr << "unknown opcode: " << opcode << std::endl;
	  break;
	}
}

int main(int argc, char *argv[])
{
  // Define variables
  chip8::Memory memory{"../roms/INVADERS"};
  chip8::RegisterMap register_map;
  chip8::Stack stack;
  chip8::Display display;

  try{
	while(true)
	{
	  cycle(memory, register_map, display);

	  memory++;

	  // Delay to not run on full cpu
	  sf::sleep(sf::milliseconds(1));
	}
  }catch(std::exception& e)
	{
	  std::cout << e.what() << std::endl;
	}

  return 0;
}
