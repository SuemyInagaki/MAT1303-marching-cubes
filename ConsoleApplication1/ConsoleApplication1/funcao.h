#include <cmath>
#include <iostream>

using namespace std;
class funcao {
	float xmin, xmax, ymin, ymax, zmin, zmax;
	int points;
public:
	funcao(float xm = -1, float xM = 1, float ym = -1, float yM =
		1, float zm = -1, float zM = 1, int p = 10) {
		xmin = xm;
		ymin = ym;
		xmax = xM;
		ymax = yM;
		zmin = zm;
		zmax = zM;
		points = p;
	}

	float f(float x, float y) {
		// return(x*x - y * y);
		return (x*x + y * y);
	}

	// torus
	
	float f2_t(float x, float y, float z) {
		double t1 = 1.0 - pow((x*x + y * y), (1.0 / 2.0));
		return ((t1*t1) + z*z - 0.5*0.5);
	}

	void f2_gradiente_t(float x, float y, float z, float *nx, float *ny, float *nz) {
		float raiz = sqrt(x*x + y * y);
		*nx = -2.0*x*(-1*raiz + 1.0)/raiz;
		*ny = -2.0*y*(-1 * raiz + 1.0) / raiz;
		*nz = 2 * z;
	}

	// esfera
	
	float f2(float x, float y, float z) {
		return (x*x + y * y + z * z - 1.0);
	}
	
	void f2_gradiente(float x, float y, float z, float *nx, float *ny, float *nz) {
		*nx = 2 * x;
		*ny = 2 * y;
		*nz = 2 * z;
	}
	
	void plota_funcao();
};