#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double calcular_area(double a, 
					 double b,
					 int n,		// n de pontos no calculo da soma
					 double (*fptr)(double)){
	double dx = (double) (b-a)/(double) n;
	double sum = 0.0;
	double x = a;
	for(int k=0; k<n; k++){
		sum += fptr(x) * dx;
		x += dx;
	}
	return sum;
}

double f(double x){
	double y = sqrt(1.0 -x*x);
	return y;
}

int main(){
	printf("Calcular Pi\n");

	double a = 0.0;
	double b = 1.0;
	int n = 1000000;
	
	double y = calcular_area(a,b,n,&f);
	double pi = 4*y;

	printf("%g\n", pi);
	
	return EXIT_SUCCESS;
}



