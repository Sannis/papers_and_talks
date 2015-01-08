#include <cstdlib>
#include <cmath>
#include <cstdio>

int main(int argc, char** argv)
{
	if( argc != 2 ) {
		printf("Usage: marburger.e <p>\n");
		return 1;
	}
	
	double r = atof(argv[1]);
	
	double zf = 0.367/sqrt(pow(sqrt(r/3.77) - 0.852, 2) - 0.0219);
	
	printf("zf(%lf) = %lf\n", r, zf);
	
	return 0;
}