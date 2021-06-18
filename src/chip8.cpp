#include "chip8.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <algorithm>

using namespace chip8;

// Variable initialization
u8 m_delay_timer{0};
u8 m_sound_timer{0};

// Define variables
Memory g_memory;
RegisterMap g_register_map;
Stack g_stack;
Display g_display;
