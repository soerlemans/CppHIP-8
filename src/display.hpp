#pragma once

#include <array>

#include <SFML/Graphics.hpp>

#include "utils.hpp"

enum Color : u8 {
  Black = 0,
  // Change this to one if switching to bit display
  White = 255
};

class Display {
private:
  // TOOD: Maybe make the size 8 * 4 and just set the bits
  // Since the display is only black and white
  std::array<u8, 64 * 32> m_display;
  
  sf::RenderWindow m_window;
	
public:
  Display();

  void print();
};

