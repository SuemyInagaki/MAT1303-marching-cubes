#include <iostream>
#include <gl\glut.h>
#include <math.h>
#include "funcao.h"
#include "MarchingCube.h"

float theta = 135;
float phi = 45;
float gamma = 90;
float scale = 1.0;
int xb, xm, yb, ym;
funcao f;
MarchingCube marching;

#define NUM_CUBOS 1000
#define TAM_GRID 10 // GRID = TAM_GRID x TAM_GRID x TAM_GRID = raiz cubica de NUM_CUBOS

#define X_MIN -2.0
#define Y_MIN -2.0
#define Z_MIN -2.0
#define X_MAX 2.0
#define Y_MAX 2.0
#define Z_MAX 2.0

int animacao = 1; // animacao = 1 se quer ver o algoritmo animado
int index = 0;

using namespace std;

MarchingCube::GRIDCELL dados_cubos[NUM_CUBOS];

void preenche_vetor_dados_cubos() {
	int i_cubo = 0;
	double dx, dy, dz;
	double x, y, z;
	dx = (X_MAX - X_MIN) / TAM_GRID;
	dy = (Y_MAX - Y_MIN) / TAM_GRID;
	dz = (Z_MAX - Z_MIN) / TAM_GRID;

	x = X_MIN; y = Y_MIN; z = Z_MIN;

	for (int i = 0; i < TAM_GRID; i++) {
		y = Y_MIN;
		for (int j = 0; j < TAM_GRID; j++) {
			z = Z_MIN;
			for (int k = 0; k < TAM_GRID; k++) {

				dados_cubos[i_cubo].p[2].x = x;
				dados_cubos[i_cubo].p[2].y = y;
				dados_cubos[i_cubo].p[2].z = z;

				dados_cubos[i_cubo].p[3].x = x + dx;
				dados_cubos[i_cubo].p[3].y = y;
				dados_cubos[i_cubo].p[3].z = z;

				dados_cubos[i_cubo].p[0].x = x + dx;
				dados_cubos[i_cubo].p[0].y = y + dy;
				dados_cubos[i_cubo].p[0].z = z;

				dados_cubos[i_cubo].p[1].x = x;
				dados_cubos[i_cubo].p[1].y = y + dy;
				dados_cubos[i_cubo].p[1].z = z;

				dados_cubos[i_cubo].p[6].x = x;
				dados_cubos[i_cubo].p[6].y = y;
				dados_cubos[i_cubo].p[6].z = z + dz;

				dados_cubos[i_cubo].p[7].x = x + dx;
				dados_cubos[i_cubo].p[7].y = y;
				dados_cubos[i_cubo].p[7].z = z + dz;

				dados_cubos[i_cubo].p[4].x = x + dx;
				dados_cubos[i_cubo].p[4].y = y + dy;
				dados_cubos[i_cubo].p[4].z = z + dz;

				dados_cubos[i_cubo].p[5].x = x;
				dados_cubos[i_cubo].p[5].y = y + dy;
				dados_cubos[i_cubo].p[5].z = z + dz;

				for (int h = 0; h < 8; h++) {
					dados_cubos[i_cubo].val[h] = f.f2(dados_cubos[i_cubo].p[h].x, dados_cubos[i_cubo].p[h].y, dados_cubos[i_cubo].p[h].z);
				}
				z += dz;
				i_cubo++;
			}
			y += dy;
		}
		x += dx;
	}
}


