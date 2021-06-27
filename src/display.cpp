#include "display.hpp"

#include <SFML/System/Vector2.hpp>

using namespace chip8;

// Graphics
Display::Display()
  :m_window{sf::VideoMode(640, 320), "CppHIP-8"}
{
  m_display.fill(0);

  m_window.setPosition(sf::Vector2<int>{0, 0});

  m_window.clear(sf::Color::Black);
  m_window.display();
}

void Display::clear()
{
  m_window.clear();
}

void Display::print() const
{
  //
}
