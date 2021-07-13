#include "registermap.hpp"
#include "utils.hpp"

using namespace chip8;

// RegisterMap:
RegisterMap::RegisterMap()
{
  m_registers.fill(0);
}

std::array<u8, 16>::const_iterator RegisterMap::begin() const
{
  return m_registers.begin();
}
std::array<u8, 16>::const_iterator RegisterMap::end() const
{
  return m_registers.end(); 
}

u8& RegisterMap::operator[](const u8 t_index)
{
  // Add bounds checking ??
  return m_registers[t_index];
}
