/**
* Read in an image in from a BMP inout and applied a filter to it.
*
*
* @author Ian Taylor
* @version 1.0
*/

/**
 * OUTSTANDING...
 */

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BmpProcessor.h"
#include "PixelProcessor.h"
#include <math.h>
#include <pthread.h>


//TODO: finish me

//UNCOMMENT BELOW LINE IF USING SER334 LIBRARY/OBJECT FOR BMP SUPPORT
//#include "BmpProcessor.h"


////////////////////////////////////////////////////////////////////////////////
//MACRO DEFINITIONS

//problem assumptions
#define BMP_HEADER_SIZE 14
#define BMP_DIB_HEADER_SIZE 40
#define MAXIMUM_IMAGE_SIZE 4096
#define THREAD_COUNT 4


//TODO: finish me


////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES

typedef struct blurInfo {
    int start_col, end_col, width, height;
    struct Pixel** pArr;
} blurInfo;

struct Pixel** globalArr;

////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE
/**
 * This method is called by threads and used to blur an image
 * @param param
 * @return 0
 */
void* runnerBlur (void* param){
    int red, green, blue;
    int counter = 0;
    int i, j, z;

    int start_col, end_col, width;

    blurInfo* data = (blurInfo*) param;

    start_col = data->start_col;
    end_col = data->end_col + 1;
    width = data->width;

    printf("data->start_col = %d, data->end_col = %d\n", start_col, end_col);

    for(i = 0; i < width; i++) {
        for (j = start_col; j <= end_col; j++) {
            red = green = blue = 0;
            counter = 0;

            // row above, col to the left
            if (i + 1 < width && j - 1 >= start_col) {
                red += data->pArr[i + 1][j - 1].red;
                green += data->pArr[i + 1][j - 1].green;
                blue += data->pArr[i + 1][j - 1].blue;
                counter++;
            }
            // row above, col to the right
            if (i + 1 < width && j + 1 < end_col) {
                red += data->pArr[i + 1][j + 1].red;
                green += data->pArr[i + 1][j + 1].green;
                blue += data->pArr[i + 1][j + 1].blue;
                counter++;
            }
            // row above, col to the center
            if (i + 1 < width) {
                red += data->pArr[i + 1][j].red;
                green += data->pArr[i + 1][j].green;
                blue += data->pArr[i + 1][j].blue;
                counter++;
            }

            // same row, col to the right
            if (j + 1 < end_col) {
                red += data->pArr[i][j + 1].red;
                green += data->pArr[i][j + 1].green;
                blue += data->pArr[i][j + 1].blue;
                counter++;
            }

            // same row, col to the left
            if (j - 1 >= start_col) {
                red += data->pArr[i][j - 1].red;
                green += data->pArr[i][j - 1].green;
                blue += data->pArr[i][j - 1].blue;
                counter++;
            }

            // add the one in question
            red += data->pArr[i][j].red;
            green += data->pArr[i][j].green;
            blue += data->pArr[i][j].blue;
            counter++;

            // row below, col to the left
            if (i - 1 >= 0 && j - 1 >= start_col) {
                red += data->pArr[i - 1][j - 1].red;
                green += data->pArr[i - 1][j - 1].green;
                blue += data->pArr[i - 1][j - 1].blue;
                counter++;
            }
            // row below, col to the left
            if (i - 1 >= 0 && j + 1 < end_col) {
                red += data->pArr[i - 1][j + 1].red;
                green += data->pArr[i - 1][j + 1].green;
                blue += data->pArr[i - 1][j + 1].blue;
                counter++;
            }
            // row below, col to the left
            if (i - 1 >= 0) {
                red += data->pArr[i - 1][j].red;
                green += data->pArr[i - 1][j].green;
                blue += data->pArr[i - 1][j].blue;
                counter++;
            }
            globalArr[i][j].red = (int)round(red/counter) ;
            globalArr[i][j].green = (int)round(green/counter);
            globalArr[i][j].blue = (int)round( blue/counter );
        }
    }
    return 0;
}

/**
 * Creates large holes in the image
 * @param param - data of the image
 * @return
 */
