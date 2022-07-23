/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				Abel Yagubyan
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"


int neighbors(uint32_t rule, int bit){
	if (rule & (1 << (bit - 1))){
       	return 1;
   	}else{
       	return 0;
   	}
}

int find_index(int a[], int value)
{
   int i;
   for (i=0; i<9; i++)
   {
	 if (a[i] == -2)
	 {
	    return -1;
	 }
	 if (a[i] == value)
	 {
	    return 1;
	 }
   }
   return -1;
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
    Color *ourcolor = malloc(sizeof(Color));
    if (ourcolor == NULL){
        return NULL;
    }
	int dead[9] = {};
	int alive[9] = {};
	int i,j,k,retval;
	j=0;k=0;
	for (i = 1; i < 19; i++){
		retval = neighbors(rule,i);
		if (i < 10){
		if (retval == 1){
			dead[j] = i-1;
			j++;
		}
		}else{
		if (retval == 1){
			alive[k] = i-10;
			k++;
		}
		}
	}
	while (j < 10){
		dead[j] = -2;
      	j++;
	}
	while (k < 10){
		alive[k] = -2;
      	k++;
	}

    uint8_t redval = image->image[row*image->cols+col]->R;
    uint8_t greenval = image->image[row*image->cols+col]->G;
    uint8_t blueval = image->image[row*image->cols+col]->B;

	uint32_t maxrows = image->rows;
	uint32_t maxcols = image->cols;
	uint32_t neigbor1,neigbor2,neigbor3,neigbor4,neigbor5,neigbor6,neigbor7,neigbor8;
	if (row == 0 && col == 0){
	neigbor1 = ((maxrows-1)*maxcols)+(maxcols-1);
	}else if (col == 0){
	neigbor1 = ((row-1)*maxcols)+(maxcols-1);
	}else if (row == 0){
	neigbor1 = ((maxrows-1)*maxcols)+(col-1);
	}else{
	neigbor1 = ((row-1)*maxcols)+col-1;
	}

	if (row == 0 && col == maxcols-1){
	neigbor2 = ((maxrows-1)*maxcols);
	}else if (row == 0){
	neigbor2 = ((maxrows-1)*maxcols)+col+1;
	}else if (col == maxcols-1){
	neigbor2 = ((row-1)*maxcols);
	}else{
	neigbor2 = ((row-1)*maxcols)+col+1;
	}

	if (row == 0){
		neigbor3 = ((maxrows-1)*maxcols)+col;
	}else{
		neigbor3 = ((row-1)*maxcols)+col;
	}

	if (col == maxcols-1){
		neigbor4 = (row*maxcols);
	}else{
		neigbor4 = (row*maxcols)+col+1;
	}

	if (col == 0 && row == maxrows-1){
	neigbor5 = maxcols-1;
	}else if (row == maxrows-1){
	neigbor5 = col-1;
	}else if (col == 0){
	neigbor5 = ((row+1)*maxcols)+maxcols-1;
	}else{
	neigbor5 = ((row+1)*maxcols)+col-1;
	}

	if (col == maxcols-1 && row == maxrows-1){
	neigbor6 = 0;
	}else if (row == maxrows-1){
	neigbor6 = col+1;
	}else if (col == maxcols-1){
	neigbor6 = ((row+1)*maxcols);
	}else{
	neigbor6 = ((row+1)*maxcols)+col+1;
	}

	if (row == maxrows-1){
		neigbor7 = col;
	}else{
		neigbor7 = ((row+1)*maxcols)+col;
	}

	if (col == 0){
		neigbor8 = (row*maxcols)+maxcols-1;
	}else{
		neigbor8 = (row*maxcols)+col-1;
	}

int counter = 0;
uint32_t neighborlst[8] = {neigbor1,neigbor2,neigbor3,neigbor4,neigbor5,neigbor6,neigbor7,neigbor8};
int ab;
for (i=0;i<3;i++){
	if (i==0){
		for (j=0;j<8;j++){
			retval = neighbors(redval,j+1);
			for (ab=0;ab<8;ab++){
			if (1 == neighbors(image->image[neighborlst[ab]]->R,j+1)){
				counter++;
			}
		}
			if (retval == 0){
				if(find_index(dead,counter) == 1){
					redval = (redval & ~(1 << j)) | (1 << j);
				}else{
					redval = (redval & ~(1 << j)) | (0 << j);
				}
			}else{
				if(find_index(alive,counter) == 1){
					redval = (redval & ~(1 << j)) | (1 << j);
				}else{
					redval = (redval & ~(1 << j)) | (0 << j);
				}
			}
			counter = 0;
		}
		(*ourcolor).R = redval;
	}else if (i==1){
		for (j=0;j<8;j++){
			retval = neighbors(greenval,j+1);	
			for (ab=0;ab<8;ab++){
			if (1 == neighbors(image->image[neighborlst[ab]]->G,j+1)){
				counter++;
			}
		}
			if (retval == 0){
				if(find_index(dead,counter) == 1){
					greenval = (greenval & ~(1 << j)) | (1 << j);
				}else{
					greenval = (greenval & ~(1 << j)) | (0 << j);
				}
			}else{
				if(find_index(alive,counter) == 1){
					greenval = (greenval & ~(1 << j)) | (1 << j);
				}else{
					greenval = (greenval & ~(1 << j)) | (0 << j);
				}
			}
			counter = 0;
		}
	(*ourcolor).G = greenval;
	}else if (i==2){
		for (j=0;j<8;j++){
			retval = neighbors(blueval,j+1);
			for (ab=0;ab<8;ab++){
			if (1 == neighbors(image->image[neighborlst[ab]]->B,j+1)){
				counter++;
			}
		}
			if (retval == 0){
				if(find_index(dead,counter) == 1){
					blueval = (blueval & ~(1 << j)) | (1 << j);
				}else{
					blueval = (blueval & ~(1 << j)) | (0 << j);
				}
			}else{
				if(find_index(alive,counter) == 1){
					blueval = (blueval & ~(1 << j)) | (1 << j);
				}else{
					blueval = (blueval & ~(1 << j)) | (0 << j);
				}
			}
			counter = 0;
		}
	(*ourcolor).B = blueval;
	}
}

    return ourcolor;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
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

	uint32_t i,j;
	for (i = 0;i < image->rows;i++){
		for (j = 0;j < image->cols;j++){
			Color *val = evaluateOneCell(image,i,j,rule);
            if (val == NULL){
            	uint32_t posit = (i*image->cols)+j;
                while (posit > 0){
                    free(ourcolors[posit-1]);
                    posit--;
                }
                free(ourcolors);
                free(img);
                freeImage(image);
                exit(-1);
            }
			*(ourcolors+(i*image->cols + j)) = val;
		}
	}
	img->image = ourcolors;
	return img;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.
argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc != 3){
        printf("usage: ./gameOfLife filename rule\n");
        printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
        printf("rule is a hex number beginning with 0x; Life is 0x1808.");
        exit(-1);
	}
	uint32_t number = (uint32_t)strtol(argv[2], NULL, 0);
	Image *img;
	Image *newimg;
	img = readData(argv[1]);
	newimg = life(img,number);
	writeData(newimg);
	freeImage(newimg);
	freeImage(img);
	return 0;
}
