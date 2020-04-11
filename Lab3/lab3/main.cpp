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
	void afisarePuncteCerc(int x, int y);
	void umplereElipsa(int x0, int y0, int a, int b, double val);
	void afisareCerc(int raza);
};

GrilaCarteziana::GrilaCarteziana(int linie, int coloana)
{
	this->linie = linie;
	this->coloana = coloana;
	this->ratie = 2 / (float)(coloana + 1);
}

void GrilaCarteziana::writePixel(int linie, int coloana) {
	
	std::cout << linie << " " << coloana << std::endl;
	float ratiePixel = ratie / 3;
	float coloanaDesen = -1+(coloana * ratie + ratie) ;
	float linieDesen = -1+(linie * ratie + ratie);


	std::cout << coloanaDesen << std::endl;
	std::cout << linieDesen << std::endl;

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

void GrilaCarteziana::afisarePuncteCerc(int x, int y) {
	writePixel(x, y);
	writePixel(x, y - 1);
	writePixel(x, y + 1);
	if (x != y) {
		writePixel(x, y);
		writePixel(x, y - 1);
		writePixel(x, y + 1);
	}
}

void GrilaCarteziana::umplereElipsa(int x0, int y0, int a, int b, double val)
{
	int xi = 0, x = 0, y = b;
	double fxpyp = 0.0;
	double deltaE, deltaSE, deltaS;
	writePixel(y + y0, xi + x0);
	ssm.vidare();
	ssm.adauga(y + y0, xi + x0, x + x0);

	//regiunea 1
	while (a * a * (y - 0.5) > b * b * (x + 1)) {
		deltaE = b * b * (2 * x + 1);
		deltaSE = b * b * (2 * x + 1) + a * a * (-2 * y + 1);
		if (fxpyp + deltaE <= 0.0) {
			fxpyp += deltaE;
			x++;
			ssm.setare(y + y0, xi + x0, x + x0);
			//writePixel(y + y0, xi + x0);
		}
		else if (fxpyp + deltaSE < 0.0) {
			fxpyp += deltaSE;
			x++;
			y--;
			ssm.adauga(y + y0, xi + x0, x + x0);
			//writePixel(y + y0, xi + x0);
		}
	}

	//regiunea 2
	while (y > 0) {
		deltaSE = b * b * (2 * x + 1) + a * a * (-2 * y + 1);
		deltaS = a * a * (-2 * y + 1);
		if (fxpyp + deltaSE <= 0.0) {
			fxpyp += deltaSE;
			x++;
			y--;
		}
		else {
			fxpyp += deltaS;
			y--;
		}
		ssm.adauga(y + y0, xi + x0, x + x0);
	}
	ssm.desenare(val, *this, sablon);
}

void GrilaCarteziana::afisareCerc(int r) {
	int x = 0, y = r;
	double d = 1 - r;
	int dE = 3, dSE = -2 * r + 5;
	afisarePuncteCerc(x, y);
	while (y > x) {
		if (d < 0) {
			d += dE;
			dE += 2;
			dSE += 2;
		}
		else {
			d += dSE;
			dE += 2;
			dSE += 4;
			y--;
		}
		x++;
		afisarePuncteCerc(x, y);
	}
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
	grilacarteziana.umplereElipsa(0,0,4,7,0);
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