#include <iostream>
#include <gl\glut.h>
#include <math.h>
#include "funcao.h"
float theta = 135;
float phi = 45;
float gamma = 90;
float scale = 1.0;
int xb, xm, yb, ym;
funcao f;

#define NUM_CUBOS 64
#define TAM_GRID 4 // GRID = TAM_GRID x TAM_GRID x TAM_GRID 
#define X_MIN -2.0
#define Y_MIN -2.0
#define Z_MIN -2.0
#define X_MAX 2.0
#define Y_MAX 2.0
#define Z_MAX 2.0

#define NUM_PONTOS 125 // (TAM_GRID + 1)^3;

double p[NUM_PONTOS][3];
int animacao = 0; // animacao = 1 se quer ver o algoritmo animado
int index = 0;

using namespace std;

void preenche_vetor_pontos() {
	double dx, dy, dz;
	double x, y, z;
	dx = (X_MAX - X_MIN) / TAM_GRID;
	dy = (Y_MAX - Y_MIN) / TAM_GRID;
	dz = (Z_MAX - Z_MIN) / TAM_GRID;

	x = X_MIN; y = Y_MIN; z = Z_MIN;
	int n = 0;
	for (int i = 0; i <= TAM_GRID; i++) {
		y = Y_MIN;
		for (int j = 0; j <= TAM_GRID; j++) {
			z = Z_MIN;
			for (int k = 0; k <= TAM_GRID; k++) {
				p[n][0] = x;
				p[n][1] = y;
				p[n][2] = z;
				z += dz;
				n++;
			}
			y += dy;
		}
		x += dx;
	}
}

void percorre_pontos() {
	// apaga o ponto anterior
	glPointSize(3.0);
	if (index > 0) {
		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_POINTS);
			glVertex3f(p[index -1][0], p[index -1][1], p[index -1][2]);
		glEnd();
	}
	else {
		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_POINTS);
			glVertex3f(p[63][0], p[63][1], p[63][2]);
		glEnd();
	}
	
	// pinta o ponto atual
	glColor3f(1.0, 0.5, 0.0);
	glBegin(GL_POINTS);
		glVertex3f(p[index][0], p[index][1], p[index][2]);
	glEnd();
	glLineWidth(1);
	
}

void desenha_grid() 
{
	double dx, dy, dz;
	double x, y, z;
	int numLados = (int) cbrt(NUM_CUBOS);
	dx = (X_MAX - X_MIN) / numLados;
	dy = (Y_MAX - Y_MIN) / numLados;
	dz = (Z_MAX - Z_MIN) / numLados;
	
	x = X_MIN; y = Y_MIN; z = Z_MIN;

	glColor3f(0.2, 0.0, 0.2);
	glLineWidth(1);
	for (int i = 0; i <= numLados; i++) {
		y = Y_MIN;
		for (int j = 0; j <= numLados; j++) {
			z = Z_MIN;
			for (int k = 0; k <= numLados; k++) {
				glBegin(GL_LINES);
					glVertex3f(x, y, z);
					if (x + dx < numLados - 1) {
						glVertex3f(x + dx, y, z);
					}
				glEnd();
				glBegin(GL_LINES);
					glVertex3f(x, y, z);
					if (y + dy < numLados - 1) {
						glVertex3f(x, y + dy, z);
					}
				glEnd();
				glBegin(GL_LINES);
					glVertex3f(x, y, z);
					if (z + dz < numLados - 1) {
						glVertex3f(x, y, z + dz);
					}
				glEnd();

				glColor3f(0.0, 0.0, 1.0);
				glBegin(GL_POINTS);
					glVertex3f(x, y, z);
				glEnd();
				glColor3f(0.2, 0.0, 0.2);
				z += dz;
			}
			y += dy;
		}
		x += dx;
	}
}


void plota_eixos()
{
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(10.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 10.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 10.0);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();
}
void inicia_config()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-3.0, 3.0, -3.0, 3.0, -20.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(gamma, 0.0, 0.0, 1.0);
	glRotatef(phi, 0.0, 1.0, 0.0);
	glRotatef(theta, 0.0, 0.0, 1.0);
	glScalef(scale, scale, scale);
}
void redesenha()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	desenha_grid();
	plota_eixos();
	if (animacao == 1) {
		percorre_pontos();
	}
	f.plota_funcao();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{

	case 'S':
		scale += 0.2;
		inicia_config();
		redesenha();
		break;
	case 's':
		scale -= 0.2;
		inicia_config();
		redesenha();
		break;
	case 'n':
		if (animacao == 1) {
			index++;
			if (index < NUM_PONTOS) {
				inicia_config();
				redesenha();
			}
			cout << index << endl;
		}
		break;
	}
}
void botao_mouse(int b, int state, int x, int y)
{
	switch (b) {
	case GLUT_LEFT_BUTTON:
		switch (state) {
		case GLUT_DOWN:
			xb = x;
			yb = y;
			break;
		case GLUT_UP:
			theta = theta + xm - xb;
			phi = phi - ym + yb;
			break;
		}
		break;
	}
}
void mov_mouse(int x, int y)
{
	xm = x;
	ym = y;
	theta = theta + xm - xb;
	phi = phi - ym + yb;
	inicia_config();
	xb = xm;
	yb = ym;
	redesenha();
}
void main(int argc, char **argv)
{
	preenche_vetor_pontos();
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Marching Cubes");
	glutDisplayFunc(redesenha);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(botao_mouse);
	glutMotionFunc(mov_mouse);
	inicia_config();
	glutMainLoop();
}