void* runnerCheeseFilterLargeHoles (void* param){
    int s, centerRow, centerCol, i, j;

    blurInfo* data = (blurInfo*) param;

    // radius for the holes
    int largerRadius = (int) round(data->width * 0.15);

    // get 10% of width to find number of holes
    int numOfHoles = (int) round(data->height * 0.1);


    //int largeHoles = (int) round(numOfHoles * 0.25);
    int largeHoles = (numOfHoles * 0.25);

    printf("Number of largeHoles: %d \n", largeHoles);

    for(s = 0; s <  largeHoles; s ++) {
        centerRow = rand() % data->width;
        centerCol = rand() % data->height;

        for(i = 0; i < data->width; i++) {
            for (j = 0; j < data->height; j++) {
                double d = sqrt(((i - centerRow)*(i-centerRow)) +((j-centerCol)*(j-centerCol)));
                if(d < largerRadius){
                    globalArr[i][j].red = 0;
                    globalArr[i][j].green = 0;
                    globalArr[i][j].blue = 0;
                }
            }
        }
    }
    return 0;
}

/**
 * Creates small holes in the image
 * @param param - struct holding data of the image
 * @return - 0
 */
void* runnerCheeseFilterSmallHoles (void* param){
    int s, centerRow, centerCol, i, j;
    blurInfo* data = (blurInfo*) param;

    int smallRadius = (int) round(data->width * 0.05);
    int numOfHoles = (int) round(data->height * 0.1);

    int smallHoles = (int) round(numOfHoles * 0.25);
    // int smallHoles = (numOfHoles - (numOfHoles - (numOfHoles / 2)) / 2);
    printf("Number of smallHoles: %d \n", smallHoles);

    for(s = 0; s <  smallHoles; s ++) {
        centerRow = rand() % data->width;
        centerCol = rand() % data->height;

        for(i = 0; i < data->width; i++) {
            for (j = 0; j < data->height; j++) {
                double d = sqrt(((i - centerRow)*(i-centerRow)) +((j-centerCol)*(j-centerCol)));
                if(d < smallRadius){
                    globalArr[i][j].red = 0;
                    globalArr[i][j].green = 0;
                    globalArr[i][j].blue = 0;
                }
            }
        }
    }
    return 0;
}
/**
 * Creates average sized holes on the image
 * @param param - data struct holding info on image
 * @return
 */
void* runnerCheeseFilterAverHoles (void* param){
    printf("\nIn runnerCheeseFilterAverHoles\n");
    blurInfo* data = (blurInfo*) param;
    int s, centerRow, centerCol, i, j;

    // Average holes
    int averRadius = (int) round(data->width * 0.1);
    int numOfHoles = (int) round(data->height * 0.1);
    int avergHoles = (int) round(numOfHoles * 0.5);

    srand(time(0));

    for(s = 0; s <  avergHoles; s ++) {
        centerRow = rand() % (data->width + 1 - 0);
        centerCol = rand() % (data->height + 1 - 0);

        for(i = 0; i < data->width; i++) {
            for (j = 0; j < data->height; j++) {
                double d = sqrt(((i - centerRow)*(i-centerRow)) +((j-centerCol)*(j-centerCol)));
                if(d < averRadius){
                    globalArr[i][j].red = 0;
                    globalArr[i][j].green = 0;
                    globalArr[i][j].blue = 0;
                }
            }
        }
    }
    return 0;
}

/**
 * Function used to copy and edit bpm file
 * @param inPutFileName
 * @param outPutFileName
 * @param filter
 */
