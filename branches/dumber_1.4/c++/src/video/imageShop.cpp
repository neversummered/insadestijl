/*
 * camera.cpp
 *
 *  Created on: 18 janv. 2011
 *      Author: P.E. Hladik
 */

#include "imageShop.h"
#include "../data/arena.h"
#include "Image.h"

namespace robotInsa {

    void drawArena(Image img, Arena arena){
        drawBox(img.ipl(), arena.getBox());
    }

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

    void drawRec(IplImage *img, CvRect rect) {
        CvPoint2D32f pt[4];
        CvPoint pt1, pt0;
        int i;

        if (rect.height != 0) {
            pt[0].x = rect.x;
            pt[0].y = rect.y;
            pt[1].x = pt[0].x + rect.width;
            pt[1].y = pt[0].y;
            pt[2].x = pt[1].x;
            pt[2].y = pt[0].y + rect.height;
            pt[3].x = pt[0].x;
            pt[3].y = pt[2].y;
            for (i = 0; i < 4; i++) {
                pt0.x = cvRound(pt[i].x);
                pt0.y = cvRound(pt[i].y);
                pt1.x = cvRound(pt[(i + 1) % 4].x);
                pt1.y = cvRound(pt[(i + 1) % 4].y);
                cvLine(img, pt0, pt1, CV_RGB(255, 0, 0), 5, CV_AA, 0);
            }
        }

    }

    void drawPosition(Image img, Position pos) {
        CvScalar color = CV_RGB(0, 255, 0);

        CvPoint p1 = cvPoint((int) pos.getX(), (int) pos.getY());
        CvPoint p2 = cvPoint(p1.x + (int) 30 * cos(pos.getOrientation()),
                p1.y + (int) 30 * sin(pos.getOrientation()));

        cvCircle(img.ipl(), p1, 2, color, 5, CV_AA, 0);
        cvLine(img.ipl(), p1, p2, color, 1, CV_AA, 0);
    }

    void showImage(IplImage *img, char *title) {
        cvNamedWindow(title, CV_WINDOW_AUTOSIZE);
        cvShowImage(title, img);
        cvWaitKey(0);
    }
}
