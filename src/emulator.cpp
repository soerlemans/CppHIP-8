#include "emulator.hpp"

#include "memory.hpp"
#include "registermap.hpp"
#include "stack.hpp"
#include "display.hpp"

#include "keys.hpp"
#include "utils.hpp"
#include <cstdint>

// Specifies an operation between two registers
#define R_OP(x, y, OP)							\
  rm[x] OP rm[y]

namespace chip8
{
  Emulator::Emulator(Memory *t_memory, RegisterMap *t_rm,
					 Stack* t_stack, Display *t_display)
	:m_memory{t_memory}, m_rm{t_rm}, m_stack{t_stack}, m_display{t_display},
	 m_keys{0}, m_jumped{false}, m_gen32{(uint_least32_t)time(0)},
	 m_delay_timer{0}, m_sound_timer{0}
  {
  }

  void Emulator::timers()
  {
	if(m_delay_timer)
	  m_delay_timer--;

	if(m_sound_timer)
	  {
		m_sound_timer--;
	  }
  }

  // Remove all the std::cout's or lock them behind a debug mode
  void Emulator::register_operations()
  {
	// TODO: Make a macro for these operations
	const u16 opcode{m_memory->get_opcode()};
	const u8 x{(u8)((opcode & 0x0F00) >> 8)};
	const u8 y{(u8)((opcode & 0x00F0) >> 4)};

	auto& rm{*m_rm};
	
	switch(opcode & 0xF00F)
	  {
	  case Opcode::LD_R:
		std::cout << "LD";
		R_OP(x, y, =);
		break;
		
	  case Opcode::OR_R: 
		std::cout << "OR";
		R_OP(x, y, |=);
		break;
		
	  case Opcode::AND_R: 
		std::cout << "AND";
		R_OP(x, y, &=);
		break;
		
	  case Opcode::XOR_R: 
		std::cout << "XOR";
		R_OP(x, y, ^=);
		break;
		
	  case Opcode::ADD_R: 
		std::cout << "ADD";
		// Account for integer overflow
		if(rm[x] + rm[y] > 255)
		  rm[RegisterName::VF] = 1;

		R_OP(x, y, +=);
		break;
		
	  case Opcode::SUB_R: 
		std::cout << "SUB";
		// Not complete
		if(rm[x] < rm[y])
		  rm[RegisterName::VF] = 1;

		R_OP(x, y, -=);
		break;

	  case Opcode::SHR_R:
		// Shift right
		std::cout << "SHR";
		// Not complete
		rm[RegisterName::VF] = (rm[x] & 0x01) ?
		  1 : 0;
		
		rm[x] /= 2;
		break;

	  case Opcode::SUBN_R:
		std::cout << "SUBN";
		// Not complete
		if(rm[y] < rm[x])
		  rm[RegisterName::VF] = 1;

		rm[x] = rm[y] - rm[x];
		break;

	  case Opcode::SHL_R:
		// Shift left
		std::cout << "SHL";
		// Not complete
		rm[RegisterName::VF] = (rm[x] & 0x80) ?
		  1 : 0;
		
		rm[x] *= 2;
		break;
	  }

	std::cout << ": " << std::hex << "x: " << (u16)x << " y: " << (u16)y << std::dec << " Rx: " << (u16)rm[x] << " Ry: " << (u16)rm[y];
  }
  
