#ifndef TRICKGENERATOR_H
#define TRICKGENERATOR_H

#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>

/* -------- Constant -------- */

#define DURATION 1000
#define INTERVAL 20
#define DEFAULT_VALUE_MIN -5
#define DEFAULT_VALUE_MAX 5
#define DEFAULT_VALUE (DEFAULT_VALUE_MIN + (rand() % (int)(DEFAULT_VALUE_MAX - DEFAULT_VALUE_MIN + 1)))

class TrickGenerator
{
	public:
		TrickGenerator();
		TrickGenerator(unsigned int duration, unsigned int interval);
		std::vector<int> run(int input);

	private:
		unsigned int duration;
		unsigned int interval;
};

#endif // TRICKGENERATOR_H
