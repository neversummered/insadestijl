/* 
 * File:   show.h
 * Author: Piro
 *
 * Created on 21 décembre 2010, 10:38
 */

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#ifndef SHOW_H
#define	SHOW_H

/**
 * Draws an image
 * @param image the image to draw
 * @param name the name of the show window
 * @param wait a boolean value to wait an enter key or not
 */
void showImage(IplImage *image, char *name, bool wait);

/**
 * Draws a point on an image
 * @param image the background image
 * @param p the point to draw
 * @param wait a boolean value to wait an enter key or not
 */
void showPoint(IplImage *image, CvPoint p, bool wait);

void showPixel(IplImage *image, CvPoint p, bool wait);

void showPixel2D32F(IplImage *image, CvPoint2D32f p, bool wait);

void showLine(IplImage *image, CvPoint p1, CvPoint p2, bool wait);

void showLine2D32F(IplImage *image, CvPoint2D32f p1, CvPoint2D32f p2, bool wait);

/**
 * Draws a box on an image
 * @param image the background image
 * @param box the box to draw
 * @param wait a boolean value to wait an enter key or not
 */
void showBox(IplImage *image, CvBox2D box, bool wait);

/**
 * Draws a rectangle on an image
 * @param image the background image
 * @param rect the rectangle to draw
 * @param wait a boolean value to wait an enter key or not
 */
void showRectangle(IplImage *image, CvRect rect, bool wait);

/**
 * Draws contours on an image
 * @param image the background image
 * @param contour the contours to draw
 * @param wait a boolean value to wait an enter key or not
 */
void showContours(IplImage *image, CvSeq* contour, bool wait);

void showContours2(IplImage *image, CvSeq* contour, bool wait);

CvPoint cvpoint2d32fTocvpoint(CvPoint2D32f p);

#endif	/* SHOW_H */

