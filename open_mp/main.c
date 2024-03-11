#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <time.h>

#define N 3

int old_main(int argc, char *argv[])
{
    int t_start = time(0);
    int t_end;
    int sum=0;

    #pragma omp parallel for num_threads(32) 
    for (int k=0; k<4*8; k++){
        sum += k;
        int tid = omp_get_thread_num();
        int t_start = omp_get_wtime();
        printf("Vai tu, k %d thread %d\n", k, tid);
        if (k%2) {
            sleep(5);  
        }else{
            sleep(2);  
        }
        int t_end = omp_get_wtime();

        printf("Tempo thread %d\n", t_end-t_start);
    }
  
    t_end = time(0);

    printf("Tempo total %d\n", t_end-t_start);
    printf("soma de k = %d\n", sum);

    return EXIT_SUCCESS;
}

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
    
    double t_start = time(0);

    #pragma omp parallel for  
    for(int line=0; line<N; line++){
        int sum = 0.0;
    
        #pragma omp parallel for reduction(+:sum) 
        for (int col = 0;col < N;col++) {
                int idx = line * N + col;
                sum += A[idx]*x[line];
                sleep(1);
            }
        y[line] = sum + b[line];
        printf("y[%d] = %g\n",line,y[line]);
    }


    double t_end = time(0);

    printf("Tempo %g\n",t_end-t_start);
    free(x);
    free(y);    
    free(b);
    free(A);
}

int main(int argc, char *argv[])
{
    //old_main();
    //mult_vector();
    mult_matrix();
}
