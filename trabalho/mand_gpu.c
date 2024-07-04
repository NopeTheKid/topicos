// mandelbrot.c
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
    unsigned char r, g, b;
} Color;

Color getColor(int iter) {
    Color black = {0, 0, 0};
    Color grey = {155, 155, 155};

    Color colors[] = {
        grey, {25, 7, 26}, {9, 1, 47}, {4, 4, 73},
        {0, 7, 100}, {12, 44, 138}, {24, 82, 177}, {57, 125, 209},
        {134, 181, 229}, {211, 236, 248}, {241, 233, 191}, {248, 201, 95},
        {255, 170, 0}, {204, 128, 0}, {153, 87, 0}, black
    };
    int numColors = sizeof(colors) / sizeof(colors[0]);

    if (iter > 16)
        return colors[iter % numColors];
    else {
        return black;
    }
}

void mand(unsigned char *output) {
    clock_t t_inicio, t_final;
    t_inicio = clock();
    int num_devices = omp_get_num_devices();

    #pragma omp parallel num_threads(num_devices)
    {
        int dev = omp_get_thread_num();
        int delta_rows = HEIGHT / num_devices;
        unsigned char *output_dev = malloc(delta_rows * WIDTH * 3);

        #pragma omp target teams device(dev) distribute parallel for collapse(2)
        for (int y = dev * delta_rows; y < (dev + 1) * delta_rows; y++) {
            for (int x = 0; x < WIDTH; x++) {
                double zx = 0, zy = 0;
                double cx = (x - WIDTH / 2.0) * 4.0 / WIDTH;
                double cy = (y - HEIGHT / 2.0) * 4.0 / HEIGHT;
                int iter = 0;

                while (zx * zx + zy * zy < 4 && iter < MAX_ITER) {
                    double tmp = zx * zx - zy * zy + cx;
                    zy = 2 * zx * zy + cy;
                    zx = tmp;
                    iter++;
                }

                Color color = getColor(iter);
                int local_idx = ((y - dev * delta_rows) * WIDTH + x) * 3;
                output_dev[local_idx] = color.r;     // R
                output_dev[local_idx + 1] = color.g; // G
                output_dev[local_idx + 2] = color.b; // B
            }
        }

        #pragma omp critical
        {
            for (int y = 0; y < delta_rows; y++) {
                for (int x = 0; x < WIDTH; x++) {
                    int global_idx = ((dev * delta_rows + y) * WIDTH + x) * 3;
                    int local_idx = (y * WIDTH + x) * 3;
                    output[global_idx] = output_dev[local_idx];
                    output[global_idx + 1] = output_dev[local_idx + 1];
                    output[global_idx + 2] = output_dev[local_idx + 2];
                }
            }
        }

        free(output_dev);
    }

    t_final = clock();
    printf("Execution time: %g seconds\n", (double)(t_final - t_inicio) / CLOCKS_PER_SEC);
}

int main() {
    unsigned char *output = malloc(WIDTH * HEIGHT * 3);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    mand(output);

    if (!stbi_write_jpg("mand.jpg", WIDTH, HEIGHT, 3, output, 100)) {
        fprintf(stderr, "Error writing image\n");
        free(output);
        return 1;
    }

    free(output);
    printf("Image created!\n");
    return 0;
}
