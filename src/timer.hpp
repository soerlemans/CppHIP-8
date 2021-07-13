#pragma once

#include <chrono>

template <typename T, typename R>
using duration = std::chrono::duration<T, R>;
using time_point = std::chrono::time_point<std::chrono::system_clock>;

class Timer {
private:
  duration<long, std::milli> m_duration;
  time_point m_start;

public:
  Timer(duration<long, std::milli> t_duration);
  bool check();
};
