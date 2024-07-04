#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <math.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
 
#define WIDTH 7680
#define HEIGHT 4320
//#define WIDTH 1920
//#define HEIGHT 1080
#define MAX_ITER 1000

typedef struct {
    unsigned char r,g,b
} Color;

Color getColor(int iter){
    Color black;
    black.r = 0;
    black.g = 0;
    black.b = 0;
    Color grey;
    grey.r = 155;
    grey.g = 155;
    grey.b = 155;
    
    Color colors[] = {
        grey, {25, 7, 26}, {9, 1, 47}, {4, 4, 73},
        {0, 7, 100}, {12, 44, 138}, {24, 82, 177}, {57, 125, 209},
        {134, 181, 229}, {211, 236, 248}, {241, 233, 191}, {248, 201, 95},
        {255, 170, 0}, {204, 128, 0}, {153, 87, 0}, black
    };
    int numColors = sizeof(colors)/sizeof(colors[0]);
   
    if(iter > 16)
        return colors[iter % numColors];
    else {
        return black;
    }
}

void mand_device(unsigned char *output,
                 unsigned char *output_dev, 
                 int nrows, int ncols,
                 int dev, int num_devices){
    const int delta_rows = nrows / num_devices;
    int cc=0;
    for (int row = dev*delta_rows; row < delta_rows*(dev+1); row++) {
        for (int col=0; col < ncols; col++) {
            int idx = (row * WIDTH + col)*3;
            output_dev[cc] = (int) output[idx];
            cc++;
        }
    }
}

void mand(unsigned char *output){
    clock_t t_inicio, t_final;
    int num_devices = 4;
    t_inicio = clock();
    for (int dev=0;dev < num_devices; dev++) {
        unsigned char *output_dev = malloc((WIDTH) * (HEIGHT) * 3);

        #pragma omp target teams device(dev) distribute parallel for collapse(1)
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

                Color color = getColor(iter);
                output[(y * WIDTH + x)*3] = color.r;     // R
                output[(y * WIDTH + x)*3+1] = color.g;   // G
                output[(y * WIDTH + x)*3+2] = color.b;   // B
            } 
        }

        mand_device(output, output_dev, HEIGHT, WIDTH, dev, num_devices);
        free(output_dev);
    }

    t_final = clock();
    printf("Tempo de execução %g segundos\n",(double)(t_final-t_inicio) / CLOCKS_PER_SEC);
}
 
int main() {
    unsigned char *output = malloc((WIDTH) * (HEIGHT) * 3);
    mand(output);

    if(!stbi_write_jpg("mand.jpg", WIDTH, HEIGHT, 3, output, 100)){
        fprintf(stderr, "Erro ao escrever imagem\n");
        free(output);
        return 1;
    }

    free(output);
    printf("imagem feita!\n");
    return 0;
}
