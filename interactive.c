#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "imageprocessing.h"
#include "bmp.h"

extern int ***alocare(int N, int M);

extern void dealocare(int ***m, int N , int M);

typedef struct imagine
{
    int ***ptr;
    int N;
    int M;
} imagine;

typedef struct filtru
{
    float **ptr;
    int size;
} filtru;

int main()
{
    // TODO Task 7
	char buffer[256];
    imagine *imagini=NULL;
    filtru *filtre = NULL;
    int size_imagini=0;
    int size_filtre = 0;
    while(1)
    {
        scanf("%s", buffer);
        if(strcmp(buffer, "e") == 0)
        {
            break;
        }
        else if(strcmp(buffer, "l") == 0)
        {
            int N, M;
            scanf("%d%d%s", &N, &M, buffer);
            int ***m = alocare(N, M);
            read_from_bmp(m, N, M, buffer);
            size_imagini++;
            imagini = realloc(imagini, sizeof(imagine) * size_imagini);
            imagini[size_imagini-1].ptr=m;
            imagini[size_imagini-1].N = N;
            imagini[size_imagini-1].M = M;
        }
        else if(strcmp(buffer,"s") == 0)
        {   
            int index;
            scanf("%d%s", &index, buffer);
            imagine i = imagini[index];
            write_to_bmp(i.ptr, i.N, i.M, buffer);

        }
        else if(strcmp(buffer, "ah") == 0)
        {
            int index;
            scanf("%d", &index);
            imagine i = imagini[index];
            imagini[index].ptr = flip_horizontal(i.ptr, i.N, i.M);

        }
        else if(strcmp(buffer, "ar") == 0)
        {
            int index;
            scanf("%d", &index);
            imagine i = imagini[index];
            imagini[index].ptr = rotate_left(i.ptr, i.N, i.M);
            imagini[index].N = i.M;
            imagini[index].M = i.N;
        }
        else if(strcmp(buffer,"ac") == 0)
        {
            int index, x, y, w, h;
            scanf("%d%d%d%d%d", &index, &x, &y, &w, &h);
            imagine i = imagini[index];
            imagini[index].ptr=crop(i.ptr, i.N, i.M, x, y, h, w);
            imagini[index].N = h;
            imagini[index].M = w;
        }
        else if (strcmp(buffer, "ae") == 0)
        {
            int index, rows, cols, R, G, B;
            scanf("%d%d%d%d%d%d", &index, &rows, &cols, &R, &G, &B);
            imagine i = imagini[index];
            imagini[index].ptr = extend(i.ptr, i.N, i.M, rows, cols, R, G, B);
            imagini[index].N = 2 * rows + i.N;
            imagini[index].M = 2 * cols + i.M;
        }
        else if (strcmp(buffer, "ap") == 0)
        {
            int index_dst, index_src, x, y;
            scanf("%d%d%d%d", &index_dst, &index_src, &x, &y);
            imagine i = imagini[index_dst], j = imagini[index_src];
            imagini[index_dst].ptr = paste(i.ptr, i.N, i.M, j.ptr, j.N, j.M, x, y);
        }
        else if (strcmp(buffer, "cf") == 0)
        {
            int size;
            scanf("%d", &size);
            filtru f;
            f.size = size;
            f.ptr = malloc(size * sizeof(float *));
            for(int i = 0; i < size; i++)
            {
                f.ptr[i] = malloc(size * sizeof(float));
                for(int j = 0; j < size; j++)
                {
                    scanf("%f", &f.ptr[i][j]);
                }
            }
            size_filtre++;
            filtre = realloc(filtre, sizeof(filtru) * size_filtre);
            filtre[size_filtre - 1] = f;
        }
        else if (strcmp(buffer, "af") == 0)
        {
            int index_img, index_filter;
            scanf("%d%d", &index_img, &index_filter);
            imagine i = imagini[index_img];
            filtru f = filtre[index_filter];
            imagini[index_img].ptr = apply_filter(i.ptr, i.N, i.M, f.ptr, f.size);
        }
        else if (strcmp(buffer, "df") == 0)
        {
            int index;
            scanf("%d", &index);
            filtru f = filtre[index];
            for(int i = 0; i < f.size; i++)
            {
                free(f.ptr[i]);
            }
            free(f.ptr);
            size_filtre--;
            for(int i = index; i < size_filtre; i++)
            {
                filtre[i] = filtre[i + 1];
            }
        }
        else if (strcmp(buffer, "di") == 0)
        {
            int index;
            scanf("%d", &index);
            imagine i = imagini[index];
            dealocare(i.ptr, i.N, i.M);
            size_imagini--;
            for(int i = index; i < size_imagini; i++)
            {
                imagini[i] = imagini[i + 1];
            }
        }
        
    }
    return 0;
}
