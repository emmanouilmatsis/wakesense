#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <cmath>
#include <string>
#include <vector>
#include <stdlib.h>
#include <GLUT/glut.h>
#include "../TrickDatabase/TrickDatabase.h"

#define FRAME_RATE 1000/26

class Visualizer          
{
	public:
		static int time;
	 	static std::vector<std::vector<int> > inputData;
		static std::vector<std::vector<int> > baseData;
		static int dataRow;

		Visualizer(int argc, char** argv);
    void run(std::string filename, std::string name, std::vector<std::vector<int> >inputData);

	private:
		
		static void init();
		static void idle();
		static void display();
		static void reshape(int h, int w);
		static void keyboard(unsigned char key, int x, int y);
		static void draw();
};

#endif // VISUALIZER_H
