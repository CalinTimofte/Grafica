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
	void afisareSegmentDreapta3(int x0, int y0, int xf, int yf);
};

GrilaCarteziana::GrilaCarteziana(int linie, int coloana)
{
	this->linie = linie;
	this->coloana = coloana;
	this->ratie = 2 / (float)(coloana + 1);
}

//functie de desenare pixel sub forma de cerc, deocamdata romb
void GrilaCarteziana::writePixel(int linie, int coloana) {
	
	float ratiePixel = ratie / 4;
	float coloanaDesen;
	if (coloana < this->coloana/2)
		coloanaDesen = -((coloana * ratie - ratie)+1);
	else
		coloanaDesen = (coloana * ratie - ratie)-1 ;
	std::cout << coloanaDesen;
	float linieDesen;
	if (linie < this->linie / 2)
		linieDesen = ((linie * ratie - ratie)+1);
	else
		linieDesen = -((linie * ratie-ratie) -1) ;
	std::cout << linieDesen;
	
	float theta = 2 * 3.1415926 / (float)(200);
	float c = cosf(theta);
	float s = sinf(theta);
	float t;

	float x = ratiePixel;
	float y = 0;
	/*glBegin(GL_POLYGON);
	for (int i = 0; i <= 200; i ++) {
		glVertex2f(x+coloanaDesen, y+linieDesen);
		t = x;
		x = c * x - s * y;
		y = s * x + c * y;
	}*/
	glBegin(GL_POLYGON);
	float i, j;
	for (i = coloanaDesen - ratiePixel, j = linieDesen; i <= coloanaDesen, j<= linieDesen+ratiePixel; i += 0.002, j+=0.002) {
		glVertex2f(i, j);
	}
	
	for (i = coloanaDesen , j = linieDesen+ratiePixel; i <= coloanaDesen + ratiePixel, j >= linieDesen; i += 0.002, j -= 0.002) {
		glVertex2f(i, j);
	}
	for (i = coloanaDesen + ratiePixel, j = linieDesen; i >= coloanaDesen, j >= linieDesen - ratiePixel; i -= 0.002, j -= 0.002) {
		glVertex2f(i, j);
	}

	for (i = coloanaDesen, j = linieDesen-ratiePixel; i >= coloanaDesen - ratiePixel, j <= linieDesen; i -= 0.002, j += 0.002) {
		glVertex2f(i, j);
	}
	glEnd();
}

// formule de calculat pt (-1,1) pe ox si oy
// folosit writePixel pentru a desena pixelii cei mai apropiati
void GrilaCarteziana::afisareSegmentDreapta3(int x0,int y0,int xf,int yf)
{
	// valoarea initiala a variabile de decizie
	// dx, dy sunt constante - a se vedea mai sus
	int dx = xf - x0;
	int dy = yf - y0;
	int d = 2 * dy - dx;
	int dE = 2 * dy;
	int dNE = 2 * (dy - dx);
	int x = x0, y = y0;
	glBegin(GL_LINE_STRIP);
	while (x < xf)
	{
		if (d <= 0) { /* alegem E */ d += dE; x++; }
		else { /* alegem NE */ d += dNE; x++; y++; }
		glVertex2d(x, y);
	}
	glEnd();
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
	grilacarteziana.afisareSegmentDreapta3(0, 0,10,10);
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