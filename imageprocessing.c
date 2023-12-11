#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"
#define COLOR_LIMIT 255

int ***alocare(int N, int M) {
    int ***m = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        m[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            m[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    return m;
}

void dealocare(int ***m, int N , int M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(m[i][j]);
        }
        free(m[i]);
    }
    free(m);
}

int ***flip_horizontal(int ***image, int N, int M) {
    for (int i = 0; i < M / 2; i++) {
        for (int j = 0; j < N; j++) {
            int r = 0, g = 0, b = 0;
            r = image[j][i][0];
            g = image[j][i][1];
            b = image[j][i][2];
            image[j][i][0] = image[j][M-i-1][0];
            image[j][i][1] = image[j][M-i-1][1];
            image[j][i][2] = image[j][M-i-1][2];
            image[j][M-i-1][0] = r;
            image[j][M-i-1][1] = g;
            image[j][M-i-1][2] = b;
        }
    }
    return image;
}

int ***rotate_left(int ***image, int N, int M) {
    int ***m = alocare(M, N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            m[j][i][0] = image[i][M-j-1][0];
            m[j][i][1] = image[i][M-j-1][1];
            m[j][i][2] = image[i][M-j-1][2];
        }
    }
    dealocare(image, N, M);
    return m;
}

int ***crop(int ***image, int N, int M, int x, int y, int h, int w) {
    int ***m = alocare(h, w);
    for (int i = y; i < y + h; i++) {
        for (int j = x; j < x + w; j++) {
            m[i-y][j-x][0] = image[i][j][0];
            m[i-y][j-x][1] = image[i][j][1];
            m[i-y][j-x][2] = image[i][j][2];
        }
    }
    dealocare(image, N, M);
    return m;
}

int ***extend(int ***image, int N, int M, int rows, int cols, int new_R, int new_G, int new_B) {
    int ***m = (int ***)malloc((N + 2 * rows) * sizeof(int **));
    for (int i = 0; i < N + 2 * rows; i++) {
        m[i] = (int **)malloc((M + 2 * cols) * sizeof(int *));
        for (int j = 0; j < M + 2 * cols; j++) {
            m[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    for (int i = 0; i < N + 2 * rows; i++) {
        for (int j = 0; j < M + 2 * cols; j++) {
            m[i][j][0] = new_R;
            m[i][j][1] = new_G;
            m[i][j][2] = new_B;
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            m[i + rows][j + cols][0] = image[i][j][0];
            m[i + rows][j + cols][1] = image[i][j][1];
            m[i + rows][j + cols][2] = image[i][j][2];
        }
    }
    dealocare(image, N, M);
    return m;
}

int ***paste(int ***image_dst, int N_dst, int M_dst, int *** image_src, int N_src, int M_src, int x, int y) {
    for (int i = 0; i < N_src && i + y < N_dst; i++) {
        for (int j = 0; j < M_src && j + x < M_dst; j++) {
            for (int k = 0; k < 3; k++) {
                image_dst[i + y][j + x][k] = image_src[i][j][k];
            }
        }
    }
    return image_dst;
}

int float_to_pixel(float val) {
    int x = 0;
    x = (int) val;
    if (x < 0) x = 0;
    if (x > COLOR_LIMIT) x = COLOR_LIMIT;
    return x;
}
int ***apply_filter(int ***image, int N, int M, float **filter, int filter_size) {
    int ***m = alocare(N, M);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            float r = 0, g = 0, b = 0;
            for (int i1 = i - filter_size / 2; i1 <= i + filter_size / 2; i1++) {
                for (int j1 = j - filter_size / 2; j1 <= j + filter_size / 2; j1++) {
                    if (i1 < 0 || j1 < 0 || i1 >= N || j1 >= M) continue;
                    int i2 = i1 - i + filter_size / 2, j2 = j1 - j + filter_size / 2;
                    r += (float) image[i1][j1][0] * filter[i2][j2];
                    g += (float) image[i1][j1][1] * filter[i2][j2];
                    b += (float) image[i1][j1][2] * filter[i2][j2];
                }
            }
            m[i][j][0] = float_to_pixel(r);
            m[i][j][1] = float_to_pixel(g);
            m[i][j][2] = float_to_pixel(b);
        }
    }
    dealocare(image, N, M);
    return m;
}
