#pragma once
class MarchingCube
{
private:

public:
	typedef struct {
		double x;
		double y;
		double z;
	} XYZ;

	typedef struct {
		XYZ p[3];
	} TRIANGLE;

	typedef struct {
		XYZ p[8]; // Coordenada dos 8 vértices de um cubo
		double val[8]; //Valor de f(x, y, z) para cada vértice do cubo
	} GRIDCELL;

	MarchingCube(){
	}
	XYZ VertexInterp(double isolevel, XYZ p1, XYZ p2, double valp1, double valp2);
	int Polygonise(GRIDCELL grid, double isolevel, TRIANGLE *triangles);
};

