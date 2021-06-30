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
  bool write(const u8 t_x, const u8 t_y, Begin t_begin, End t_end)
  {
	bool erased{false};
	const int start{t_x + t_y * m_width};
	for(int index{0}; t_begin < t_end; t_begin++, index++)
	  {
		u8& pixel{m_display[start + index]};
		if(pixel && * t_begin)
		  erased = true;
		
		pixel ^= *t_begin;
	  }

	return erased;
  }

  void print();
};

}
