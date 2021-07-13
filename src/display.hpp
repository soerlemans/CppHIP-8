#pragma once

#include <array>

#include <SFML/Graphics.hpp>

#include "utils.hpp"

namespace chip8
{
enum Color : u8 {
  Black = 0,
  // Change this to one if switching to bit display
  White = 255
};

class Display {
private:
  // Can define and declare static member on same line?
  // Is this cause C++20? Cause this rocks
  static constexpr u8 m_width{64};
  static constexpr u8 m_height{32};

  // TOOD: Maybe make the size 8 * 4 and just set the bits
  // Since the display is only black and white
  std::array<u8, m_width * m_height> m_display;
  
  sf::RenderWindow m_window;
	
public:
  Display();
  
  void clear();

  template <typename Begin, typename End>
  bool draw_sprite(const u8 t_x, const u8 t_y, Begin t_begin, End t_end)
  {
	bool erased{false};
	// Draw the sprite starting on the specified coordinates
	const int start_coordinate{t_x + t_y * m_width};

	for(int index_y{0}; t_begin < t_end; t_begin++, index_y++)
	  { // Sprites are 8 pixels wide
		for(int index_x{0}; index_x < 8; index_x++)
		  {
			u16 coordinate{(u16)(start_coordinate + index_x + index_y * m_width)};

			if(coordinate > m_display.size())
			  return erased;
			
			u8& pixel{m_display[coordinate]};

			// The sprites are stored as bits 
			const bool sprite_bit{*t_begin << index_x & 0x80};

			if(pixel && sprite_bit)
			  erased = true;

			pixel ^= sprite_bit;
		  }
	  }

	return erased;
  }

  void print();
};

}
