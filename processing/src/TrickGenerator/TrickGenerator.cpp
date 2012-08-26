#include "TrickGenerator.h"

/* -------- Public -------- */

TrickGenerator :: TrickGenerator()
  : duration(DURATION), interval(INTERVAL)
{
  // Initialize random number generator
  srand(static_cast<unsigned int>(time(0)));
}

/* ---------------- */

TrickGenerator :: TrickGenerator(unsigned int duration, unsigned int interval)
  : duration(duration), interval(interval)
{
  // Initialize random number generator
  srand(static_cast<unsigned int>(time(0)));
}

/* ---------------- */

std::vector<int> TrickGenerator :: run(int input)
{
  std::vector<int> result;

  double step = input / static_cast<double>(DURATION / INTERVAL);

  for (int i = 0; i < DURATION / INTERVAL; i++)
  {
    if (input == 0)
    {
			result.push_back(DEFAULT_VALUE);
    }
    else
    {
      // -180
      if (static_cast<int>((step * i) / 180) % 2)
        if (input > 0)
          result.push_back(-180 + (static_cast<int>(step * i) % 180));
        else
          result.push_back(180 + (static_cast<int>(step * i) % 180));
      // 180
      else
        result.push_back(static_cast<int>(step * i) % 180);
    }
  }

  return result;
}
