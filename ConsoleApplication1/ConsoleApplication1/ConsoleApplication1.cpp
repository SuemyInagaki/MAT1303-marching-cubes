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

#define NUM_CUBOS 64
#define TAM_GRID 4 // GRID = TAM_GRID x TAM_GRID x TAM_GRID = raiz cubica de NUM_CUBOS
#define NUM_PONTOS 125 // (TAM_GRID + 1)^3;

#define X_MIN -2.0
#define Y_MIN -2.0
#define Z_MIN -2.0
#define X_MAX 2.0
#define Y_MAX 2.0
#define Z_MAX 2.0


double p[NUM_PONTOS][3];
int animacao = 1; // animacao = 1 se quer ver o algoritmo animado
int index = 0;

using namespace std;


typedef struct {
	double x;
	double y;
	double z;
} XYZ;

// vertices do triangulo
typedef struct {
	XYZ p[3];
} TRIANGLE;

// Um cubo com 8 vértices e 8 valores de f(x, y, z)
typedef struct {
	XYZ p[8]; // Coordenada dos 8 vértices de um cubo
	double val[8]; //Valor de f(x, y, z) para cada vértice do cubo
} GRIDCELL;

// uma lista de NUM_CUBOS posicoes do tipo GRIDCELL
GRIDCELL dados_cubos[NUM_CUBOS];

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


void preenche_vetor_dados_cubos() {
	int i_cubo = 0;
	double dx, dy, dz;
	double x, y, z;
	dx = (X_MAX - X_MIN) / TAM_GRID;
	dy = (Y_MAX - Y_MIN) / TAM_GRID;
	dz = (Z_MAX - Z_MIN) / TAM_GRID;

	x = X_MIN; y = Y_MIN; z = Z_MIN;
	int n = 0;
	int i = 0;
	while (i < NUM_PONTOS) {
		if (i % 5 != 4 && (i < 20 || i > 24) && (i < 45 || i > 49) && (i < 70 || i > 74) && i < 95) {
			dados_cubos[i_cubo].p[0].x = p[i][0];
			dados_cubos[i_cubo].p[0].y = p[i][1];
			dados_cubos[i_cubo].p[0].z = p[i][2];

			dados_cubos[i_cubo].p[1].x = p[i + 1][0];
			dados_cubos[i_cubo].p[1].y = p[i + 1][1];
			dados_cubos[i_cubo].p[1].z = p[i + 1][2];

			dados_cubos[i_cubo].p[2].x = p[i + 5][0];
			dados_cubos[i_cubo].p[2].y = p[i + 5][1];
			dados_cubos[i_cubo].p[2].z = p[i + 5][2];

			dados_cubos[i_cubo].p[3].x = p[i + 6][0];
			dados_cubos[i_cubo].p[3].y = p[i + 6][1];
			dados_cubos[i_cubo].p[3].z = p[i + 6][2];

			dados_cubos[i_cubo].p[4].x = p[i + 25][0];
			dados_cubos[i_cubo].p[4].y = p[i + 25][1];
			dados_cubos[i_cubo].p[4].z = p[i + 25][2];

			dados_cubos[i_cubo].p[5].x = p[i + 26][0];
			dados_cubos[i_cubo].p[5].y = p[i + 26][1];
			dados_cubos[i_cubo].p[5].z = p[i + 26][2];

			dados_cubos[i_cubo].p[6].x = p[i + 30][0];
			dados_cubos[i_cubo].p[6].y = p[i + 30][1];
			dados_cubos[i_cubo].p[6].z = p[i + 30][2];

			dados_cubos[i_cubo].p[7].x = p[i + 31][0];
			dados_cubos[i_cubo].p[7].y = p[i + 31][1];
			dados_cubos[i_cubo].p[7].z = p[i + 31][2];

			for (int j = 0; j < 8; j++) {
				dados_cubos[i_cubo].val[j] = f.f2(dados_cubos[i_cubo].p[j].x, dados_cubos[i_cubo].p[j].y, dados_cubos[i_cubo].p[j].z);
			}
			i_cubo++;
		}		
		i++;
	}
}

void percorre_cubos() {
	
	for (int j = 0; j < 8; j++) {
		glColor3f(1.0, 0.5, 0.0);
		glBegin(GL_POINTS);
		glVertex3f(dados_cubos[index].p[j].x, dados_cubos[index].p[j].y, dados_cubos[index].p[j].z);
		glEnd();
		glLineWidth(1);		
	}
	glBegin(GL_LINES);
		glVertex3f(dados_cubos[index].p[0].x, dados_cubos[index].p[0].y, dados_cubos[index].p[0].z);
		glVertex3f(dados_cubos[index].p[1].x, dados_cubos[index].p[1].y, dados_cubos[index].p[1].z);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(dados_cubos[index].p[0].x, dados_cubos[index].p[0].y, dados_cubos[index].p[0].z);
		glVertex3f(dados_cubos[index].p[2].x, dados_cubos[index].p[2].y, dados_cubos[index].p[2].z);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(dados_cubos[index].p[3].x, dados_cubos[index].p[3].y, dados_cubos[index].p[3].z);
		glVertex3f(dados_cubos[index].p[2].x, dados_cubos[index].p[2].y, dados_cubos[index].p[2].z);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(dados_cubos[index].p[3].x, dados_cubos[index].p[3].y, dados_cubos[index].p[3].z);
		glVertex3f(dados_cubos[index].p[1].x, dados_cubos[index].p[1].y, dados_cubos[index].p[1].z);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(dados_cubos[index].p[0].x, dados_cubos[index].p[0].y, dados_cubos[index].p[0].z);
		glVertex3f(dados_cubos[index].p[4].x, dados_cubos[index].p[4].y, dados_cubos[index].p[4].z);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(dados_cubos[index].p[5].x, dados_cubos[index].p[5].y, dados_cubos[index].p[5].z);
		glVertex3f(dados_cubos[index].p[4].x, dados_cubos[index].p[4].y, dados_cubos[index].p[4].z);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(dados_cubos[index].p[6].x, dados_cubos[index].p[6].y, dados_cubos[index].p[6].z);
		glVertex3f(dados_cubos[index].p[4].x, dados_cubos[index].p[4].y, dados_cubos[index].p[4].z);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(dados_cubos[index].p[5].x, dados_cubos[index].p[5].y, dados_cubos[index].p[5].z);
		glVertex3f(dados_cubos[index].p[7].x, dados_cubos[index].p[7].y, dados_cubos[index].p[7].z);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(dados_cubos[index].p[6].x, dados_cubos[index].p[6].y, dados_cubos[index].p[6].z);
		glVertex3f(dados_cubos[index].p[7].x, dados_cubos[index].p[7].y, dados_cubos[index].p[7].z);
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

void usa_marching_cube() {
	for (int i = 0; i < NUM_CUBOS; i++) {

	}
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
	//f.plota_funcao();
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
	preenche_vetor_pontos();
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