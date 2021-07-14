#pragma once

#include <iostream>
#include <cstdint>

//#define DEBUG
#ifdef DEBUG

#define DEBUG_MSG(args)							\
  do{											\
	std::cout << args;							\
  }while(false)

#else

#define DEBUG_MSG(args) \
  do{					\
  }while(false)

#endif

using u8 = uint8_t;
using u16 = uint16_t;
