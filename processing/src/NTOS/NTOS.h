#ifndef NTOS_H
#define NTOS_H

#include <sstream>

template <typename T>
std::string ntos(T number)
{
  std::ostringstream ss;
  ss << number;
  return ss.str();
}

#endif // NTOS_H
