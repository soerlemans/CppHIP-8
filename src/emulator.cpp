#include "emulator.hpp"

#include "memory.hpp"
#include "registermap.hpp"
#include "stack.hpp"
#include "display.hpp"

#include "keys.hpp"

// Specifies an operation between two registers
#define R_OP(x, y, OP)							\
  rm[x] OP rm[y]

namespace chip8
{
  
  // Variable initialization
  u8 g_delay_timer{0};
  u8 g_sound_timer{0};
  
  Emulator::Emulator(Memory *t_memory, RegisterMap *t_rm,
					 Stack* t_stack, Display *t_display)
	:m_memory{t_memory}, m_rm{t_rm}, m_stack{t_stack}, m_display{t_display},
	 m_keys{0}, m_jumped{false}
  {
  }
  
  void Emulator::next_instruction()
  {
	auto& memory{*m_memory};

	// Dont increment after a jump
	if(!m_jumped)
	  memory++;

	m_jumped = false;
  }
  
  void Emulator::register_operations()
  {
	// TODO: Make a macro for these operations
	const u16 opcode{m_memory->get_opcode()};
	const u8 x{(u8)((opcode & 0x0F00) >> 8)};
	const u8 y{(u8)((opcode & 0x00F0) >> 4)};
	
	auto& rm{*m_rm};
	
	std::cout << "Register operation: " << ((opcode & 0xF000) >> 12) << " Rx: " << (u16)rm[x] << " Ry: " << (u16)rm[y] << ' '; 
	
	switch(opcode)
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
		rm[RegisterName::VF] = (rm[x] & 0x80) ?
		  1 : 0;
		
		rm[x] *= 2;
		break;
	  }
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
	
	std::cout << memory.get_pc() << ": ";
	
	// TODO: Cut the function down and spread the functionalities
	switch(opcode & 0xF000)
	  {
	  case 0x0000:
		switch (opcode)
		  {
		  case Opcode::CLS:
			std::cout << "CLS\n";
			display.clear();
			break;
			
		  case Opcode::RET: {
			const u16 addr{stack.pop()};
			std::cout << "RET: " << addr << '\n';
			memory.jump(addr);
			m_jumped = true;
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
		memory.jump(addr);
		m_jumped = true;
		break;
	  }
		
	  case Opcode::CALL:
		std::cout << "CALL: " << (opcode & 0x0FFF) << '\n';
		stack.push(m_memory->get_pc());
		memory.jump(opcode & 0x0FFF);
		m_jumped = true;
		break;
		
	  case Opcode::SE:
		std::cout << "SE: " << (u16)rm[x] << " == " << (u16)byte << "\n";
		if(rm[x] == byte)
		  m_memory++;
		break;
		
	  case Opcode::SNE:
		std::cout << "SNE" << "\n";
		if(rm[x] != byte)
		  memory++;
		break;
		
	  case Opcode::SE_R:
		std::cout << "SNE_R" << "\n";
		if(rm[x] == rm[y])
		  memory++;
		break;
		
	  case Opcode::LD: 
		std::cout << "LD X: " << (int)x << " R: " << (int)rm[x] << " byte: " << (int)byte << std::endl;
		rm[x] = byte;
		break;
		
	  case Opcode::ADD:
		std::cout << "ADD: " << (u16)rm[x] << " += " << (u16)byte << "\n";
		rm[x] += byte;
		break;
		
	  case 0x8000:
		register_operations();
		break;
		
	  case Opcode::SNE_R:
		std::cout << "SNE_R" << "\n";
		if(rm[x] != rm[y])
		  memory++;
		break;
		
	  case Opcode::LD_I:
		std::cout << "LD_I " << (opcode &0x0FFF) << "\n";
		memory.set_ir(opcode & 0x0FFF);
		break;
		
	  case Opcode::JMP_V0:
		std::cout << "JMP_V0" << "\n";
		memory.jump(rm[RegisterName::V0] + (opcode & 0x0FFF));
		m_jumped = true;
		break;
		
	  case Opcode::RND: {
		std::cout << "RND: " << (u16)x;
		const u8 bm{(u8)(opcode & 0x00FF)};
		const u8 rb{(u8)(m_gen32() & 0x00FF)};
		
		rm[x] = rb& bm;
		std::cout << " = " << (rb & bm) << " bm: " << (u16)bm << " rb: " << (u16)rb <<'\n';
		break;
	  }
		
	  case Opcode::DRW: {
		const u8 sprite_len{(u8)(opcode & 0x000F)};
		std::cout << "x: " << (u16)x << " y: " << (u16)y << " DRW: " << (u16)sprite_len << std::endl;
		
		std::vector<u8> sprite{sprite_len};
		memory.copy_nth(sprite.begin(), rm[x], sprite_len);
		
		display.write(x, y, sprite.cbegin(), sprite.cend());
		break;
	  }
		
	  case 0xE000:
		switch (opcode & 0xF0FF)
		  {
		  case Opcode::SKP:
			std::cout << "SKP\n";
			if(m_keys >> (rm[x] - 1))
			  memory++;
			break;
			
		  case Opcode::SKNP:
			std::cout << "SKNP\n";
			if(!(m_keys >> (rm[x] - 1)))
			  memory++;
			break;
		  }
		break;
		
	  case 0xF000:
		switch(opcode)
		  {
		  case Opcode::LD_DT_R:
			std::cout << "LD_DT_R\n";
			rm[x] = g_delay_timer;
			break;
			
		  case Opcode::LD_K_R: {
			std::cout << "LD_K_R\n";
			while(!(m_keys = get_keys()))
			  sf::sleep(sf::milliseconds(1));
			
			for(int index{0}; index < 16; index++)
			  if(m_keys & (1 << index))
				{
				  rm[x] = index;
				  break;
				}
			break;
		  }
			
		  case Opcode::LD_R_DT:
			std::cout << "LD_R_DT\n";
			g_delay_timer = rm[x];
			break;
			
		  case Opcode::LD_R_ST:
			std::cout << "LD_R_ST\n";
			g_sound_timer = rm[x];
			break;
			
		  case Opcode::ADD_I:
			std::cout << "ADD_I\n";
			memory.set_ir(rm[x]);
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
	next_instruction();
  }
}
