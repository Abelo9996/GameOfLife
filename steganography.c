/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				Abel Yagubyan
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
    Color *ourcolor = malloc(sizeof(Color));
    if (ourcolor == NULL){
        return NULL;
    }
    uint8_t blueval = image->image[row*image->cols+col]->B;
    uint8_t LSB = blueval & 1;
    if (LSB == 0){
        (*ourcolor).R = 0;
        (*ourcolor).G = 0;
        (*ourcolor).B = 0;
    }else{
        (*ourcolor).R = 255;
        (*ourcolor).G = 255;
        (*ourcolor).B = 255;
    }
    return ourcolor;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	Image *img;
    img = (Image *)malloc(sizeof(Image));
    if (img == NULL) {
        freeImage(image);
        exit(-1);
    }
    img->rows = image->rows;
    img->cols = image->cols;
    Color **ourcolors = (Color **) malloc(image->cols * image->rows * sizeof(Color *));
    if (ourcolors == NULL){
        free(img);
        freeImage(image);
        exit(-1);
    }
    uint32_t totalpixels = image->rows * image->cols;
    uint32_t i,row,col;
    row = 0;
    col = 0;
    for(i = 0; i < totalpixels; i++)
    {
    	if (col == img->cols - 1){
            Color *val = evaluateOnePixel(image,row,col);
            if (val == NULL){
                while (i > 0){
                    free(ourcolors[i-1]);
                    i--;
                }
                free(ourcolors);
                free(img);
                freeImage(image);
                exit(-1);
            }
    		*(ourcolors+i) = val;
            col = 0;
            row ++;
    	}else{
            Color *val = evaluateOnePixel(image,row,col);
            if (val == NULL){
                while (i > 0){
                    free(ourcolors[i-1]);
                    i--;
                }
                free(ourcolors);
                free(img);
                freeImage(image);
                exit(-1);
            }
            *(ourcolors+i) = val;
            col ++;
    	}
    }
    img->image = ourcolors;
    return img;
}

/*
Loads a .ppm from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("usage: %s filename\n",argv[0]);
        printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
        exit(-1);
    }
	Image *img;
	Image *newimg;
	img = readData(argv[1]);
	newimg = steganography(img);
	writeData(newimg);
    freeImage(img);
    freeImage(newimg);
	return 0;
}
