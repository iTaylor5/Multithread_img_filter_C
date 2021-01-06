//
// Created by Ian Taylor on 31/8/2020.
//

#ifndef SER334_MODULE_3_BMPPROCESSOR_H
#define SER334_MODULE_3_BMPPROCESSOR_H

//////////////////////////////////////////////////////////////////////
//Include Files
#include <stdio.h>
#include <stdlib.h>
#include "PixelProcessor.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////
//Type Definitions
struct BMP_Header {
    char signature[3];      // ID field : 0x4d42
    //uint16_t  signature;
    int size;               // File size in bytes
    short reserved1;        // Application specific
    short reserved2;        // Application specific
    int offset_pixel_array; // Offset to image data in bytes from beginning of file (54 bytes)

};

struct DIB_Header{
    int DIB_header_size;    // DIB Header size in bytes (40 bytes)
    int image_width;        // Width of the image
    int image_height;       // Height of image
    short num_of_planes;      // Number of color planes
    short bits_per_pixel;     // Bits per pixel
    int compression;        // Compression type
    int image_size_bytes;   // Image size in bytes
    int x_resolution_ppm;   // Pixels per meter x
    int y_resolution_ppm;   // Pixels per meter y
    int num_colors;         // NUmber of colors
    int important_colors;   // Important colors

};

//////////////////////////////////////////////////////////////////////
//Function Declarations
/**
 * read BMP header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination BMP header
 */
void readBMPHeader(FILE* file, struct BMP_Header* header);

/**
 * write BMP header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: The header made by makeBMPHeader function
 */
void writeBMPHeader(FILE* file, struct BMP_Header* header);

/**
 * read DIB header from a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination DIB header
 */
void readDIBHeader(FILE* file, struct DIB_Header* header);

/**
 * write DIB header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: The header made by makeDIBHeader function
 */
void writeDIBHeader(FILE* file, struct DIB_Header* header);

/**
 * make BMP header based on width and height.
 * The purpose of this is to create a new BMPHeader struct using the information
 * from a PPMHeader when converting from PPM to BMP.
 *
 * @param  header: Pointer to the destination DIB header
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void makeBMPHeader(struct BMP_Header* header, int width, int height);


/**
* Makes new DIB header based on width and height. Useful for converting files from PPM to BMP.
*
* @param  header: Pointer to the destination DIB header
* @param  width: Width of the image that this header is for
* @param  height: Height of the image that this header is for
*/
void makeDIBHeader(struct DIB_Header* header, int width, int height);


/**
 * read Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height);


/**
 * write Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height);


#endif //SER334_MODULE_3_BMPPROCESSOR_H
