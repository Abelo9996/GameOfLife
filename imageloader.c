/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**      Abel Yagubyan
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
    if (filename == NULL){
        exit(-1);
    }
    Image *img;
    FILE *fp;
    int someval;
    fp = fopen(filename, "r");
    img = (Image *)malloc(sizeof(Image));
    if (img == NULL){
        exit(-1);
    }
    fscanf(fp, "P3 %u %u %u", &img->cols, &img->rows, &someval);
    Color **ourcolors = (Color **) malloc(img->cols * img->rows * sizeof(Color *));
    if (ourcolors == NULL){
        free(img);
        exit(-1);
    }
    uint32_t totalpixels = img->rows * img->cols;
    uint32_t i;
    for(i = 0; i < totalpixels; ++i)
    {
        Color *ourcolor = malloc(sizeof(Color));
        if (ourcolor == NULL){
            while (i > 0){
                free(ourcolors[i-1]);
                i--;
            }
            free(ourcolors);
            free(img);
            exit(-1);
        }
        uint8_t Red = 0;
        uint8_t Green = 0;
        uint8_t Blue = 0;
        fscanf(fp, "%hhu %hhu %hhu",&Red,&Green,&Blue);
        (*ourcolor).R = Red;
        (*ourcolor).G = Green;
        (*ourcolor).B = Blue;
        *(ourcolors+i) = ourcolor;
    }
    img->image = ourcolors;
    fclose(fp);
    return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
    uint32_t totalpixels = image->rows * image->cols;
    uint32_t colpixels = image->cols;
    printf("P3\n");
    printf("%u %u\n",image->cols,image->rows);
    printf("255\n");
    uint32_t i;
    for(i = 1; i < totalpixels+1; ++i)
    {
        if (i%colpixels == 0){
        printf("%3hhu %3hhu %3hhu\n", image->image[i-1]->R, image->image[i-1]->G, image->image[i-1]->B);
        }else{
            printf("%3hhu %3hhu %3hhu   ", image->image[i-1]->R, image->image[i-1]->G, image->image[i-1]->B);
        }
    }
}

//Frees an image
void freeImage(Image *image)
{
    uint32_t i;
    for (i=0;i<image->rows*image->cols;i++){
        free(image->image[i]);
    }
    free(image->image);
    free(image);
}
