#include "Visualizer.h"

/* -------- Public -------- */

Visualizer :: Visualizer()
{
  //Initialize graphics window
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
  glutInitWindowSize (512, 512); 
  glutInitWindowPosition (0, 0);
  glutCreateWindow ("Visualization");

  //Initialize OpenGL
  init();

  //Initialize callback functions
  glutDisplayFunc(display); 
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  //Start rendering 
  glutMainLoop();
}

/* ---------------- */

void Visualizer :: run()
{
}

/* -------- Private -------- */

void Visualizer :: init()
{
}

/* ---------------- */

void Visualizer :: display()
{
}

/* ---------------- */

void Visualizer :: reshape()
{
  glViewport (0, 0, (GLsizei)w, (GLsizei)h); 
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(50, (GLfloat)w/(GLfloat)h, 1.0f, 100.0f);
  glMatrixMode (GL_MODELVIEW);
}

/* ---------------- */

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 27: // ESC
    exit(0);
    break;
  }
}
