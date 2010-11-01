#ifndef METHODS_H
#define METHODS_H

#include "opencv/cv.h"
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

class Methods
{
public:
    Methods();
    void Methods::getObjectColor(int x, int y, IplImage *img, int *h, int *s, int *v);
    void binarization(IplImage* imgSrc, IplImage *imgDest, int h, int s, int v,  int tolerance);
    CvRect computeROI(CvRect rect, int padding);
    void detectArea(IplImage *img, CvPoint2D32f pt, CvBox2D *pBox, CvRect *pRect);
    void findShapeAreaAroundPoint(IplImage *img_bin, CvPoint2D32f pt, CvBox2D *pBox, CvRect *pRect);
    void FindBox(IplImage *img_bin, CvBox2D *pBox, CvRect *pRect);
    void drawBox(IplImage *img, CvBox2D box, CvRect rect);
    void detectGoal(IplImage *img, CvRect ROI, int x, int y, CvBox2D *pBox, CvRect *pRect);
    void testMethods();
private:
    void Print_Box(CvBox2D box) ;
    void Print_Rect(CvRect rect);
    void showImage(IplImage *img_originale, char *name);
    void afficher_box(IplImage *img_originale, CvBox2D box, CvRect rect);
    void afficher_Point(IplImage *image, CvPoint p);
};

#endif // METHODS_H
