#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <GL/glut.h>

class Visualizer
{
	public:
		Visualizer();
		void run();

	private:
		void init();
		void display();
		void reshape();
};

#endif // VISUALIZER_H
