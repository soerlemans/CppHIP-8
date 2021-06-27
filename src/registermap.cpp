#include "registermap.hpp"
#include "utils.hpp"

using namespace chip8;

// RegisterMap:
RegisterMap::RegisterMap()
{
  m_registers.fill(0);
}

u8& RegisterMap::operator[](const u8 t_index)
{
  // Add bounds checking ??
  return m_registers[t_index];
}
