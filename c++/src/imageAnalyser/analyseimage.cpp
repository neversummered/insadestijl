/*
 *  position.h
 *  
 *
 *  Created by Pirem on 19/01/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "analyseimage.h"

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

namespace robotInsa {
    CvPoint2D32f tmpVecPosition;
    CvPoint2D32f tmpVecOrientation;

    IplImage *imgSrc;
    IplImage *imgResized;
    IplImage *imgBinarized;
    IplImage *roiBinarized;

    CvRect roi;

    CvPoint2D32f spots[3];

    /* Parameters */
    int imageFactor = 2;
    int filterThreshold = 200;
    int nbIterErode = 0;
    int nbIterDilate = 0;

    /* Valeurs pour distinguer le robot des autres éléments */
    int robotHeihtReference = 38;
    int robotWidthReference = 46;
    float robotFactorReference = 1.2;

    Position AnalyseImage::computeRobotPosition(IplImage *image) {
        imgSrc = cvCloneImage(image);
        resizeWithFactor();
        hardFilter();
        findROI();
        applyROI();
        filterROI();
        findPositionPoints();
        tmpVecPosition = computeTriangleMasscenter(spots);
        tmpVecOrientation = computeOrientation(spots);
        freeROI();

        Position pos(tmpVecPosition.x, tmpVecPosition.y, polarConversion(tmpVecOrientation));
        return pos;
    }

    void AnalyseImage::loadSrcImage(char *fileName) {
        imgSrc = cvLoadImage(fileName);
    }

    void AnalyseImage::resizeWithFactor() {
        // declare a destination IplImage object with correct size, depth and channels
        imgResized = cvCreateImage(
                cvSize(
                (int) (imgSrc->width / imageFactor),
                (int) (imgSrc->height / imageFactor)),
                imgSrc->depth,
                imgSrc->nChannels);

        //use cvResize to resize source to a destination image
        cvResize(imgSrc, imgResized);
    }

    void AnalyseImage::hardFilter() {
        // create memory storage that will contain all the dynamic data
        int flip = 0;

        //Initialization
        IplImage *img_nvg_tmp = cvCreateImage(cvGetSize(imgResized), imgResized->depth, 1);
        imgBinarized = cvCloneImage(img_nvg_tmp);

        //Morphology Kernel definition
        IplConvKernel *kernel = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT);

        //Control the orientation
        if (imgResized->origin != IPL_ORIGIN_TL) {
            flip = CV_CVTIMG_FLIP;
        }

        cvConvertImage(imgResized, img_nvg_tmp, flip);

        //Binarization
        cvThreshold(img_nvg_tmp, imgBinarized, filterThreshold, 255, CV_THRESH_BINARY);

        //Morphology
        // Morphological opening (inverse because we have white pixels on black background)
        if (nbIterDilate != 0)
            cvDilate(imgBinarized, imgBinarized, kernel, nbIterDilate);
        if (nbIterErode != 0)
            cvErode(imgBinarized, imgBinarized, kernel, nbIterErode);
    }

    void AnalyseImage::findROI() {
        float h = 0.0;
        float w = 0.0;
        float tmp = 0.0;
        float factor = 0.0;
        float minFactor = 99999.9;
        // create memory storage that will contain all the dynamic data
        CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* contours;
        CvBox2D *pBox;

        cvFindContours(imgBinarized, storage, &contours, sizeof (CvContour),
                CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

        /* recherche de la forme la plus proche du robot */
        for (; contours != 0; contours = contours->h_next) {
            *pBox = cvMinAreaRect2(contours);
            h = pBox->size.height;
            w = pBox->size.width;
            if ((h != 0) && (w != 0)) {
                if (h > w) {
                    tmp = h;
                    h = w;
                    w = tmp;
                }
                factor = fabs(h - robotHeihtReference) / robotHeihtReference
                        + fabs(w - robotWidthReference) / robotWidthReference
                        + fabs((w / h) - robotFactorReference) / robotFactorReference;

                if (factor < minFactor) {
                    minFactor = factor;
                    roi = cvBoundingRect(contours, 1);
                }
            }
        }

        /* Increase the ROI to be sure to capture all the robot */
        roi.height = roi.height * imageFactor + 20;
        roi.width = roi.width * imageFactor + 20;
        roi.x = roi.x * imageFactor - 10;
        roi.y = roi.y * imageFactor - 10;
    }

    void AnalyseImage::applyROI() {
        cvSetImageROI(imgSrc, roi);
    }

    void AnalyseImage::filterROI() {
        // create memory storage that will contain all the dynamic data
        int flip = 0;

        //Initialization
        IplImage *img_nvg_tmp = cvCreateImage(cvGetSize(imgSrc), imgSrc->depth, 1);
        roiBinarized = cvCloneImage(img_nvg_tmp);

        //Morphology Kernel definition
        IplConvKernel *kernel = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT);

        //Control the orientation
        if (imgSrc->origin != IPL_ORIGIN_TL) {
            flip = CV_CVTIMG_FLIP;
        }

        cvConvertImage(imgSrc, img_nvg_tmp, flip);

        //Binarization
        cvThreshold(img_nvg_tmp, roiBinarized, filterThreshold, 255, CV_THRESH_OTSU);

        cvErode(roiBinarized, roiBinarized, kernel, 1);
        cvDilate(roiBinarized, roiBinarized, kernel, 1);
    }

    void AnalyseImage::findPositionPoints() {
        CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq *contour;
        CvSeq *c;
        CvRect *pRect;
        CvPoint pt;
        int cmpt = 0;

        cvFindContours(roiBinarized, storage, &contour, sizeof (CvContour),
                CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

        for (; contour != 0; contour = contour->h_next) {
            if (contour->v_next != 0) {
                c = contour->v_next;
                for (; c != 0; c = c->h_next) {
                    //Trouver le rect du contour
                    *pRect = cvBoundingRect(c, 1);
                    spots[cmpt] = computeMassCenter(c, pRect);
                    cmpt++;
                }
            }
        }
        if (cmpt != 3) {
            printf("ERROR spots\n");
        }
    }

    CvPoint2D32f AnalyseImage::computeMassCenter(CvSeq *c, CvRect *r) {
        int isInside = -1;
        CvPoint2D32f pt = cvPoint2D32f(0, 0);
        int x, y;
        float Mx = 0.0;
        float My = 0.0;
        int nbPt = 0;

        for (x = r->x; x < r->x + r->width; x++) {
            for (y = r->y; y < r->y + r->height; y++) {
                pt.x = x;
                pt.y = y;
                if (cvPointPolygonTest(c, pt, 0)) {
                    Mx += x;
                    My += y;
                    nbPt++;
                }
            }
        }
        pt.x = Mx / nbPt;
        pt.y = My / nbPt;

        return pt;
    }

    CvPoint2D32f AnalyseImage::computeTriangleMasscenter(CvPoint2D32f vertex[3]) {
        CvPoint2D32f mc = cvPoint2D32f(0.0, 0.0);

        mc.x = vertex[2].x / 3 + ((vertex[0].x + vertex[1].x) / 3);
        mc.y = vertex[2].y / 3 + ((vertex[0].y + vertex[1].y) / 3);

        return mc;
    }

    CvPoint2D32f AnalyseImage::computeOrientation(CvPoint2D32f vertex[3]) {
        CvPoint2D32f vectorOrientation = cvPoint2D32f(0.0, 0.0);
        float x, y;
        float maxLength = 0.0;
        float length;
        int i, j;
        CvPoint2D32f tmpVertex[3];

        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                tmpVertex[j] = vertex[(i + j) % 3];
            }
            /* Compute median */
            x = ((tmpVertex[0].x + tmpVertex[1].x) / 2) - tmpVertex[2].x;
            y = ((tmpVertex[0].y + tmpVertex[1].y) / 2) - tmpVertex[2].y;
            /* find longest median */
            length = pow(x, 2) + pow(y, 2);
            if (maxLength < length) {
                maxLength = length;
                vectorOrientation.x = x;
                vectorOrientation.y = y;
            }
        }

        return vectorOrientation;
    }

    void AnalyseImage::freeROI() {
        cvResetImageROI(imgSrc);

        tmpVecPosition.x += roi.x;
        tmpVecPosition.y += roi.y;
    }

    float AnalyseImage::polarConversion(CvPoint2D32f vec) {
        float orientation = 0;
        if ((vec.x > 0) && (vec.y >= 0)) {
            orientation = atan(vec.y / vec.x);
        } else if ((vec.x > 0.0) && (vec.y >= 0.0)) {
            orientation = atan(vec.y / vec.x) + 2 * M_PI;
        } else if (vec.x < 0.0) {
            orientation = atan(vec.y / vec.x) + M_PI;
        } else if ((vec.x == 0.0) && (vec.y > 0.0)) {
            orientation = M_PI / 2;
        } else if ((vec.x == 0.0) && (vec.y < 0.0)) {
            orientation = 3 * M_PI / 2;
        }
        return orientation;
    }
}