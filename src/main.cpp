#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>

#include <array>
#include <exception>
#include <iostream>
#include <random>
#include <sstream>

#include "chip8.hpp"
#include "utils.hpp"

void register_operations(const u16 t_opcode, chip8::RegisterMap& t_rm)
{
  using namespace chip8;

  // TOOD: Make a macro for these operations
  const u8 x{(u8)((t_opcode & 0x0F00) >> 8)};
  const u8 y{(u8)((t_opcode & 0x00F0) >> 4)};

  std::cout << "Register operation: " << t_opcode << '\n';
  
  switch(t_opcode)
	{
  	case Opcode::LD_R: 
	  t_rm[x] = t_rm[y];
	  break;

	case Opcode::OR_R: 
	  t_rm[x] |= t_rm[y];
	  break;

	case Opcode::AND_R: 
	  t_rm[x] &= t_rm[y];
	  break;

	case Opcode::XOR_R: 
	  t_rm[x] ^= t_rm[y];
	  break;
  
	case Opcode::ADD_R: 
	  t_rm[x] += t_rm[y];
	  break;
  
	case Opcode::SUB_R: 
	  t_rm[x] -= t_rm[y];
	  break;

	case Opcode::SHL_R:
	  t_rm[RegisterName::VF] = (t_rm[x] & 0x80) ?
		1 : 0;

	  t_rm[x] *= 2;
	  break;
	}
}

// Create a CPU class out of this?
void cycle(const u16 t_keys, chip8::Memory& t_memory, chip8::RegisterMap& t_rm, chip8::Display& t_display)
{
  using namespace chip8;

  const u16 opcode{t_memory.get_opcode()};
  const u8 x{(u8)((opcode & 0x0F00) >> 8)};
  const u8 y{(u8)((opcode & 0x00F0) >> 4)};
  const u8 byte{(u8)(opcode & 0x00FF)};

  // TODO: Cut the function down and spread the functionalities
  switch(opcode & 0xF000)
	{
	case 0x0000:
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
	  const u16 addr{(u16)(opcode & 0x0FFF)};
	  t_memory.jump(addr);
	  break;
	}

	case Opcode::CALL:
	  // PC = 0nnn, pop current location to stack
	  break;

	case Opcode::SE:
	  std::cout << "SE: " << byte << "\n";
	  if(t_rm[x] == byte)
		t_memory++;
	  break;

	case Opcode::SNE:
	  std::cout << "SNE" << "\n";
	  if(t_rm[x] != byte)
		t_memory++;
	  break;

	case Opcode::SE_R:
	  std::cout << "SNE_R" << "\n";
	  if(t_rm[x] == t_rm[y])
		t_memory++;
	  break;

	case Opcode::LD: 
	  t_rm[x] = byte;
	  std::cout << "LD: " << (int)x << ", " << (int)t_rm[x] << " byte: " << (int)byte << std::endl;
	  break;

	case Opcode::ADD:
	  std::cout << "ADD" << "\n";
	  t_rm[x] += byte;
	  break;

	case 0x8000:
	  register_operations(opcode, t_rm);
	  break;

	case Opcode::SNE_R:
	  std::cout << "SNE_R" << "\n";
	  if(t_rm[x] != t_rm[y])
		t_memory++;
	  break;

	case Opcode::LD_I:
	  std::cout << "LD_I" << "\n";
	  t_memory.set_ir(opcode & 0x0FFF);
	  break;

	case Opcode::JMP_V0:
	  std::cout << "JMP_V0" << "\n";
	  t_memory.jump(t_rm[RegisterName::V0] + (opcode & 0x0FFF));
	  break;

	case Opcode::RND: {
	  std::cout << "RND" << "\n";
	  const u8 bitmask{(u8)(opcode & 0x00FF)};

	  std::mt19937 gen32;
	  const u8 random_byte{(u8)(gen32() & 0x00FF)};

	  t_rm[x] = random_byte & bitmask;
	  break;
	}

	case Opcode::DRW: {
	  const u8 sprite_len{(u8)(opcode & 0x000F)};

	  std::cout << "x: " << x << " y: " << y << " DRW: " << sprite_len << std::endl;

	  std::vector<u8> sprite{sprite_len};
	  t_memory.copy_nth(sprite.begin(), t_rm[x], sprite_len);
	  
	  t_display.write(x, y, sprite.cbegin(), sprite.cend());
	  break;
	}

	case 0xE000:
	  switch (opcode & 0xF0FF)
		{
		case Opcode::SKP:
		  std::cout << "SKP\n";
		  if(t_keys >> (t_rm[x] - 1))
			t_memory++;
		  break;

		case Opcode::SKNP:
		  std::cout << "SKNP\n";
		  if(!(t_keys >> (t_rm[x] - 1)))
			t_memory++;
		  break;
	  }
	  break;

	case 0xF000:
	  switch(opcode)
		{
		case Opcode::LD_DT_R:
		  break;

		case Opcode::LD_K_R:
		  break;

		case Opcode::LD_R_DT:
		  break;

		case Opcode::LD_R_ST:
		  break;

		case Opcode::ADD_I:
		  t_memory.set_ir(t_rm[x]);
		  break;

		case Opcode::LD_R_M:
		  break;

		case Opcode::LD_M_R:
		  break;
		}
	  break;

	default:
	  std::cerr << "unknown opcode: " << ((opcode & 0xF000) >> 12) << std::endl;
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
		const u16 keys{chip8::get_keys()};

		cycle(keys, memory, rm, display);
		memory++;

		display.print();

		// Delay to not run on full cpu
		sf::sleep(sf::milliseconds(1));
	  }
  }catch(std::exception& e)
	{
	  std::cout << e.what() << std::endl;
	}

  return 0;
}
