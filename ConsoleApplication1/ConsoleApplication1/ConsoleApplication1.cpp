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
double ox, oy, oz;
#define NUM_CUBOS 2197 // Preferencialmente um numero cubo perfeito
int TAM_GRID = (int) cbrt(NUM_CUBOS);


#define X_MIN -2.0
#define Y_MIN -2.0
#define Z_MIN -2.0
#define X_MAX 2.0
#define Y_MAX 2.0
#define Z_MAX 2.0

int mostra_curva = 0;
int mostra_silhueta = 1;
int desenha_normal = 0;
int mostra_grid = 0;
int index = 0;
float modelview[16];

using namespace std;

int toro_ou_esfera;

MarchingCube::GRIDCELL dados_cubos[NUM_CUBOS];
MarchingCube::TRIANGLE lista_triangulos[10*NUM_CUBOS];
int quantidade_triangulos = 0;

float normais[8 * NUM_CUBOS][3][3]; 

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
					if (toro_ou_esfera == 1) {
						dados_cubos[i_cubo].val[h] = f.f2(dados_cubos[i_cubo].p[h].x, dados_cubos[i_cubo].p[h].y, dados_cubos[i_cubo].p[h].z);
					}
					else {
						dados_cubos[i_cubo].val[h] = f.f2_t(dados_cubos[i_cubo].p[h].x, dados_cubos[i_cubo].p[h].y, dados_cubos[i_cubo].p[h].z);
					}
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

void acha_normal() {
	float n1x, n2x, n3x, n1y, n2y, n3y, n1z, n2z, n3z;
	int k = 0;
	for (int i = 0; i < quantidade_triangulos; i++) {
		if (toro_ou_esfera == 1) {
			f.f2_gradiente(lista_triangulos[i].p[0].x, lista_triangulos[i].p[0].y, lista_triangulos[i].p[0].z, &n1x, &n1y, &n1z);
			f.f2_gradiente(lista_triangulos[i].p[1].x, lista_triangulos[i].p[1].y, lista_triangulos[i].p[1].z, &n2x, &n2y, &n2z);
			f.f2_gradiente(lista_triangulos[i].p[2].x, lista_triangulos[i].p[2].y, lista_triangulos[i].p[2].z, &n3x, &n3y, &n3z);
		}
		else {
			f.f2_gradiente_t(lista_triangulos[i].p[0].x, lista_triangulos[i].p[0].y, lista_triangulos[i].p[0].z, &n1x, &n1y, &n1z);
			f.f2_gradiente_t(lista_triangulos[i].p[1].x, lista_triangulos[i].p[1].y, lista_triangulos[i].p[1].z, &n2x, &n2y, &n2z);
			f.f2_gradiente_t(lista_triangulos[i].p[2].x, lista_triangulos[i].p[2].y, lista_triangulos[i].p[2].z, &n3x, &n3y, &n3z);
		}
		

		normais[i][0][0] = n1x;
		normais[i][0][1] = n1y;
		normais[i][0][2] = n1z;

		normais[i][1][0] = n2x;
		normais[i][1][1] = n2y;
		normais[i][1][2] = n2z;

		normais[i][2][0] = n3x;
		normais[i][2][1] = n3y;
		normais[i][2][2] = n3z;
	}
}

