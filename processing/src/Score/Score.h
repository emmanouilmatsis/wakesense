#ifndef SCORE_H
#define SCORE_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include "../TrickDatabase/TrickDatabase.h"

class Score
{
	public:
		Score();
		void run(std::string name, double correlation, std::vector<std::vector<int> > data);
		double getExecution();
		double getIntensity();
		double getComposition();
		double getTotal();

	private:
		TrickDatabase trickDatabase;
		double execution;
		double intensity;
		double composition;
		double total;
};

#endif // SCORE_H
