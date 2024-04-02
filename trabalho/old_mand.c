#include <stdlib.h> 
#include <math.h>
#include <complex.h>

#define X 100
#define Y 100

double complex mand(double complex c, int count, double x)
{ 
    double complex cn = pow(c,2) + x + (I * count);
    if(count < 50){
        printf("[%d,%d] %.1f+%.1fi\n", x, count, creal(cn), cimag(cn));    

        mand(cn, ++count, x);
    }else{
        return cn;
    }
}

void main()
{
    double complex c = mand(0,0,0.3);
    
    printf("%.1f+%.1fi\n", creal(c), cimag(c));    
    return 0;  
}
