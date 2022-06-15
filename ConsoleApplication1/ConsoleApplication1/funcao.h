class funcao {
	float xmin, xmax, ymin, ymax;
	int points;
public:
	funcao(float xm = -1, float xM = 1, float ym = -1, float yM =
		1, int p = 10) {
		xmin = xm;
		ymin = ym;
		xmax = xM;
		ymax = yM;
		points = p;
	}

	float f(float x, float y) {
		// return(x*x - y * y);
		return (x*x + y * y);
	}

	float f2(float x, float y, float z) {
		return (x*x + y * y + z * z - 1.0);
	}

	void plota_funcao();
};