#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>

#include <array>
#include <cstdint>
#include <exception>
#include <iostream>
#include <random>
#include <sstream>

#include "chip8.hpp"
#include "utils.hpp"

// Specifies an operation between two registers
#define R_OP(x, y, OP)							\
	t_rm[x] OP t_rm[y]

void register_operations(const u16 t_opcode, chip8::RegisterMap& t_rm)
{
  using namespace chip8;

  // TOOD: Make a macro for these operations
  const u8 x{(u8)((t_opcode & 0x0F00) >> 8)};
  const u8 y{(u8)((t_opcode & 0x00F0) >> 4)};

  std::cout << "Register operation: " << ((t_opcode & 0xF000) >> 12) << " Rx: " << (u16)t_rm[x] << " Ry: " << (u16)t_rm[y] << ' '; 
  
  switch(t_opcode)
	{
  	case Opcode::LD_R:
	  std::cout << "LD\n";
	  R_OP(x, y, =);
	  break;

	case Opcode::OR_R: 
	  std::cout << "OR\n";
	  R_OP(x, y, |=);
	  break;

	case Opcode::AND_R: 
	  std::cout << "AND\n";
	  R_OP(x, y, &=);
	  break;

	case Opcode::XOR_R: 
	  std::cout << "XOR\n";
	  R_OP(x, y, ^=);
	  break;
  
	case Opcode::ADD_R: 
	  std::cout << "ADD\n";
	  R_OP(x, y, +=);
	  break;
  
	case Opcode::SUB_R: 
	  std::cout << "SUB\n";
	  R_OP(x, y, -=);
	  break;

	case Opcode::SHL_R:
	  std::cout << "SHL\n";
	  t_rm[RegisterName::VF] = (t_rm[x] & 0x80) ?
		1 : 0;

	  t_rm[x] *= 2;
	  break;
	}
}

// Create a CPU class out of this?
void cycle(const u16 t_keys, std::mt19937& t_gen32, chip8::Memory& t_memory, chip8::Stack& t_stack, chip8::RegisterMap& t_rm, chip8::Display& t_display)
{
  using namespace chip8;

  const u16 opcode{t_memory.get_opcode()};
  const u8 x{(u8)((opcode & 0x0F00) >> 8)};
  const u8 y{(u8)((opcode & 0x00F0) >> 4)};
  const u8 byte{(u8)(opcode & 0x00FF)};

  std::cout << t_memory.get_pc() << ": ";

  // TODO: Cut the function down and spread the functionalities
  switch(opcode & 0xF000)
	{
	case 0x0000:
	  switch (opcode)
		{
		case Opcode::CLS:
		  std::cout << "CLS\n";
		  t_display.clear();
		  break;

		case Opcode::RET: {
		  const u16 addr{t_stack.pop()};
		  std::cout << "RET: " << addr << '\n';
		  t_memory.jump(addr);
		  break;
		}

		default:
		  // Runs assembly on host machine (deprecated, functionality)
		  std::cout << "SYS: " << (opcode & 0x0FFF) << '\n';
		  break;
		}
	  break;

	case Opcode::JMP: {
	  std::cout << "JMP: " << (opcode & 0x0FFF) << '\n';
	  const u16 addr{(u16)(opcode & 0x0FFF)};
	  // Memory gets incremented in the loop
	  t_memory.jump(addr - 2);
	  break;
	}

	case Opcode::CALL:
	  std::cout << "CALL: " << (opcode & 0x0FFF) << '\n';
	  t_stack.push(t_memory.get_pc());
	  t_memory.jump(opcode & 0x0FFF);
	  break;

	case Opcode::SE:
	  std::cout << "SE: " << (u16)(t_rm[x]) << " == " << (u16)byte << "\n";
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
	  std::cout << "LD X: " << (int)x << " R: " << (int)t_rm[x] << " byte: " << (int)byte << std::endl;
	  t_rm[x] = byte;
	  break;

	case Opcode::ADD:
	  std::cout << "ADD: " << (u16)t_rm[x] << " += " << (u16)byte << "\n";
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
	  std::cout << "LD_I " << (opcode &0x0FFF) << "\n";
	  t_memory.set_ir(opcode & 0x0FFF);
	  break;

	case Opcode::JMP_V0:
	  std::cout << "JMP_V0" << "\n";
	  t_memory.jump(t_rm[RegisterName::V0] + (opcode & 0x0FFF));
	  break;

	case Opcode::RND: {
	  std::cout << "RND: " << (u16)x;
	  const u8 bitmask{(u8)(opcode & 0x00FF)};

	  // Place it in a different scope you idiot
	  const u8 random_byte{(u8)(t_gen32() & 0x00FF)};

	  t_rm[x] = random_byte & bitmask;
	  std::cout << " = " << (random_byte & bitmask) << " bm: " << (u16)bitmask << " rb: " << (u16)random_byte <<'\n';
	  break;
	}

	case Opcode::DRW: {
	  const u8 sprite_len{(u8)(opcode & 0x000F)};
	  std::cout << "x: " << (u16)x << " y: " << (u16)y << " DRW: " << (u16)sprite_len << std::endl;

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
		  std::cout << "LD_DT_R\n";
		  t_rm[x] = g_delay_timer;
		  break;

		case Opcode::LD_K_R: {
		  std::cout << "LD_K_R\n";
		  int keys{0};
		  while(!(keys = get_keys()))
			sf::sleep(sf::milliseconds(1));

		  for(int index{0}; index < 16; index++)
			  if(keys & (1 << index))
				{
				  t_rm[x] = index;
				  break;
				}
		  break;
		}

		case Opcode::LD_R_DT:
		  std::cout << "LD_R_DT\n";
		  g_delay_timer = t_rm[x];
		  break;

		case Opcode::LD_R_ST:
		  std::cout << "LD_R_ST\n";
		  g_sound_timer = t_rm[x];
		  break;

		case Opcode::ADD_I:
		  std::cout << "ADD_I\n";
		  t_memory.set_ir(t_rm[x]);
		  break;

		case Opcode::LD_F:
		  std::cout << "LD_F\n";
		  break;

		case Opcode::LD_B:
		  std::cout << "LD_B\n";
		  break;

		case Opcode::LD_R_M:
		  std::cout << "LD_R_M\n";
		  break;

		case Opcode::LD_M_R:
		  std::cout << "LD_M_R\n";
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
  chip8::Memory memory{"../roms/PONG"};
  memory.start();
  
  chip8::RegisterMap rm;
  chip8::Stack stack;
  chip8::Display display;

  std::mt19937 gen32{(std::uint_least32_t)time(nullptr)};

  try{
	while(true)
	  {
		const u16 keys{chip8::get_keys()};

		cycle(keys, gen32, memory, stack, rm, display);
		memory++;

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
