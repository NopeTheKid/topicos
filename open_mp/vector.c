#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <time.h>

#define N 1024

void mult_vector(void){
    double *x = (double*) malloc(sizeof(double)*N);
    double *y = (double*) malloc(sizeof(double)*N);
    const double a = 1.0;
    const double b = 2.0;

    #pragma omp parallel for
    for(int k=0; k<N; k++){
        x[k]=1.0;
    }

    #pragma omp parallel for  
    for(int k=0; k<N; k++){
        y[k] = a*x[k]+b;
        printf("y[%d] = %f\n",k,y[k]);
    }

    free(x);
    free(y);
}

int main(int argc, char *argv[])
{
    mult_vector();
    return EXIT_SUCCESS;
}
