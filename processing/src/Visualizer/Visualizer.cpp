#include "Visualizer.h"

/* -------- Public -------- */

Visualizer :: Visualizer(int argc, char** argv)
{
  //Initialize graphics window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1440, 415);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("WakeSense");

  //Initialize OpenGL
  init();

  //Initialize callback functions
  glutIdleFunc(idle);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
}

/* ---------------- */

void Visualizer::run(std::string filename, std::string name, std::vector<std::vector<int> >inputData)
{
  // Initialize
	Visualizer::inputData = inputData;

	TrickDatabase trickDatabase(filename);
  Trick trick = trickDatabase.get(name);

  for (unsigned int i = 0; i < trick.yaw.size(); i++)
  {
    Visualizer::baseData.push_back(std::vector<int>(3));
    Visualizer::baseData[i][0] = (trick.yaw[i]);
    Visualizer::baseData[i][1] = (trick.pitch[i]);
    Visualizer::baseData[i][2] = (trick.roll[i]);
  }

	Visualizer::dataRow = 0;
	Visualizer::time = 0;

  //Start rendering
  glutMainLoop();
}

/* -------- Private -------- */

void Visualizer :: init()
{
  glEnable(GL_DEPTH_TEST);
}

/* ---------------- */

void Visualizer :: idle()
{
  // Framerate
  if (glutGet(GLUT_ELAPSED_TIME) - time > FRAME_RATE)
  {
    time = glutGet(GLUT_ELAPSED_TIME);

    // Data
    if (++dataRow >= 50)
      dataRow = 0;

    glutPostRedisplay();
  }
}

/* ---------------- */

void Visualizer :: display()
{
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -5.0f);
  Visualizer::draw();
  glutSwapBuffers();
}

/* ---------------- */
void Visualizer :: reshape(int h, int w)
{
  glViewport(0, 0, (GLsizei)h, (GLsizei)w);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30, (GLfloat)h/(GLfloat)w, 1.0f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
}

/* ---------------- */

void Visualizer :: keyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
  case 27: // ESC
    exit(0);
    break;
  }
}

/* ---------------- */

void Visualizer :: draw()
{
	float x, y, r = 1.0;

	/* Input */

  glPushMatrix();

  // Data
  int inputYaw = (inputData[dataRow][0] < 0) ? (180 + (180 + inputData[dataRow][0])) : inputData[dataRow][0];
  int inputPitch = (inputData[dataRow][1] < 0) ? (180 + (180 + inputData[dataRow][1])) : inputData[dataRow][1];
  int inputRoll = (inputData[dataRow][2] < 0) ? (180 + (180 + inputData[dataRow][2])) : inputData[dataRow][2];

  // Transform
  glTranslatef(-2.0f, 0.0f, 0.0f);
  glRotatef(inputYaw, 0.0f, 1.0f, 0.0f);
  glRotatef(inputPitch, 0.0f, 0.0f, 1.0f);
  glRotatef(inputRoll, 1.0f, 0.0f, 0.0f);

	// Yaw
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.0f, 0.0f, 0.0f);
	glEnd();  

	// Pitch
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, -0.0f, 0.0f);
	glEnd();  

	// Roll
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -0.0f);
	glEnd();  

	// Sphere
  glColor3f(1.0f, 0.0f, 0.0f);
  glutWireSphere(0.25f, 15, 15);
  glColor3f(0.1f, 0.1f, 0.1f);
  glutSolidSphere(0.24f, 15, 15);

  glPopMatrix();

	/* Base */

  glPushMatrix();

  // Data
  int baseYaw = (baseData[dataRow][0] < 0) ? 180 + (180 + baseData[dataRow][0]) : baseData[dataRow][0];
  int basePitch = (baseData[dataRow][1] < 0) ? 180 + (180 + baseData[dataRow][1]) : baseData[dataRow][1];
  int baseRoll = (baseData[dataRow][2] < 0) ? 180 + (180 + baseData[dataRow][2]) : baseData[dataRow][2];

  // Transfrom
  glTranslatef(2.0f, 0.0f, 0.0f);
  glRotatef(baseYaw, 0.0f, 1.0f, 0.0f);
  glRotatef(basePitch, 0.0f, 0.0f, 1.0f);
  glRotatef(baseRoll, 1.0f, 0.0f, 0.0f);

	// Yaw
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();  

	// Pitch
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();  

	// Roll
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();  

	// Sphere
  glColor3f(1.0f, 0.0f, 0.0f);
  glutWireSphere(0.25f, 15, 15);
  glColor3f(0.1f, 0.1f, 0.1f);
  glutSolidSphere(0.24f, 15, 15);

  glPopMatrix();
}

/* ---------------- */

std::vector<std::vector<int> > Visualizer::inputData;
std::vector<std::vector<int> > Visualizer::baseData;
int Visualizer::dataRow;
int Visualizer::time;
