#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <math.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define MAX_ITER 256

void mand(unsigned char *output){
    for (int y = 0;y < HEIGHT; y++) {
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
            output[(y * WIDTH + x)*3] = iter;
            output[(y * WIDTH + x)*3+1] = iter;            
            output[(y * WIDTH + x)*3+2] = iter;
        }
    }
}

int main() {
    unsigned char *output = malloc(WIDTH * HEIGHT * 3);
    mand(output);

    if(!stbi_write_jpg("mand.jpg", WIDTH, HEIGHT, 3, output,1)){
        fprintf(stderr, "Erro ao escrever imagem\n");
        free(output);
        return 1;
    }

    free(output);
    printf("imagem feita!\n");
    return 0;
}
