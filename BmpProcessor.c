//
// Created by Ian Taylor on 31/8/2020.
//

//////////////////////////////////////////////////////////////////////
//Include Files
#include "BmpProcessor.h"
#include "PixelProcessor.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
//Type Definitions


//////////////////////////////////////////////////////////////////////
//Function Declarations

/**
 * read BMP header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination BMP header
 */
void readBMPHeader(FILE* file, struct BMP_Header* header){

    printf("In readBMPHeader()\n");
    fread(&header->signature, 2,1,file);
    // fread(header->signature, sizeof(char)*2,1,file);
    fread(&header->size, sizeof(int),1,file);
    fread(&header->reserved1, sizeof(short), 1, file);
    fread(&header->reserved2, sizeof(short), 1, file);
    fread(&header->offset_pixel_array, sizeof(int),1, file);

}

/**
 * write BMP header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: The header made by makeBMPHeader function
 */
void writeBMPHeader(FILE* file, struct BMP_Header* header){
    fwrite(&header->signature, 2, 1, file);
    fwrite(&header->size, sizeof(int),1,file);
    fwrite(&header->reserved1, sizeof(short), 1, file);
    fwrite(&header->reserved2, sizeof(short), 1, file);
    fwrite(&header->offset_pixel_array, sizeof(int),1, file);

}

/**
 * read DIB header from a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination DIB header
 */
void readDIBHeader(FILE* file, struct DIB_Header* header){
    fread(&header->DIB_header_size, sizeof(int),1,file);
    fread(&header->image_width, sizeof(int),1,file);
    fread(&header->image_height, sizeof(int), 1, file);
    fread(&header->num_of_planes, sizeof(short), 1, file);
    fread(&header->bits_per_pixel, sizeof(short),1, file);
    fread(&header->compression, sizeof(int),1, file);
    fread(&header->image_size_bytes, sizeof(int),1, file);
    fread(&header->x_resolution_ppm, sizeof(int),1, file);
    fread(&header->y_resolution_ppm, sizeof(int),1, file);
    fread(&header->num_colors, sizeof(int),1, file);
    fread(&header->important_colors, sizeof(int),1, file);
}

/**
* write DIB header of a file. Useful for converting files from PPM to BMP.
*
* @param  file: A pointer to the file being read or written
* @param  header: The header made by makeDIBHeader function
*/
void writeDIBHeader(FILE* file, struct DIB_Header* header){
    printf("In readDIBHeader()\n");

    //read the bitmap file header
    fwrite(&header->DIB_header_size, sizeof(int),1,file);
    fwrite(&header->image_width, sizeof(int),1,file);
    fwrite(&header->image_height, sizeof(int), 1, file);
    fwrite(&header->num_of_planes, sizeof(short), 1, file);
    fwrite(&header->bits_per_pixel, sizeof(short),1, file);
    fwrite(&header->compression, sizeof(int),1, file);
    fwrite(&header->image_size_bytes, sizeof(int),1, file);
    fwrite(&header->x_resolution_ppm, sizeof(int),1, file);
    fwrite(&header->y_resolution_ppm, sizeof(int),1, file);
    fwrite(&header->num_colors, sizeof(int),1, file);
    fwrite(&header->important_colors, sizeof(int),1, file);


}

/**
 * make BMP header based on width and height.
 * The purpose of this is to create a new BMPHeader struct using the information
 * from a PPMHeader when converting from PPM to BMP.
 *
 * @param  header: Pointer to the destination DIB header
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void makeBMPHeader(struct BMP_Header* header, int width, int height){

    header->signature[0] = 'B';
    header->signature[1] = 'M';
    header->signature[2] = '\0';
    header->size = ((width * height)*3) +54;
    header->reserved1 = 0;
    header->reserved2 = 0;
    header->offset_pixel_array = 54;

}


/**
* Makes new DIB header based on width and height. Useful for converting files from PPM to BMP.
*
* @param  header: Pointer to the destination DIB header
* @param  width: Width of the image that this header is for
* @param  height: Height of the image that this header is for
*/
void makeDIBHeader(struct DIB_Header* header, int width, int height){
    header->DIB_header_size = 40;
    header->image_width = width;
    header->image_height = height;
    header->num_of_planes = 1;
    header->bits_per_pixel = 24;
    header->compression = 0;
    header->image_size_bytes = ((width * height)*3);
    header->x_resolution_ppm = 3780;
    header->y_resolution_ppm = 3780;
    header->num_colors = 0;
    header->important_colors = 0;

    printf("---- Read in DIBHeader-----");
}

/**
 * read Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height){

    printf("In readPixelsBMP\n");

    int i, j, padding;
    char temp[4];
    padding = width % 4;

    if (padding != 0) {
        padding = (4-(3*width)%4)%4;
    }
    // read in data
    for(i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            fread(&pArr[i][j].blue, 1, 1, file);
            fread(&pArr[i][j].green, 1, 1, file);
            fread(&pArr[i][j].red, 1, 1, file);
        }
        if (padding != 0) {
            if (fread(&temp, padding, 1, file) != 1)
                printf("\nError reading padding in row %d \n", i);
        }
    }
}


/**
 * write Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height){
    int i, j, padding;
    char temp[4];
    padding = width % 4;

    if (padding != 0) {
        padding = (4-(3*width)%4)%4;
    }

    for(i = 0; i < width; i++){
        for(j =0; j < height; j++){
            //printf("%d, %d, %d\n", pArr[i][j].blue, pArr[i][j].green, pArr[i][j].red);
            fwrite(&pArr[i][j].blue, 1, 1, file);
            fwrite(&pArr[i][j].green, 1, 1, file);
            fwrite(&pArr[i][j].red, 1, 1, file);
        }
        if(padding != 0) {
            fwrite(&temp, padding, 1, file);
        }
    }
}

