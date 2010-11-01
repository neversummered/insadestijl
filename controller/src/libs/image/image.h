/*
 * image.h
 *
 *  Created on: 1 nov. 2010
 *      Author: pehladik
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include "/home/pehladik/Projects/IP_test/IP_test/communication/message.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#include <jpeglib.h>

#include <opencv/highgui.h>
#include <opencv/cv.h>

#define _DEBUG_

/**
 * Initialize the webcam
 * @param *mCpature points to the cvCapture structure to initialize
 */
void Init_Webcam();

/**
 * Take picture with a webcam
 * @param *mCapture points to the webcam to take picture
 * @param *frame is the frame with the picture
 */
void Take_Picture_On_Webcam();

/**
 * Initialize and take a picture
 * @param *frame is the frame with the picture
 * @value return the duration to
 */
double Take_Picture_On_Webcam2();

/**
 * Load a image in a frame
 * @param filename is the path and name of the picture
 * @param frameDest is the frame which encode the picture
 */
void Load_Image(const char * filename);

/**
 * Convert a frame to a jpeg byte array
 * @param frameSrc is the frame to convert
 * @param bufDest is the buffer
 */
void Convert_To_Jpeg();

/**
 * Convert a buffered jpeg image in a message
 * @param bufSrc is the buffer with the jpeg image
 * @param pMsg points to the message
 */
void Convert_To_Msg(MESSAGE *pMsg);

/**
 * frame getter
 */
IplImage * get_Frame();

/**
 * jpeg buffer getter
 */
CvMat * get_Jpeg_Buffer();

#endif /* IMAGE_H_ */
