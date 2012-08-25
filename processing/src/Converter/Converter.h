#ifndef CONVERTER_H
#define CONVERTER_H

#include <sstream>

/* ---------------- */

template <typename T>
T btoi(const unsigned char* bytes)
{
  T result = 0;
  unsigned short int a=0x1234;

  if (*((unsigned char*) &a)==0x12) // big endian
    for (unsigned int n = 0; n < sizeof(result); n++)
      result = (result << 8) + bytes[n];
  else // little endian
    for (int n = sizeof(result); n >= 0; n--)
      result = (result << 8) + bytes[n];

  return result;
}

/* ---------------- */

template <typename T>
std::string ntos(T number)
{
  std::ostringstream ss;
  ss << number;
  return ss.str();
}

/* ---------------- */

template <typename T>
T ston(const std::string text)
{
  std::istringstream ss(text);
  T result;
  return ss >> result ? result : 0;
}

#endif // CONVERTER_H
