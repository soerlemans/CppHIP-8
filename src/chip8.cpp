#include "chip8.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <algorithm>
#include <array>

namespace chip8
{
  // Variable initialization
  u8 g_delay_timer{0};
  u8 g_sound_timer{0};


  u16 get_keys()
  {
	using namespace sf;

	u16 keys{0};
	if(Keyboard::isKeyPressed(Keyboard::Num0))
	  keys |= Num0;
	  
	if(Keyboard::isKeyPressed(Keyboard::Num1))
	  keys |= Num1;

	if(Keyboard::isKeyPressed(Keyboard::Num2))
	  keys |= Num2;

	if(Keyboard::isKeyPressed(Keyboard::Num3))
	  keys |= Num3;

	if(Keyboard::isKeyPressed(Keyboard::Num4))
	  keys |= Num4;

	if(Keyboard::isKeyPressed(Keyboard::Num5))
	  keys |= Num5;

	if(Keyboard::isKeyPressed(Keyboard::Num6))
	  keys |= Num6;

	if(Keyboard::isKeyPressed(Keyboard::Num7))
	  keys |= Num7;

	if(Keyboard::isKeyPressed(Keyboard::Num8))
	  keys |= Num8;

	if(Keyboard::isKeyPressed(Keyboard::Num9))
	  keys |= Num9;

	if(Keyboard::isKeyPressed(Keyboard::A))
	  keys |= A;
	
	if(Keyboard::isKeyPressed(Keyboard::B))
	  keys |= B;
	
	if(Keyboard::isKeyPressed(Keyboard::C))
	  keys |= C;
	
	if(Keyboard::isKeyPressed(Keyboard::D))
	  keys |= D;
	
	if(Keyboard::isKeyPressed(Keyboard::E))
	  keys |= E;
	
	if(Keyboard::isKeyPressed(Keyboard::F))
	  keys |= F;

 	return keys;
  }
}
