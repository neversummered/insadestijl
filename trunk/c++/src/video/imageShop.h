/*
 * camera.h
 *
 *  Created on: 18 janv. 2011
 *      Author: P.E. Hladik
 *  Inspired of code from http://www.siteduzero.com/ by NoHar
 */

#ifndef IMAGESHOP_H
#define IMAGESHOP_H

#include <opencv/highgui.h>
#include <opencv/cv.h>
#include "../data/position.h"

namespace robotInsa {
    
    void drawBox(IplImage *img, CvBox2D box);
    void drawPosition(IplImage *img, Position pos);
    void showImage(IplImage *img, char *title);
}
#endif