  void Emulator::cycle()
  {
	auto& memory{*m_memory};
	auto& rm{*m_rm};
	auto& stack{*m_stack};
	auto& display{*m_display};
	
	const u16 opcode{memory.get_opcode()};
	const u8 x{(u8)((opcode & 0x0F00) >> 8)};
	const u8 y{(u8)((opcode & 0x00F0) >> 4)};
	const u8 byte{(u8)(opcode & 0x00FF)};
	
	// Update the keys every cycle
	m_keys = get_keys();
	
	std::cout << memory.get_pc() << 'x' << std::hex << opcode << std::dec << ": ";
	
	// TODO: Cut the function down and spread the functionalities
	switch(opcode & 0xF000)
	  {
	  case 0x0000:
		switch(opcode & 0x00FF)
		  {
		  case Opcode::CLS:
			std::cout << "CLS";
			display.clear();
			break;
			
		  case Opcode::RET: {
			const u16 addr{stack.pop()};
			std::cout << "RET: " << addr;
			memory.jump(addr);
			break;
		  }
			
		  default:
			// Runs assembly on host machine (deprecated, functionality)
			std::cout << "SYS: " << (opcode & 0x0FFF);
			break;
		  }
		break;
		
	  case Opcode::JMP: {
		std::cout << "JMP: " << (opcode & 0x0FFF);
		const u16 addr{(u16)(opcode & 0x0FFF)};
		memory.jump(addr);

		m_jumped = true;
		break;
	  }
		
	  case Opcode::CALL:
		std::cout << "CALL: " << (opcode & 0x0FFF);
		stack.push(memory.get_pc());
		memory.jump(opcode & 0x0FFF);

		m_jumped = true;
		break;
		
	  case Opcode::SE:
		std::cout << "SE: " << (u16)rm[x] << " == " << (u16)byte;
		if(rm[x] == byte)
		  memory++;
		break;
		
	  case Opcode::SNE:
		std::cout << "SNE: " << (u16)rm[x] << " != " << (u16)byte;
		if(rm[x] != byte)
		  memory++;
		break;
		
	  case Opcode::SE_R:
		std::cout << "SE_R: " << (u16)rm[x] << " == " << (u16)rm[y];
		if(rm[x] == rm[y])
		  memory++;
		break;
		
	  case Opcode::LD: 
		std::cout << "LD: x: " << std::hex << (int)x << std::dec << " byte: " << (int)byte;
		rm[x] = byte;
		break;
		
	  case Opcode::ADD:
		std::cout << "ADD: " << (u16)rm[x] << " += " << (u16)byte;
		rm[x] += byte;
		break;
		
	  case 0x8000:
		register_operations();
		break;
		
	  case Opcode::SNE_R:
		std::cout << "SNE_R: " << (u16)rm[x] << " != " << (u16)rm[y];
		if(rm[x] != rm[y])
		  memory++;
		break;
		
	  case Opcode::LD_I:
		std::cout << "LD_I: " << (opcode &0x0FFF);
		memory.set_ir(opcode & 0x0FFF);
		break;
		
	  case Opcode::JMP_V0:
		std::cout << "JMP_V0: " << (rm[RegisterName::V0] + (opcode & 0x0FFF));
		memory.jump(rm[RegisterName::V0] + (opcode & 0x0FFF));

		m_jumped = true;
		break;
		
	  case Opcode::RND: {
		std::cout << "RND: ";
		const u8 bm{(u8)(opcode & 0x00FF)};
		const u8 rb{(u8)(m_gen32() & 0x00FF)};
		
		std::cout << std::hex << (u16)x << std::dec << " = " << (rb & bm); 
		rm[x] = rb & bm;
		break;
	  }
		
	  case Opcode::DRW: {
		const u8 sprite_len{(u8)(opcode & 0x000F)};
		std::cout << "DRW: " << " I: " << memory.get_ir() << " x: " << (u16)rm[x] << " y: " << (u16)rm[y] << " n: " << (u16)sprite_len;

		std::vector<u8> sprite{sprite_len};
		sprite.resize(sprite_len);
		memory.copy_nth(memory.get_ir(), sprite_len, sprite.begin());

		rm[RegisterName::VF] = 0;
		rm[RegisterName::VF] = display.draw_sprite(rm[x], rm[y], sprite.cbegin(), sprite.cend());
		break;
	  }
		
	  case 0xE000:
		switch (opcode & 0xF0FF)
		  {
		  case Opcode::SKP:
			std::cout << "SKP: " << (u16)rm[x];
			if(IS_PRESSED(m_keys, rm[x]))
			  memory++;
			break;
			
		  case Opcode::SKNP:
			std::cout << "SKNP: " << (u16)rm[x];
			if(IS_NOT_PRESSED(m_keys, rm[x]))
			  memory++;
			break;
		  }
		break;
		
	  case 0xF000:
		switch(opcode & 0xF0FF)
		  {
		  case Opcode::LD_DT_R:
			std::cout << "LD_DT_R: " << (u16)rm[x];
			rm[x] = m_delay_timer;
			break;
			
		  case Opcode::LD_K_R: {
			std::cout << "LD_K_R: ";
			while(!(m_keys = get_keys()))
			  sf::sleep(sf::milliseconds(1));
			
			for(int index{0}; index < 16; index++)
			  if(m_keys & (1 << index))
				{
				  rm[x] = index;
				  break;
				}
			}
			break;
			
		  case Opcode::LD_R_DT:
			std::cout << "LD_R_DT: " << (u16)rm[x];
			m_delay_timer = rm[x];
			break;
			
		  case Opcode::LD_R_ST:
			std::cout << "LD_R_ST: " << (u16)rm[x];
			m_sound_timer = rm[x];
			break;
			
		  case Opcode::ADD_I:
			std::cout << "ADD_I: " << memory.get_ir() << " += " << (u16)rm[x]; 
			memory.inc_ir(rm[x]);
			break;
			
		  case Opcode::LD_F:
			std::cout << "LD_F: ";
			break;
			
		  case Opcode::LD_B:
			std::cout << "LD_B: ";
			break;
			
		  case Opcode::LD_R_M:
			std::cout << "LD_R_M: ";
			break;
			
		  case Opcode::LD_M_R:
			std::cout << "LD_M_R: ";
			break;
		  }
		break;
		
	  default:
		std::cerr << "unknown opcode: " << ((opcode & 0xF000) >> 12);
		break;
	  }

	std::cout << '\n';

	if(!m_jumped)
	  memory++;

	m_jumped = false;
  }
}
