#include <iostream>
#include "glut.h"

#define dim 300
unsigned char prevKey;

class GrilaCarteziana {
public:
	int linie, coloana;
	float ratie;

	GrilaCarteziana(int linie, int coloana);
	void writePixel(int linie, int coloana);
};

GrilaCarteziana::GrilaCarteziana(int linie, int coloana)
{
	this->linie = linie;
	this->coloana = coloana;
	this->ratie = 2 / (float)(coloana + 1);
}

//functie de desenare pixel sub forma de cerc, nu-mi dau seama de formule si de unde sa incep
void GrilaCarteziana::writePixel(int linie, int coloana) {
	
	float ratiePixel = ratie / 3;
	float coloanaDesen = (-1) + coloana * ratie;
	float linieDesen = (-1) + linie * ratie;
	glBegin(GL_POLYGON);
	for (float i = coloanaDesen - ratiePixel; i < coloanaDesen + ratiePixel; i += 0.002) {
		glVertex2f(i, linieDesen - i);
	}
	glEnd();
	/*glBegin(GL_POLYGON);
	for (float i = coloanaDesen - ratiePixel; i < coloanaDesen + ratiePixel; i += 0.002) {
		glVertex2f(i,-i);
	}
	glEnd();*/
}

void Display1() {
	GrilaCarteziana grilacarteziana(16, 16);
	float ratie = 2/(float)(grilacarteziana.coloana+1);
	glColor3d(0, 0, 0);
	for (float i = -1+ratie; i < 1-ratie; i += ratie) {
		glBegin(GL_LINE_STRIP);
		glVertex2f(i, -1+ratie);
		glVertex2f(i, 1-ratie);
		glEnd();
	}
	for (float i = -1+ratie; i < 1-ratie; i += ratie) {
		glBegin(GL_LINE_STRIP);
		glVertex2f(-1+ratie, i);
		glVertex2f(1-ratie, i);
		glEnd();
	}
	grilacarteziana.writePixel(3, 3);
}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	switch (prevKey) {
	case '1':
		Display1();
		break;
	}

	glFlush();
}

void Init(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(1);

	//   glPointSize(4);

	glPolygonMode(GL_FRONT, GL_LINE);
}

void Reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
	prevKey = key;
	if (key == 27) // escape
		exit(0);
	glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitWindowSize(dim, dim);

	glutInitWindowPosition(100, 100);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutCreateWindow(argv[0]);

	Init();

	glutReshapeFunc(Reshape);

	glutKeyboardFunc(KeyboardFunc);

	glutMouseFunc(MouseFunc);

	glutDisplayFunc(Display);

	glutMainLoop();

	return 0;
}