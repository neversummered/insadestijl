#include "show.h"
#include <stdio.h>

void showImage(IplImage *image, char *name, bool wait) {
    cvNamedWindow(name, CV_WINDOW_AUTOSIZE);

    cvShowImage(name, image);
    if (wait)
        cvWaitKey(0);
}

void showPoint(IplImage *image, CvPoint p, bool wait) {
    IplImage *temp = cvCloneImage(image);

    printf("x:%d, y:%d\n", p.x, p.y);
    cvCircle(temp, p, 2, CV_RGB(0, 255, 0), 5, CV_AA, 0);

    showImage(temp, "Point", wait);
}

void showPixel(IplImage *image, CvPoint p, bool wait) {
    IplImage *tmp = cvCloneImage(image);

    CvScalar color = CV_RGB(0, 255, 0);
    cvSet2D(tmp, p.y, p.x, color);

    showImage(tmp, "Pixel", wait);
}

void showPixel2D32F(IplImage *image, CvPoint2D32f p, bool wait) {
    CvPoint pt = cvpoint2d32fTocvpoint(p);
    
    showPixel(image, pt, true);
}


void showLine(IplImage *image, CvPoint p1, CvPoint p2, bool wait) {
    IplImage *tmp = cvCloneImage(image);

    CvScalar color = CV_RGB(0, 255, 0);
    cvLine(tmp, p1, p2, color, 1, CV_AA, 0);

    showImage(tmp, "Line", wait);
}

void showLine2D32F(IplImage *image, CvPoint2D32f p1, CvPoint2D32f p2, bool wait) {
    
    CvPoint pt1 = cvpoint2d32fTocvpoint(p1);
    CvPoint pt2 = cvpoint2d32fTocvpoint(p2);

    showLine(image, pt1, pt2, true);
}

CvPoint cvpoint2d32fTocvpoint(CvPoint2D32f p) {
    int x = (int) p.x;
    int y = (int) p.y;

    if ((p.x- (int)p.x) > 0.5)
        x += 1;
    if ((p.y - (int)p.y) > 0.5)
        y += 1;
    return cvPoint((int) x, (int) y);
}

void showBox(IplImage *image, CvBox2D box, bool wait) {
    CvPoint2D32f box_vtx[4];
    CvPoint pt, pt0;
    int i;

    IplImage *temp = cvCloneImage(image);

    if (box.size.height != 0) {
        cvBoxPoints(box, box_vtx); // Finds box vertices
        for (i = 0; i < 4; i++) {
            pt0.x = cvRound(box_vtx[i].x);
            pt0.y = cvRound(box_vtx[i].y);
            pt.x = cvRound(box_vtx[(i + 1) % 4].x);
            pt.y = cvRound(box_vtx[(i + 1) % 4].y);
            cvLine(temp, pt0, pt, CV_RGB(0, 255, 0), 5, CV_AA, 0);
        }
    }

    showImage(temp, "Box Area", wait);
}

void showRectangle(IplImage *image, CvRect rect, bool wait) {
    CvPoint2D32f box_vtx[4];
    CvPoint pt, pt0;
    int i;

    IplImage *temp = cvCloneImage(image);

    pt0.x = rect.x;
    pt0.y = rect.y;
    pt.x = rect.x + rect.width;
    pt.y = rect.y + rect.height;
    cvRectangle(temp, pt0, pt, CV_RGB(0, 0, 255), 5, CV_AA, 0);

    showImage(temp, "Rectangle Area", wait);
}

void showContours(IplImage *image, CvSeq* contour, bool wait) {
    int i;

    IplImage *temp = cvCreateImage(cvGetSize(image), 8, 3);

    for (; contour != 0; contour = contour->h_next) {
        CvScalar color = CV_RGB(0, 255, 0);
        /* replace CV_FILLED with 1 to see the outlines */

        cvDrawContours(temp, contour, color, color, -1, 1, 8);

    }

    showImage(temp, "Contours Area", wait);
}

void showContours2(IplImage *image, CvSeq* contour, bool wait) {
    int i;
    CvSeq* c;

    IplImage *temp = cvCreateImage(cvGetSize(image), 8, 3);
    CvScalar color;
    for (; contour != 0; contour = contour->h_next) {
        if (contour->v_next != 0) {
            color = CV_RGB(0, 255, 0);
            //cvDrawContours(temp, contour, color, color, 0, 1, 8);
            c = contour->v_next;
            for (; c != 0; c = c->h_next) {
                color = CV_RGB(255, 255, 0);
                cvDrawContours(temp, c, color, color, 0, CV_FILLED, 8);
            }
        } else {
            color = CV_RGB(255, 0, 0);
            //cvDrawContours(temp, contour, color, color, 0, 1, 8);
        }
        /* replace CV_FILLED with 1 to see the outlines */


    }

    showImage(temp, "Contours Area", wait);
}