void calcula_produto_interno() {
	double p1, p2, p3;
	double ax, ay, az;
	double bx, by, bz;
	int pinta = 0;
	for (int i = 0; i < quantidade_triangulos; i++) {
		pinta = 0;
		
		// calcula produto interno
		p1 = normais[i][0][0] * ox + normais[i][0][1] * oy + normais[i][0][2] * oz;
		p2 = normais[i][1][0] * ox + normais[i][1][1] * oy + normais[i][1][2] * oz;
		p3 = normais[i][2][0] * ox + normais[i][2][1] * oy + normais[i][2][2] * oz;
		// avalia a mudança de sinal
		if ((p1*p3) < 0 && (p2*p3) < 0) {
			ax = (lista_triangulos[i].p[0].x + lista_triangulos[i].p[2].x) / 2.0 ;
			ay = (lista_triangulos[i].p[0].y + lista_triangulos[i].p[2].y) / 2.0;
			az = (lista_triangulos[i].p[0].z + lista_triangulos[i].p[2].z) / 2.0;

			bx = (lista_triangulos[i].p[1].x + lista_triangulos[i].p[2].x) / 2.0;
			by = (lista_triangulos[i].p[1].y + lista_triangulos[i].p[2].y) / 2.0;
			bz = (lista_triangulos[i].p[1].z + lista_triangulos[i].p[2].z) / 2.0;
			pinta = 1;
		}
		else if ((p1*p2) < 0 && (p1*p3) < 0) {
			ax = (lista_triangulos[i].p[0].x + lista_triangulos[i].p[2].x) / 2.0;
			ay = (lista_triangulos[i].p[0].y + lista_triangulos[i].p[2].y) / 2.0;
			az = (lista_triangulos[i].p[0].z + lista_triangulos[i].p[2].z) / 2.0;

			bx = (lista_triangulos[i].p[1].x + lista_triangulos[i].p[0].x) / 2.0;
			by = (lista_triangulos[i].p[1].y + lista_triangulos[i].p[0].y) / 2.0;
			bz = (lista_triangulos[i].p[1].z + lista_triangulos[i].p[0].z) / 2.0;
			pinta = 1;
		} 
		else if ((p1*p2) < 0 && (p2*p3) < 0) {
			ax = (lista_triangulos[i].p[1].x + lista_triangulos[i].p[2].x) / 2.0;
			ay = (lista_triangulos[i].p[1].y + lista_triangulos[i].p[2].y) / 2.0;
			az = (lista_triangulos[i].p[1].z + lista_triangulos[i].p[2].z) / 2.0;

			bx = (lista_triangulos[i].p[1].x + lista_triangulos[i].p[0].x) / 2.0;
			by = (lista_triangulos[i].p[1].y + lista_triangulos[i].p[0].y) / 2.0;
			bz = (lista_triangulos[i].p[1].z + lista_triangulos[i].p[0].z) / 2.0;
			pinta = 1;
		}
		// traça a linha da curva silhueta
		if (pinta == 1 && mostra_silhueta == 1) {
			glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_LINES);
				glVertex3f(ax, ay, az);
				glVertex3f(bx, by, bz);
			glEnd();
		}
		
	}
	
		
}

void usa_marching_cube() {
	quantidade_triangulos = 0;
	MarchingCube::TRIANGLE triangulos[10];
	int t_index = 0;
	for (int i = 0; i < NUM_CUBOS; i++) {
		int n_triangulos = marching.Polygonise(dados_cubos[i], 0.0, triangulos);
		quantidade_triangulos += n_triangulos;
		for (int i = 0; i < n_triangulos; i++) {
			if (mostra_curva == 1) {
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
			lista_triangulos[t_index].p[0] = triangulos[i].p[0];
			lista_triangulos[t_index].p[1] = triangulos[i].p[1];
			lista_triangulos[t_index].p[2] = triangulos[i].p[2];
			t_index++;
		}
	}
	// acha a normal de cada vértice do triangulo
	acha_normal();
	// calcula produto interno e traça a silhueta
	calcula_produto_interno();
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
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	/*
	ox = modelview[2];
	oy = modelview[6];
	oz = modelview[10];
	*/
	ox = modelview[8];
	oy = modelview[9];
	oz = modelview[10];
}
void redesenha()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (mostra_grid) {
		desenha_grid();
	}
	plota_eixos();
	usa_marching_cube();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	// mostra a curva
	case 'c':
		if (mostra_curva == 1) {
			mostra_curva = 0;
		}
		else {
			mostra_curva = 1;
		}
		inicia_config();
		redesenha();
		break;
	// mostra a silhueta
	case 's':
		if (mostra_silhueta == 1) {
			mostra_silhueta = 0;
		}
		else {
			mostra_silhueta = 1;
		}
		inicia_config();
		redesenha();
		break;
	case 'g':
		if (mostra_grid == 1) {
			mostra_grid = 0;
		}
		else {
			mostra_grid = 1;
		}
		
		inicia_config();
		redesenha();
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
int main(int argc, char **argv)
{
	
	cout << "CURVA SILHUETA" << endl;
	cout << "Digite:" << endl;
	cout << "1 para ESFERA" << endl << "0 para TORO" << endl;
	cin >> toro_ou_esfera;

	cout << "COMANDOS:" << endl;
	cout << "c -> mostra/oculta a curva resultante do marching cubes" << endl;
	cout << "s -> mostra/oculta a silhueta" << endl;
	cout << "g -> mostra/oculta o grid" << endl;

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