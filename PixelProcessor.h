//
// Created by Ian Taylor on 31/8/2020.
//
#ifndef SER334_MODULE_3_PIXELPROCESSOR_H
#define SER334_MODULE_3_PIXELPROCESSOR_H


//////////////////////////////////////////////////////////////////////
//Include Files
#include <stdint.h>


//////////////////////////////////////////////////////////////////////
//Type Definitions

struct Pixel{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};
//////////////////////////////////////////////////////////////////////
//Function Declarations

/**
 * Shift color of Pixel array. The dimension of the array is width * height. The shift value of RGB is
 * rShift, gShift，bShift. Useful for color shift.
 *
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 * @param  rShift: the shift value of color r shift
 * @param  gShift: the shift value of color g shift
 * @param  bShift: the shift value of color b shift
 */
void colorShiftPixels(struct Pixel** pArr, int width, int height, int rShift, int gShift, int bShift);

#endif //SER334_MODULE_3_PIXELPROCESSOR_H
