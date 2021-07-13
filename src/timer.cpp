#include "timer.hpp"
#include <chrono>

using system_clock = std::chrono::system_clock;

Timer::Timer(duration<long, std::milli> t_duration)
  :m_duration(t_duration)
{
  m_start = system_clock::now();
}

bool Timer::check()
{
  time_point now{system_clock::now()};

  auto cur_duration{now - m_start};
  if(cur_duration.count() > m_duration.count())
	{
	  m_start = system_clock::now();
	  return true;
	}

  return false;
}
