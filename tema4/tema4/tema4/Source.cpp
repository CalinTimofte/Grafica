#include <iostream>
#include "glut.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

ifstream infile("input.txt");

#define dim 300
unsigned char prevKey;
struct varf {
	int x;
	int y;
};
struct muchie {
	varf vi;
	varf vf;
};

class Poligon {
public:
	int nbOfVertexes;
	std::vector<varf> varfuri;
	std::vector<muchie> muchii;
	Poligon() {};
};
struct intersectie {
	int ymax;
	double xmin;
	double ratia;
};
class Intersectii {
public:
	std::vector<intersectie> intersectii;
	Intersectii() {};
};

Poligon getPolygon() {
	infile.clear();
	infile.seekg(0, std::ios::beg);
	Poligon returnPoly;
	infile >> returnPoly.nbOfVertexes;
	varf* newVertexPointer = new varf;
	while (infile >> newVertexPointer->x >> newVertexPointer->y) {
		returnPoly.varfuri.push_back(*newVertexPointer);
		newVertexPointer = new varf;
	}
	muchie* newMuchiePointer = new muchie;
	for (int i = 0; i < returnPoly.varfuri.size()-1; i++) {
		newMuchiePointer->vi = returnPoly.varfuri[i];
		newMuchiePointer->vf = returnPoly.varfuri[i+1];
		returnPoly.muchii.push_back(*newMuchiePointer);
		newMuchiePointer = new muchie;
	}
	newMuchiePointer->vi = returnPoly.varfuri[returnPoly.varfuri.size()-1];
	newMuchiePointer->vf = returnPoly.varfuri[0];
	returnPoly.muchii.push_back(*newMuchiePointer);
	return returnPoly;
}

void initializareET(Poligon p, std::vector<Intersectii>& ET) {
	int xm, ym, xM, yM;
	bool change;
	//pt fiecare muchie din poligon...
	for (std::vector<muchie>::iterator m = p.muchii.begin(); m != p.muchii.end(); ++m) {
		//... care nu este orizontala
		if ((m->vi.y != m->vf.y)) {
			ym = min(m->vi.y, m->vf.y);
			yM = max(m->vi.y, m->vf.y);
			xm = (ym == m->vi.y) ? m->vi.x : m->vf.x;
			xM = (yM == m->vi.y) ? m->vi.x : m->vf.x;

			intersectie* intersectiePointer = new intersectie;
			intersectiePointer->ymax = yM;
			intersectiePointer->xmin = xm;
			intersectiePointer->ratia = (xm - xM) / (ym - yM);
			ET[ym].intersectii.push_back(*intersectiePointer);
		}
	}

	/*sortarea in ordine crescatoare conform cu 
		xm a intersectiilor dreptei de scanare cu
		muchiile poligonului*/
	//the length of ET is DOM_SCAN
	for (int i = 0; i < ET.size(); i++) {
		do
		{
			change = false;
			if (ET[i].intersectii.empty())
				break;
			for(int j = 0; j < ET[i].intersectii.size() - 1; j++){
				if (ET[i].intersectii[j].xmin > ET[i].intersectii[j + 1].xmin) {
					iter_swap(ET[i].intersectii.begin() + j, ET[i].intersectii.begin() + j + 1);
					change = true;
				}
			}
		} while (change);
	}
}

std::vector<Intersectii> calculSSM(Poligon p, std::vector<Intersectii> ET) {
	std::vector<Intersectii> finalET, nullET;
	Intersectii activeSSM;
	int y, k;
	finalET.resize(ET.size());

	// Se determina, in ordine crescatoare, care este
	// prima dreapta de scanare care intersecteaza 
	// poligonul 
	// lui y i se atribuie o valoare care nu este din DOM_SCAN 
	y = -1;
	// se determina y = min {y’ | et(y’) != \emptyset} 
	for (int i = 0; i < ET.size(); i++) {
		if (!ET[i].intersectii.empty()) {
			y = i;
			break;
		}
	}
	if (y == -1)
		return nullET;

	do
	{
		activeSSM.intersectii.insert(activeSSM.intersectii.end(), ET[y].intersectii.begin(), ET[y].intersectii.end());
		// eliminarea varfurilor cu ymax == y 
		for (int i = 0; i < activeSSM.intersectii.size(); i++) {
			if (activeSSM.intersectii[i].ymax == y)
				activeSSM.intersectii.erase(activeSSM.intersectii.begin()+i);
		}
		// sortarea activeSSM cf. cheii xmin 
		k = activeSSM.intersectii.size();
		while (k >= 2) {
			for (int i = 0; i < k-1; i++) {
				if(activeSSM.intersectii[i].xmin > activeSSM.intersectii[i+1].xmin)
					iter_swap(activeSSM.intersectii.begin() + i, activeSSM.intersectii.begin() + i + 1);
			}
			k--;
		}

		finalET[y].intersectii.clear();
		finalET[y].intersectii.insert(finalET[y].intersectii.end(), activeSSM.intersectii.begin(), activeSSM.intersectii.end());
		y++;

		// reactualizarea punctelor de intersectie pentru noua dreapta de scanare 
		for (int i = 0; i < activeSSM.intersectii.size(); i++) {
			if (activeSSM.intersectii[i].ratia != 0)
				activeSSM.intersectii[i].xmin += activeSSM.intersectii[i].ratia;
		}
	} while (!activeSSM.intersectii.empty() || !ET[y].intersectii.empty());
	return finalET;
}

class GrilaCarteziana {
public:
	int linie, coloana;
	float ratie;

	GrilaCarteziana(int linie, int coloana);
	void writePixel(int linie, int coloana);
	void afisarePuncteCerc(int x, int y);
	//void umplereElipsa(int x0, int y0, int a, int b, double val);
	void umplerePoligon();
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
	float coloanaDesen = -1 + (coloana * ratie + ratie);
	float linieDesen = -1 + (linie * ratie + ratie);


