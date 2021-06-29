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

  // TOOD: Maybe make the size 8 * 4 and just set the bits
  // Since the display is only black and white
  std::array<u8, m_width * 32> m_display;
  
  sf::RenderWindow m_window;
	
public:
  Display();
  
  void clear();

  template<typename Begin, typename End>
  void write(const u8 t_x, const u8 t_y, Begin t_begin, End t_end);

  void print();
};

}
