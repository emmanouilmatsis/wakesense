#ifndef SCORE_H
#define SCORE_H

#include <ostream>
#include <string>
#include <vector>
#include <map>
#include "../TrickDatabase/TrickDatabase.h"

class Score
{
	public:
		Score(std::string filename);
		void run(std::string name, double correlation, std::vector<std::vector<int> > data);
		friend std::ostream& operator <<(std::ostream& out, const Score& object);

	private:
		TrickDatabase trickDatabase;
		std::vector<Trick> trick;
		std::string division;
		double execution;
		double intensity;
		double composition;
		double total;

		void runDivision();
		void runExecution(double correlation, std::vector<std::vector<int> > data);
		void runIntensity();
		void runComposition();
		void runTotal();
};

#endif // SCORE_H
