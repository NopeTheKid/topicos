#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <math.h>
#include <stdio.h>
#include <omp.h>

#define WIDTH 1920
#define HEIGHT 1080
#define MAX_ITER 100

void mand(unsigned char *output){
    for (int y = 0;y < HEIGHT; y++) {
        #pragma omp parallel for num_threads(63)
        for (int x = 0;x < WIDTH; x++) {
            double zx = 0, zy = 0;
            double cx = (x- WIDTH/2.0)*4. / WIDTH;
            double cy = (y- HEIGHT/2.0)*4. / HEIGHT;
            int iter = 0;

            while(zx*zx + zy*zy < 4 && iter < MAX_ITER){
                double tmp = zx * zx - zy * zy + cx;
                zy = 2 * zx * zy + cy;
                zx = tmp;
                iter++;
            }
            output[(y * WIDTH + x)] = iter;
        }
        printf("y %d\n",y);
    }
    printf("u");
}

int main() {
    unsigned char *output = malloc(WIDTH * HEIGHT*2);
    mand(output);
printf("u");
    if(!stbi_write_jpg("mand.jpg", WIDTH, HEIGHT, 3, output,1)){
        fprintf(stderr, "Erro ao escrever imagem\n");
        free(output);
        return 1;
    }

    free(output);
    printf("imagem feita!\n");
    return 0;
}
