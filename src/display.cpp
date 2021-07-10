#include "display.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
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
  m_display.fill(0);
  m_window.clear(sf::Color::Black);
}

void Display::print()
{
  for(int x{0}; x < m_width; x++)
	for(int y{0}; y < m_height; y++)
	  {
		sf::RectangleShape rect{sf::Vector2f{10, 10}};
		rect.setFillColor(sf::Color::White);
		rect.move(x * 10, y * 10);

		if(m_display[x + m_width* y])
		  m_window.draw(rect);
	  }

  m_window.display();
}