void copyBMPFiles(char* inPutFileName, char* outPutFileName, int filter){
    printf("--- In copyBMPFiles function. Copy file from %s to %s \n", inPutFileName, outPutFileName);

    FILE *fptr1;
    fptr1 = fopen(inPutFileName, "rb");
    if (fptr1 == NULL) {
        printf("Cannot open file %s \n", inPutFileName);
        exit(0);
    } else{
        printf("File to be edited/read is open\n");
    }
    // Create a struct
    struct BMP_Header* bmpHeader;
    bmpHeader = (struct BMP_Header*)malloc(sizeof(struct BMP_Header));
    readBMPHeader(fptr1, bmpHeader); // read Header info from file

    // read in DIB_Header info from file
    struct DIB_Header* DIB_header;
    DIB_header = (struct DIB_Header*)malloc(sizeof(struct DIB_Header));
    readDIBHeader(fptr1, DIB_header);

    // This is for information store in a struct to be passed to the thread function
    blurInfo blurData;
    blurData.pArr = (struct Pixel**) malloc(sizeof(struct Pixel*) * DIB_header->image_width);
    int i;
    for(i = 0; i < DIB_header->image_width; i++){
        blurData.pArr[i] = malloc(sizeof(struct Pixel) * DIB_header->image_height);
    }
    blurData.width = DIB_header->image_width;
    blurData.height = DIB_header->image_height;

    // Lets initialise the globalArray to hold the pixels with the filter applied
    globalArr = (struct Pixel**) malloc(sizeof(struct Pixel*) * DIB_header->image_width);
    int gA;
    for(gA = 0; gA < DIB_header->image_width; gA++){
        globalArr[gA] = malloc(sizeof(struct Pixel) * DIB_header->image_height);
    }

    // Read in pixels and store in the struct array
    readPixelsBMP(fptr1, blurData.pArr, DIB_header->image_width, DIB_header->image_height);
    // close the read in file
    fclose(fptr1);

    // carry out blur filter if argument is 0 else carry out swiss cheese filter
    if (filter == 0){
        printf("in filter\n");
        int d, col_length;
        int remainder = DIB_header->image_width % THREAD_COUNT;

        if(remainder != 0){
            printf("remained is: %d", remainder);
            //DIB_header->image_width - remainder;
            col_length = (DIB_header->image_width - remainder) / THREAD_COUNT;
        }else {
            col_length = DIB_header->image_width/THREAD_COUNT;
        }
        pthread_t arrOfIDs[THREAD_COUNT];
        pthread_attr_t arrOfARGS[THREAD_COUNT];

        for(d = 0; d < THREAD_COUNT; d++) {
            blurData.start_col = d * col_length;
            if (blurData.start_col > 0)
                blurData.start_col -= 1;

            blurData.end_col = ((d+1) * col_length); // -1
            if ( d == THREAD_COUNT-1 && remainder != 0){
                printf("Should not fire");
                blurData.end_col += remainder;
            }

            pthread_attr_init(&arrOfARGS[d]);
            pthread_create(&arrOfIDs[d] , &arrOfARGS[d] , runnerBlur, &blurData);
            pthread_join(arrOfIDs[d] , NULL);
        }
    } else {
        int col_length, d;

        // apply yellow tint.
        colorShiftPixels(blurData.pArr, DIB_header->image_width, DIB_header->image_height,
                         50, 50, 0);
        printf("calling function in thread");

        pthread_t tid1, tid2, tid3;
        pthread_attr_t attr1, attr2, attr3;

        pthread_attr_init(&attr1);
        pthread_attr_init(&attr2);
        pthread_attr_init(&attr3);

        pthread_create (&tid1 , &attr1 , runnerCheeseFilterAverHoles, &blurData);
        pthread_create (&tid2 , &attr2 , runnerCheeseFilterSmallHoles, &blurData);
        pthread_create (&tid3 , &attr3 , runnerCheeseFilterLargeHoles, &blurData);

        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);
        pthread_join(tid3, NULL);
    }

    printf("\n-------------------------------\n");

    // Open bmp file to read
    FILE *outPutPtr;
    outPutPtr = fopen(outPutFileName, "wb");

    if (outPutPtr == NULL) {
        printf("Cannot open file %s \n", outPutFileName);
        exit(0);
    } else{
        printf("File to be written to is open...\n");
    }

    writeBMPHeader(outPutPtr, bmpHeader);

    writeDIBHeader(outPutPtr, DIB_header);

    //writePixelsBMP(outPutPtr, pArr, DIB_header->image_width, DIB_header->image_height);
    writePixelsBMP(outPutPtr, globalArr, DIB_header->image_width, DIB_header->image_height);

    fclose(outPutPtr);

    printf("That is the function completed!\n");

    free(*blurData.pArr);
    *blurData.pArr = NULL;

    free(bmpHeader);
    bmpHeader = NULL;

    free(DIB_header);
    DIB_header = NULL;

    fclose(outPutPtr);
}

/**
 * Main
 * @param argc
 * @param argv - input file, output file and type of filter
 */
void main(int argc, char* argv[]) {
    char* inPutFileName = NULL;
    char* outPutFileName = NULL;
    char* filter = NULL;

    //TODO: finish me
    int z;
    for(z = 1; z < argc; z++){
        if (0 == (strcmp(argv[z], "-i"))) {
            printf("Found -i!\n");
            inPutFileName = argv[z+1];
        } else if (0 == (strcmp(argv[z], "-o"))){
            printf("Found -o!\n");
            outPutFileName = argv[z+1];
        } else if (0 == (strcmp(argv[z], "-f"))){
            printf("Found -f!\n");
            filter = argv[z+1];
        }
    }

    printf("\n-i =%s\t -o =%s\t -f =%s\t\n", inPutFileName, outPutFileName, filter);
    if(0 == (strcmp(filter, "b"))){
        printf("Implement a Blur filter\n");
        copyBMPFiles(inPutFileName, outPutFileName, 0);
    } else {
        printf("Implement a cheese");
        copyBMPFiles(inPutFileName, outPutFileName, 5);
    }
}