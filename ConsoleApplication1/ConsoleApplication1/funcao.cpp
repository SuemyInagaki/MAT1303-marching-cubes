#include <gl/glut.h>
#include "funcao.h"
void funcao::plota_funcao() {
	float x, y;
	float dx = (xmax - xmin) / points;
	float dy = (ymax - ymin) / points;
	glColor3f(1.0f, 1.0f, 1.0f);
	x = xmin;
	for (int i = 0; i < points; i++) {
		y = ymin;
		for (int j = 0; j < points; j++) {
			glBegin(GL_LINE_LOOP);
			glVertex3f(x, y, f(x, y));
			glVertex3f(x + dx, y, f(x + dx, y));
			glVertex3f(x + dx, y + dy, f(x + dx, y + dy));
			glVertex3f(x, y + dy, f(x, y + dy));
			glEnd();
			y += dy;
		}
		x += dx;
	}
}