void desenha_cada_cubo(int index, float r, float g, float b) {
	glColor3f(r, g, b);
	glBegin(GL_LINES);
	glVertex3f(dados_cubos[index].p[0].x, dados_cubos[index].p[0].y, dados_cubos[index].p[0].z);
	glVertex3f(dados_cubos[index].p[1].x, dados_cubos[index].p[1].y, dados_cubos[index].p[1].z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(dados_cubos[index].p[0].x, dados_cubos[index].p[0].y, dados_cubos[index].p[0].z);
	glVertex3f(dados_cubos[index].p[3].x, dados_cubos[index].p[3].y, dados_cubos[index].p[3].z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(dados_cubos[index].p[2].x, dados_cubos[index].p[2].y, dados_cubos[index].p[2].z);
	glVertex3f(dados_cubos[index].p[3].x, dados_cubos[index].p[3].y, dados_cubos[index].p[3].z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(dados_cubos[index].p[2].x, dados_cubos[index].p[2].y, dados_cubos[index].p[2].z);
	glVertex3f(dados_cubos[index].p[1].x, dados_cubos[index].p[1].y, dados_cubos[index].p[1].z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(dados_cubos[index].p[4].x, dados_cubos[index].p[4].y, dados_cubos[index].p[4].z);
	glVertex3f(dados_cubos[index].p[5].x, dados_cubos[index].p[5].y, dados_cubos[index].p[5].z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(dados_cubos[index].p[4].x, dados_cubos[index].p[4].y, dados_cubos[index].p[4].z);
	glVertex3f(dados_cubos[index].p[7].x, dados_cubos[index].p[7].y, dados_cubos[index].p[7].z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(dados_cubos[index].p[6].x, dados_cubos[index].p[6].y, dados_cubos[index].p[6].z);
	glVertex3f(dados_cubos[index].p[7].x, dados_cubos[index].p[7].y, dados_cubos[index].p[7].z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(dados_cubos[index].p[6].x, dados_cubos[index].p[6].y, dados_cubos[index].p[6].z);
	glVertex3f(dados_cubos[index].p[5].x, dados_cubos[index].p[5].y, dados_cubos[index].p[5].z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(dados_cubos[index].p[0].x, dados_cubos[index].p[0].y, dados_cubos[index].p[0].z);
	glVertex3f(dados_cubos[index].p[4].x, dados_cubos[index].p[4].y, dados_cubos[index].p[4].z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(dados_cubos[index].p[1].x, dados_cubos[index].p[1].y, dados_cubos[index].p[1].z);
	glVertex3f(dados_cubos[index].p[5].x, dados_cubos[index].p[5].y, dados_cubos[index].p[5].z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(dados_cubos[index].p[6].x, dados_cubos[index].p[6].y, dados_cubos[index].p[6].z);
	glVertex3f(dados_cubos[index].p[2].x, dados_cubos[index].p[2].y, dados_cubos[index].p[2].z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(dados_cubos[index].p[3].x, dados_cubos[index].p[3].y, dados_cubos[index].p[3].z);
	glVertex3f(dados_cubos[index].p[7].x, dados_cubos[index].p[7].y, dados_cubos[index].p[7].z);
	glEnd();
}


void percorre_cubos() {
	glLineWidth(1);
	desenha_cada_cubo(index, 1.0, 0.5, 0.0);
	glColor3f(0.0, 0.0, 1.0);
}


void desenha_grid() {
	for (int index = 0; index < NUM_CUBOS; index++) {
		desenha_cada_cubo(index, 0.2, 0.0, 0.2);
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

void usa_marching_cube() {
	MarchingCube::TRIANGLE triangulos[10];
	for (int i = 0; i < NUM_CUBOS; i++) {
		marching.Polygonise(dados_cubos[i], 0.0, triangulos);
		
		for (int i = 0; i < 10; i++) {
			if (triangulos[i].p == NULL) {
				break;
			}
			else {
				glColor3f(1.0, 0.5, 0.0);
				glBegin(GL_LINES);
					glVertex3f(triangulos[i].p[0].x, triangulos[i].p[0].y, triangulos[i].p[0].z);
					glVertex3f(triangulos[i].p[1].x, triangulos[i].p[1].y, triangulos[i].p[1].z);
				glEnd();
				glBegin(GL_LINES);
					glVertex3f(triangulos[i].p[0].x, triangulos[i].p[0].y, triangulos[i].p[0].z);
					glVertex3f(triangulos[i].p[2].x, triangulos[i].p[2].y, triangulos[i].p[2].z);
				glEnd();
				glBegin(GL_LINES);
					glVertex3f(triangulos[i].p[2].x, triangulos[i].p[2].y, triangulos[i].p[2].z);
					glVertex3f(triangulos[i].p[1].x, triangulos[i].p[1].y, triangulos[i].p[1].z);
				glEnd();
			}
		}
	}
	glColor3f(0.0, 0.0, 1.0);
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
		percorre_cubos();
	}
	usa_marching_cube();
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
			if (index < NUM_CUBOS) {
				inicia_config();
				redesenha();
			}
			else {
				index = 0;
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
	preenche_vetor_dados_cubos();
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