	std::cout << coloanaDesen << std::endl;
	std::cout << linieDesen << std::endl;

	glBegin(GL_POLYGON);
	float i, j;
	for (i = coloanaDesen - ratiePixel, j = linieDesen; i <= coloanaDesen, j <= linieDesen + ratiePixel; i += 0.002, j += 0.002) {
		glVertex2f(i, j);
	}

	for (i = coloanaDesen, j = linieDesen + ratiePixel; i <= coloanaDesen + ratiePixel, j >= linieDesen; i += 0.002, j -= 0.002) {
		glVertex2f(i, j);
	}
	for (i = coloanaDesen + ratiePixel, j = linieDesen; i >= coloanaDesen, j >= linieDesen - ratiePixel; i -= 0.002, j -= 0.002) {
		glVertex2f(i, j);
	}

	for (i = coloanaDesen, j = linieDesen - ratiePixel; i >= coloanaDesen - ratiePixel, j <= linieDesen; i -= 0.002, j += 0.002) {
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

//void GrilaCarteziana::umplereElipsa(int x0, int y0, int a, int b, double val)
//{
//	int xi = 0, x = 0, y = b;
//	double fxpyp = 0.0;
//	double deltaE, deltaSE, deltaS;
//	writePixel(y + y0, xi + x0);
//	ssm.vidare();
//	ssm.adauga(y + y0, xi + x0, x + x0);
//
//	//regiunea 1
//	while (a * a * (y - 0.5) > b* b* (x + 1)) {
//		deltaE = b * b * (2 * x + 1);
//		deltaSE = b * b * (2 * x + 1) + a * a * (-2 * y + 1);
//		if (fxpyp + deltaE <= 0.0) {
//			fxpyp += deltaE;
//			x++;
//			ssm.setare(y + y0, xi + x0, x + x0);
//			//writePixel(y + y0, xi + x0);
//		}
//		else if (fxpyp + deltaSE < 0.0) {
//			fxpyp += deltaSE;
//			x++;
//			y--;
//			ssm.adauga(y + y0, xi + x0, x + x0);
//			//writePixel(y + y0, xi + x0);
//		}
//	}
//
//	//regiunea 2
//	while (y > 0) {
//		deltaSE = b * b * (2 * x + 1) + a * a * (-2 * y + 1);
//		deltaS = a * a * (-2 * y + 1);
//		if (fxpyp + deltaSE <= 0.0) {
//			fxpyp += deltaSE;
//			x++;
//			y--;
//		}
//		else {
//			fxpyp += deltaS;
//			y--;
//		}
//		ssm.adauga(y + y0, xi + x0, x + x0);
//	}
//	ssm.desenare(val, *this, sablon);
//}

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

void GrilaCarteziana::umplerePoligon() {
	Poligon P = getPolygon();
	int DOM_SCAN = this->linie;
	std::vector<Intersectii> ET;
	ET.resize(DOM_SCAN);
	initializareET(P, ET);
	//std::vector<Intersectii> ssms = calculSSM(P, ET);
}

void Display1() {
	GrilaCarteziana grilacarteziana(16, 16);
	float ratie = 2 / (float)(grilacarteziana.coloana + 1);
	glColor3d(0, 0, 0);
	for (float i = -1 + ratie; i < 1 - ratie; i += ratie) {
		glBegin(GL_LINE_STRIP);
		glVertex2f(i, -1 + ratie);
		glVertex2f(i, 1 - ratie);
		glEnd();
	}
	for (float i = -1 + ratie; i < 1 - ratie; i += ratie) {
		glBegin(GL_LINE_STRIP);
		glVertex2f(-1 + ratie, i);
		glVertex2f(1 - ratie, i);
		glEnd();
	}
	grilacarteziana.afisareCerc(10);
}

void Display2() {
	GrilaCarteziana grilacarteziana(16, 16);
	float ratie = 2 / (float)(grilacarteziana.coloana + 1);
	glColor3d(0, 0, 0);
	for (float i = -1 + ratie; i < 1 - ratie; i += ratie) {
		glBegin(GL_LINE_STRIP);
		glVertex2f(i, -1 + ratie);
		glVertex2f(i, 1 - ratie);
		glEnd();
	}
	for (float i = -1 + ratie; i < 1 - ratie; i += ratie) {
		glBegin(GL_LINE_STRIP);
		glVertex2f(-1 + ratie, i);
		glVertex2f(1 - ratie, i);
		glEnd();
	}
	//grilacarteziana.umplereElipsa(0, 0, 4, 7, 0);
}

void Display3() {
	GrilaCarteziana grilacarteziana(16, 16);
	float ratie = 2 / (float)(grilacarteziana.coloana + 1);
	glColor3d(0, 0, 0);
	for (float i = -1 + ratie; i < 1 - ratie; i += ratie) {
		glBegin(GL_LINE_STRIP);
		glVertex2f(i, -1 + ratie);
		glVertex2f(i, 1 - ratie);
		glEnd();
	}
	for (float i = -1 + ratie; i < 1 - ratie; i += ratie) {
		glBegin(GL_LINE_STRIP);
		glVertex2f(-1 + ratie, i);
		glVertex2f(1 - ratie, i);
		glEnd();
	}
	
	//Poligon P = getPolygon();
	/*for (int i = 0; i < P.nbOfVertexes; i++) {
		grilacarteziana.writePixel(P.varfuri[i].x, P.varfuri[i].y);
	}*/
	grilacarteziana.umplerePoligon();
}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	switch (prevKey) {
	case '1':
		Display1();
		break;
	case '2':
		Display2();
		break;
	case '3':
		Display3();
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