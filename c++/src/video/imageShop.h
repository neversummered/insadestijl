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
#include "../data/arena.h"
#include "../video/Image.h"

namespace robotInsa {

    void drawArena(Image img, Arena arena);
    void drawPosition(Image img, Position pos);

    void drawBox(IplImage *img, CvBox2D box);
    void drawRec(IplImage *img, CvRect rec);
    void showImage(IplImage *img, char *title);
}
#endif
