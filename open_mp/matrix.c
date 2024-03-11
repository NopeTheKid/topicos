#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <time.h>

#define N 3

void mult_matrix(void){
    double *x = (double*) malloc(sizeof(double)*N);
    double *y = (double*) malloc(sizeof(double)*N);
    double *b = (double*) malloc(sizeof(double)*N);
    double *A = (double*) malloc(sizeof(double)*N*N);

    #pragma omp parallel for
    for(int k=0; k<N; k++){
        x[k]=2.0;
        b[k]=1.0;
    }

    #pragma omp parallel for
    for (int line=0;line<N;line++) {
        for (int col=0;col<N;col++) {
            int idx = line * N + col;
            A[idx]=col+1;
        }
    }

    #pragma omp parallel for  
    for(int line=0; line<N; line++){
        int sum = 0.0;
     
    #pragma omp parallel for  
    for (int col = 0;col < N;col++) {
            int idx = line * N + col;
            sum += A[idx]*x[line];
        }
        y[line] = sum + b[line];
        printf("y[%d] = %g\n",line,y[line]);
    }

    free(x);
    free(y);    
    free(b);
    free(A);
}

int main(int argc, char *argv[])
{
    mult_matrix();
    return EXIT_SUCCESS;
}
