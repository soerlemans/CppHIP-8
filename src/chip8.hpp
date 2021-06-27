#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Graphics.hpp>

#include "memory.hpp"
#include "registermap.hpp"
#include "stack.hpp"
#include "display.hpp"

// Namespace for the globals
namespace chip8
{
  extern u8 m_delay_timer;
  extern u8 m_sound_timer;
}
