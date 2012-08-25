#ifndef STON_H
#define STON_H

#include <sstream>

template <typename T>
T ston(const std::string text)
{
  std::istringstream ss(text);
  T result;
  return ss >> result ? result : 0;
}

#endif // STON_H
