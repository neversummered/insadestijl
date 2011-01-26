/*
 * camera.cpp
 *
 *  Created on: 18 janv. 2011
 *      Author: P.E. Hladik
 *  Inspired of code from http://www.siteduzero.com/ by NoHar
 */

#include "imageShop.h"

namespace robotInsa {

    void drawBox(IplImage *img, CvBox2D box) {
        CvPoint2D32f box_vtx[4];
        CvPoint pt, pt0;
        int i;

        if (box.size.height != 0) {
            cvBoxPoints(box, box_vtx); // Finds box vertices
            for (i = 0; i < 4; i++) {
                pt0.x = cvRound(box_vtx[i].x);
                pt0.y = cvRound(box_vtx[i].y);
                pt.x = cvRound(box_vtx[(i + 1) % 4].x);
                pt.y = cvRound(box_vtx[(i + 1) % 4].y);
                cvLine(img, pt0, pt, CV_RGB(0, 255, 0), 5, CV_AA, 0);
            }
        }
    }

    void drawPosition(IplImage* img, Position pos) {
        CvScalar color = CV_RGB(0, 255, 0);

        CvPoint p1 = cvPoint((int) pos.getX(), (int) pos.getY());
        CvPoint p2 = cvPoint(p1.x + (int) 30 * cos(pos.getOrientation()),
                p1.y + (int) 30 * sin(pos.getOrientation()));

        cvCircle(img, p1, 2, color, 5, CV_AA, 0);
        cvLine(img, p1, p2, color, 1, CV_AA, 0);
    }

    void showImage(IplImage *img, char *title) {
        cvNamedWindow(title, CV_WINDOW_AUTOSIZE);
        cvShowImage(title, img);
        cvWaitKey(0);
    }
}
