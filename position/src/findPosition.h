/* 
* File:   findPosition.h
* Author: P.-E. hladik
*
* Created on 20 décembre 2010, 21:10
*/

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#ifndef FINDPOSITION_H
#define	FINDPOSITION_H


void computeRobotPosition(IplImage *image);

void test(char *fileName);

/**
 * Load an image
 * @param fileName the name of the source file
 * @param img the pointer to the image
 */
void loadSrcImage(char *fileName);

/**
 * Resize an image according to the public ImageFactor
 */
void resizeWithFactor();

/**
 * Filter the image to extract the shape of the robot
 */
void hardFilter();

/**
 * Find the shape of the robot and compute its gravity center
 */
void findROI();

/* Applt the ROi to the source image */
void applyROI();

/* Filter the (ROI of) source image */
void filterROI();

/* Find the three spots for the robot's position */
void findPositionPoints();


/* compute the mass center of the spots  */
CvPoint2D32f computeTriangleMasscenter(CvPoint2D32f vertex[3]);

/* Compute tue orientation vector of the robot */
CvPoint2D32f computeOrientation(CvPoint2D32f vertex[3]);

/* Free the ROI of the image source and recompute the position 
 * in the image source reference */
void freeROI();

/* Compute the mass center of a contour in space delimited by a rectangular */
CvPoint2D32f computeMassCenter(CvSeq *c, CvRect *r) ;

/* Getters and setters */
void setImagefactor(int factor);
CvPoint2D32f getPosition();
CvPoint2D32f getOrientation();


#endif	/* FINDPOSITION_H */

