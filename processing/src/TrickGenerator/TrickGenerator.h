#ifndef TRICKGENERATOR_H
#define TRICKGENERATOR_H

#include <iostream>
#include <vector>

#define DURATION 1000
#define INTERVAL 20

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
