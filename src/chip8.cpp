#include "chip8.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <algorithm>

// Variable initialization
u8 chip8::m_delay_timer{0};
u8 chip8::m_sound_timer{0};

// Define variables
Memory chip8::g_memory;
RegisterMap chip8::g_register_map;
Stack chip8::g_stack;
Display chip8::g_display;
