#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>

#include <exception>
#include <iostream>
#include <random>
#include <sstream>

#include "chip8.hpp"
#include "utils.hpp"

void register_operations(const u16 t_opcode, chip8::RegisterMap& t_rm)
{
  using namespace chip8;

  const u8 index_x{(u8)(t_opcode & 0x0F00)};
  const u8 index_y{(u8)(t_opcode & 0x00F0)};

  switch (t_opcode)
	{
  	case Opcode::LD_R: 
	  t_rm[index_x] = t_rm[index_y];
	  break;

	case Opcode::OR_R: 
	  t_rm[index_x] |= t_rm[index_y];
	  break;

	case Opcode::AND_R: 
	  t_rm[index_x] &= t_rm[index_y];
	  break;

	case Opcode::XOR_R: 
	  t_rm[index_x] ^= t_rm[index_y];
	  break;
  
	case Opcode::ADD_R: 
	  t_rm[index_x] += t_rm[index_y];
	  break;
  
	case Opcode::SUB_R: 
	  t_rm[index_x] -= t_rm[index_y];
	  break;

	case Opcode::SHL_R:
	  t_rm[RegisterName::VF] = (t_rm[index_x] & 0xF0) ?
		1 : 0;

	  t_rm[index_x] *= 2;
	  break;
	}
}

void cycle(chip8::Memory& t_memory, chip8::RegisterMap& t_rm, chip8::Display& t_display)
{
  using namespace chip8;

  // const u16 opcode{(u16)(chip8::g_memory.get_opcode() & 0xF000)};
  const u16 opcode{t_memory.get_opcode()};
  const u8 index_x{(u8)(opcode & 0x0F00)};
  const u8 index_y{(u8)(opcode & 0x00F0)};

  // TODO: Every command its own function
  switch((u8)opcode & 0xF000)
	{
	case 0x0:
	  switch (opcode)
		{
		case Opcode::CLS:
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
	  const u16 addr{opcode & 0x0FFF};
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
	  const u8 byte{(u8)(opcode & 0x00FF)};
	  t_rm[index_x] = byte;
	  std::cout << "LD: " << index_x << ", " << t_rm[index_x] << std::endl;
	  break;
	}

	case Opcode::ADD: {
	  const u8 byte{(u8)(opcode & 0x00FF)};
	  t_rm[index_x] += byte;
	  break;
	}

	case 0x8000:
	  register_operations(opcode, t_rm);
	  break;

	case Opcode::SNE:
	  if(t_rm[index_x] != t_rm[index_y])
		t_memory++;
	  break;

	case Opcode::LD_I:
	  t_memory.set_index_register(opcode & 0x0FFF);
	  break;

	case Opcode::JMP_V0:
	  t_memory.jump(t_rm[RegisterName::VF] + opcode & 0x0FFF);
	  break;

	case Opcode::RND: {
	  const u8 bitmask{(u8)(opcode & 0x00FF)};

	  std::mt19937 gen32;
	  const u8 random_byte{(u8)(gen32() & 0x00FF)};

	  t_rm[index_x] = random_byte & bitmask;
	  break;
	}

	case Opcode::DRW: {
	  const u8 sprite_len{(u8)(opcode & 0x000F)};
	  std::vector<u8> sprite{sprite_len};
	  t_memory.copy_nth(sprite.begin(), t_rm[index_x], sprite_len);

	  t_display.write(index_x, index_y, sprite.cbegin(), sprite.cend());
	  break;
	}

	default:
	  std::cerr << "unknown opcode: " << opcode << std::endl;
	  break;
	}
}

int main(int argc, char *argv[])
{
  // Define variables
  chip8::Memory memory{"../roms/INVADERS"};
  memory.start();
  
  chip8::RegisterMap rm;
  chip8::Stack stack;
  chip8::Display display;

  try{
	while(true)
	  {
		cycle(memory, rm, display);

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
